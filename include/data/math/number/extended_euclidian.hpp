// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN
#define DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/data.hpp>

namespace data::math {
    struct invalid_proof : std::exception {};
    namespace number::euclidian {
        namespace extended {
            template <typename N, typename Z>
            struct algorithm {
                N GCD;
                Z S;
                Z T;
                
                bool valid() const {
                    return valid(GCD) && valid(S) && valid(T);
                }
                
                algorithm(N a, N b, Z gcd, Z s, Z t) : GCD{gcd}, S{s}, T{s} {
                    if (!(a | gcd && b | gcd && gcd == a * s + b * t)) throw invalid_proof{};
                }
                
            private:
                algorithm(N gcd, Z q, Z r) : GCD{gcd}, division<Z>{q, r} {}
                algorithm() : GCD{}, division<Z>{} {} 
                 
                struct sequence {
                    division<N> Div;
                    Z S;
                    Z T;
                };
                
                // must provide prev.Div.Remainder > current.Div.Remainder.
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
                static algorithm run(const N Dividend, const N Divisor) {
                    sequence e = algorithm::pre(Dividend, Divisor);
                    return {e.Div.Quotient, e.S, e.T};
                }
                
                static algorithm divide(const N Dividend, const N Divisor) {
                    return pre(Dividend, Divisor).Div;
                }
            };
        }
    }
}

#endif
