
// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HMAC
#define DATA_CRYPTO_HMAC

#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>

#include <data/crypto/MAC/MAC.hpp>
#include <data/crypto/hash/hash.hpp>

namespace data::crypto::MAC {

    template <class Transform, size_t Size>
    requires std::derived_from<Transform, CryptoPP::HashTransformation>
    struct HMAC_writer : message_writer<digest<Size>, byte> {
        constexpr static size_t size = Size;

        CryptoPP::HMAC<Transform> HMAC;

        template <size_t key_size>
        HMAC_writer (const symmetric_key<key_size> k) : HMAC {k.data (), key_size} {}

        void write (const byte *b, size_t x) final override {
            HMAC.Update (b, x);
        }

        digest<size> complete () final override {
            digest<size> d;
            HMAC.Final (d.data ());
            HMAC.Restart ();
            return d;
        }

    };

}

#endif
