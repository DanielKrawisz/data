#ifndef DATA_MATH_ALGEBRA_INTEGRAL_DOMAIN
#define DATA_MATH_ALGEBRA_INTEGRAL_DOMAIN

#include <data/math/ring.hpp>

namespace data {
    
    namespace math {
        
        namespace algebra {
            
            template <typename x>
            struct integral_domain : public ring<x> {
                // integral domains are not supposed to have 
                // zero divisors. 
                x times(x a, x b) const override {
                    x result = a * b;
                    if (result == 0) throw 0;
                    return result;
                }
            };
        
        }
    
    }
    
}

#endif 
