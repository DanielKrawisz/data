// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <boost/endian/buffers.hpp>
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
    
}

#endif

