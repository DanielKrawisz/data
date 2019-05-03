#ifndef DATA_MATH_LINEAR_INNER
#define DATA_MATH_LINEAR_INNER

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
