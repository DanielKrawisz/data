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
    struct writer {
        constexpr static size_t size = Size;
        
        Transform Hash;
        
        writer() : Hash{} {}
        
        writer& update(bytes_view b) {
            Hash.Update(b.data(), b.size());
            return *this;
        }
        
        digest<size> finalize() {
            digest<size> d;
            Hash.Final(d.data());
            Hash.Restart();
            return d;
        }
        
        digest<size> operator()(bytes_view b) {
            digest<size> d;
            Hash.CalculateDigest(d.data(), b.data(), b.size());
            return d;
        }
        
    };
    
}

namespace data::crypto::hash {
    
    template <> struct RIPEMD<16> : CryptoPP::writer<CryptoPP::RIPEMD128, 16> {
        RIPEMD& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::RIPEMD128, 16>::update(b);
            return *this;
        }
    }; 
    
    template <> struct RIPEMD<20> : CryptoPP::writer<CryptoPP::RIPEMD160, 20> {
        RIPEMD& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::RIPEMD160, 20>::update(b);
            return *this;
        }
    }; 
    
    template <> struct RIPEMD<32> : CryptoPP::writer<CryptoPP::RIPEMD256, 32> {
        RIPEMD& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::RIPEMD256, 32>::update(b);
            return *this;
        }
    }; 
    
    template <> struct RIPEMD<40> : CryptoPP::writer<CryptoPP::RIPEMD320, 40> {
        RIPEMD& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::RIPEMD320, 40>::update(b);
            return *this;
        }
    }; 
    
    template <> struct SHA2<28> : CryptoPP::writer<CryptoPP::SHA224, 28> {
        SHA2& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::SHA224, 28>::update(b);
            return *this;
        }
    };
    
    template <> struct SHA2<32> : CryptoPP::writer<CryptoPP::SHA256, 32> {
        SHA2& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::SHA256, 32>::update(b);
            return *this;
        }
    };
    
    template <> struct SHA2<56> : CryptoPP::writer<CryptoPP::SHA384, 56> {
        SHA2& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::SHA384, 56>::update(b);
            return *this;
        }
    };
    
    template <> struct SHA2<64> : CryptoPP::writer<CryptoPP::SHA512, 64> {
        SHA2& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::SHA512, 64>::update(b);
            return *this;
        }
    };
    
    template <size_t size> struct SHA3 : CryptoPP::writer<CryptoPP::SHA3_Final<size>, size> {
        SHA3& update(bytes_view b) {
            CryptoPP::writer<CryptoPP::SHA3_Final<size>, size>::update(b);
            return *this;
        }
    };
    
    digest<16> inline RIPEMD_128(bytes_view b) {
        return RIPEMD<16>{}(b);
    }
    
    digest<20> inline RIPEMD_160(bytes_view b) {
        return RIPEMD<20>{}(b);
    }
    
    digest<32> inline RIPEMD_256(bytes_view b) {
        return RIPEMD<32>{}(b);
    }
    
    digest<40> inline RIPEMD_320(bytes_view b) {
        return RIPEMD<40>{}(b);
    }
    
    digest<28> inline SHA2_224(bytes_view b) {
        return SHA2<28>{}(b);
    }
    
    digest<32> inline SHA2_256(bytes_view b) {
        return SHA2<32>{}(b);
    }
    
    digest<56> inline SHA2_384(bytes_view b) {
        return SHA2<56>{}(b);
    }
    
    digest<64> inline SHA2_512(bytes_view b) {
        return SHA2<64>{}(b);
    }
    
    digest<28> inline SHA3_224(bytes_view b) {
        return SHA3<28>{}(b);
    }
    
    digest<32> inline SHA3_256(bytes_view b) {
        return SHA3<32>{}(b);
    }
    
    digest<56> inline SHA3_384(bytes_view b) {
        return SHA3<56>{}(b);
    }
    
    digest<64> inline SHA3_512(bytes_view b) {
        return SHA2<64>{}(b);
    }
    
    digest<20> inline Bitcoin_160(bytes_view b) {
        return Bitcoin<20>{}(b);
    }
    
    digest<32> inline Bitcoin_256(bytes_view b) {
        return Bitcoin<32>{}(b);
    }

}

#endif


