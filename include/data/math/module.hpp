#ifndef DATA_MATH_MODULE_HPP
#define DATA_MATH_MODULE_HPP

#include "ring.hpp"

namespace data {
    
    namespace math {
            
        template <typename M, typename R>
        struct module {
            static const group::abelian<M> r1;
            static const ring<R> r2;
                
            M times(M m, R r) const {
                return m * r;
            }
        };

        template <typename M, typename R>
        struct algebra : public module<M, R> {                
            M times(M a, M b) const {
                return a * b;
            }
        };
    
    }
    
}

