#ifndef DATA_MATH_DIFFERENTIABLE_HPP
#define DATA_MATH_DIFFERENTIABLE_HPP

#include <data/function.hpp>

namespace data {
    
    namespace math {
    
        template<typename f, typename x, typename y, uint32_t ...> struct differentiable;
        
        template <typename f, typename A, typename B>
        struct differentiable<f, A, B> : public function<f, A, B> {
            
            template <typename f2>
            f2 d(f fun) {
                f2 df = derivative(fun);
                function<f2, A, B> required{};
                return df;
            }

        };
     
        template <typename f, typename A, typename B, uint32_t n, uint32_t m>
        struct differentiable<f, A, B, n, m> : public function<f, A, B> {
            
            template <typename f2>
            f2 d(f fun, uint32_t i) {
                f2 df = derivative(fun, i);
                function<f2, A, B, n, m> required{};
                return df;
            }
            
        };
    
    }
    
}

#endif 

