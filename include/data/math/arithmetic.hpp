// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ARITHMETIC
#define DATA_MATH_ARITHMETIC

#include <data/math/sign.hpp> 

namespace data::math {
    
    template <typename X> struct first;
    
    template <std::integral X> struct first<X> {
        X operator () () {
            return X {0};
        }
    };

}

#endif
