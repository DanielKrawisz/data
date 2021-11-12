// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_BITCOIN
#define DATA_CRYPTO_HASH_BITCOIN

#include "bitcoind.hpp"

namespace data::crypto::hash {
    
    // Bitcoin hash 160 is difined to be RIPEMD_160 * SHA2_256
    template<> struct Bitcoin<20> {
        SHA2<32> Writer;
        
        constexpr static size_t size = 20;
        
        Bitcoin() : Writer{} {}
        
        Bitcoin& update(bytes_view b) {
            Writer << b;
            return *this;
        }
        
        digest<20> finalize() {
            return RIPEMD<20>{}(Writer.finalize());
        }
        
        digest<20> operator()(bytes_view b) const {
            return calculate<Bitcoin<20>>(b);
        }
    };
    
    // Bitcoin hash 256 is difined to be SHA2_256 * SHA_256
    template<> struct Bitcoin<32> {
        SHA2<32> Writer;
        
        constexpr static size_t size = 32;
        
        Bitcoin() : Writer{} {}
        
        Bitcoin& update(bytes_view b) {
            Writer << b;
            return *this;
        }
        
        digest<32> finalize() {
            return SHA2<32>{}(Writer.finalize());
        }
        
        digest<32> operator()(bytes_view b) const {
            return calculate<Bitcoin<32>>(b);
        }
    };

}

#endif


