// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_RIPEMD160_H
#define BITCOIN_CRYPTO_RIPEMD160_H

#include <cstdint>
#include <cstdlib>
#include <data/crypto/one_way.hpp>

/** A hasher class for RIPEMD-160. */
// satisfies data::hash::Engine
class CRIPEMD160 {
private:
    uint32_t s[5];
    data::byte buf[64];
    uint64_t bytes;

public:
    constexpr static const size_t DigestSize = 20;
    constexpr static const size_t BlockSize = 64;
    constexpr static const data::crypto::security Security = data::crypto::security::modern;

    CRIPEMD160 ();
    CRIPEMD160 &Update (const data::byte *data, size_t len);
    void Final (data::byte hash[DigestSize]);
    CRIPEMD160 &Restart ();
};

#endif // BITCOIN_CRYPTO_RIPEMD160_H
