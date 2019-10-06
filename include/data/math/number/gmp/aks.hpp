// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_AKS
#define DATA_MATH_NUMBER_GMP_AKS

#include <data/math/number/prime.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/Z.hpp>

namespace data::math::number {
    
    template <> struct AKS<gmp::N> {
        prime<gmp::N> is_prime(const gmp::N);
    };
    
    template <> struct AKS<gmp::Z> {
        prime<gmp::N> is_prime(const gmp::Z);
    };
    
}

#endif

