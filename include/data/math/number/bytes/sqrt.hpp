// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_SQRT
#define DATA_MATH_NUMBER_BYTES_SQRT

#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/sqrt.hpp>

namespace data::math {
    template <endian::order r, uint64 pow> 
    struct root<number::N_bytes<r>, pow> {
        set<number::N_bytes<r>> operator()(const number::N_bytes<r>& n) {
            set<number::gmp::N> roots = root<number::gmp::N, pow>{}(N(n));
            set<number::N_bytes<r>> n_roots{};
            for(const N &x : roots.values()) n_roots = data::insert(n_roots, N_bytes<r>(x));
            return n_roots;
        }
    };
    
    template <endian::order r, uint64 pow> 
    struct root<number::Z_bytes<r>, pow> {
        set<number::Z_bytes<r>> operator()(const number::Z_bytes<r>& z) {
            set<number::gmp::Z> roots = root<number::gmp::Z, pow>{}(Z(z));
            set<number::Z_bytes<r>> z_roots{};
            for(const Z &x : roots.values()) z_roots = data::insert(z_roots, Z_bytes<r>(x));
            return z_roots;
        }
    };

}

#endif

