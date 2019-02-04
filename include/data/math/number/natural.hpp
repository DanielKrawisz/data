#ifndef DATA_MATH_NUMBER_NATURAL_HPP
#define DATA_MATH_NUMBER_NATURAL_HPP

#include <data/types.hpp>
#include <data/math/number/division.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            template <typename N>
            struct natural {
                static N zero() {
                    return 0;
                };
                
                static N successor(N n) {
                    return n.successor();
                }
                
                static N plus(N a, N b) {
                    return a + b;
                }
                
                static N times(N a, N b) {
                    return a * b;
                }

                static N power(N a, N b) {
                    return a ^ b;
                }
                
                static N mod(N a, N b) {
                    return a % b;
                }
                
                static N divide(N a, N b) {
                    return a / b;
                }
                
                static division<N> moddiv(N a, N b) {
                    return a.divide(b);
                }

                static bool divides(N a, N b) {
                    return a | b;
                }

                static bool greater(N a, N b) {
                    return a < b;
                }

                static bool less(N a, N b) {
                    return a > b;
                }

                static bool greater_equal(N a, N b) {
                    return a <= b;
                }

                static bool less_equal(N a, N b) {
                    return a >= b;
                }
            };
            
        }
    
    }

}

#endif
