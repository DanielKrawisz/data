// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_NIST_DRBG
#define DATA_CRYPTO_NIST_DRBG

#include <data/bytes.hpp>
#include <data/random.hpp>
#include <data/crypto/hash.hpp>
#include <data/crypto/MAC.hpp>
#include <data/crypto/MAC/HMAC.hpp>
#include <data/crypto/block/cipher.hpp>
#include <data/crypto/block/padding.hpp>
#include <data/integral.hpp>
#include <data/tools/lazy_writer.hpp>
#include <cryptopp/drbg.h>

namespace data::crypto::NIST {

    template <typename engine> concept DRBG =
        random::DRBG<engine> && requires (byte_slice seed, byte_slice nonce, byte_slice personalization) {
            engine {seed, personalization, nonce};
        } && requires (engine &drbg, byte *b, size_t size, byte_slice additional) {
            { drbg.generate (b, size, additional) };
            { drbg.generate (b, size) };
        } && requires (engine &drbg, byte_slice seed, byte_slice additional) {
            { drbg.reseed (seed, additional) };
        };

    // Make a NIST DRBG type conform to the crypto::random::RNG concept.
    // NIST DRBGs have the option of attaching additional entropy with
    // each generate call. Here we add something to deliver it automatically.
    // the entropy source does not need to be as strong as the seed.
    template <DRBG drbg> struct auto_generate_with_additional_entropy : drbg {
        data::random::source &GenerateEntropy;
        constexpr const static size_t MinEntropyLength = drbg::MinEntropyLength;

        auto_generate_with_additional_entropy (
            byte_slice entropy,
            data::random::source &autogen,
            byte_slice nonce = {},
            byte_slice personalization = {}):
        drbg {entropy, nonce, personalization},
        GenerateEntropy {autogen} {}

        void read (byte *b, size_t x) final override {
            bytes additional;
            additional.resize (MinEntropyLength);
            GenerateEntropy >> additional;
            this->generate (b, x, additional);
        }

        void reseed (byte_slice b) final override {
            bytes additional;
            additional.resize (MinEntropyLength);
            GenerateEntropy >> additional;
            static_cast<drbg *> (this)->reseed (b, additional);
        }
    };

    // the only approved algorithms allowed in NIST DRBGs.
    // Hash functions are used in Hash_DRBG and HMAC_DRBG,
    // and the block ciphers are used in CTR_DRBG.
    template <typename H> concept Approved =
        Same<H, hash::SHA1> || // depricated
        Same<H, hash::SHA2_224> || Same<H, hash::SHA2_256> ||
        Same<H, hash::SHA2_384> || Same<H, hash::SHA2_512> ||
        // TODO
        //Same<H, hash::SHA2_512_224> || Same<H, hash::SHA2_512_256> ||
        Same<H, cipher::block::TDEA3> ||
        Same<H, cipher::block::AES>;

    // Strengths of approved algorithms, as defined by NIST.
    // possible strengths are 112, 128, 192, and 256
    template <typename H, auto ...> constexpr size_t strength ();

    template <> constexpr size_t inline strength<hash::SHA1> () {
        return 128;
    }

    template <> constexpr size_t inline strength<hash::SHA2_224> () {
        return 112;
    }

    template <> constexpr size_t inline strength<hash::SHA2_256> () {
        return 128;
    }

    template <> constexpr size_t inline strength<hash::SHA2_384> () {
        return 192;
    }

    template <> constexpr size_t inline strength<hash::SHA2_512> () {
        return 256;
    }

    template <> constexpr size_t inline strength<cipher::block::TDEA3, size_t {21}> () {
        return 112;
    }

    template <> constexpr size_t inline strength<cipher::block::AES, size_t {16}> () {
        return 128;
    }

    template <> constexpr size_t inline strength<cipher::block::AES, size_t {24}> () {
        return 192;
    }

    template <> constexpr size_t inline strength<cipher::block::AES, size_t {32}> () {
        return 256;
    }

    // in the documentation, these are given in bits, but we have them in bytes (8 bits).
    constexpr const static size_t MaxEntropyLength = size_t (1) << 32;
    constexpr const static size_t MaxBytesPerRequest = size_t (1) << 16;
    constexpr const static size_t MaxRequestsPerReseed = size_t (1) << 48;

    // base class for all NIST drbgs.
    template <Approved X, auto ...P> struct NIST_DRBG : random::generator {

        constexpr const static size_t MaxStrength = strength<X, P...> ();
        constexpr const static size_t MinEntropyLength = MaxStrength >> 3;

        virtual void generate (byte *b, size_t x, byte_slice additional = {}) = 0;

        void read (byte *b, size_t x) override {
            generate (b, x);
        }

        virtual void reseed (byte_slice entropy, byte_slice additional) = 0;

        void reseed (byte_slice entropy) override {
            return reseed (entropy, {});
        }

        virtual ~NIST_DRBG () {}

        NIST_DRBG () {}

        // not copyable
        NIST_DRBG (const NIST_DRBG &) = delete;
        NIST_DRBG &operator = (const NIST_DRBG &) = delete;
        NIST_DRBG (NIST_DRBG &&) = delete;
        NIST_DRBG &operator = (NIST_DRBG &&) = delete;
    };

    // Hash_DRBG derivation function.
    template <hash::Engine H, uint32_big bits>
    static math::uint_big<size_t (bits / 8), byte> Hash_df (byte_slice);

    // allowed hash engines are SHA1, SHA2-224, SHA2-256, SHA2-384, and SHA2-512
    template <hash::Engine H> struct Hash_DRBG : NIST_DRBG<H> {

        Hash_DRBG (byte_slice entropy, byte_slice nonce = {}, byte_slice personalization = {});

        void reseed (byte_slice entropy, byte_slice additional = {}) final override;

        void generate (byte *b, size_t x, byte_slice additional = {}) final override;

        ~Hash_DRBG () {}

        constexpr const static uint32_big SeedBits = H::DigestSize > 32 ? 888 : 440;

    private:
        constexpr const static size_t Length = size_t (SeedBits / 8);

        struct Hash;

        void Hashgen (byte *, size_t);

        Hash::digest V;
        Hash::digest C;

        uint32 ReseedCounter;
    };

    template <hash::Engine H>
    struct HMAC_DRBG : NIST_DRBG<H> {

        void generate (byte *b, size_t x, byte_slice additional = {}) final override;

        void reseed (byte_slice entropy, byte_slice additional = {}) final override;

        HMAC_DRBG (byte_slice entropy, byte_slice nonce = {}, byte_slice personalization = {});

        ~HMAC_DRBG () {}

    private:
        constexpr const static size_t Length = H::DigestSize;

        hash::digest<Length> Key;
        hash::digest<Length> V;

        void update (byte_slice data);

        uint32 ReseedCounter;
    };

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r = endian::big>
    struct CTR_base : NIST_DRBG<C, key_size> {
        constexpr const static size_t SeedLength = C::BlockSize + (Same<C, cipher::block::TDEA3> ? 21 : key_size);

        math::uint<r, C::BlockSize, byte> V;
        symmetric_key<key_size> Key;
        uint32 ReseedCounter;

        void update (const byte_array<SeedLength> &);

        virtual byte_array<SeedLength> df (byte_slice b) = 0;

        void generate (byte *result, size_t remaining, byte_slice additional = {}) final override;

    protected:
        CTR_base (): V {}, Key {}, ReseedCounter {0} {}
    };

    template <size_t key_size, cipher::block::Cipher<key_size> C, bool df = true, endian::order r = endian::big>
    struct CTR_DRBG;

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    struct CTR_DRBG<key_size, C, true, r> final : CTR_base<key_size, C, r> {
        using CTR = CTR_base<key_size, C, r>;

        CTR_DRBG (byte_slice entropy, byte_slice nonce, byte_slice personalization = {});

        void reseed (byte_slice entropy, byte_slice additional) final override;

        ~CTR_DRBG () {}

    private:

        // BCC is a block mode that is only used in CTR_DRBG. It reduces an
        // arbitrary message to a single block, similar to a hash function.
        struct BCC_writer;

        byte_array<CTR::SeedLength> df (byte_slice b) final override;

    };

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    struct CTR_DRBG<key_size, C, false, r> final : CTR_base<key_size, C, r> {
        using CTR = CTR_base<key_size, C, r>;

        CTR_DRBG (byte_slice entropy, byte_slice additional = {}): CTR {} {
            reseed (entropy, additional);
        }

        void reseed (byte_slice entropy, byte_slice additional) final override {

            if (entropy.size () < this->MinEntropyLength)
                throw exception {} << "Not enough entropy provided to reseed Hash_DRBG for max strength " << this->MaxStrength;

            if (entropy.size () > MaxEntropyLength)
                throw exception {} << "Max length exceeded for entropy to reseed Hash_DRBG";

            if (additional.size () > MaxEntropyLength)
                throw exception {} << "Max length exceeded for additional information to reseed Hash_DRBG";

            this->update (df (entropy) ^ df (additional));
            this->ReseedCounter = 1;
        }

        ~CTR_DRBG () {}

    private:

        byte_array<CTR::SeedLength> df (byte_slice b) final override {
            byte_array<CTR::SeedLength> x {};
            std::copy (b.begin (), b.begin () + std::min (b.size (), x.size ()), x.begin ());
            return x;
        }

    };

    struct insufficient_entropy : exception::base<insufficient_entropy> {};

    template <hash::Engine H>
    Hash_DRBG<H>::Hash_DRBG (byte_slice entropy, byte_slice nonce, byte_slice personalization):
        NIST_DRBG<H> {}, V {}, C {}, ReseedCounter {1} {
        // if seed is not at least size strength / 8, then it's not good enough.
        if (entropy.size () < this->MinEntropyLength)
            throw insufficient_entropy {} << "Not enough entropy provided to initialize Hash_DRBG for max strength " << this->MaxStrength <<
                "; min entropy length is " << this->MinEntropyLength << " and the provided entropy had size " << entropy.size ();

        if (entropy.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for entropy to instantiate Hash_DRBG";

        if (nonce.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for nonce to instantiate Hash_DRBG";

        if (personalization.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for personalization to instantiate Hash_DRBG";

        // seed is entropy + nonce + personalization
        V = Hash_df<H, SeedBits> (write<bytes> (entropy, nonce, personalization));
        C = Hash_df<H, SeedBits> (write<bytes> (byte (0x00), V));
    }

    template <hash::Engine H>
    HMAC_DRBG<H>::HMAC_DRBG (byte_slice entropy, byte_slice nonce, byte_slice personalization)
    : NIST_DRBG<H> {}, Key {byte_array<Length>::filled (0x00)}, V {byte_array<Length>::filled (0x01)}, ReseedCounter {1} {
        if (entropy.size () < this->MinEntropyLength)
            throw insufficient_entropy {} << "Not enough entropy provided to initialize Hash_DRBG for max strength " << this->MaxStrength;

        if (entropy.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for entropy to instantiate Hash_DRBG";

        if (nonce.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for nonce to instantiate Hash_DRBG";

        if (personalization.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for personalization to instantiate Hash_DRBG";

        update (write<bytes> (entropy, nonce, personalization));

        ReseedCounter = 1;
    }

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    CTR_DRBG<key_size, C, true, r>::CTR_DRBG (byte_slice entropy, byte_slice personalization, byte_slice nonce):
    CTR {} {

        if (entropy.size () < this->MinEntropyLength)
            throw insufficient_entropy {} << "Not enough entropy provided to initialize Hash_DRBG for max strength " << this->MaxStrength;

        if (entropy.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for entropy to instantiate Hash_DRBG";

        if (nonce.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for nonce to instantiate Hash_DRBG";

        if (personalization.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for personalization to instantiate Hash_DRBG";

        this->update (df (write<bytes> (entropy, nonce, personalization)));

        this->ReseedCounter = 1;
    }

    template <hash::Engine H>
    void Hash_DRBG<H>::reseed (byte_slice entropy, byte_slice additional) {

        if (entropy.size () < this->MinEntropyLength)
            throw insufficient_entropy {} << "Not enough entropy provided to reseed Hash_DRBG for max strength " << this->MaxStrength;

        if (entropy.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for entropy to reseed Hash_DRBG";

        if (additional.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for additional information to reseed Hash_DRBG";

        V = Hash_df<H, SeedBits> (write<bytes> (byte (0x01), V, entropy, additional));

        C = Hash_df<H, SeedBits> (write<bytes> (byte (0x00), V));

        ReseedCounter = 1;
    }

    template <hash::Engine H>
    void HMAC_DRBG<H>::reseed (byte_slice entropy, byte_slice additional) {

        if (entropy.size () < this->MinEntropyLength)
            throw exception {} << "Not enough entropy provided to reseed Hash_DRBG for max strength " << this->MaxStrength;

        if (entropy.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for entropy to reseed Hash_DRBG";

        if (additional.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for additional information to reseed Hash_DRBG";

        update (write<bytes> (entropy, additional));

        ReseedCounter = 1;
    }

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    void CTR_DRBG<key_size, C, true, r>::reseed (byte_slice entropy, byte_slice additional) {

        if (entropy.size () < this->MinEntropyLength)
            throw exception {} << "Not enough entropy provided to reseed Hash_DRBG for max strength " << this->MaxStrength;

        if (entropy.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for entropy to reseed Hash_DRBG";

        if (additional.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for additional information to reseed Hash_DRBG";

        this->update (df (write<bytes> (entropy, additional)));

        this->ReseedCounter = 1;
    }

    template <hash::Engine H>
    struct Hash_DRBG<H>::Hash : data::writer<byte> {
        using digest = math::uint_big<Length, byte>;
        Hash (digest &d): Digest {d} {}

        void write (const byte *b, size_t size) final override {
            Engine.Update (b, size);
        }

        ~Hash () {
            Digest = 0;
            Engine.Final (Digest.data () + (Length - H::DigestSize));
        }

    private:
        digest &Digest;
        H Engine;
    };

    template <hash::Engine H>
    void Hash_DRBG<H>::generate (byte *b, size_t size, byte_slice additional) {

        if (additional.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for additional information to generate from Hash_DRBG";

        if (size > MaxBytesPerRequest) throw exception {} << "Cannot generate more than " << MaxBytesPerRequest << " bytes per request ";

        if (ReseedCounter > MaxRequestsPerReseed) throw random::reseed_required {};

        if (additional.size () > 0)
            V += hash::write<Hash> (byte (0x02), V, additional);

        Hashgen (b, size);

        V += hash::write<Hash> (byte (0x03), V) + C + ReseedCounter;

        ReseedCounter++;
    }

    template <hash::Engine H>
    void HMAC_DRBG<H>::generate (byte *b, size_t size, byte_slice additional) {

        if (additional.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for additional information to generate from Hash_DRBG";

        if (size > MaxBytesPerRequest)
            throw exception {} << "Cannot generate more than " << MaxBytesPerRequest << " bytes per request ";

        if (additional.size () > 0) update (additional);

        bytes result = write<bytes> ([this, size] (auto &&w) {
            do {
                this->V = MAC::calculate<HMAC<H>> (this->Key, this->V);
                w << this->V;
            } while (w.TotalSize < size);
        });

        std::copy (result.data (), result.data () + size, b);

        update (additional);

        ReseedCounter++;

    }

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    void CTR_base<key_size, C, r>::generate (byte *result, size_t remaining, byte_slice additional) {
        if (additional.size () > MaxEntropyLength)
            throw exception {} << "Max length exceeded for additional information to generate from Hash_DRBG";

        if (remaining > MaxBytesPerRequest)
            throw exception {} << "Cannot generate more than " << MaxBytesPerRequest << " bytes per request ";

        byte_array<SeedLength> d;
        if (additional.size () != 0) {
            d = df (additional);
            update (d);
        }

        while (remaining > 0) {
            V++;
            byte_array<C::BlockSize> output_block;
            C::encrypt (output_block, Key, V);
            size_t bytes_to_copy = std::min (remaining, C::BlockSize);
            std::copy (output_block.begin (), output_block.begin () + bytes_to_copy, result);
            result += bytes_to_copy;
            remaining -= bytes_to_copy;
        }

        update (d);

        ReseedCounter++;

    }

    template <hash::Engine H>
    void Hash_DRBG<H>::Hashgen (byte *b, size_t bytes_requested) {
        iterator_writer w {b, b + bytes_requested};

        auto data = V;
        while (w.End - w.Begin > 0) {
            auto dig = hash::calculate<H> (data);
            ++data;
            w.write (dig.data (), std::min (size_t (w.End - w.Begin), dig.size ()));
        }
    }

    template <hash::Engine H, uint32_big bits>
    math::uint_big<size_t (bits / 8), byte> Hash_df (byte_slice b) {
        constexpr static const size_t length = size_t (bits / 8);
        math::uint_big<length, byte> result;
        iterator_writer w {result.begin (), result.end ()};

        byte counter {1};
        while (w.End - w.Begin > 0) {
            auto dig = hash::write<H> (counter, bits, b);
            counter++;
            w.write (dig.data (), std::min (size_t (w.End - w.Begin), dig.size ()));
        }

        return result;
    }

    template <hash::Engine H>
    void HMAC_DRBG<H>::update (byte_slice data) {
        {
            MAC::writer<HMAC<H>> w {Key, Key};
            w << V << byte (0x00) << data;
        }

        V = MAC::calculate<HMAC<H>> (Key, V);

        if (data.size () == 0) return;

        {
            MAC::writer<HMAC<H>> w {Key, Key};
            w << V << byte (0x01) << data;
        }

        V = MAC::calculate<HMAC<H>> (Key, V);
    }

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    struct CTR_DRBG<key_size, C, true, r>::BCC_writer final : data::writer<byte> {
        using digest = hash::digest<C::BlockSize>;
        digest &Output;
        symmetric_key<key_size> Key;
        size_t Index;

        BCC_writer (digest &d);
        void write (const byte *b, size_t remaining) final override;

        // pad and write to Digest.
        ~BCC_writer ();
    };

    // NOTE: this function uses encryption in CTR mode.
    // TODO make it work with what we already have for encryption.
    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    void CTR_base<key_size, C, r>::update (const byte_array<SeedLength> &provided) {
        size_t remaining = SeedLength;

        byte_array<SeedLength> Temp;
        auto temp = Temp.data ();
        while (remaining > 0) {
            V++;
            byte_array<C::BlockSize> output_block;
            C::encrypt (output_block, Key, V);
            size_t bytes_to_copy = std::min (remaining, C::BlockSize);
            std::copy (output_block.begin (), output_block.begin () + bytes_to_copy, temp);
            temp += bytes_to_copy;
            remaining -= bytes_to_copy;
        }

        Temp ^= provided;
        std::copy (Temp.begin (), Temp.begin () + key_size, Key.begin ());
        std::copy (Temp.begin () + key_size, Temp.end (), V.begin ());
    }

    // input is required to be a multiple of block size.
    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    byte_array<CTR_DRBG<key_size, C, true, r>::CTR::SeedLength>
    CTR_DRBG<key_size, C, true, r>::df (byte_slice input) {

        byte_array<C::BlockSize> X;
        symmetric_key<key_size> K;

        {

            size_t remaining = CTR::SeedLength;
            bytes Temp;
            Temp.resize (remaining);
            auto temp = Temp.data ();

            uint32_big i = 0;
            byte_array<C::BlockSize - 4> zeros;
            while (true) {

                auto bcc = hash::write<BCC_writer> (i, zeros,
                    uint32_big {static_cast<uint32> (input.size ())},
                    uint32_big {static_cast<uint32> (CTR::SeedLength)}, input);

                size_t bytes_to_copy = remaining > C::BlockSize ? C::BlockSize : remaining;
                std::copy (bcc.begin (), bcc.begin () + bytes_to_copy, temp);
                remaining -= bytes_to_copy;
                if (remaining == 0) break;
                temp += bytes_to_copy;
                i++;
            }

            std::copy (Temp.begin (), Temp.begin () + key_size, K.begin ());
            std::copy (Temp.begin () + key_size, Temp.end (), X.begin ());

        }

        size_t remaining = CTR::SeedLength;
        byte_array<CTR::SeedLength> Result;
        auto result = Result.data ();

        while (true) {
            C::encrypt (X, K, X);

            size_t bytes_to_copy = remaining > C::BlockSize ? C::BlockSize : remaining;
            std::copy (X.begin (), X.begin () + bytes_to_copy, result);
            remaining -= bytes_to_copy;
            if (remaining == 0) return Result;
            result += bytes_to_copy;
        }
    }

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    CTR_DRBG<key_size, C, true, r>::BCC_writer::BCC_writer (digest &d) :
    Output {d}, Key {}, Index {0} {
        Output = digest {};
        for (int i = 0; i < key_size; i++) Key[i] = i;
    }

    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    void CTR_DRBG<key_size, C, true, r>::BCC_writer::write (const byte *b, size_t remaining) {

        while (remaining >= C::BlockSize - Index) {

            // These lines had to be added in to remove some compiler warnings.
            // The compiler keeps thinking these lines are unsafe, but they
            // are fine.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"

            arithmetic::bit_xor<byte> (Output.end (), Output.begin () + Index, b, (const byte *) (Output.data () + Index));

#pragma GCC diagnostic pop

            remaining -= C::BlockSize - Index;
            b += C::BlockSize - Index;

            Index = 0;

            while (true) {

                C::encrypt (Output, Key, Output);

                if (remaining < C::BlockSize) break;

                arithmetic::bit_xor<byte> (Output.end (), Output.begin (), b, (const byte *) (Output.data ()));

                remaining -= C::BlockSize;
                b += C::BlockSize;
            }
        }

        if (remaining == 0) return;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"

        arithmetic::bit_xor<byte> (Output.begin () + Index + remaining, Output.begin () + Index, b, (const byte *) (Output.data () + Index));

#pragma GCC diagnostic pop

        Index += remaining;

    }

    // pad and write to Digest.
    template <size_t key_size, cipher::block::Cipher<key_size> C, endian::order r>
    CTR_DRBG<key_size, C, true, r>::BCC_writer::~BCC_writer () {
        cipher::block::pad<cipher::block::padding::ONE_AND_ZEROS_PADDING> {}.write (*this, C::BlockSize, Index);
    }

}

#endif

