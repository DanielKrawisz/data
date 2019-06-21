// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTION
#define DATA_FUNCTION

#include <data/types.hpp>
#include <data/tuple.hpp>

namespace data {
    
    // return type for functions that can never really return. 
    struct unconstructable final {
        unconstructable() = delete;
    };
    
    namespace function {
        
        // function from x to y. 
        template<typename f, typename y, typename ... x> struct definition {
            // type f must be callable, and must take an argument of type x
            // and return an argument of type y. 
            static y callable(f fun, x... arg) {
                return fun(arg...);
            }
        };
    
        // function from x to y. 
        template<typename y, typename x> struct abstract {
            virtual y operator()(const x) const noexcept = 0;
            
            using domain = x;
            using range = y;
            
            constexpr static definition<abstract&, x, y> is_function{};
        };
        
        // functions to contradictions are not required to be noexcept.
        template <typename x> struct abstract<x, unconstructable> {
            virtual const unconstructable operator()(const x) const = 0;
            
            using domain = x;
            using range = unconstructable;
            
            constexpr static definition<abstract&, x, unconstructable> is_function{};
        };
    
    }
    
    // It is always possible to construct the identity function. 
    template <typename X>
    struct identity : function::abstract<X, X> {
        X operator()(X x) const {
            return x;
        }
    };
    
    // Given an element of Y, we can construct a function from X to Y. 
    template <typename X, typename Y>
    struct constant : function::abstract<X, Y> {
        Y Constant;
        
        constant(Y c) : Constant{c} {}
        
        Y operator()(X) const {
            return Constant;
        }
    };

}

#endif
