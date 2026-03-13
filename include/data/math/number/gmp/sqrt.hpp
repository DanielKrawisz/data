// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_SQRT
#define DATA_MATH_NUMBER_GMP_SQRT

#include <data/encoding/integer.hpp>
#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/math/root.hpp>

namespace data::math::number::GMP {
    
    set<N> root (const N &n, uint32 p);
    set<Z> root (const Z &n, uint32 p);

}

namespace data::math {
    
    template <uint64 pow> struct root<N, pow> {
        set<N> operator () (const N &n) {
            return number::GMP::root (n, pow);
        }
    };
    
    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator () (const Z &z) {
            return number::GMP::root (z, pow);
        }
    };

}

#endif
