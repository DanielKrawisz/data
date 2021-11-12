// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_FUNCTIONS
#define DATA_CRYPTO_HASH_FUNCTIONS

#include "digest.hpp"
#include <data/crypto/one_way.hpp>

namespace data::crypto::hash {
    
    // a cryptographic hash function also needs to be a one-way function
    // and indistinguishable from a random function. 
    template <typename f, typename d = digest<f::size>> 
    concept function = requires {
        { f::size };
    } && one_way<f, d, bytes_view>;
    
    // supported hash functions.
    digest<16> RIPEMD_128(bytes_view);
    digest<20> RIPEMD_160(bytes_view);
    digest<32> RIPEMD_256(bytes_view);
    digest<40> RIPEMD_320(bytes_view);
    
    digest<28> SHA2_224(bytes_view);
    digest<32> SHA2_256(bytes_view);
    digest<56> SHA2_384(bytes_view);
    digest<64> SHA2_512(bytes_view);
    
    digest<28> SHA3_224(bytes_view);
    digest<32> SHA3_256(bytes_view);
    digest<56> SHA3_384(bytes_view);
    digest<64> SHA3_512(bytes_view);
    
    digest<20> Bitcoin_160(bytes_view);
    digest<32> Bitcoin_256(bytes_view);
    
    template <typename W> 
    concept writer = requires {
        { W{} };
        { W::size };
    } && requires (W w, bytes_view b) {
        { w.update(b) } -> std::same_as<W&>;
    } && requires(W w) {
        { w.finalize() } -> std::same_as<digest<W::size>>;
    };
    
    template <writer W> 
    W inline &operator<<(W &w, bytes_view b) {
        return w.update(b);
    }
    
    template <writer W>
    digest<W::size> inline calculate(bytes_view b) {
        return W{}.update(b).finalize();
    }
    
    // these are both functions and writers. 
    template <size_t size> struct RIPEMD;
    template <size_t size> struct SHA2;
    template <size_t size> struct SHA3;
    template <size_t size> struct Bitcoin;

}

namespace data::crypto {
    
    template <size_t size> struct one_way_function<hash::RIPEMD<size>, digest<size>, bytes_view> {};
    template <size_t size> struct one_way_function<hash::SHA2<size>, digest<size>, bytes_view> {};
    template <size_t size> struct one_way_function<hash::SHA3<size>, digest<size>, bytes_view> {};
    template <size_t size> struct one_way_function<hash::Bitcoin<size>, digest<size>, bytes_view> {};
    
}

#endif

