// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA
#define DATA_MATH_ALGEBRA

#include <data/math/nonnegative.hpp>
#include <data/arithmetic.hpp>

namespace data::math {

    template <typename F, typename x> struct identity;
    template <typename F, typename x> struct inverse;

    template <typename X> struct identity<plus<X>, X> {
        X operator () () {
            return X {0};
        }
    };

    template <typename X> struct identity<times<X>, X> {
        X operator () () {
            return X {1};
        }
    };

    template <std::signed_integral X> struct inverse<plus<X>, X> {
        X operator () (X a, X b) {
            return b - a;
        }
    };

    template <typename X> struct inverse<times<X>, nonzero<X>> {
        nonzero<X> operator () (const nonzero<X> &a, const nonzero<X> &b) {
            return b / a;
        }
    };

    template <typename X> struct identity<times<X>, nonzero<X>> {
        nonzero<X> operator () () {
            return 1;
        }
    };
    
}

#endif
