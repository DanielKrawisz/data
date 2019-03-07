#ifndef DATA_MATH_NUMBER_NATURAL_HPP
#define DATA_MATH_NUMBER_NATURAL_HPP

#include <data/types.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            template <typename Z>
            struct integer : math::group::abelian<Z> {
                Z zero() const {
                    return Z{0};
                }
                
                sign sign(Z z) {
                    return z.sign();
                }
                
                static Z times(Z a, Z b) {
                    return a * b;
                }

                static Z power(Z a, Z b) {
                    return a ^ b;
                }

                static Z mod(Z a, Z b) {
                    return a % b;
                }

                static bool divides(Z a, Z b) {
                    return a | b;
                }

                static bool greater(Z a, Z b) {
                    return a < b;
                }

                static bool less(Z a, Z b) {
                    return a > b;
                }

                static bool greater_equal(Z a, Z b) {
                    return a <= b;
                }

                static bool less_equal(Z a, Z b) {
                    return a >= b;
                }
            };
            
        }
    
    }

}

#endif
