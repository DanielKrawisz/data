#ifndef DATA_MATH_LINEAR_ALGEBRA_HPP
#define DATA_MATH_LINEAR_ALGEBRA_HPP

#include "module.hpp"
#include "field.hpp"

namespace data {
    
    namespace math {
            
        template <typename V, typename S>
        struct vector {
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
            
        template <typename M, typename v1, typename v2, typename S>
        struct matrix {
            static const vector<M, S> r1;
            static const vector<v1, S> r2;
            static const vector<v2, S> r3;
                
            v2 multiply(M m, v1 v) const {
                return m * v;
            }
                
            M multiply(M m, M n) const {
                return m * n;
            }
                
        };
    
    }
    
}
