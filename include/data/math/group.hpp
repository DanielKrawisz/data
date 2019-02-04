#ifndef DATA_MATH_GROUP_HPP
#define DATA_MATH_GROUP_HPP

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
