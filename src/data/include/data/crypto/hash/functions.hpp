// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_FUNCTIONS
#define DATA_CRYPTO_HASH_FUNCTIONS

#include <data/crypto/digest.hpp>
#include <data/crypto/one_way.hpp>

namespace data::crypto {

    // supported hash functions.
    digest160 SHA1 (bytes_view);
    digest160 SHA1 (string_view);

    digest224 SHA2_224 (bytes_view);
    digest224 SHA2_224 (string_view);
    digest256 SHA2_256 (bytes_view);
    digest256 SHA2_256 (string_view);
    digest384 SHA2_384 (bytes_view);
    digest384 SHA2_384 (string_view);
    digest512 SHA2_512 (bytes_view);
    digest512 SHA2_512 (string_view);

    template <size_t size> digest<size> SHA3 (bytes_view);
    template <size_t size> digest<size> SHA3 (string_view);

    digest224 SHA3_224 (bytes_view);
    digest224 SHA3_224 (string_view);
    digest256 SHA3_256 (bytes_view);
    digest256 SHA3_256 (string_view);
    digest384 SHA3_384 (bytes_view);
    digest384 SHA3_384 (string_view);
    digest512 SHA3_512 (bytes_view);
    digest512 SHA3_512 (string_view);

    digest128 RIPEMD_128 (bytes_view);
    digest128 RIPEMD_128 (string_view);
    digest160 RIPEMD_160 (bytes_view);
    digest160 RIPEMD_160 (string_view);
    digest256 RIPEMD_256 (bytes_view);
    digest256 RIPEMD_256 (string_view);
    digest320 RIPEMD_320 (bytes_view);
    digest320 RIPEMD_320 (string_view);

    digest160 Bitcoin_160 (bytes_view);
    digest256 Bitcoin_256 (bytes_view);
    digest160 Bitcoin_160 (string_view);
    digest256 Bitcoin_256 (string_view);
}

namespace data::crypto::hash {

    // a cryptographic hash function also needs to be a one-way function
    // and indistinguishable from a random function. 
    template <typename f, typename d = digest<f::size>> 
    concept function = requires {
        { f::size };
    } && one_way<f, d, bytes_view>;
    
    template <typename W> 
    concept writer = std::derived_from<W, message_writer<digest<W::size>, byte>> && requires {
        { W {} };
        { W::size };
    };
    
    template <writer W>
    digest<W::size> inline calculate (bytes_view b) {
        W w {};
        w.write (b.data (), b.size ());
        return w.complete ();
    }
    
    // these are both functions and writers. 
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

    digest160 inline SHA1 (string_view b) {
        return SHA1 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest224 inline SHA2_224 (string_view b) {
        return SHA2_224 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest256 inline SHA2_256 (string_view b) {
        return SHA2_256 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest384 inline SHA2_384 (string_view b) {
        return SHA2_384 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest512 inline SHA2_512 (string_view b) {
        return SHA2_512 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    template <size_t size> digest<size> SHA3 (string_view b) {
        return SHA3<size> (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest224 inline SHA3_224 (string_view b) {
        return SHA3_224 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest256 inline SHA3_256 (string_view b) {
        return SHA3_256 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest384 inline SHA3_384 (string_view b) {
        return SHA3_384 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest512 inline SHA3_512 (string_view b) {
        return SHA3_512 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest128 inline RIPEMD_128 (string_view b) {
        return RIPEMD_128 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest160 inline RIPEMD_160 (string_view b) {
        return RIPEMD_160 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest256 inline RIPEMD_256 (string_view b) {
        return RIPEMD_256 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest320 inline RIPEMD_320 (string_view b) {
        return RIPEMD_320 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest160 inline Bitcoin_160 (string_view b) {
        return Bitcoin_160 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

    digest256 inline Bitcoin_256 (string_view b) {
        return Bitcoin_256 (bytes_view {(const byte *) (b.data ()), b.size ()});
    }

}

#endif

