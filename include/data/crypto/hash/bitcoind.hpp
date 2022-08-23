// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_BITCOIND
#define DATA_CRYPTO_HASH_BITCOIND

#ifdef USE_BITCOIND_HASH_FUNCTIONS

#include "functions.hpp"
#include <sv/crypto/sha1.h>
#include "sv/crypto/ripemd160.h"
#include "sv/crypto/sha256.h"


namespace data::crypto::hash::bitcoind {
    template <class hash, size_t Size> 
    struct writer : data::writer<byte> {
        constexpr static size_t size = Size;
        
        hash Hash;
        
        writer() : Hash{} {}
        
        void write(const byte *b, size_t x) override {
            Hash.Write(b, x);
        }
        
        digest<size> finalize() {
            digest<size> d;
            Hash.Finalize(d.data());
            Hash.Reset();
            return d;
        }
        
    };
    
}

namespace data::crypto::hash {
    
    struct SHA1 : bitcoind::writer<CSHA1, 20> {}; 
    
    template <> struct RIPEMD<20> : bitcoind::writer<CRIPEMD160, 20> {}; 
    
    template <> struct SHA2<32> : bitcoind::writer<CSHA256, 32> {};
    
    digest<20> inline RIPEMD_160(bytes_view b) {
        return calculate<RIPEMD<20>>(b);
    }
    
    digest<32> inline SHA2_256(bytes_view b) {
        return calculate<SHA2<32>>(b);
    }

}

#endif 
#endif
