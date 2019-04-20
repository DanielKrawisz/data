#ifndef DATA_FUNCTION_HPP
#define DATA_FUNCTION_HPP

#include <data/types.hpp>

namespace data {
    
    namespace function {
    
        template<typename f, typename x, typename y, uint32_t ...> struct definition;
        
        // function from x to y. 
        template<typename f, typename x, typename y> struct definition<f, x, y> {
            // type f must be callable, and must take an argument of type x
            // and return an argument of type y. 
            static const y callable(const f fun, const x arg) {
                return fun(arg);
            }
        };
        
        // function from x^n to y^m. 
        template<typename f, typename x, typename y, uint32_t n, uint32_t m>
        struct definition<f, x, y, n, m> {
            // type f must be callable, and must take an argument of type x
            // and return an argument of type y. 
            static const array<y, m> callable(const f fun, const array<x, n> arg) {
                return fun(arg);
            } 
        };
        
        template <typename x, typename y, typename f, typename g>
        struct composition {
            y operator()(x input) {
                return f{}(g{}(input));
            }
        };
    
    }

}

#endif
