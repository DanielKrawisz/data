// Copyright (c) 20202 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ROOT
#define DATA_MATH_ROOT

#include <data/tools.hpp>

namespace data::math {

    // root{}(number) => set<number>
    template <typename number, uint64 power> struct root;
}

namespace data {
    
    template <uint64 pow, typename X> 
    set<X> root (const X &x) {
        return math::root<X, pow> {} (x);
    }
    
    template <typename X> 
    set<X> sqrt (const X &x){
        return math::root<X, 2> {} (x);
    }
    
}
    
#endif
