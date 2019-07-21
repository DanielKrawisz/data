// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/types.hpp>

namespace data {
    
    namespace math {
        
        namespace group {
            
            template <typename x>
            struct abelian {
                x zero() const {
                    return x{};
                }

                x plus(x a, x b) const {
                    return a + b;
                }

                x minus(x a, x b) const {
                    return a - b;
                }
                
                x negative(x a) const {
                    return -a;
                }
            };
            
            template <typename x>
            struct nonabelian {
                x identity() const {
                    return x::identity();
                }
                
                x times(x a, x b) const {
                    return a * b;
                }
                
                x inverse(x a) const {
                    return a.inverse();
                }
            };
            
        }
    
    }
    
}

#endif 
