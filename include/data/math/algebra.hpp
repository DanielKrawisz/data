#ifndef DATA_MATH_ALGEBRA
#define DATA_MATH_ALGEBRA

#include <data/math/linear/space.hpp>
#include <data/math/nonnegative.hpp>

namespace data {
    
    namespace math {
            
        template <typename x, typename r>
        struct algebra : public linear::space<x, r> {
            x times(x a, x b) const {
                return a ^ b;
            }
        };
            
        template <typename x, typename r>
        struct division_algebra : public algebra<x, r> {
            x divide(x a, x b) const {
                return a / b;
            }
        };
            
        template <typename x, typename r>
        struct normed_division_algebra : public ring<x> {
            nonnegative<r> norm(x a) const {
                return a.norm();
            }
        };
    
    }
    
}

#endif
