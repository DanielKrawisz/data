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

namespace data {
    template <std::integral X> X inline increment (const X &x) {
        return x + 1;
    }
    
    template <std::signed_integral X> X inline decrement (const X &x) {
        return x - 1;
    }
    
    template <std::unsigned_integral X> X inline decrement (const X &x) {
        return x == 0 ? 0 : x - 1;
    }

    template <typename X> X inline increment (const X &x) {
        X n = x;
        return ++n;
    }

    template <typename X> X inline decrement (const X &x) {
        X n = x;
        return --n;
    }
}

#endif
