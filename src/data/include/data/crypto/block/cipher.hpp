// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_CIPHER
#define DATA_CRYPTO_BLOCK_CIPHER

#include <data/crypto/one_way.hpp>
#include <data/crypto/encrypted.hpp>
#include <data/bytes.hpp>

namespace data::crypto::cipher::block {

    // a block cipher begins with a function and its invese which takes a key
    // and a block of fixed size and returns another fixed size block.
    template <typename cipher, size_t key_size>
    concept Cipher = requires (
        slice<byte, cipher::BlockSize> result,
        slice<const byte, cipher::BlockSize> block,
        const symmetric_key<key_size> &key) {
        { cipher::encrypt (result, key, block) };
        { cipher::decrypt (result, key, block) };
    };

    template <size_t key_size, Cipher<key_size> cipher>
    byte_array<cipher::BlockSize> inline encrypt (const symmetric_key<key_size> &key, slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> x;
        cipher::encrypt (x, key, block);
        return x;
    }

    template <size_t key_size, Cipher<key_size> cipher>
    byte_array<cipher::BlockSize> inline decrypt (const symmetric_key<key_size> &key, slice<const byte, cipher::BlockSize> block) {
        byte_array<cipher::BlockSize> x;
        cipher::decrypt (x, key, block);
        return x;
    }

    // Supported block ciphers.
    struct Rijndael;
    using AES = Rijndael;

    // other AES candidates
    struct Serpent;
    struct Twofish;
    struct RC6;
    struct MARS;

    struct DES; // the original classic.
    struct TDA2; // triple DES with 2 keys
    struct TDA3; // triple DES with 3 keys

    struct Rijndael {
        static constexpr security Security = security::modern;
        constexpr static size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<const byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct Serpent {
        static constexpr security Security = security::modern;
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct Twofish {
        static constexpr security Security = security::modern;
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct RC6 {
        static constexpr security Security = security::modern;
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct MARS {
        static constexpr security Security = security::modern;
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        template <size_t key_size> requires (key_size >= 16 && key_size <= 56 && key_size % 8 == 0)
        static void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <size_t key_size> requires (key_size >= 16 && key_size <= 56 && key_size % 8 == 0)
        static void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    struct DES {
        static constexpr security Security = security::depricated;
        static constexpr size_t BlockSize = 8;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static void encrypt (block_out, const symmetric_key<8> &key, block_in);
        static void decrypt (block_out, const symmetric_key<8> &key, block_in);
    };

    struct TDA2 {
        static constexpr security Security = security::depricated;
        static constexpr size_t BlockSize = 8;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static void encrypt (block_out, const symmetric_key<16> &key, block_in);
        static void decrypt (block_out, const symmetric_key<16> &key, block_in);
    };

    struct TDA3 {
        static constexpr security Security = security::depricated;
        static constexpr size_t BlockSize = 8;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static void encrypt (block_out, const symmetric_key<24> &key, block_in);
        static void decrypt (block_out, const symmetric_key<24> &key, block_in);
    };

}

#endif

