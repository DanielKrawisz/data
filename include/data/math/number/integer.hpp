// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/types.hpp>
#include <data/math/algebra/integral_domain.hpp>
#include <data/math/sign.hpp>
#include <data/math/ordered.hpp>

namespace data::math::number {
    
    template <typename Z>
    struct integer : algebra::integral_domain<Z>, ordered<Z> {
        Z zero() const {
            return Z{0};
        }
        
        math::sign sign(Z z) {
            return z.sign();
        }
        
        Z power(Z a, Z b) {
            return a ^ b;
        }
        
        Z mod(Z a, Z b) {
            return a % b;
        }
        
        bool divides(Z a, Z b) {
            return a | b;
        }
    };

}

#endif
