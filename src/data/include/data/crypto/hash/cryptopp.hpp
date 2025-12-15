// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_CRYPTOPP
#define DATA_CRYPTO_HASH_CRYPTOPP

#include "functions.hpp"
#include "cryptopp/cryptlib.h"
#include "cryptopp/ripemd.h"
//#include "cryptopp/md5.h"
#include "cryptopp/sha.h"
#include "cryptopp/sha3.h"

namespace data::crypto::hash::CryptoPP {
    using namespace ::CryptoPP;
    
    template <class Transform, size_t size>
    requires std::derived_from<Transform, HashTransformation>
    struct engine : Transform {
        constexpr static size_t Size = size;
        using Transform::Transform;
    };
    
}

namespace data::crypto::hash {
    
#ifndef USE_BITCOIND_HASH_FUNCTIONS
    struct SHA1 : CryptoPP::engine<CryptoPP::SHA1, 20> {};
#endif

    //struct MD5 : CryptoPP::engine<CryptoPP::MD5, 16> {};

    template <> struct RIPEMD<16> : CryptoPP::engine<CryptoPP::RIPEMD128, 16> {};

#ifndef USE_BITCOIND_HASH_FUNCTIONS
    template <> struct RIPEMD<20> : CryptoPP::engine<CryptoPP::RIPEMD160, 20> {};
#endif

    template <> struct RIPEMD<32> : CryptoPP::engine<CryptoPP::RIPEMD256, 32> {};

    template <> struct RIPEMD<40> : CryptoPP::engine<CryptoPP::RIPEMD320, 40> {};

    template <> struct SHA2<28> : CryptoPP::engine<CryptoPP::SHA224, 28> {};

#ifndef USE_BITCOIND_HASH_FUNCTIONS
    template <> struct SHA2<32> : CryptoPP::engine<CryptoPP::SHA256, 32> {};
#endif

    template <> struct SHA2<48> : CryptoPP::engine<CryptoPP::SHA384, 48> {};

    template <> struct SHA2<64> : CryptoPP::engine<CryptoPP::SHA512, 64> {};

    template <size_t size> struct SHA3 : CryptoPP::engine<CryptoPP::SHA3_Final<size>, size> {};
    
}

namespace data::crypto {

#ifndef USE_BITCOIND_HASH_FUNCTIONS
    hash::digest160 inline SHA1 (byte_slice b) {
        return hash::calculate<hash::SHA1> (b);
    }
#endif

    hash::digest128 inline RIPEMD_128 (byte_slice b) {
        return hash::calculate<hash::RIPEMD<16>> (b);
    }

#ifndef USE_BITCOIND_HASH_FUNCTIONS
    hash::digest160 inline RIPEMD_160 (byte_slice b) {
        return hash::calculate<hash::RIPEMD<20>> (b);
    }
#endif

    hash::digest256 inline RIPEMD_256 (byte_slice b) {
        return hash::calculate<hash::RIPEMD<32>> (b);
    }

    hash::digest320 inline RIPEMD_320 (byte_slice b) {
        return hash::calculate<hash::RIPEMD<40>> (b);
    }

    hash::digest224 inline SHA2_224 (byte_slice b) {
        return hash::calculate<hash::SHA2<28>> (b);
    }

#ifndef USE_BITCOIND_HASH_FUNCTIONS
    hash::digest256 inline SHA2_256 (byte_slice b) {
        return hash::calculate<hash::SHA2<32>> (b);
    }
#endif

    template <size_t size> hash::digest<size> inline SHA3 (byte_slice b) {
        return hash::calculate<hash::SHA3<size>> (b);
    }
    
    hash::digest384 inline SHA2_384 (byte_slice b) {
        return hash::calculate<hash::SHA2<48>> (b);
    }

    hash::digest512 inline SHA2_512 (byte_slice b) {
        return hash::calculate<hash::SHA2<64>> (b);
    }

    hash::digest224 inline SHA3_224 (byte_slice b) {
        return hash::calculate<hash::SHA3<28>> (b);
    }

    hash::digest256 inline SHA3_256 (byte_slice b) {
        return hash::calculate<hash::SHA3<32>> (b);
    }

    hash::digest384 inline SHA3_384 (byte_slice b) {
        return hash::calculate<hash::SHA3<48>> (b);
    }

    hash::digest512 inline SHA3_512 (byte_slice b) {
        return hash::calculate<hash::SHA2<64>> (b);
    }

}

#endif


