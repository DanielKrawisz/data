// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/types.hpp>
#include <data/math/countable.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/ordered.hpp>
#include <data/math/ring.hpp>
#include <data/math/division.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>

namespace data::interface {
    template <typename N>
    struct natural : 
        countable<N>, ordered<N>, 
        math::associative<plus<N>, N>, 
        math::commutative<plus<N>, N>, 
        math::associative<times<N>, N>, 
        math::commutative<times<N>, N> {
    private:
        using require_plus_operator = typename std::enable_if<meta::has_plus_operator<N>::value, void>::type;
        using require_minus_operator = typename std::enable_if<meta::has_minus_operator<N>::value, void>::type;
        using require_times_operator = typename std::enable_if<meta::has_times_operator<N>::value, void>::type;
        using require_division_operator = typename std::enable_if<meta::has_divide_operator<N, N, N>::value, void>::type;
        using require_mod_operator = typename std::enable_if<meta::has_mod_operator<N, N, N>::value, void>::type;
    };
}

namespace data::math::number::natural {
    
    // Generic division algorithm. 
    // TODO this assumes that << and >> are multiplications and divisions by 2. 
    // Not necessarily the case. 
    template <typename N>
    static division<N> divide(const N Dividend, const N Divisor) {
        if (Divisor == 0) throw division_by_zero{};
        if (Divisor == 1) return {Dividend, 0};
        N pow = 1;
        N exp = Divisor;
        N remainder = Dividend;
        N quotient = 0;
        while (exp <= remainder) {
            exp<<=1;
            pow<<=1;
        } 
        while (pow > 0) {
            while (exp > remainder) {
                exp>>=1;
                pow>>=1;
            }
            quotient += pow;
            remainder -= exp;
        }
        return {quotient, remainder};
    }
    /*
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
    */
}

#endif
