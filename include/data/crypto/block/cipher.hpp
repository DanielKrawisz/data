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
    struct TDEA2; // triple DES with 2 keys
    struct TDEA3; // triple DES with 3 keys

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

        static void encrypt (block_out, const symmetric_key<7> &key, block_in);
        static void decrypt (block_out, const symmetric_key<7> &key, block_in);
    };

    struct TDEA2 {
        static constexpr security Security = security::depricated;
        static constexpr size_t BlockSize = 8;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static void encrypt (block_out, const symmetric_key<16> &key, block_in);
        static void decrypt (block_out, const symmetric_key<16> &key, block_in);

        static void encrypt (block_out, const symmetric_key<14> &key, block_in);
        static void decrypt (block_out, const symmetric_key<14> &key, block_in);
    };

    struct TDEA3 {
        static constexpr security Security = security::depricated;
        static constexpr size_t BlockSize = 8;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static void encrypt (block_out, const symmetric_key<24> &key, block_in);
        static void decrypt (block_out, const symmetric_key<24> &key, block_in);

        static void encrypt (block_out, const symmetric_key<21> &key, block_in);
        static void decrypt (block_out, const symmetric_key<21> &key, block_in);
    };

    inline void DES_set_parity (byte &b) {
        // count bits excluding LSB
        int ones = __builtin_popcount (b & 0xFE);
        if ((ones & 1) == 0) b |= 0x01;
        else b &= 0xFE;
    }

    template <size_t key_size>
    symmetric_key<(key_size * 8) / 7> DES_expand_key (const symmetric_key<key_size> &k) {
        symmetric_key<key_size> old_key = k;
        symmetric_key<(key_size * 8) / 7> new_key;
        for (int i = 0; i < (key_size * 8) / 7; i++) {
            new_key[i] = old_key[0] & ~1;
            DES_set_parity (new_key[i]);
            old_key <<= 7;
        }
        return new_key;
    }


    void inline DES::encrypt (DES::block_out o, const symmetric_key<7> &key, DES::block_in i) {
        encrypt (o, DES_expand_key (key), i);
    }

    void inline DES::decrypt (DES::block_out o, const symmetric_key<7> &key, DES::block_in i) {
        decrypt (o, DES_expand_key (key), i);
    }

    void inline TDEA2::encrypt (TDEA2::block_out o, const symmetric_key<14> &key, TDEA2::block_in i) {
        encrypt (o, DES_expand_key (key), i);
    }

    void inline TDEA2::decrypt (TDEA2::block_out o, const symmetric_key<14> &key, TDEA2::block_in i) {
        decrypt (o, DES_expand_key (key), i);
    }

    void inline TDEA3::encrypt (TDEA3::block_out o, const symmetric_key<21> &key, TDEA3::block_in i) {
        encrypt (o, DES_expand_key (key), i);
    }

    void inline TDEA3::decrypt (TDEA3::block_out o, const symmetric_key<21> &key, TDEA3::block_in i) {
        decrypt (o, DES_expand_key (key), i);
    }

}

#endif

