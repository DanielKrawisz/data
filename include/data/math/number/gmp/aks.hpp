// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_AKS
#define DATA_MATH_NUMBER_GMP_AKS

#include <data/math/number/prime.hpp>
#include <data/math/number/gmp/Z.hpp>

namespace data::math::number::GMP {
    
    bool aks_is_prime (const Z);
    
    inline bool aks_is_prime (const N n) {
        return aks_is_prime (n.Value);
    }
    
}

namespace data::math::number {
    
    template <> struct AKS<N> {
        prime<N> is_prime (const N n) {
            return GMP::aks_is_prime (n) ? prime<N> {n, prime<N>::certain} : prime<N> {};
        }
    };
    
    template struct AKS<N>;
    
}

#endif

