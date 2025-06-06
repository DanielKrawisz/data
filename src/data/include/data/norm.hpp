// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NORM
#define DATA_NORM

#include <data/complex.hpp>

namespace data {

    template <typename A> constexpr auto norm (const A &);

    template <typename A> constexpr auto quadrance (const A &);
}

namespace data::math {

    template <typename X> struct norm;
    template <typename X> struct quadrance;

    // if abs and quadrance are defined, then norm is abs quadrance.
    template <typename X> requires requires (const X &x) {
        { abs<X> {} (quadrance<X> {} (x)) };
    } struct norm<X> {
        auto operator () (const X &x) {
            return abs<X> {} (quadrance<X> {} (x));
        }
    };

    // for real values quadrance is just the square.
    template <typename X> struct quadrance : square<X> {};

    // if inner is defined, then quadrance x is inner x x.
    template <typename X> requires (!data::real<X>) && requires (const X &x) {
        { inner<X, X> {} (x) };
    } struct quadrance<X> {
        constexpr auto operator () (const X &x) {
            return inner<X, X> {} (x, x);
        }
    };
}

namespace data {

    template <typename A> constexpr auto inline quadrance (const A &x) {
        return math::quadrance<A> {} (x);
    }

    template <typename A> constexpr auto inline norm (const A &x) {
        return math::norm<A> {} (x);
    }

    // NOTE: if abs is defined, then it must return the same type as norm.
    template <typename F> concept normed = requires (const F &x) {
        { norm (x) };
    } && ordered<decltype (norm (std::declval<F> ()))>;
}

#endif
