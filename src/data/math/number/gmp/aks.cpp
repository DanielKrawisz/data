// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/aks.hpp>
#include <rotella/akslib.h>

namespace data::math::number::GMP {
    
    bool aks_is_prime(const Z z) {
        return ::aks(mpz_class(&z.MPZ[0]));
    }
    
}

