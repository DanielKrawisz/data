// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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
    
        // function from x to y. 
        template<typename x, typename y> struct abstract {
            virtual y operator()(const x) const noexcept = 0;
        };
        
        // a contradiction is type that cannot exist. 
        struct unconstructable final {
            unconstructable() = delete;
        };
        
        // functions to contradictions are not required to be noexcept.
        template <typename x> struct abstract<x, unconstructable> {
            virtual const unconstructable operator()(const x) const = 0;
        };
    
    }

}

#endif
