// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INCREMENT
#define DATA_MATH_NUMBER_INCREMENT

#include <data/math/nonnegative.hpp>

namespace data::math::number {

    template <typename N> struct increment;
    template <typename N> struct decrement;

}

namespace data {
    template <typename N> auto inline increment (const N &n) {
        return math::number::increment<N> {} (n);
    }

    template <typename N> auto inline decrement (const math::nonzero<N> &n) {
        return math::number::decrement<N> {} (n);
    }

    template <typename N> auto inline decrement (const N &n) {
        return math::number::decrement<N> {} (n);
    }
}

namespace data::math::number {

    template <std::unsigned_integral N> struct increment<N> {
        nonzero<N> operator () (const N &);
    };

    template <std::unsigned_integral N> struct decrement<N> {
        N operator () (const nonzero<N> &);
        N operator () (const N &);
    };

    template <std::signed_integral Z> struct increment<Z> {
        Z operator () (const Z &);
    };

    template <std::signed_integral Z> struct decrement<Z> {
        Z operator () (const Z &);
    };

    template <std::unsigned_integral N> nonzero<N> inline increment<N>::operator () (const N &n) {
        nonzero<N> x {n};
        ++x.Value;
        return x;
    }

    template <std::unsigned_integral N> N inline decrement<N>::operator () (const nonzero<N> &n) {
        return n.Value - 1;
    }

    template <std::unsigned_integral N> N inline decrement<N>::operator () (const N &n) {
        if (n == 0) return n;
        return n - 1;
    }

    template <std::signed_integral Z> Z inline increment<Z>::operator () (const Z &z) {
        return z + 1;
    }

    template <std::signed_integral Z> Z inline decrement<Z>::operator () (const Z &z) {
        return z - 1;
    }

}

#endif
