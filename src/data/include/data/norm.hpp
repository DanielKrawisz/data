// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NORM
#define DATA_NORM

#include <data/abs.hpp>

namespace data {

    // norm is supposed to return an ordered type that satisfies the triangle inequality.
    template <typename A> constexpr auto norm (const A &);

    // quadrance generalizes the square function.
    template <typename A> constexpr auto quadrance (const A &);
}

namespace data::math::def {

    template <typename X> struct norm;
    template <typename X> struct quadrance;

}

namespace data {

    template <typename A> constexpr auto inline quadrance (const A &x) {
        return math::def::quadrance<A> {} (x);
    }

    template <typename A> constexpr auto inline norm (const A &x) {
        return math::def::norm<A> {} (x);
    }

    // NOTE: if abs is defined, then it must return the same type as norm.
    template <typename F> concept Normed = requires (const F &x) {
        { norm (x) };
    } && Ordered<decltype (norm (std::declval<F> ()))>;
}

namespace data::math::def {

    template <std::integral X> struct norm<X> {
        constexpr auto operator () (X x) {
            return data::abs (x);
        }
    };

}

#endif
