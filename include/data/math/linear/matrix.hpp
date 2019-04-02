#ifndef DATA_MATH_LINEAR_MATRIX_HPP
#define DATA_MATH_LINEAR_MATRIX_HPP

#include <data/math/linear/space.hpp>

namespace data {
    
    namespace math {
        
        namespace linear {
                
            template <typename M, typename v1, typename v2, typename S>
            struct matrix {
                static const space<M, S> r1;
                static const space<v1, S> r2;
                static const space<v2, S> r3;
                    
                v2 multiply(M m, v1 v) const {
                    return m * v;
                }
                    
                M multiply(M m, M n) const {
                    return m * n;
                }
                    
            };
            
        }
    
    }
    
}

#endif
