// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/N.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::gmp {
        
    Z::Z() : MPZ{MPZInvalid} {
        mpz_init(&MPZ);
    }
    
    Z::Z(N n) : MPZ{n.Value.MPZ} {}
    
    Z::Z(const string& x) : MPZ{MPZInvalid} {
        if (!encoding::integer::valid(x)) return;
        mpz_init(&MPZ);
        if (x == "") return;
        if (encoding::integer::negative(x)) {
            mpz_set_str(&MPZ, x.c_str() + 1, 0);
            MPZ._mp_size *= -1;
        } else mpz_set_str(&MPZ, x.c_str(), 0);
        
    }

}
