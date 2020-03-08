// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTION
#define DATA_FUNCTION

#include <data/types.hpp>

namespace data::meta {
    
    template <typename range, typename function, typename ... domain> class is_function;
    
    template <typename range, typename function, typename domain>
    class is_function<range, function, domain> {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>()(std::declval<const domain>())), range>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<function>(0)), yes>::value;
    };
    
    template <typename range, typename function, typename x, typename y>
    class is_function<range, function, x, y> {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>()(std::declval<const x>(), std::declval<const y>())), range>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<function>(0)), yes>::value;
    };
    
    template <typename F, typename x> using is_binary_operation = is_function<x, F, x, x>;
    
}

namespace data {
    
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
