// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_SQRT
#define DATA_MATH_NUMBER_BYTES_SQRT

#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/sqrt.hpp>

namespace data::math {
    template <endian::order r, std::unsigned_integral word, uint64 pow>
    struct root<number::N_bytes<r, word>, pow> {
        set<number::N_bytes<r, word>> operator () (const number::N_bytes<r, word> &n) {
            return set<number::N_bytes<r, word>> (root<N, pow> {} (N (n)));
        }
    };
    
    template <endian::order r, number::complement zz, std::unsigned_integral word, uint64 pow>
    struct root<number::Z_bytes<r, zz, word>, pow> {
        set<number::Z_bytes<r, zz, word>> operator () (const number::Z_bytes<r, zz, word> &z) {
            return set<number::Z_bytes<r, zz, word>> (root<Z, pow> {} (Z (z)));
        }
    };

}

#endif

