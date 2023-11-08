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
    
    template <typename F, typename x> struct identity;
    template <typename F, typename x> struct inverse;
    
    template <typename X> struct plus {
        X operator () (const X &a, const X &b) {
            return a + b;
        }
    };
    
    template <typename X> struct identity<plus<X>, X> {
        X operator () () {
            return X {0};
        }
    };
    
    template <typename X> struct times {
        X operator () (const X &a, const X &b) {
            return a * b;
        }
    };
    
    template <typename X> struct identity<times<X>, X> {
        X operator () () {
            return X {1};
        }
    };
    
    template <> struct inverse<plus<int8_t>, int8_t> {
        int8_t operator () (const int8_t &a, const int8_t &b) {
            return b - a;
        }
    };
    
    template <std::integral X> struct inverse<plus<X>, X> {
        X operator () (X a, X b) {
            return b - a;
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
