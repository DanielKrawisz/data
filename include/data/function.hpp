#ifndef DATA_FUNCTION_HPP
#define DATA_FUNCTION_HPP

#include <data/types.hpp>

namespace data {
    
    template<typename f, typename x, typename y, uint32_t ...> struct function;
    
    // function from x to y. 
    template<typename f, typename x, typename y> struct function<f, x, y> {
        // type f must be callable, and must take an argument of type x
        // and return an argument of type y. 
        static const y callable(const f fun, const x arg) {
            return fun(arg);
        }
    };
    
    // function from x^n to y^m. 
    template<typename f, typename x, typename y, uint32_t n, uint32_t m>
    struct function<f, x, y, n, m> {
        // type f must be callable, and must take an argument of type x
        // and return an argument of type y. 
        static const array<y, m> callable(const f fun, const array<x, n> arg) {
            return fun(arg);
        } 
    };

}

#endif
