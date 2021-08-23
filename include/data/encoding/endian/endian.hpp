// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <data/types.hpp>
#include <boost/endian/conversion.hpp>

namespace data::endian {
    
    using order = boost::endian::order;
    constexpr order big = boost::endian::order::big;
    constexpr order little = boost::endian::order::little;
    
    constexpr inline order opposite(order o) {
        return o == big ? little : big;
    } 
    
    // convert native to and from the given endian ordering. 
    template <typename X, order> struct native;
    
    template <typename X> struct native<X, big> {
        static X from(const X x) {
            return boost::endian::native_to_big<X>(x);
        }
        
        static X to(const X x) {
            return boost::endian::big_to_native<X>(x);
        }
    };
    
    template <typename X> struct native<X, little> {
        static X from(const X x) {
            return boost::endian::native_to_little<X>(x);
        }
        
        static X to(const X x) {
            return boost::endian::little_to_native<X>(x);
        }
    };
    
    template <bool is_signed, std::size_t bytes> struct native_representation;
    
    template <bool is_signed, std::size_t bytes> using to_native = typename native_representation<is_signed, bytes>::type;
    
    template <> struct native_representation<true, 1>{
        using type = int_least8_t;
    };
    
    template <> struct native_representation<true, 2>{
        using type = int_least16_t;
    };
    
    template <> struct native_representation<true, 3> {
        using type = int_least32_t;
    };
    
    template <> struct native_representation<true, 4> {
        using type = int_least32_t;
    };
    
    template <> struct native_representation<true, 5> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 6> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 7> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 8> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<false, 1>{
        using type = uint_least8_t;
    };
    
    template <> struct native_representation<false, 2>{
        using type = uint_least16_t;
    };
    
    template <> struct native_representation<false, 3> {
        using type = uint_least32_t;
    };
    
    template <> struct native_representation<false, 4> {
        using type = uint_least32_t;
    };
    
    template <> struct native_representation<false, 5> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 6> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 7> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 8> {
        using type = uint_least64_t;
    };
    
}

#endif

