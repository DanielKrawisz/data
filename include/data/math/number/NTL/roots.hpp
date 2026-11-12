
#pragma once

#include <data/math/number/NTL/prime.hpp>

#include <data/arithmetic.hpp>
#include <data/math/root.hpp>

namespace data::math::number::NTL {
    using namespace ::NTL;
}

namespace data::math::number::NTL {

    set<N> roots (const N &, uint64 pow);
    set<Z> roots (const Z &, uint64 pow);

    set<N> sqrt_mod (const N &, const N &);
    set<N> sqrt_mod (const Z &, const N &);

}

namespace data::math::def {

    template <uint64 pow> struct root<N, pow> {
        set<N> operator () (const N &n) {
            return number::NTL::roots (n, pow);
        }
    };

    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator () (const Z &n) {
            return number::NTL::roots (n, pow);
        }
    };

    template <Integer X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<Z, pow> {} (data::math::convert<Z> (n)));
        }
    };

    template <Natural X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<N, pow> {} (data::math::convert<N> (n)));
        }
    };

    template <Signed X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<Z, pow> {} (data::math::convert<Z> (n)));
        }
    };

    template <Unsigned X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<N, pow> {} (data::math::convert<N> (n)));
        }
    };

    template <> struct root_mod<N, 2, N> {
        set<N> operator () (const N &n, const N &m) {
            return number::NTL::sqrt_mod (n, m);
        }
    };

    template <> struct root_mod<Z, 2, N> {
        set<N> operator () (const Z &n, const N &m) {
            return number::NTL::sqrt_mod (n, m);
        }
    };

    template <Integer X, Natural mod> struct root_mod<X, 2, mod> {
        set<mod> operator () (const X &n, const mod &m) {
            return set<mod> (root_mod<Z, 2, N> {} (data::math::convert<Z> (n), data::math::convert<N> (m)));
        }
    };

    template <Natural X, Natural mod> struct root_mod<X, 2, mod> {
        set<mod> operator () (const X &n, const mod &m) {
            return set<mod> (root_mod<N, 2, N> {} (data::math::convert<N> (n), data::math::convert<N> (m)));
        }
    };

    template <Signed X> struct root_mod<X, 2, X> {
        set<X> operator () (const X &n, const X &m) {
            return set<X> (root_mod<Z, 2, N> {} (data::math::convert<Z> (n), data::math::convert<N> (m)));
        }
    };

    template <Unsigned X> struct root_mod<X, 2, X> {
        set<X> operator () (const X &n, const X &m) {
            return set<X> (root_mod<N, 2, N> {} (data::math::convert<N> (n), data::math::convert<N> (m)));
        }
    };

}

