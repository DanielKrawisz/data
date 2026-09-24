
#pragma once

#include <data/math/number/NTL/Z.hpp>

#include <data/math/root.hpp>

namespace data::math::number::NTL {
    using namespace ::NTL;
}

namespace data::math::number::NTL {

    set<N> roots (const N &, uint64 pow);
    set<Z> roots (const Z &, uint64 pow);

}

namespace data::math {

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
            return set<X> (root<Z, pow> (convert<Z> (n)));
        }
    };

    template <Natural X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<N, pow> (convert<N> (n)));
        }
    };

}

