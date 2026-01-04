// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_MODE
#define DATA_CRYPTO_BLOCK_MODE

#include <data/crypto/cipher.hpp>
#include <data/crypto/block/cipher.hpp>
#include <data/arithmetic/complementary.hpp>

namespace data::crypto::cipher::block {

/*  -------------------------------------------------------------------
 *  BLOCK CIPHER MODES
 *  -------------------------------------------------------------------
*/
    enum class mode : byte {
        streamable_asymmetric = 8,
        streamable_symmetric = 16,
        authenticated = 32,

        // Standard block cipher modes.

        // Modes that require padding.
        ECB = 0,
        CBC = 1,

        // The remaining modes can act like stream ciphers.
        CFB = 2 + 8,
        OFB = 3 + 16,
        CTR = 4 + 16,

        // The remaining modes are also authenticated.

        GCM = 5 + 32 + 8,

        // Not an acronym for anything.
        EAX = 6 + 32 + 8,

        // Patented but free for non-commerical use.
        OCB = 7 + 32 + 8
    };

    constexpr bool inline is_streamable (mode m) {
        return byte (m) & (byte (mode::streamable_symmetric) | byte (mode::streamable_asymmetric));
    }

    constexpr bool inline is_streamable_symmetric (mode m) {
        return byte (m) & byte (mode::streamable_symmetric);
    }

    constexpr bool inline is_streamable_asymmetric (mode m) {
        return byte (m) & byte (mode::streamable_asymmetric);
    }

    constexpr bool inline is_authenticated (mode m) {
        return byte (m) & byte (mode::authenticated);
    }

    std::ostream &operator << (std::ostream &, mode);

/*  -------------------------------------------------------------------
 *  ENCRYPTION AND DECRIPTION INTERNAL STATES
 *  -------------------------------------------------------------------
*/
    // the internal state of a block cipher.
    template <mode m, auto...> struct state;

    template <size_t block_size> using mode_electronic_code_book = state<mode::ECB>;
    template <size_t block_size> using mode_cipher_block_chain = state<mode::CBC, block_size>;
    template <size_t block_size> using mode_cipher_feedback = state<mode::CFB, block_size>;
    template <size_t block_size> using mode_output_feedback = state<mode::OFB, block_size>;
    template <size_t block_size, endian::order r> using mode_counter = state<mode::OFB, block_size, r>;

/*  -------------------------------------------------------------------
 *  INITIALIZE BLOCK CIPHER STATES
 *  -------------------------------------------------------------------
 *
 *  use initialize<mode> to initialize a block cipher mode using
 *  arguments to its constructor. ECB takes nothing, CTR takes a
 *  uint<r, size, byte>, and the rest take an initialization vector.
 *
*/
    namespace {
        state<mode::ECB> init_ECB ();

        template <size_t block_size> state<mode::CBC, block_size> init_CBC (const initialization_vector<block_size> &iv);

        template <size_t block_size> state<mode::OFB, block_size> init_OFB (const initialization_vector<block_size> &iv);

        template <size_t block_size> state<mode::CFB, block_size> init_CFB (const initialization_vector<block_size> &iv);

        template <size_t block_size, endian::order r> state<mode::CTR, block_size, r> init_CTR (const math::uint<r, block_size, byte> &iv);
    }

    template <mode m, typename ...T> auto inline initialize (T &&...z) {
        if constexpr (m == mode::ECB) return init_ECB (z...);
        if constexpr (m == mode::OFB) return init_OFB (z...);
        if constexpr (m == mode::CBC) return init_CBC (z...);
        if constexpr (m == mode::CFB) return init_CFB (z...);
        if constexpr (m == mode::CTR) return init_CTR (z...);
    }
/*  -------------------------------------------------------------------
 *  ENCRYPT AND DECRYPT FUNCTIONS
 *  -------------------------------------------------------------------
*/
    // encrypt and decrypt whole blocks.
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (!is_streamable (m))
    byte_array<cipher::BlockSize> encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block);

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (!is_streamable (m))
    byte_array<cipher::BlockSize> decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block);

    // encrypt and decrypt single blocks for streamable modes.
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_asymmetric (m))
    byte_array<cipher::BlockSize> encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block);

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_asymmetric (m))
    byte_array<cipher::BlockSize> decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block);

    // encrypt and decrypt single blocks for streamable modes.
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (m))
    byte_array<cipher::BlockSize> encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block);

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (m))
    byte_array<cipher::BlockSize> decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block);

    // encrypt and decrypt using iterators.
    template <typename cipher, mode m, auto ...n, size_t key_size, typename out, std::input_iterator in, typename sen>
    requires Cipher<cipher, key_size> &&
        std::output_iterator<out, slice<byte, cipher::BlockSize>> &&
        std::sentinel_for<sen, in>
    void encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        out &o, in &i, const sen &x);

    template <typename cipher, mode m, auto...n, size_t key_size, typename out, std::input_iterator in, typename sen>
    requires Cipher<cipher, key_size> &&
        std::output_iterator<out, slice<byte, cipher::BlockSize>> &&
        std::sentinel_for<sen, in>
    void decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        out &o, in &i, const sen &x);

    // encrypt and decrypt a sequence of blocks
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size>
    cross<byte_array<cipher::BlockSize>> encrypt (
        state<m, n...> &z,
        const cross<byte_array<cipher::BlockSize>> &message,
        const symmetric_key<key_size> &k);

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size>
    cross<byte_array<cipher::BlockSize>> decrypt (
        state<m, n...> &z,
        const cross<byte_array<cipher::BlockSize>> &encrypted,
        const symmetric_key<key_size> &k);

/*  -------------------------------------------------------------------
 *  BLOCK CIPHER STATES
 *  -------------------------------------------------------------------
*/
    template <> struct state<mode::ECB> {
        constexpr static const mode Mode = mode::ECB;
        state () {}

        template <size_t block_size> using block_in = slice<const byte, block_size>;
        template <size_t block_size> using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size, size_t block_size>
        requires Cipher<cipher, key_size> && (block_size == cipher::BlockSize)
        void encrypt (block_out<block_size> o, const symmetric_key<key_size> &key, block_in<block_size> i) {
            cipher::encrypt (o, key, i);
        }

        template <typename cipher, size_t key_size, size_t block_size>
        requires Cipher<cipher, key_size> && (block_size == cipher::BlockSize)
        void decrypt (block_out<block_size> o, const symmetric_key<key_size> &key, block_in<block_size> i) {
            cipher::decrypt (o, key, i);
        }
    };

    template <size_t block_size> struct state<mode::CBC, block_size> {

        constexpr static const mode Mode = mode::CBC;
        constexpr const static size_t BlockSize = block_size;

        initialization_vector<block_size> IV;
        state (const initialization_vector<block_size> &iv): IV {iv} {}

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size>
        requires Cipher<cipher, key_size> && (block_size == cipher::BlockSize)
        void encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
            byte_array<block_size> xored;
            arithmetic::bit_xor<byte> (xored.end (), xored.begin (), i.data (), (const byte *) (IV.data ()));
            cipher::encrypt (o, key, xored);
            std::copy (o.begin (), o.end (), IV.begin ());
        }

        template <typename cipher, size_t key_size>
        requires Cipher<cipher, key_size> && (block_size == cipher::BlockSize)
        void decrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
            cipher::decrypt (o, key, i);
            arithmetic::bit_xor<byte> (o.end (), o.begin (), o.data (), IV.data ());
            std::copy (i.begin (), i.end (), IV.begin ());
        }
    };

    template <size_t block_size> struct streamable_base {
        constexpr const static size_t BlockSize = block_size;

        byte_array<block_size> IV {};
        size_t Index {BlockSize};

        template <typename cipher, size_t key_size>
        requires Cipher<cipher, key_size> && (block_size == cipher::BlockSize)
        void initialize (const symmetric_key<key_size> &key) {
            cipher::encrypt (IV, key, IV);
            Index = 0;
        }

        streamable_base (const initialization_vector<block_size> &iv): IV {iv} {}
    };

    template <size_t block_size> struct state<mode::CFB, block_size> : streamable_base<block_size> {

        constexpr static const mode Mode = mode::CFB;

        state (const initialization_vector<block_size> &iv): streamable_base<block_size> {iv} {}

        byte encrypt (byte b) {
            byte result = this->IV[this->Index] ^= b;
            this->Index++;
            return result;
        }

        byte decrypt (byte b) {
            byte result = this->IV[this->Index] ^ b;
            this->IV[this->Index] = b;
            this->Index++;
            return result;
        }
    };

    template <size_t block_size> struct state<mode::OFB, block_size> : streamable_base<block_size> {

        constexpr static const mode Mode = mode::OFB;

        state (const initialization_vector<block_size> &iv): streamable_base<block_size> {iv} {}

        // both operations are the same.
        byte crypt (byte b) {
            byte result = this->IV[this->Index] ^ b;
            this->Index++;
            return result;
        }

        byte next () {
            return this->IV[this->Index++];
        }

    };

    // since there is no rule that says what endian
    // the counter should be, we allow both.
    template <size_t block_size, endian::order r>
    struct state<mode::CTR, block_size, r> : streamable_base<block_size> {

        constexpr static const mode Mode = mode::CTR;

        math::uint<r, block_size, byte> Count;

        state (const math::uint<r, block_size, byte> &iv) : streamable_base<block_size> {iv}, Count {iv} {}

        // both operations are the same.
        byte crypt (byte b) {
            byte result = this->IV[this->Index] ^ b;
            this->Index++;
            if (this->Index == this->BlockSize) {
                Count++;
                this->IV = Count;
            }
            return result;
        }

        byte next () {
            byte result = this->IV[this->Index];
            this->Index++;
            if (this->Index == this->BlockSize) {
                Count++;
                this->IV = Count;
            }
            return result;
        }
    };

/*  -------------------------------------------------------------------
 *  WRITERS AND READERS FOR ENCRYPTING AND DECRYPTING
 *  -------------------------------------------------------------------
*/
    template <typename cipher, typename state, size_t key_size, direction dir> struct writer;
    template <typename cipher, typename state, size_t key_size, direction dir> struct reader;

    template <size_t key_size> struct writer_base : data::writer<byte> {
        symmetric_key<key_size> Key;
        data::writer<byte> &Out;
        size_t Index;

        writer_base (const symmetric_key<key_size> &k, data::writer<byte> &next):
        Key {k}, Out {next}, Index {0} {}
    };

    template <size_t key_size> struct reader_base : data::reader<byte> {
        symmetric_key<key_size> Key;
        data::reader<byte> &In;
        size_t Index;

        reader_base (const symmetric_key<key_size> &k, data::reader<byte> &in):
        Key {k}, In {in}, Index {0} {}
    };

    template <typename cipher, typename state, size_t key_size, direction dir>
    requires Cipher<cipher, key_size> && (!is_streamable (state::Mode))
    struct writer<cipher, state, key_size, dir> : writer_base<key_size> {
        state State;
        byte Text[cipher::BlockSize];

        writer (const state &z, const symmetric_key<key_size> &k, data::writer<byte> &next):
        writer_base<key_size> {k, next}, State {z} {}

        void write (const byte *b, size_t size) final override {

            const byte *remaining = b;
            size_t remaining_bytes = size;
            while (remaining_bytes > 0) {

                size_t bytes_to_copy = cipher::BlockSize - this->Index > remaining_bytes ? remaining_bytes : cipher::BlockSize - this->Index;
                std::copy (remaining, remaining + bytes_to_copy, Text + this->Index);
                remaining += bytes_to_copy;
                remaining_bytes -= bytes_to_copy;

                this->Index += bytes_to_copy;
                if (this->Index == cipher::BlockSize) {
                    if constexpr (dir == encryption) {
                        this->Out << encrypt<cipher> (State, this->Key, byte_slice {Text, cipher::BlockSize});
                    } else {
                        this->Out << decrypt<cipher> (State, this->Key, byte_slice {Text, cipher::BlockSize});
                    } this->Index = 0;
                }
            }
        }

        ~writer () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        void complete () {
            if (this->Index != 0) throw exception {} << "complete called on incomplete message; message size must be a multiple of block size";
        }
    };

    template <typename cipher, typename state, size_t key_size, direction dir>
    requires Cipher<cipher, key_size> && (is_streamable_asymmetric (state::Mode))
    struct writer<cipher, state, key_size, dir> : writer_base<key_size> {
        state State;

        writer (const state &z, const symmetric_key<key_size> &k, data::writer<byte> &next):
        writer_base<key_size> {k, next}, State {z} {}

        void write (const byte *b, size_t size) final override {

            byte result[size];
            size_t index = 0;
            while (index != size) {

                if (this->Index == 0) State.template initialize<cipher> (this->Key);

                if constexpr (dir == encryption) {
                    result[index] = State.encrypt (b[index]);
                } else {
                    result[index] = State.decrypt (b[index]);
                }

                this->Index = (this->Index + 1) % cipher::BlockSize;
                index++;

            }
            this->Out.write (result, size);
        }

        ~writer () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        void complete () {}
    };

    template <typename cipher, typename state, size_t key_size, direction dir>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (state::Mode))
    struct writer<cipher, state, key_size, dir> : writer_base<key_size> {
        state State;

        writer (const state &z, const symmetric_key<key_size> &k, data::writer<byte> &next):
        writer_base<key_size> {k, next}, State {z} {}

        void write (const byte *b, size_t size) final override {

            byte result[size];
            size_t index = 0;
            while (index != size) {

                if (this->Index == 0) State.template initialize<cipher> (this->Key);

                result[index] = State.next () ^ b[index];

                this->Index = (this->Index + 1) % cipher::BlockSize;
                index++;

            }

            this->Out.write (result, size);
        }

        ~writer () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        void complete () {}
    };

    template <typename cipher, typename state, size_t key_size, direction dir>
    requires Cipher<cipher, key_size> && (!is_streamable (state::Mode))
    struct reader<cipher, state, key_size, dir> : reader_base<key_size> {
        state State;
        byte Text[cipher::BlockSize];

        reader (const state &z, const symmetric_key<key_size> &k, data::reader<byte> &next):
        reader_base<key_size> {k, next}, State {z} {}

        void read (byte *b, size_t size) final override {
            byte *remaining = b;
            size_t remaining_bytes = size;
            while (remaining_bytes > 0) {
                if (this->Index == 0) {
                    this->In.read (Text, cipher::BlockSize);
                    byte_array<cipher::BlockSize> crypted = dir == encryption ?
                        encrypt<cipher> (State, this->Key, byte_slice {Text, cipher::BlockSize}):
                        decrypt<cipher> (State, this->Key, byte_slice {Text, cipher::BlockSize});
                    std::copy (crypted.begin (), crypted.end (), Text);
                }

                size_t bytes_to_copy = (cipher::BlockSize - this->Index) > remaining_bytes ?
                    remaining_bytes :
                    (cipher::BlockSize - this->Index);

                std::copy (Text + this->Index, Text + this->Index + bytes_to_copy, remaining);
                remaining += bytes_to_copy;
                remaining_bytes -= bytes_to_copy;

                this->Index += bytes_to_copy;
                this->Index %= cipher::BlockSize;
            }
        }

        // this could be done much more efficiently, but we don't want to think about it.
        void skip (size_t size) final override {
            byte skipped[size];
            read (skipped, size);
        }

        ~reader () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        void complete () {
            if (this->Index != 0) throw exception {} << "complete called on incomplete message; message size must be a multiple of block size";
        }
    };

    template <typename cipher, typename state, size_t key_size, direction dir>
    requires Cipher<cipher, key_size> && (is_streamable_asymmetric (state::Mode))
    struct reader<cipher, state, key_size, dir> : reader_base<key_size> {
        state State;

        reader (const state &z, const symmetric_key<key_size> &k, data::reader<byte> &next):
        reader_base<key_size> {k, next}, State {z} {}

        void read (byte *b, size_t size) final override {
            byte result[size];
            this->In.read (result, size);
            size_t index = 0;
            while (index != size) {

                if (this->Index == 0) State.template initialize<cipher> (this->Key);

                if constexpr (dir == encryption)
                    b[index] = State.encrypt (result[index]);
                else
                    b[index] = State.decrypt (result[index]);

                this->Index = (this->Index + 1) % cipher::BlockSize;
                index++;

            }
        }

        // this could be done much more efficiently, but we don't want to think about it.
        void skip (size_t size) final override {
            byte skipped[size];
            read (skipped, size);
        }

        ~reader () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        void complete () {}
    };

    template <typename cipher, typename state, size_t key_size, direction dir>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (state::Mode))
    struct reader<cipher, state, key_size, dir> : reader_base<key_size> {
        state State;

        reader (const state &z, const symmetric_key<key_size> &k, data::reader<byte> &next):
        reader_base<key_size> {k, next}, State {z} {}

        void read (byte *b, size_t size) final override {

            byte result[size];
            this->In.read (result, size);

            size_t index = 0;
            while (index != size) {

                if (this->Index == 0) State.template initialize<cipher> (this->Key);

                b[index] = State.next () ^ *(result + index);

                this->Index = (this->Index + 1) % cipher::BlockSize;
                index++;

            }
        }

        // this could be done much more efficiently, but we don't want to think about it.
        void skip (size_t size) final override {
            byte skipped[size];
            read (skipped, size);
        }

        ~reader () noexcept (false) {
            if (std::uncaught_exceptions () == 0) {
                complete ();
            }
        }

    private:
        void complete () {}
    };

    template <typename cipher, typename state, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (state::Mode))
    struct stream : reader_base<key_size> {
        state State;

        stream (const state &z, const symmetric_key<key_size> &k, data::reader<byte> &next):
        reader_base<key_size> {k, next}, State {z} {}

        void read (byte *b, size_t size) final override {

            byte result[size];
            this->In.read (result, size);

            size_t index = 0;
            while (index != size) {

                if (this->Index == 0) State.template initialize<cipher> (this->Key);

                result[index] = State.next ();

                this->Index = (this->Index + 1) % cipher::BlockSize;
                index++;

            }
        }

        // this could be done much more efficiently, but we don't want to think about it.
        void skip (size_t size) final override {
            byte skipped[size];
            read (skipped, size);
        }

        ~stream () noexcept (false) {}
    };

    // encrypt and decrypt whole blocks.
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (!is_streamable (m))
    byte_array<cipher::BlockSize> inline encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> ciphertext;
        z.template encrypt<cipher> (data::slice<byte, cipher::BlockSize> (ciphertext), key, block);
        return ciphertext;
    }

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (!is_streamable (m))
    byte_array<cipher::BlockSize> inline decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> plaintext;
        z.template decrypt<cipher> (data::slice<byte, cipher::BlockSize> (plaintext), key, block);
        return plaintext;
    }

    // encrypt and decrypt single blocks for streamable modes.
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_asymmetric (m))
    byte_array<cipher::BlockSize> inline encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> ciphertext;
        z.template initialize<cipher> (key);
        for (size_t i = 0; i < cipher::BlockSize; i++) ciphertext[i] = z.encrypt (block[i]);
        return ciphertext;
    }

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_asymmetric (m))
    byte_array<cipher::BlockSize> inline decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> plaintext;
        z.template initialize<cipher> (key);
        for (size_t i = 0; i < cipher::BlockSize; i++) plaintext[i] = z.decrypt (block[i]);
        return plaintext;
    }

    // encrypt and decrypt single blocks for streamable modes.
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (m))
    byte_array<cipher::BlockSize> inline encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> result;
        z.template initialize<cipher> (key);
        for (size_t i = 0; i < cipher::BlockSize; i++) result[i] = z.crypt (block[i]);
        return result;
    }

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size> && (is_streamable_symmetric (m))
    byte_array<cipher::BlockSize> inline decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        slice<const byte, cipher::BlockSize> block) {
        return encrypt<cipher> (z, key, block);
    }

    // encrypt and decrypt using iterators.
    template <typename cipher, mode m, auto ...n, size_t key_size, typename out, std::input_iterator in, typename sen>
    requires Cipher<cipher, key_size> &&
        std::output_iterator<out, slice<byte, cipher::BlockSize>> &&
        std::sentinel_for<sen, in>
    void encrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        out &o, in &i, const sen &x) {
        while (i != x) {
            *o = encrypt (z, key, *i);
            i++;
            o++;
        }
    }

    template <typename cipher, mode m, auto...n, size_t key_size, typename out, std::input_iterator in, typename sen>
    requires Cipher<cipher, key_size> &&
        std::output_iterator<out, slice<byte, cipher::BlockSize>> &&
        std::sentinel_for<sen, in>
    void decrypt (
        state<m, n...> &z,
        const symmetric_key<key_size> &key,
        out &o, in &i, const sen &x) {
        while (i != x) {
            *o = decrypt (z, key, *i);
            i++;
            o++;
        }
    }

    // encrypt and decrypt a sequence of blocks
    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size>
    cross<byte_array<cipher::BlockSize>> encrypt (
        state<m, n...> &z,
        const cross<byte_array<cipher::BlockSize>> &message,
        const symmetric_key<key_size> &k) {
        cross<byte_array<cipher::BlockSize>> result;
        result.resize (message.size ());
        encrypt (m, k, result.begin (), message.begin (), message.end ());
        return result;
    }

    template <typename cipher, mode m, auto ...n, size_t key_size>
    requires Cipher<cipher, key_size>
    cross<byte_array<cipher::BlockSize>> decrypt (
        state<m, n...> &z,
        const cross<byte_array<cipher::BlockSize>> &encrypted,
        const symmetric_key<key_size> &k) {
        cross<byte_array<cipher::BlockSize>> result;
        result.resize (encrypted.size ());
        decrypt (m, k, result.begin (), encrypted.begin (), encrypted.end ());
        return result;
    }

    namespace {
        state<mode::ECB> inline init_ECB () {
            return state<mode::ECB> {};
        }

        template <size_t block_size> state<mode::CBC, block_size> inline init_CBC (const initialization_vector<block_size> &iv) {
            return state<mode::CBC, block_size> {iv};
        }

        template <size_t block_size> state<mode::OFB, block_size> inline init_OFB (const initialization_vector<block_size> &iv) {
            return state<mode::OFB, block_size> {iv};
        }

        template <size_t block_size> state<mode::CFB, block_size> inline init_CFB (const initialization_vector<block_size> &iv) {
            return state<mode::CBC, block_size> {iv};
        }

        template <size_t block_size, endian::order r> state<mode::CTR, block_size, r> inline init_CTR (const math::uint<r, block_size, byte> &iv) {
            return state<mode::CTR, block_size, r> {iv};
        }
    }

}

#endif
