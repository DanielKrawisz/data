// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_BITCOIND
#define DATA_CRYPTO_HASH_BITCOIND

#include "functions.hpp"
#include "sv/crypto/ripemd160.h"
#include "sv/crypto/sha256.h"

namespace data::crypto::hash::bitcoind {
    template <class hash, size_t Size> 
    struct writer {
        constexpr static size_t size = Size;
        
        hash Hash;
        
        writer() : Hash{} {}
        
        writer& update(bytes_view b) {
            Hash.Write(b.data(), size);
            return *this;
        }
        
        digest<size> finalize() {
            digest<size> d;
            Hash.Finalize(d.data());
            Hash.Reset();
            return d;
        }
        
        digest<size> operator()(bytes_view b) {
            return update(b).finalize();
        }
        
    };
    
}

namespace data::crypto::hash {
    
    template <> struct RIPEMD<20> : bitcoind::writer<CRIPEMD160, 20> {
        
        RIPEMD() : bitcoind::writer<CRIPEMD160, 20>{} {}
        
        RIPEMD& update(bytes_view b) {
            bitcoind::writer<CRIPEMD160, 20>::update(b);
            return *this;
        }
    }; 
    
    template <> struct SHA2<32> : bitcoind::writer<CSHA256, 32> {
        
        SHA2() : bitcoind::writer<CSHA256, 32>{} {}
        
        SHA2& update(bytes_view b) {
            bitcoind::writer<CSHA256, 32>::update(b);
            return *this;
        }
    };
    
    digest<20> inline RIPEMD_160(bytes_view b) {
        return RIPEMD<20>{}(b);
    }
    
    digest<32> inline SHA2_256(bytes_view b) {
        return SHA2<32>{}(b);
    }

}

#endif
