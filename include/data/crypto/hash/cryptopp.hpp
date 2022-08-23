// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_CRYPTOPP
#define DATA_CRYPTO_HASH_CRYPTOPP

#include "functions.hpp"
#include "cryptopp/cryptlib.h"
#include "cryptopp/ripemd.h"
#include "cryptopp/sha.h"
#include "cryptopp/sha3.h"

namespace data::crypto::hash::CryptoPP {
    using namespace ::CryptoPP;
    
    template <class Transform, size_t Size> 
    requires std::derived_from<Transform, HashTransformation>
    struct writer : data::writer<byte> {
        constexpr static size_t size = Size;
        
        Transform Hash;
        
        writer() : Hash{} {}
        
        void write(const byte *b, size_t x) override {
            Hash.Update(b, x);
        }
        
        digest<size> finalize() {
            digest<size> d;
            Hash.Final(d.data());
            Hash.Restart();
            return d;
        }
        
    };
    
}

namespace data::crypto::hash {
    
#ifndef USE_BITCOIND_HASH_FUNCTIONS
    struct SHA1 : CryptoPP::writer<CryptoPP::SHA1, 20> {}; 
#endif
    
    template <> struct RIPEMD<16> : CryptoPP::writer<CryptoPP::RIPEMD128, 16> {}; 
    
#ifndef USE_BITCOIND_HASH_FUNCTIONS
    template <> struct RIPEMD<20> : CryptoPP::writer<CryptoPP::RIPEMD160, 20> {}; 
#endif
    
    template <> struct RIPEMD<32> : CryptoPP::writer<CryptoPP::RIPEMD256, 32> {}; 
    
    template <> struct RIPEMD<40> : CryptoPP::writer<CryptoPP::RIPEMD320, 40> {}; 
    
    template <> struct SHA2<28> : CryptoPP::writer<CryptoPP::SHA224, 28> {};
    
#ifndef USE_BITCOIND_HASH_FUNCTIONS
    template <> struct SHA2<32> : CryptoPP::writer<CryptoPP::SHA256, 32> {};
#endif
    
    template <> struct SHA2<48> : CryptoPP::writer<CryptoPP::SHA384, 48> {};
    
    template <> struct SHA2<64> : CryptoPP::writer<CryptoPP::SHA512, 64> {};
    
    template <size_t size> struct SHA3 : CryptoPP::writer<CryptoPP::SHA3_Final<size>, size> {};
    
    digest<16> inline RIPEMD_128(bytes_view b) {
        return calculate<RIPEMD<16>>(b);
    }
    
#ifndef USE_BITCOIND_HASH_FUNCTIONS
    digest<20> inline RIPEMD_160(bytes_view b) {
        return calculate<RIPEMD<20>>(b);
    }
#endif
    
    digest<32> inline RIPEMD_256(bytes_view b) {
        return calculate<RIPEMD<32>>(b);
    }
    
    digest<40> inline RIPEMD_320(bytes_view b) {
        return calculate<RIPEMD<40>>(b);
    }
    
    digest<28> inline SHA2_224(bytes_view b) {
        return calculate<SHA2<28>>(b);
    }
    
#ifndef USE_BITCOIND_HASH_FUNCTIONS
    digest<32> inline SHA2_256(bytes_view b) {
        return calculate<SHA2<32><(b);
    }
#endif
    
    digest<48> inline SHA2_384(bytes_view b) {
        return calculate<SHA2<48>>(b);
    }
    
    digest<64> inline SHA2_512(bytes_view b) {
        return calculate<SHA2<64>>(b);
    }
    
    digest<28> inline SHA3_224(bytes_view b) {
        return calculate<SHA3<28>>(b);
    }
    
    digest<32> inline SHA3_256(bytes_view b) {
        return calculate<SHA3<32>>(b);
    }
    
    digest<48> inline SHA3_384(bytes_view b) {
        return calculate<SHA3<48>>(b);
    }
    
    digest<64> inline SHA3_512(bytes_view b) {
        return calculate<SHA2<64>>(b);
    }

}

#endif


