// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMPLEMENT
#define DATA_MATH_COMPLEMENT

#include <ostream>

namespace data::math::number {
    
    enum class complement {
        nones = 0, 
        ones = 1, 
        twos = 2
    };

    template <complement x>
    struct signed_complement {
        static const bool value = true;
    };

    template <>
    struct signed_complement<complement::nones> {
        static const bool value = false;
    };
    
    std::ostream inline &operator<<(std::ostream &o, complement c) {
        switch(c) {
            default: return o << "none's complement"; 
            case complement::ones: return o << "one's complement";
            case complement::twos: return o << "two's complement";
        }
    }
    
}

#endif
