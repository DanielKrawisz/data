// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/math/ordered.hpp>

namespace data::math::number {
    
    template <typename N>
    struct natural : ordered<N> {
        static N zero() {
            return 0;
        };
        
        static N successor(N n) {
            return n.successor();
        }
        
        static N plus(N a, N b) {
            return a + b;
        }
        
        static N times(N a, N b) {
            return a * b;
        }
        
        static N power(N a, N b) {
            return a ^ b;
        }
        
        static N mod(N a, N b) {
            return a % b;
        }
        
        static N divide(N a, N b) {
            return a / b;
        }
        
        static division<N> moddiv(N a, N b) {
            return a.divide(b);
        }
        
        // division by powers of two.
        static bool rshift(N a, N b) {
            return a >> b;
        }
        
        // multiplication by powers of two. 
        static bool lshift(N a, N b) {
            return a << b;
        }
        
        template <typename> struct construct;
        
        template <uint32 ...> struct Digits;
        
        template <typename> struct Successor;
        
        template <typename...> struct Plus;
        
        template <typename...> struct Times;
        
        template <typename, typename> struct Power;
        
        template <> struct construct<Digits<>> {
            N operator()(N x) const {
                return x;
            }
            
            N operator*() {
                return 0;
            }
        };
        
        template <uint32 big, uint32 ... rest> struct construct<Digits<big, rest...>> {
            N operator()(N x) const {
                return construct<Digits<rest...>>{}((x << 32) + N{big});
            }
            
            N operator*() {
                return operator()(0);
            }
        };
        
        template <typename X> struct construct<Successor<X>> {
            N operator*() {
                return *construct<X>{} + 1;
            }
        };
        
        template <typename first, typename second> struct construct<Plus<first, second>> {
            N operator*() {
                return *construct<first>{} + *construct<second>{};
            }
        };
        
        template <typename first, typename... rest> struct construct<Plus<first, rest...>> {
            N operator*() {
                return construct<first>{}* + *construct<Plus<rest...>>{};
            }
        };
        
        template <typename first, typename second> struct construct<Times<first, second>> {
            N operator*() {
                return *construct<first>{} * *construct<second>{};
            }
        };
        
        template <typename first, typename... rest> struct construct<Times<first, rest...>> {
            N operator*() {
                return *construct<first>{} * *construct<Times<rest...>>{};
            }
        };
        
        template <typename first, typename second> struct construct<Power<first, second>> {
            N operator*() {
                return *construct<first>{} ^ *construct<second>{};
            }
        };
        
    };
    
}

#endif
