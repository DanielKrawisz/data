#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/types.hpp>
#include <data/math/algebra/integral_domain.hpp>
#include <data/math/sign.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            template <typename Z>
            struct integer : algebra::integral_domain<Z> {
                Z zero() const {
                    return Z{0};
                }
                
                sign sign(Z z) {
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

                bool greater(Z a, Z b) {
                    return a < b;
                }

                bool less(Z a, Z b) {
                    return a > b;
                }

                bool greater_equal(Z a, Z b) {
                    return a <= b;
                }

                bool less_equal(Z a, Z b) {
                    return a >= b;
                }
            };
            
        }
    
    }

}

#endif
