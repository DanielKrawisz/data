// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_FUNCTIONS
#define DATA_CRYPTO_HASH_FUNCTIONS

#include <data/hash.hpp>
#include <data/crypto/one_way.hpp>

namespace data::crypto::hash {
    using namespace data::hash;
}

namespace data::crypto {

    // supported hash functions.

    hash::digest160 SHA1 (byte_slice);
    hash::digest160 SHA1 (string_view);

    hash::digest128 MD5 (byte_slice);
    hash::digest128 MD5 (string_view);

    hash::digest224 SHA2_224 (byte_slice);
    hash::digest224 SHA2_224 (string_view);
    hash::digest256 SHA2_256 (byte_slice);
    hash::digest256 SHA2_256 (string_view);
    hash::digest384 SHA2_384 (byte_slice);
    hash::digest384 SHA2_384 (string_view);
    hash::digest512 SHA2_512 (byte_slice);
    hash::digest512 SHA2_512 (string_view);

    template <size_t size> hash::digest<size> SHA3 (byte_slice);
    template <size_t size> hash::digest<size> SHA3 (string_view);

    hash::digest224 SHA3_224 (byte_slice);
    hash::digest224 SHA3_224 (string_view);
    hash::digest256 SHA3_256 (byte_slice);
    hash::digest256 SHA3_256 (string_view);
    hash::digest384 SHA3_384 (byte_slice);
    hash::digest384 SHA3_384 (string_view);
    hash::digest512 SHA3_512 (byte_slice);
    hash::digest512 SHA3_512 (string_view);

    hash::digest128 RIPEMD_128 (byte_slice);
    hash::digest128 RIPEMD_128 (string_view);
    hash::digest160 RIPEMD_160 (byte_slice);
    hash::digest160 RIPEMD_160 (string_view);
    hash::digest256 RIPEMD_256 (byte_slice);
    hash::digest256 RIPEMD_256 (string_view);
    hash::digest320 RIPEMD_320 (byte_slice);
    hash::digest320 RIPEMD_320 (string_view);

    hash::digest160 Bitcoin_160 (byte_slice);
    hash::digest256 Bitcoin_256 (byte_slice);
    hash::digest160 Bitcoin_160 (string_view);
    hash::digest256 Bitcoin_256 (string_view);

}

namespace data::crypto::hash {

    template <typename W>
    concept Writer = data::hash::Writer<W> && requires {
        requires byte (W::Security) >= byte (security::modern) ;
    };
    
    // All of these satisfy hash::Engine.
    struct SHA1;
    template <size_t size> struct RIPEMD;
    template <size_t size> struct SHA2;
    template <size_t size> struct SHA3;
    template <size_t size> struct Bitcoin;

    template <> struct RIPEMD<16>;
    template <> struct RIPEMD<20>;
    template <> struct RIPEMD<32>;
    template <> struct RIPEMD<40>;

    template <> struct SHA2<28>;
    template <> struct SHA2<32>;
    template <> struct SHA2<48>;
    template <> struct SHA2<64>;

}

namespace data::crypto {

    hash::digest160 inline SHA1 (string_view b) {
        return SHA1 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest128 inline MD5 (string_view b) {
        return MD5 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest224 inline SHA2_224 (string_view b) {
        return SHA2_224 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest256 inline SHA2_256 (string_view b) {
        return SHA2_256 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest384 inline SHA2_384 (string_view b) {
        return SHA2_384 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest512 inline SHA2_512 (string_view b) {
        return SHA2_512 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    template <size_t size> hash::digest<size> SHA3 (string_view b) {
        return SHA3<size> (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest224 inline SHA3_224 (string_view b) {
        return SHA3_224 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest256 inline SHA3_256 (string_view b) {
        return SHA3_256 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest384 inline SHA3_384 (string_view b) {
        return SHA3_384 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest512 inline SHA3_512 (string_view b) {
        return SHA3_512 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest128 inline RIPEMD_128 (string_view b) {
        return RIPEMD_128 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest160 inline RIPEMD_160 (string_view b) {
        return RIPEMD_160 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest256 inline RIPEMD_256 (string_view b) {
        return RIPEMD_256 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest320 inline RIPEMD_320 (string_view b) {
        return RIPEMD_320 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest160 inline Bitcoin_160 (string_view b) {
        return Bitcoin_160 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

    hash::digest256 inline Bitcoin_256 (string_view b) {
        return Bitcoin_256 (byte_slice {(const byte *) (b.data ()), b.size ()});
    }

}

#endif

