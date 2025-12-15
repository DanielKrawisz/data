// Copyright (c) 2021-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_BITCOIN
#define DATA_CRYPTO_HASH_BITCOIN

#include <data/crypto/hash/functions.hpp>

namespace data::crypto::hash {
    using namespace data;
    
    // Bitcoin hash 160 is difined to be RIPEMD_160 * SHA2_256
    template<> struct Bitcoin<20> {

        SHA2<32> Engine;
        
        constexpr static size_t Size = 20;
        
        Bitcoin () : Engine {} {}
        
        Bitcoin<20> &Update (const byte *b, size_t x) {
            Engine.Update (b, x);
            return *this;
        }
        
        void Final (byte b[Size]) {
            byte x[32];
            Engine.Final (x);
            CRIPEMD160 ().Update (x, 32).Final (b);
        }

        Bitcoin<20> &Restart () {
            Engine.Restart ();
            return *this;
        }
    };
    
    // Bitcoin hash 256 is difined to be SHA2_256 * SHA_256
    template<> struct Bitcoin<32> {
        
        SHA2<32> Engine;
        
        constexpr static size_t Size = 32;
        
        Bitcoin () : Engine {} {}
        
        Bitcoin<32> &Update (const byte *b, size_t x) {
            Engine.Update (b, x);
            return *this;
        }
        
        void Final (byte b[Size]) {
            byte x[32];
            Engine.Final (x);
            CSHA256 ().Update (x, 32).Final (b);
        }

        Bitcoin<32> &Restart () {
            Engine.Restart ();
            return *this;
        }
    };

}

namespace data::crypto {

    hash::digest160 inline Bitcoin_160 (byte_slice b) {
        return hash::calculate<hash::Bitcoin<20>> (b);
    }

    hash::digest256 inline Bitcoin_256 (byte_slice b) {
        return hash::calculate<hash::Bitcoin<32>> (b);
    }
}

#endif


