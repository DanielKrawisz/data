// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_SHA512_H
#define BITCOIN_CRYPTO_SHA512_H

#include <cstdint>
#include <cstdlib>
#include "data/crypto/one_way.hpp"

/** A hasher class for SHA-512. */
// satisfies data::hash::Engine
class CSHA512 {
private:
    uint64_t s[8];
    data::byte buf[128];
    uint64_t bytes;

public:
    constexpr static const size_t DigestSize = 64;
    constexpr static const size_t BlockSize = 128;
    constexpr static const data::crypto::security Security = data::crypto::security::modern;

    CSHA512 ();
    CSHA512 &Update (const data::byte *data, size_t len);
    void Final (data::byte hash[DigestSize]);
    CSHA512 &Restart ();
};

#endif // BITCOIN_CRYPTO_SHA512_H
