// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_COMPLEMENT
#define DATA_MATH_NUMBER_COMPLEMENT

#include <ostream>

namespace data::math::number {
    
    enum complement {
        nones = 0, 
        ones = 1, 
        twos = 2
    };
    
    std::ostream inline &operator<<(std::ostream &o, complement c) {
        switch(c) {
            default: return o << "none's complement"; 
            case ones: return o << "one's complement";
            case twos: return o << "two's complement";
        }
    }
    
}

#endif
