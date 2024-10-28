// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_PKCS5_PBKDF2_HMAC
#define DATA_CRYPTO_PKCS5_PBKDF2_HMAC

#include <chrono>
#include <cryptopp/cryptlib.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#include <data/crypto/hash/hash.hpp>

namespace data::crypto {

    template <size_t size, typename hash_func>
    requires std::derived_from<hash, CryptoPP::HashTransformation>
    byte_array<size> PKCS5_PBKDF2_HMAC (bytes password, int iterations, const bytes salt = {}) {
        CryptoPP::PKCS5_PBKDF2_HMAC<hash> pbkdf;
        byte_array<size> key;
        pbkdf.DeriveKey (key.data (), size, false, password.data (), password.size (), salt.data (), salt.size (), iterations, 0.0f);
        return key;
    }

    template <size_t size, typename hash>
    requires std::derived_from<hash, CryptoPP::HashTransformation>
    byte_array<size> PKCS5_PBKDF2_HMAC (bytes password, std::chrono::duration<float> seconds, const bytes salt = {}) {
        CryptoPP::PKCS5_PBKDF2_HMAC<hash> pbkdf;
        byte_array<size> key;
        pbkdf.DeriveKey (key.data (), size, false, password.data (), password.size (), salt.data (), salt.size (), 0, seconds.count ());
        return key;
    }

}

#endif
