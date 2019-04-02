#ifndef DATA_MATH_LINEAR_SPACE_HPP
#define DATA_MATH_LINEAR_SPACE_HPP

#include <data/math/module.hpp>
#include <data/math/field.hpp>

namespace data {
    
    namespace math {
        
        namespace linear {
            
            template <typename V, typename S>
            struct space {
                static const module<V, S> r1{};
                static const field<S> r2{};
                    
                V times(V v, S s) const {
                    return v * s;
                }
                    
                V plus(V a, V b) const {
                    return a + b;
                }
                    
                V zero() const {
                    return 0;
                }
            };
            
        }
    
    }
    
}

#endif
