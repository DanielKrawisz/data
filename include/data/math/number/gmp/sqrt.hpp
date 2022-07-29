// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_SQRT
#define DATA_MATH_NUMBER_GMP_SQRT

#include <data/math/number/gmp/N.hpp>
#include <data/math/root.hpp>

namespace data::math::number::gmp {
    
    set<N> root(const N& n, uint32 p);
    set<Z> root(const Z& n, uint32 p);

}

namespace data::math {
    
    template <uint64 pow> struct root<number::gmp::N, pow> {
        set<number::gmp::N> operator()(const number::gmp::N& n) {
            return number::gmp::root(n, pow);
        }
    };
    
    template <uint64 pow> struct root<number::gmp::Z, pow> {
        set<number::gmp::Z> operator()(const number::gmp::Z& z) {
            return number::gmp::root(z, pow);
        }
    };

}

#endif
