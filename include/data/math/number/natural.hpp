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
        
    };
    
    template <typename N, typename> struct construct;
    
    template <uint32 ...> struct Digits;
    
    template <typename> struct Successor;
    
    template <typename...> struct Plus;
    
    template <typename...> struct Times;
    
    template <typename, typename> struct Power;

    template <typename N> struct construct<N, Digits<>> {
        N operator()(N x) const {
            return x;
        }
        
        N operator*() {
            return 0;
        }
    };
    
    template <typename N, uint32 big, uint32 ... rest> struct construct<N, Digits<big, rest...>> {
        N operator()(N x) const {
            return construct<N, Digits<rest...>>{}((x << 32) + N{big});
        }
        
        N operator*() {
            return operator()(0);
        }
    };
    
    template <typename N, typename X> struct construct<N, Successor<X>> {
        N operator*() {
            return *construct<N, X>{} + 1;
        }
    };
    
    template <typename N, typename first, typename second> struct construct<N, Plus<first, second>> {
        N operator*() {
            return *construct<N, first>{} + *construct<N, second>{};
        }
    };
    
    template <typename N, typename first, typename... rest> struct construct<N, Plus<first, rest...>> {
        N operator*() {
            return construct<N, first>{}* + *construct<N, Plus<rest...>>{};
        }
    };
    
    template <typename N, typename first, typename second> struct construct<N, Times<first, second>> {
        N operator*() {
            return *construct<N, first>{} * *construct<N, second>{};
        }
    };
    
    template <typename N, typename first, typename... rest> struct construct<N, Times<first, rest...>> {
        N operator*() {
            return *construct<N, first>{} * *construct<N, Times<rest...>>{};
        }
    };
    
    template <typename N, typename first, typename second> struct construct<N, Power<first, second>> {
        N operator*() {
            return *construct<N, first>{} ^ *construct<N, second>{};
        }
    };

}

#endif
