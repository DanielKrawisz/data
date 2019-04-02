#ifndef DATA_MATH_LINEAR_DUAL_HPP
#define DATA_MATH_LINEAR_DUAL_HPP

#include <data/math/linear/space.hpp>

namespace data {
    
    namespace math {
        
        namespace linear {
            
            template <typename D, typename V, typename S>
            struct dual : space<V, S> {
                
                S apply(D d, V v) {
                    return d(v);
                }
                
            };
            
        }
    
    }
    
}

#endif

