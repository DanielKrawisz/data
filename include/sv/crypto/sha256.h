// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_SHA256_H
#define BITCOIN_CRYPTO_SHA256_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <data/crypto/one_way.hpp>

/** A hasher class for SHA-256. */
// satisfies data::hash::Engine
class CSHA256 {
private:
    uint32_t s[8];
    uint8_t buf[64];
    uint64_t bytes;

public:
    constexpr static const size_t DigestSize = 32;
    constexpr static const size_t BlockSize = 64;
    constexpr static const data::crypto::security Security = data::crypto::security::modern;

    CSHA256 ();
    CSHA256 &Update (const uint8_t *data, size_t len);
    void Final (uint8_t hash[DigestSize]);
    CSHA256 &Restart ();
};

/**
 * Autodetect the best available SHA256 implementation.
 * Returns the name of the implementation.
 */
std::string SHA256AutoDetect ();

#endif // BITCOIN_CRYPTO_SHA256_H
