// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_CRYPTOPP
#define DATA_CRYPTO_BLOCK_CRYPTOPP

#include <data/crypto/block/cipher.hpp>
#include <data/crypto/block/writer.hpp>
#include <cryptopp/modes.h>

#include <cryptopp/rijndael.h>
#include <cryptopp/serpent.h>
#include <cryptopp/twofish.h>
#include <cryptopp/rc6.h>
#include <cryptopp/mars.h>
#include <cryptopp/des.h>

namespace data::crypto::cipher::block::CryptoPP {
    template <>
    struct get_cipher_type<block::Rijndael> {
        using type = CryptoPP::Rijndael;
    };

    template <>
    struct get_cipher_type<block::Serpent> {
        using type = CryptoPP::Serpent;
    };

    template <>
    struct get_cipher_type<block::Twofish> {
        using type = CryptoPP::Twofish;
    };

    template <>
    struct get_cipher_type<block::RC6> {
        using type = CryptoPP::RC6;
    };

    template <>
    struct get_cipher_type<block::MARS> {
        using type = CryptoPP::MARS;
    };

    template <size_t block_size, typename cipher>
    struct get_block_mode_type<block::ECB<block_size>, cipher> {
        using type = ECB_Mode<typename get_cipher_type<cipher>::type>;
    };

    template <size_t block_size, typename cipher>
    struct get_block_mode_type<block::CBC<block_size>, cipher> {
        using type = CBC_Mode<typename get_cipher_type<cipher>::type>;
    };

    template <size_t block_size, typename cipher>
    struct get_block_mode_type<block::CFB<block_size>, cipher> {
        using type = CFB_Mode<typename get_cipher_type<cipher>::type>;
    };

    template <size_t block_size, typename cipher>
    struct get_block_mode_type<block::OFB<block_size>, cipher> {
        using type = OFB_Mode<typename get_cipher_type<cipher>::type>;
    };

    template <size_t block_size, typename cipher>
    struct get_block_mode_type<block::CTR<block_size, endian::big>, cipher> {
        using type = CTR_Mode<typename get_cipher_type<cipher>::type>;
    };

}

namespace data::crypto::cipher::block {

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
