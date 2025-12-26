// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_MODE
#define DATA_CRYPTO_BLOCK_MODE

#include <data/crypto/cipher.hpp>
#include <data/crypto/block/cipher.hpp>
#include <data/arithmetic/complementary.hpp>
#include <data/io/log.hpp>

namespace data::crypto::cipher::block {

    // a block cipher mode takes a block cypher and defines an algorithm
    // that works on a series of blocks.

    template <typename mode, typename cipher, size_t key_size>
    concept StandardMode = Cipher<cipher, mode::BlockSize, key_size> &&
        requires (
            mode m,
            slice<byte, mode::BlockSize> result,
            slice<const byte, mode::BlockSize> block,
            const symmetric_key<key_size> &key
        ) {
            { m.template encrypt<key_size, cipher> (result, key, block) };
            { m.template decrypt<key_size, cipher> (result, key, block) };
        };

    template <typename mode, typename cipher, size_t key_size>
    concept AsymmetricStreamableMode = Cipher<cipher, mode::BlockSize, key_size> &&
        requires (mode m, const symmetric_key<key_size> &key) {
            { m.template initialize<key_size, cipher> (key) };
        } && requires (mode m, byte b) {
            { m.encrypt (b) } -> Same<byte>;
            { m.decrypt (b) } -> Same<byte>;
        };

    template <typename mode, typename cipher, size_t key_size>
    concept SymmetricStreamableMode = Cipher<cipher, mode::BlockSize, key_size> &&
        requires (mode m, const symmetric_key<key_size> &key) {
            { m.template initialize<key_size, cipher> (key) };
        } && requires (mode m, byte b) {
            { m.crypt (b) } -> Same<byte>;
        };

    template <typename mode, typename cipher, size_t key_size>
    concept StreamableMode =
        AsymmetricStreamableMode<mode, cipher, key_size> ||
        SymmetricStreamableMode<mode, cipher, key_size>;

    template <typename mode, typename cipher, size_t key_size>
    concept Mode =
        StreamableMode<mode, cipher, key_size> ||
        StandardMode<mode, cipher, key_size>;

    // encrypt and decrypt whole blocks.
    template <typename cipher, size_t key_size, StandardMode<cipher, key_size> mode>
    byte_array<mode::BlockSize> inline encrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, mode::BlockSize> block) {
        byte_array<mode::BlockSize> x;
        m.template encrypt<key_size, cipher> (x, key, block);
        return x;
    }

    template <typename cipher, size_t key_size, StandardMode<cipher, key_size> mode>
    byte_array<mode::BlockSize> inline decrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, mode::BlockSize> block) {
        byte_array<mode::BlockSize> x;
        m.template decrypt<key_size, cipher> (x, key, block);
        return x;
    }

    // encrypt and decrypt single blocks for streamable modes.
    template <typename cipher, size_t key_size,
        AsymmetricStreamableMode<cipher, key_size> mode>
    byte_array<mode::BlockSize> inline encrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, mode::BlockSize> block) {
        byte_array<mode::BlockSize> ciphertext;
        m.template initialize<key_size, cipher> (key);
        for (size_t i = 0; i < mode::BlockSize; i++) ciphertext[i] = m.encrypt (block[i]);
        return ciphertext;
    }

    template <typename cipher, size_t key_size, AsymmetricStreamableMode<cipher, key_size> mode>
    byte_array<mode::BlockSize> inline decrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, mode::BlockSize> block) {
        byte_array<mode::BlockSize> plaintext;
        m.template initialize<key_size, cipher> (key);
        for (size_t i = 0; i < mode::BlockSize; i++) plaintext[i] = m.decrypt (block[i]);
        return plaintext;
    }

    // encrypt and decrypt single blocks for streamable modes.
    template <typename cipher, size_t key_size, SymmetricStreamableMode<cipher, key_size> mode>
    byte_array<mode::BlockSize> inline encrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, mode::BlockSize> block) {
        byte_array<mode::BlockSize> result;
        m.template initialize<key_size, cipher> (key);
        for (size_t i = 0; i < mode::BlockSize; i++) result[i] = m.crypt (block[i]);
        return result;
    }

    template <typename cipher, size_t key_size,
        SymmetricStreamableMode<cipher, key_size> mode>
    byte_array<mode::BlockSize> inline decrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, mode::BlockSize> block) {
        return encrypt<cipher> (m, key, block);
    }

    // encrypt and decrypt using iterators.
    template <typename cipher, typename mode, size_t key_size, typename out, std::input_iterator in, typename sen>
    requires Mode<mode, cipher, key_size> &&
        std::output_iterator<out, slice<byte, mode::BlockSize>> &&
        std::sentinel_for<sen, in>
    void encrypt (mode &m, const symmetric_key<key_size> &key, out &o, in &i, const sen &x) {
        while (i != x) {
            *o = encrypt (m, key, *i);
            i++;
            o++;
        }
    }

    template <typename cipher, typename mode, size_t key_size, typename out, std::input_iterator in, typename sen>
    requires Mode<mode, cipher, key_size> &&
        std::output_iterator<out, slice<byte, mode::BlockSize>> &&
        std::sentinel_for<sen, in>
    void decrypt (mode &m, const symmetric_key<key_size> &key, out &o, in &i, const sen &x) {
        while (i != x) {
            *o = decrypt (m, key, *i);
            i++;
            o++;
        }
    }

    // encrypt and decrypt a whole message
    template <typename cipher, size_t key_size, Mode<cipher, key_size> mode>
    cross<byte_array<mode::BlockSize>> encrypt (mode &m, const cross<byte_array<mode::BlockSize>> &message, const symmetric_key<key_size> &k) {
        cross<byte_array<mode::BlockSize>> result;
        result.resize (message.size ());
        encrypt (m, k, result.begin (), message.begin (), message.end ());
        return result;
    }

    template <typename cipher, size_t key_size, Mode<cipher, key_size> mode>
    cross<byte_array<mode::BlockSize>> decrypt (mode &m, const cross<byte_array<mode::BlockSize>> &encrypted, const symmetric_key<key_size> &k) {
        cross<byte_array<mode::BlockSize>> result;
        result.resize (encrypted.size ());
        decrypt (m, k, result.begin (), encrypted.begin (), encrypted.end ());
        return result;
    }

    // Standard block cipher modes.

    // Modes that require padding.
    template <size_t block_size> struct mode_electronic_code_book;
    template <size_t block_size> using ECB = mode_electronic_code_book<block_size>;
    template <size_t block_size> struct mode_cipher_block_chain;
    template <size_t block_size> using CBC = mode_cipher_block_chain<block_size>;

    // The remaining modes can act like stream ciphers.
    template <size_t block_size> struct mode_cipher_feedback;
    template <size_t block_size> using CFB = mode_cipher_feedback<block_size>;
    template <size_t block_size> struct mode_output_feedback;
    template <size_t block_size> using OFB = mode_output_feedback<block_size>;

    // The standard doesn't say whether the counter is big or little endian, so we support both.
    template <size_t block_size, endian::order r> struct mode_counter;
    template <size_t block_size, endian::order r> using CTR = mode_counter<block_size, r>;

    // The remaining modes are also authenticated.
    template <size_t block_size> struct mode_Galois_counter;
    template <size_t block_size> using GCM = mode_Galois_counter<block_size>;

    // Not an acronym for anything.
    template <size_t block_size> struct EAX;

    // Patented but free for non-commerical use.
    template <size_t block_size> struct mode_offset_codebook;
    template <size_t block_size> using OCB = mode_offset_codebook<block_size>;

    template <size_t block_size> struct mode_electronic_code_book {
        mode_electronic_code_book () {}
        constexpr const static size_t BlockSize = block_size;

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <size_t key_size, Cipher<block_size, key_size> cipher>
        void encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
            cipher::encrypt (o, key, i);
        }

        template <size_t key_size, Cipher<block_size, key_size> cipher>
        void decrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
            cipher::decrypt (o, key, i);
        }
    };

    template <size_t block_size> struct mode_cipher_block_chain {
        constexpr const static size_t BlockSize = block_size;
        initialization_vector<block_size> IV;
        mode_cipher_block_chain (const initialization_vector<block_size> &iv): IV {iv} {}

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <size_t key_size, Cipher<block_size, key_size> cipher>
        void encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
            byte_array<block_size> xored;
            arithmetic::bit_xor<byte> (xored.end (), xored.begin (), i.data (), (const byte *) (IV.data ()));
            cipher::encrypt (o, key, xored);
            std::copy (o.begin (), o.end (), IV.begin ());
        }

        template <size_t key_size, Cipher<block_size, key_size> cipher>
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

        template <size_t key_size, Cipher<block_size, key_size> cipher>
        void initialize (const symmetric_key<key_size> &key) {
            cipher::encrypt (IV, key, IV);
            Index = 0;
        }

        streamable_base (const initialization_vector<block_size> &iv): IV {iv} {}
    };

    template <size_t block_size> struct mode_cipher_feedback : streamable_base<block_size> {
        constexpr const static size_t BlockSize = block_size;
        mode_cipher_feedback (const initialization_vector<block_size> &iv): streamable_base<block_size> {iv} {}

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

    template <size_t block_size> struct mode_output_feedback : streamable_base<block_size> {

        constexpr const static size_t BlockSize = block_size;
        mode_output_feedback (const initialization_vector<block_size> &iv): streamable_base<block_size> {iv} {}

        // both operations are the same.
        byte crypt (byte b) {
            byte result = this->IV[this->Index] ^ b;
            this->Index++;
            return result;
        }
    };

    // since there is no rule that says what endian
    // the counter should be, we allow both.
    template <size_t block_size, endian::order r>
    struct mode_counter : streamable_base<block_size> {
        constexpr const static size_t BlockSize = block_size;

        math::uint<r, block_size, byte> Count;

        mode_counter (const math::uint<r, block_size, byte> &iv) : streamable_base<block_size> {iv}, Count {iv} {}

        // both operations are the same.
        byte crypt (byte b) {
            byte result = this->IV[this->Index] ^ b;
            this->Index++;
            if (this->Index == BlockSize) {
                Count++;
                this->IV = Count;
            }
            return result;
        }
    };

    template <size_t block_size, endian::order r> mode_counter (const math::uint<r, block_size, byte> &iv) -> mode_counter<block_size, r>;

}

#endif
