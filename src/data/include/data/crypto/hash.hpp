// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_HASH
#define DATA_CRYPTO_HASH_HASH

#define USE_BITCOIND_HASH_FUNCTIONS

#ifdef USE_BITCOIND_HASH_FUNCTIONS
// Hash functions provided by bitcoind. 
#include <data/crypto/hash/bitcoind.hpp>
#endif

#include <data/crypto/hash/cryptopp.hpp>

// Bitcoin hash functions
#include <data/crypto/hash/bitcoin.hpp>

#undef USE_BITCOIND_HASH_FUNCTIONS

#endif

