// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META_EQUAL
#define DATA_META_EQUAL

#include <data/types.hpp>

namespace data::meta {
    
    // type that can only be constructed if two types are equal.
    template <typename x, typename y> struct Equal;
    
    template <typename x> struct Equal<x, x>{};
        
    // type that can only be constructed if two values are equal.
    template <uint32 left, uint32 right> struct equal {
        equal() = delete;
    };
    
    template <uint32 val> struct equal<val, val> {};
    
    template <uint32 left, uint32 right> struct unequal {};
    
    template <uint32 val> struct unequal<val, val> {
        unequal() = delete;
    };
    
}

#endif

        
