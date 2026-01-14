// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_BLOCK_CRYPTOPP
#define DATA_CRYPTO_BLOCK_CRYPTOPP

#include <data/crypto/block/cipher.hpp>
#include <cryptopp/modes.h>

#include <cryptopp/rijndael.h>
#include <cryptopp/serpent.h>
#include <cryptopp/twofish.h>
#include <cryptopp/rc6.h>
#include <cryptopp/mars.h>
#include <cryptopp/des.h>

namespace data::crypto::cipher::block::CryptoPP {
    using namespace ::CryptoPP;

    // should conatin a typedef named type that returns the cryptopp type.
    template <typename cipher> struct get_cipher_type;

    // should conatin a typedef named type that returns the cryptopp type.
    template <typename mode, typename cipher> struct get_block_mode_type;

    template <typename T, CryptoPP::CipherDir Dir>
    struct select_cipher_direction;

    template <typename T>
    struct select_cipher_direction<T, CryptoPP::ENCRYPTION> {
        using type = typename T::Encryption;
    };

    template <typename T>
    struct select_cipher_direction<T, CryptoPP::DECRYPTION> {
        using type = typename T::Decryption;
    };

}

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

    void inline DES::encrypt (DES::block_out out, const symmetric_key<8> &key, DES::block_in in) {
        auto k = key;
        CryptoPP::DES::CorrectKeyParityBits (k.data ());
        //if (!CryptoPP::DES::CheckKey (k)) throw exception {} << "DES with weak key";
        CryptoPP::DES::Encryption {k.data (), 8}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    void inline DES::decrypt (DES::block_out out, const symmetric_key<8> &key, DES::block_in in) {
        auto k = key;
        CryptoPP::DES::CorrectKeyParityBits (k.data ());
        //if (!CryptoPP::DES::CheckKey (k)) throw exception {} << "DES with weak key";
        CryptoPP::DES::Decryption {k.data (), 8}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    void inline TDEA2::encrypt (block_out out, const symmetric_key<16> &k, block_in in) {
        CryptoPP::DES_EDE2::Encryption {k.data (), 16}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    void inline TDEA2::decrypt (block_out out, const symmetric_key<16> &k, block_in in) {
        CryptoPP::DES_EDE2::Decryption {k.data (), 16}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    void inline TDEA3::encrypt (block_out out, const symmetric_key<24> &k, block_in in) {
        CryptoPP::DES_EDE3::Encryption {k.data (), 24}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }

    void inline TDEA3::decrypt (block_out out, const symmetric_key<24> &k, block_in in) {
        CryptoPP::DES_EDE3::Decryption {k.data (), 24}.ProcessBlock (in.data (), const_cast<byte *> (out.data ()));
    }
}

#endif
