// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/math/ordered.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            template <typename N>
            struct natural : ordered<N> {
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

                // division by powers of two.
                static bool rshift(N a, N b) {
                    return a >> b;
                }

                // multiplication by powers of two. 
                static bool lshift(N a, N b) {
                    return a << b;
                }
            };
            
        }
    
    }

}

#endif
