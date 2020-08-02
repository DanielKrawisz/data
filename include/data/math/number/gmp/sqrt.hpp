// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_SQRT
#define DATA_MATH_NUMBER_GMP_SQRT

#include <data/math/number/gmp/N.hpp>
#include <data/math/number/sqrt.hpp>

namespace data::math::number::gmp {
        
    N sqrt(const N& n);
    N root(const N& n, uint32 p);

}

namespace data::math::number {
        
    template <> struct sqrt<gmp::N, gmp::N> {
        gmp::N operator()(const gmp::N& n) {
            return gmp::sqrt(n);
        }
    };
        
    template <> struct sqrt<gmp::N, gmp::Z> {
        gmp::N operator()(const gmp::Z& z) {
            if (z < 0) return gmp::N{};
            return gmp::sqrt(gmp::N{z});
        }
    };

}

#endif
