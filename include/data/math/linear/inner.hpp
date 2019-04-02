#ifndef DATA_MATH_LINEAR_INNER_HPP
#define DATA_MATH_LINEAR_INNER_HPP

#include <data/math/linear/space.hpp>

namespace data {
    
    namespace math {
        
        namespace linear {
            
            template <typename V, typename S>
            struct inner : space<V, S> {
                
                S norm(V a, V b) {
                    return a.dot(b);
                }
                
            };
            
        }
    
    }
    
}

#endif
