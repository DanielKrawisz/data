// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_HMAC_SHA512_H
#define BITCOIN_CRYPTO_HMAC_SHA512_H

#include <sv/crypto/sha512.h>

#include <cstdint>
#include <cstdlib>
#include <data/crypto/one_way.hpp>

/** A hasher class for HMAC-SHA-512. */
class CHMAC_SHA512 {
private:
    CSHA512 outer;
    CSHA512 inner;

public:
    static const size_t Size = 64;

    CHMAC_SHA512 (const data::byte *key, size_t keylen);

    CHMAC_SHA512 &Update (const data::byte *data, size_t len) {
        inner.Update (data, len);
        return *this;
    }

    void Final (data::byte hash[Size]);
};

#endif // BITCOIN_CRYPTO_HMAC_SHA512_H
