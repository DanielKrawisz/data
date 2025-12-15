// Copyright (c) 2021-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_BITCOIND
#define DATA_CRYPTO_HASH_BITCOIND

#ifdef USE_BITCOIND_HASH_FUNCTIONS

#include "functions.hpp"
#include <sv/crypto/sha1.h>
#include "sv/crypto/ripemd160.h"
#include "sv/crypto/sha256.h"

namespace data::crypto::hash {
    
    struct SHA1 : CSHA1 {};
    
    template <> struct RIPEMD<20> : CRIPEMD160 {};
    
    template <> struct SHA2<32> : CSHA256 {};
    
}

namespace data::crypto {

    hash::digest160 inline SHA1 (byte_slice b) {
        return hash::calculate<hash::SHA1> (b);
    }

    hash::digest160 inline RIPEMD_160 (byte_slice b) {
        return hash::calculate<hash::RIPEMD<20>> (b);
    }

    hash::digest256 inline SHA2_256 (byte_slice b) {
        return hash::calculate<hash::SHA2<32>> (b);
    }

}

#endif 
#endif
