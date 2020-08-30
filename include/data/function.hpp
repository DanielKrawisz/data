// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTION
#define DATA_FUNCTION

#include <concepts>
#include <data/types.hpp>

namespace data {
    
    template< typename F, typename output, typename... Args >
    concept function = std::regular_invocable<F, Args...> && requires(F&& f, Args&&... args) {
        {std::invoke(std::forward<F>(f), std::forward<Args>(args)...)} -> std::same_as<output>;
    };
    
    // It is always possible to construct the identity function. 
    template <typename X>
    struct identity {
        X operator()(const X& x) const {
            return x;
        }
    };
    
    // Given an element of Y, we can construct a function from X to Y. 
    template <typename X, typename Y>
    struct constant {
        Y Constant;
        
        constant(Y c) : Constant{c} {}
        
        Y operator()(const X&) const {
            return Constant;
        }
    };
    
    template <typename F, typename X, typename Y> struct inverse;
    
    template <typename X> struct inverse<identity<X>, X, X> {
        X operator()(const X& x) {
            return identity<X>{}(x);
        }
    };
    
    template <typename F, typename X> struct action {
        F Function;
        X Value;
        X operator()(const X& x) {
            return Function(Value, x);
        }
    };

}

#endif
