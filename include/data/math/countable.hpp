// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COUNTABLE
#define DATA_MATH_COUNTABLE

#include <data/types.hpp>
#include <type_traits>

namespace data {
    
    namespace meta {
        
        template <typename X>
        class has_zero_value {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(U::zero()), U>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X, bool has_successor_method> struct zero;
        
        template <typename X> struct zero<X, true> {
            
            X operator()() {
                return X::zero();
            }
        };
        
        template <typename X>
        class has_preincrement_operator {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(++std::declval<const U>()), U&>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X, bool has_successor_method> struct successor;
        
        template <typename X> struct successor<X, true> {
            
            X operator()(const X& x) {
                return ++x;
            }
        };
        
    }
    
    namespace interface {
        
        template <typename L>
        class countable {
            using require_preincrement_operator = typename std::enable_if<meta::has_preincrement_operator<L>::value, void>::type;
            using require_zero_value = typename std::enable_if<meta::has_zero_value<L>::value, void>::type;
        }; 
        
    }

    template <typename X> struct successor {
        X operator()(const X& x) {
            return meta::successor<X, meta::has_preincrement_operator<X>::value>{}(x);
        }
    };

}

#endif
