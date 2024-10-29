// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_CYPHER
#define DATA_CRYPTO_BLOCK_CYPHER

#include <cryptopp/rijndael.h>
#include <cryptopp/serpent.h>
#include <cryptopp/twofish.h>
#include <cryptopp/rc6.h>
#include <cryptopp/mars.h>

#include <data/numbers.hpp>
#include <data/crypto/encrypted.hpp>
#include "writer.hpp"

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

    template <size_t block_size, size_t key_size, block_cipher<block_size, key_size> cipher>
    byte_array<block_size> inline encrypt (const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        cipher::encrypt (x, key, block);
        return x;
    }

    template <size_t block_size, size_t key_size, block_cipher<block_size, key_size> cipher>
    byte_array<block_size> inline decrypt (const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        cipher::decrypt (x, key, block);
        return x;
    }

    // a block cipher mode takes a cipher to produce
    // a procedure that can be replied repeatedly to
    // encrypt or decrypt a whole message.
    template <typename mode, typename cipher, size_t block_size, size_t key_size>
    concept block_cipher_mode = block_cipher<cipher, block_size, key_size> && requires (
        mode m,
        slice<byte, block_size> result,
        slice<const byte, block_size> block,
        const symmetric_key<key_size> &key) {
        { m.template encrypt<cipher, key_size> (result, key, block) };
        { m.template decrypt<cipher, key_size> (result, key, block) };
    };

    template <typename cipher, size_t block_size, size_t key_size, block_cipher_mode<cipher, block_size, key_size> mode>
    byte_array<block_size> inline encrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        m.template encrypt<cipher, key_size> (x, key, block);
        return x;
    }

    template <typename cipher, size_t block_size, size_t key_size, block_cipher_mode<cipher, block_size, key_size> mode>
    byte_array<block_size> inline decrypt (mode &m, const symmetric_key<key_size> &key, slice<const byte, block_size> block) {
        byte_array<block_size> x;
        m.template decrypt<cipher, key_size> (x, key, block);
        return x;
    }

    // standard block cipher modes.
    template <size_t block_size> struct mode_electronic_code_book;
    template <size_t block_size> using ECB = mode_electronic_code_book<block_size>;
    template <size_t block_size> struct mode_cipher_block_chain;
    template <size_t block_size> using CBC = mode_cipher_block_chain<block_size>;
    template <size_t block_size> struct mode_cipher_feedback;
    template <size_t block_size> using CFB = mode_cipher_feedback<block_size>;
    template <size_t block_size> struct mode_output_feedback;
    template <size_t block_size> using OFB = mode_output_feedback<block_size>;
    template <size_t block_size, endian::order r> struct mode_counter;
    template <size_t block_size, endian::order r> using CTR = mode_output_feedback<block_size>;

    // supported block ciphers.
    struct Rijndael;
    using AES = Rijndael;
    struct Serpent;
    struct Twofish;
    struct RC6;
    struct MARS;
    struct DES; // the original classic.
    struct TrippleDES;

    // Given a block writer, we can turn any block cipher mode into a block writer to encrypt messages.
    template <typename cipher, size_t block_size, size_t key_size,
        block_cipher_mode<cipher, block_size, key_size> mode,
        block_writer<block_size> W>
    struct block_encryptor {
        W &Encrypted;
        mode Mode;
        symmetric_key<key_size> Key;
        void write (slice<const byte, block_size> block) {
            Encrypted.write (encrypt<cipher> (Mode, Key, block));
        }
    };

    // Given a block writer, we can turn any block cipher mode into a writer to block writer to decrypt messages.
    template <typename cipher, size_t block_size, size_t key_size,
        block_cipher_mode<cipher, block_size, key_size> mode,
        block_writer<block_size> W>
    struct block_decryptor {
        W &Decrypted;
        mode Mode;
        symmetric_key<key_size> Key;
        void write (slice<const byte, block_size> block) {
            Decrypted.write (decrypt<cipher> (Mode, Key, block));
        }
    };

    // encrypt and decrypt a whole message
    template <typename cipher, size_t block_size, size_t key_size, block_cipher_mode<cipher, block_size, key_size> mode>
    cross<byte_array<block_size>> encrypt (mode, const cross<byte_array<block_size>> &message, const symmetric_key<key_size>);

    template <typename cipher, size_t block_size, size_t key_size, block_cipher_mode<cipher, block_size, key_size> mode>
    bytes decrypt (mode, const cross<byte_array<block_size>> &encrypted, const symmetric_key<key_size>);

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

        static void encrypt (block_out, symmetric_key<7> &key, block_in);
        static void decrypt (block_out, symmetric_key<7> &key, block_in);
    };

    struct TrippleDES {
        static constexpr size_t BlockSize = 16;
        using block_in = slice<const byte, BlockSize>;
        using block_out = slice<byte, BlockSize>;

        static void encrypt (block_out, symmetric_key<14> &key, block_in);
        static void decrypt (block_out, symmetric_key<14> &key, block_in);
    };

    // there are 6 standard block cipher modes.
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
        byte_array<block_size> Last;
        mode_cipher_block_chain (const initialization_vector<block_size> &);

        using block_in = slice<const byte, block_size>;
        using block_out = slice<byte, block_size>;

        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void encrypt (block_out, const symmetric_key<key_size> &key, block_in);
        template <typename cipher, size_t key_size> requires block_cipher<cipher, block_size, key_size>
        void decrypt (block_out, const symmetric_key<key_size> &key, block_in);
    };

    template <size_t block_size> struct mode_output_feedback {
        byte_array<block_size> Last;
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
        byte_array<block_size> Last;
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
        math::uint<r, block_size, byte> Last;
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
    inline mode_cipher_block_chain<block_size>::mode_cipher_block_chain (const initialization_vector<block_size> &iv) : Last {iv} {}

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_cipher_block_chain<block_size>::encrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        cipher::encrypt (o, key, Last ^ i);
        Last = o;
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    void inline mode_cipher_block_chain<block_size>::decrypt (block_out o, const symmetric_key<key_size> &key, block_in i) {
        cipher::decrypt (o, key, i);
        o ^= Last;
        Last = i;
    }

    template <size_t block_size>
    inline mode_output_feedback<block_size>::mode_output_feedback (const initialization_vector<block_size> &iv) : Last {iv} {}

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
        Last = update (key, Last);
        out = in ^ Last;
    }

    template <size_t block_size>
    template <typename cipher, size_t key_size>
    requires block_cipher<cipher, block_size, key_size>
    byte_array<block_size> inline mode_output_feedback<block_size>::update (const symmetric_key<key_size> &key, const byte_array<block_size> &block) {
        return crypto::encrypt<block_size, key_size, cipher> (key, block);
    }

    template <size_t block_size>
    inline mode_cipher_feedback<block_size>::mode_cipher_feedback (const initialization_vector<block_size> &iv) : Last {iv} {}
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
    inline mode_counter<block_size, r>::mode_counter (const math::uint<r, block_size, byte> &iv) : Last {iv} {}
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

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline Rijndael::encrypt (Rijndael::block_out out, const symmetric_key<key_size> &key, Rijndael::block_in in) {
        CryptoPP::Rijndael::Encryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline Rijndael::decrypt (Rijndael::block_out out, const symmetric_key<key_size> &key, Rijndael::block_in in) {
        CryptoPP::Rijndael::Decryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline Serpent::encrypt (Serpent::block_out out, const symmetric_key<key_size> &key, Serpent::block_in in) {
        CryptoPP::Serpent::Encryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline Serpent::decrypt (Serpent::block_out out, const symmetric_key<key_size> &key, Serpent::block_in in) {
        CryptoPP::Serpent::Decryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline Twofish::encrypt (Twofish::block_out out, const symmetric_key<key_size> &key, Twofish::block_in in) {
        CryptoPP::Twofish::Encryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline Twofish::decrypt (Twofish::block_out out, const symmetric_key<key_size> &key, Twofish::block_in in) {
        CryptoPP::Twofish::Decryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline RC6::encrypt (RC6::block_out out, const symmetric_key<key_size> &key, RC6::block_in in) {
        CryptoPP::RC6::Encryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 32 && key_size % 8 == 0)
    void inline RC6::decrypt (RC6::block_out out, const symmetric_key<key_size> &key, RC6::block_in in) {
        CryptoPP::RC6::Decryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 56 && key_size % 8 == 0)
    void inline MARS::encrypt (MARS::block_out out, const symmetric_key<key_size> &key, MARS::block_in in) {
        CryptoPP::MARS::Encryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    template <size_t key_size> requires (key_size >= 16 && key_size <= 56 && key_size % 8 == 0)
    void inline MARS::decrypt (MARS::block_out out, const symmetric_key<key_size> &key, MARS::block_in in) {
        CryptoPP::MARS::Decryption {key.data (), key_size}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }
}

#endif

