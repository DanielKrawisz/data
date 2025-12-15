
// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HMAC
#define DATA_CRYPTO_HMAC

#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>

#include <data/crypto/hash.hpp>

namespace data::crypto::MAC {

    template <class Transform, size_t size>
    requires std::derived_from<Transform, CryptoPP::HashTransformation>
    struct HMAC_writer : writer<byte> {
        constexpr static size_t Size = size;
        using digest = hash::digest<Size>;

        HMAC_writer (digest &d, byte_slice k) : Digest {d}, HMAC {k.data (), k.size ()} {}

        void write (const byte *b, size_t x) final override {
            HMAC.Update (b, x);
        }

        ~HMAC_writer () {
            HMAC.Final (Digest.data ());
        }

    private:
        digest &Digest;
        CryptoPP::HMAC<Transform> HMAC;
    };

}

#endif
