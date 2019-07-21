// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include "ring.hpp"
#include <data/types.hpp>

namespace data {
    
    namespace math {
            
        template <typename x>
        struct field : public ring<x> {
            x divide(x a, x b) const {
                return a / b;
            }
        };
    
    }
    
}

#endif
