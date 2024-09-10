#ifndef DATA_MATH_DIFFERENTIABLE_HPP
#define DATA_MATH_DIFFERENTIABLE_HPP

#include <data/function.hpp>

namespace data {
    
    namespace math {
        
        template <typename f, typename A, typename B>
        struct differentiable : public function::definition<f, A, B> {
            constexpr static function::definition<typename f::derivative, A, B> d{};
        };
    
    }
    
}

#endif 
