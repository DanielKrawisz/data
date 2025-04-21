// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_CIPHER
#define DATA_CRYPTO_BLOCK_CIPHER

#include <data/crypto/encrypted.hpp>
#include <data/numbers.hpp>

namespace data::crypto {

    // a block cipher begins with a function and its invese which takes a key
    // and a block of fixed size and returns another fixed size block.
    template <typename cipher, size_t block_size, size_t key_size>
    concept block_cipher = requires (
        slice<byte, block_size> result,
        slice<const byte, block_size> block,
        const symmetric_key<key_size> &key) {
        { cipher::encrypt (result, key, block) };
        { cipher::decrypt (result, key, block) };
    };

    template <typename cipher, size_t block_size, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    byte_array<block_size> inline encrypt (const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        cipher::encrypt (x, key, block);
        return x;
    }

    template <typename cipher, size_t block_size, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    byte_array<block_size> inline decrypt (const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        cipher::decrypt (x, key, block);
        return x;
    }

    // Supported block ciphers.
    struct Rijndael;
    using AES = Rijndael;
    struct Serpent;
    struct Twofish;
    struct RC6;
    struct MARS;
    struct DES; // the original classic.
    struct TripleDES;

    struct Rijndael {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<const byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct Serpent {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct Twofish {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct RC6 {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct MARS {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 56 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 56 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct DES {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        // DES uses a 64 bit key but 8 bits are actually ignored.
        static symmetric_key<8> expand_key (const symmetric_key<7> &);
        static symmetric_key<7> contract_key (const symmetric_key<8> &);

        static void encrypt (block_out, const symmetric_key<7> &key, block_in);
        static void decrypt (block_out, const symmetric_key<7> &key, block_in);

        static void encrypt (block_out, const symmetric_key<8> &key, block_in);
        static void decrypt (block_out, const symmetric_key<8> &key, block_in);
    };

    struct TripleDES {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static symmetric_key<16> expand_key (const symmetric_key<14> &);
        static symmetric_key<14> contract_key (const symmetric_key<16> &);

        static symmetric_key<24> expand_key (const symmetric_key<21> &);
        static symmetric_key<21> contract_key (const symmetric_key<24> &);

        static void encrypt (block_out, const symmetric_key<14> &key, block_in);
        static void decrypt (block_out, const symmetric_key<14> &key, block_in);

        static void encrypt (block_out, const symmetric_key<16> &key, block_in);
        static void decrypt (block_out, const symmetric_key<16> &key, block_in);

        static void encrypt (block_out, const symmetric_key<21> &key, block_in);
        static void decrypt (block_out, const symmetric_key<21> &key, block_in);

        static void encrypt (block_out, const symmetric_key<24> &key, block_in);
        static void decrypt (block_out, const symmetric_key<24> &key, block_in);
    };

    void inline DES::encrypt (DES::block_out out, const symmetric_key<7> &key, DES::block_in in) {
        return encrypt (out, expand_key (key), in);
    }

    void inline DES::decrypt (DES::block_out out, const symmetric_key<7> &key, DES::block_in in) {
        return decrypt (out, expand_key (key), in);
    }

    void inline TripleDES::encrypt (TripleDES::block_out out, const symmetric_key<14> &key, TripleDES::block_in in) {
        return encrypt (out, expand_key (key), in);
    }

    void inline TripleDES::decrypt (TripleDES::block_out out, const symmetric_key<14> &key, TripleDES::block_in in) {
        return decrypt (out, expand_key (key), in);
    }

    void inline TripleDES::encrypt (TripleDES::block_out out, const symmetric_key<21> &key, TripleDES::block_in in) {
        return encrypt (out, expand_key (key), in);
    }

    void inline TripleDES::decrypt (TripleDES::block_out out, const symmetric_key<21> &key, TripleDES::block_in in) {
        return decrypt (out, expand_key (key), in);
    }

}

#endif

