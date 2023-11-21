// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_BITCOIN
#define DATA_CRYPTO_HASH_BITCOIN

#include <data/crypto/hash/functions.hpp>

namespace data::crypto::hash {
    
    // Bitcoin hash 160 is difined to be RIPEMD_160 * SHA2_256
    template<> struct Bitcoin<20> : data::writer<byte> {
        SHA2<32> Writer;
        
        constexpr static size_t size = 20;
        
        Bitcoin () : Writer {} {}
        
        void write (const byte *b, size_t x) override {
            Writer.write (b, x);
        }
        
        digest<20> finalize () {
            return calculate<RIPEMD<20>> (Writer.finalize ());
        }
    };
    
    // Bitcoin hash 256 is difined to be SHA2_256 * SHA_256
    template<> struct Bitcoin<32> : data::writer<byte> {
        SHA2<32> Writer;
        
        constexpr static size_t size = 32;
        
        Bitcoin () : Writer {} {}
        
        void write (const byte *b, size_t x) override {
            Writer.write (b, x);
        }
        
        digest<32> finalize () {
            return calculate<SHA2<32>> (Writer.finalize ());
        }
    };

}

namespace data::crypto {

    digest160 inline Bitcoin_160 (bytes_view b) {
        return hash::calculate<hash::Bitcoin<20>> (b);
    }

    digest256 inline Bitcoin_256 (bytes_view b) {
        return hash::calculate<hash::Bitcoin<32>> (b);
    }
}

#endif


