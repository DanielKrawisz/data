#ifndef DATA_MATH_AFFINE_HPP
#define DATA_MATH_AFFINE_HPP

#include <data/math/linear/space.hpp>

namespace data {
    
    namespace math {
    
        template<typename A, typename V, typename S>
        struct affine : public linear::space<V, S> {
            A plus(A a, V v) {
                return a + v;
            }
            
            V minus(A a, A b) {
                return a - b;
            }
        };
    
    }
    
}

#endif 
