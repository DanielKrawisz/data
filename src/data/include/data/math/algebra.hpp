// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA
#define DATA_MATH_ALGEBRA

#include <data/math/nonzero.hpp>
#include <data/arithmetic.hpp>

// TODO make functions for identity and inverse in data::math

// TODO we can enable these types to take functions as parameters

namespace data::math::def {

    template <typename F, typename x> struct identity;
    template <typename F, typename x> struct inverse;

    template <typename X> struct identity<plus<X>, X> {
        constexpr X operator () () {
            return X {0};
        }
    };

    template <typename X> struct identity<times<X>, X> {
        constexpr X operator () () {
            return X {1};
        }
    };

    template <std::integral X> struct inverse<plus<X>, X> {
        constexpr X operator () (X a, X b) {
            return b - a;
        }
    };

    template <typename X> struct inverse<times<X>, math::nonzero<X>> {
        constexpr math::nonzero<X> operator () (const math::nonzero<X> &a, const math::nonzero<X> &b) {
            return b / a;
        }
    };

    template <typename X> struct identity<times<X>, math::nonzero<X>> {
        constexpr math::nonzero<X> operator () () {
            return 1;
        }
    };
    
}

#endif
