// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_CRYPTOPP
#define DATA_CRYPTO_BLOCK_CRYPTOPP

#include <data/crypto/block/writer.hpp>
#include <cryptopp/modes.h>

#include <cryptopp/rijndael.h>
#include <cryptopp/serpent.h>
#include <cryptopp/twofish.h>
#include <cryptopp/rc6.h>
#include <cryptopp/mars.h>
#include <cryptopp/des.h>

namespace data::crypto::CryptoPP {
    template <size_t block_size, size_t key_size>
    struct get_cipher_type<crypto::Rijndael, block_size, key_size> {
        using value = Rijndael;
    };

    template <size_t block_size, size_t key_size>
    struct get_cipher_type<crypto::Serpent, block_size, key_size> {
        using value = Serpent;
    };

    template <size_t block_size, size_t key_size>
    struct get_cipher_type<crypto::Twofish, block_size, key_size> {
        using value = Twofish;
    };

    template <size_t block_size, size_t key_size>
    struct get_cipher_type<crypto::RC6, block_size, key_size> {
        using value = RC6;
    };

    template <size_t block_size, size_t key_size>
    struct get_cipher_type<crypto::MARS, block_size, key_size> {
        using value = MARS;
    };

    template <typename cipher, size_t block_size, size_t key_size>
    struct get_mode_type<crypto::ECB<block_size>, cipher, block_size, key_size> {
        using value = ECB_Mode<typename get_cipher_type<cipher, block_size, key_size>::value>;
    };

    template <typename cipher, size_t block_size, size_t key_size>
    struct get_mode_type<crypto::CBC<block_size>, cipher, block_size, key_size> {
        using value = CBC_Mode<typename get_cipher_type<cipher, block_size, key_size>::value>;
    };

    template <typename cipher, size_t block_size, size_t key_size>
    struct get_mode_type<crypto::CFB<block_size>, cipher, block_size, key_size> {
        using value = CFB_Mode<typename get_cipher_type<cipher, block_size, key_size>::value>;
    };

    template <typename cipher, size_t block_size, size_t key_size>
    struct get_mode_type<crypto::OFB<block_size>, cipher, block_size, key_size> {
        using value = OFB_Mode<typename get_cipher_type<cipher, block_size, key_size>::value>;
    };

}

namespace data::crypto {

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
