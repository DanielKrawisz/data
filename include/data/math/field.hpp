#ifndef DATA_MATH_FIELD_HPP
#define DATA_MATH_FIELD_HPP

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
