// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_MODE
#define DATA_CRYPTO_BLOCK_MODE

#include <data/crypto/block/cipher.hpp>

namespace data::crypto {

    // a block cipher mode takes a block cypher and defines an algorithm
    // that works on a series of blocks.
    template <typename mode, typename cipher, size_t block_size, size_t key_size>
    concept block_cipher_mode = block_cipher<cipher, block_size, key_size> && requires (
        mode m,
        slice<byte, block_size> result,
        slice<const byte, block_size> block,
        const symmetric_key<key_size> &key) {
        { m.template encrypt<cipher, key_size> (result, key, block) };
        { m.template decrypt<cipher, key_size> (result, key, block) };
    };

    // encrypt and decrypt using iterators.
    template <typename cipher, typename mode, size_t key_size, typename out, std::input_iterator in, typename sen,
        size_t block_size = decltype (*std::declval<in> ())::size>
    requires block_cipher_mode<mode, cipher, block_size, key_size> &&
        std::output_iterator<out, slice<byte, block_size>> &&
        std::sentinel_for<sen, in>
    void encrypt (mode &m, const symmetric_key<key_size> &key, out &o, in &i, const sen &x) {
        while (i != x) {
            m.template encrypt<cipher, key_size> (*o, key, *i);
            i++;
            o++;
        }
    }

    template <typename cipher, typename mode, size_t key_size, typename out, std::input_iterator in, typename sen,
        size_t block_size = decltype (*std::declval<in> ())::size>
    requires block_cipher_mode<mode, cipher, block_size, key_size> &&
        std::output_iterator<out, slice<byte, block_size>> &&
        std::sentinel_for<sen, in>
    void decrypt (mode &m, const symmetric_key<key_size> &key, out &o, in &i, const sen &x) {
        while (i != x) {
            m.template decrypt<cipher, key_size> (*o, key, *i);
            i++;
            o++;
        }
    }

    // encrypt and decrypt single blocks.
    template <typename cipher, typename mode, size_t key_size, size_t block_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size>
    byte_array<block_size> inline encrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        m.template encrypt<cipher, key_size> (x, key, block);
        return x;
    }

    template <typename cipher, typename mode, size_t key_size, size_t block_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size>
    byte_array<block_size> inline decrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        m.template decrypt<cipher, key_size> (x, key, block);
        return x;
    }

    // encrypt and decrypt a whole message
    template <typename cipher, typename mode, size_t key_size, size_t block_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size>
    cross<byte_array<block_size>> encrypt (mode &m, const cross<byte_array<block_size>> &message, const symmetric_key<key_size> &k) {
        cross<byte_array<block_size>> result;
        result.resize (message.size ());
        encrypt (m, k, result.begin (), message.begin (), message.end ());
        return result;
    }

    template <typename cipher, typename mode, size_t key_size, size_t block_size>
    requires block_cipher_mode<mode, cipher, block_size, key_size>
    cross<byte_array<block_size>> decrypt (mode &m, const cross<byte_array<block_size>> &encrypted, const symmetric_key<key_size> &k) {
        cross<byte_array<block_size>> result;
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
    template <size_t block_size, endian::order r> using CTR = mode_output_feedback<block_size>;

    // The remaining modes are also authenticated.
    template <size_t block_size> struct mode_Galois_counter;
    template <size_t block_size> using GCM = mode_Galois_counter<block_size>;

    // Not an acronym for anything.
    template <size_t block_size> struct EAX;

    // Patented but free for non-commerical use.
    template <size_t block_size> struct mode_offset_codebook;
    template <size_t block_size> using OCB = mode_offset_codebook<block_size>;


    template <size_t block_size> struct mode_electronic_code_book {
        mode_electronic_code_book ();

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    template <size_t block_size> struct mode_cipher_block_chain {
        initialization_vector<block_size> IV;
        mode_cipher_block_chain (const initialization_vector<block_size> &);

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    template <size_t block_size> struct mode_output_feedback {
        initialization_vector<block_size> IV;
        mode_output_feedback (const initialization_vector<block_size> &iv);

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void decrypt (block_out, const symmetric_key<key_size> &key, block_in);

        // both operations are the same.
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void crypt (block_out, const symmetric_key<key_size> &key, block_in);

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        static byte_array<block_size> update (const symmetric_key<key_size> &key, const byte_array<block_size> &);
    };

    template <size_t block_size> struct mode_cipher_feedback {
        initialization_vector<block_size> IV;
        mode_cipher_feedback (const initialization_vector<block_size> &iv);

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    // since there is no rule that says what endian
    // the counter should be, we allow both.
    template <size_t block_size, endian::order r>
    struct mode_counter {
        math::uint<r, block_size, byte> IV;
        mode_counter (const math::uint<r, block_size, byte> &iv);

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void encrypt (block_out, block_in, const symmetric_key<key_size> &key);
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void decrypt (block_out, block_in, const symmetric_key<key_size> &key);
    };

    template <size_t block_size>
    inline mode_electronic_code_book<block_size>::mode_electronic_code_book () {}

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_electronic_code_book<block_size>::encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        cipher::encrypt (o, key, i);
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_electronic_code_book<block_size>::decrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        cipher::decrypt (o, key, i);
    }

    template <size_t block_size>
    inline mode_cipher_block_chain<block_size>::mode_cipher_block_chain (const initialization_vector<block_size> &iv) : IV {iv} {}

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_cipher_block_chain<block_size>::encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        cipher::encrypt (o, key, IV ^ i);
        IV = o;
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_cipher_block_chain<block_size>::decrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        cipher::decrypt (o, key, i);
        o ^= IV;
        IV = i;
    }

    template <size_t block_size>
    inline mode_output_feedback<block_size>::mode_output_feedback (const initialization_vector<block_size> &iv) : IV {iv} {}

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_output_feedback<block_size>::encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        crypt (o, key, i);
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_output_feedback<block_size>::decrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        crypt (o, key, i);
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_output_feedback<block_size>::crypt (block_out out, const symmetric_key<key_size> &key, block_in in) {
        IV = update (key, IV);
        out = in ^ IV;
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    byte_array<block_size> inline mode_output_feedback<block_size>::update (const symmetric_key<key_size> &key, const byte_array<block_size> &block) {
        return crypto::encrypt<cipher, block_size, key_size> (key, block);
    }

    template <size_t block_size>
    inline mode_cipher_feedback<block_size>::mode_cipher_feedback (const initialization_vector<block_size> &iv) : IV {iv} {}
/*
    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_cipher_feedback<block_size>::encrypt (block_out, block_in, const symmetric_key<key_size> &key);

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_cipher_feedback<block_size>::decrypt (block_out, block_in, const symmetric_key<key_size> &key);
*/
    template <size_t block_size, endian::order r>
    inline mode_counter<block_size, r>::mode_counter (const math::uint<r, block_size, byte> &iv) : IV {iv} {}
/*
    template <size_t block_size, endian::order r>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_counter<block_size, r>::encrypt (block_out, block_in, const symmetric_key<key_size> &key);

    template <size_t block_size, endian::order r>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_counter<block_size, r>::decrypt (block_out, block_in, const symmetric_key<key_size> &key);*/

    // it seems like there is no reason that the const_cast should be necessary in the following functions.
    // block_out is supposed to contain a non-const pointer.
}

#endif
