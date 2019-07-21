// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN
#define DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/data.hpp>

namespace data {
    
    namespace math {
        
        namespace number {
            template <typename ...> struct extended_euclidian;
            
            template <typename Z>
            struct extended_euclidian<Z> {
                Z GCD;
                Z BezoutS;
                Z BezoutT;
                
                bool valid() const {
                    return valid(GCD) && valid(BezoutS) && valid(BezoutT);
                }
                
                extended_euclidian(Z a, Z b, Z gcd, Z s, Z t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
                    if (!(a | gcd && b | gcd && gcd == a * s + b * t)) throw 0;
                }
                
            private:
                extended_euclidian(Z gcd, Z q, Z r) : GCD{gcd}, division<Z>{q, r} {}
                extended_euclidian() : GCD{}, division<Z>{} {} 
                
                struct sequence {
                    division<Z> Div;
                    Z S;
                    Z T;
                };
                
                // assume prev.Div.Remainder > current.Div.Remainder.
                // we can do that because it's a private function. 
                static sequence loop(const sequence prev, const sequence current) {
                    division<Z> div = prev.Div.Remainder / current.Div.Remainder;
                    if (div.Remainder == 0) return current;
                    return loop(current, {div, {prev.S - current.S * div.Quotient, prev.T - current.T * div.Quotient}});
                }
                
                static sequence init(const Z R0, const Z R1) {
                    return loop(sequence{{0, R0}, 1, 0}, sequence{{0, R1}, 0, 1});
                }
                
                static sequence pre(const Z Dividend, const Z Divisor) {
                    if (Dividend > Divisor) return init(Dividend, Divisor);
                    sequence euclidian = init(Divisor, Dividend);
                    return {euclidian.Div, euclidian.T, euclidian.S};
                }
                
            public: 
                static extended_euclidian algorithm(const Z Dividend, const Z Divisor) {
                    sequence euclidian = pre(Dividend, Divisor);
                    return {euclidian.Div.Quotient, euclidian.S, euclidian.T};
                }
                
                static division<Z> divide(const Z Dividend, const Z Divisor) {
                    return pre(Dividend, Divisor).Div;
                }
            };
            
            template <typename N, typename Z>
            struct extended_euclidian<N, Z> {
                N GCD;
                Z BezoutS;
                Z BezoutT;
                
                bool valid() const {
                    return valid(GCD) && valid(BezoutS) && valid(BezoutT);
                }
                
                extended_euclidian(N a, N b, N gcd, Z s, Z t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
                    if (!(a | gcd && b | gcd && gcd == a * s + b * t)) throw 0;
                }
                
            private:
                extended_euclidian(N gcd, Z q, Z r) : GCD{gcd}, division<Z>{q, r} {}
                extended_euclidian() : GCD{}, division<Z>{} {} 
                
                struct sequence {
                    division<N> Div;
                    Z S;
                    Z T;
                };
                
                // assume prev.Div.Remainder > current.Div.Remainder.
                // we can do that because it's a private function. 
                static sequence loop(const sequence prev, const sequence current) {
                    division<Z> div = prev.Div.Remainder / current.Div.Remainder;
                    if (div.Remainder == 0) return current;
                    return loop(current, {div, {prev.S - current.S * div.Quotient, prev.T - current.T * div.Quotient}});
                }
                
                static sequence init(const N R0, const N R1) {
                    return loop(sequence{{0, R0}, 1, 0}, sequence{{0, R1}, 0, 1});
                }
                
                static sequence pre(const N Dividend, const N Divisor) {
                    if (Dividend > Divisor) return init(Dividend, Divisor);
                    sequence euclidian = init(Divisor, Dividend);
                    return {euclidian.Div, euclidian.T, euclidian.S};
                }
                
            public: 
                static extended_euclidian algorithm(const N Dividend, const N Divisor) {
                    sequence euclidian = pre(Dividend, Divisor);
                    return {euclidian.Div.Quotient, euclidian.S, euclidian.T};
                }
                
                static division<N> divide(const N Dividend, const N Divisor) {
                    return pre(Dividend, Divisor).Div;
                }
            };
        
        }
    
    }

}

#endif
