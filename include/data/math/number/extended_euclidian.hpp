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
    namespace number{
        namespace euclidian {
            template <typename ...> struct extended;
            template <typename Z>
            struct extended<Z> {
                Z GCD;
                Z S;
                Z T;
                
                bool valid() const {
                    return valid(GCD) && valid(S) && valid(T);
                }
                static bool valid_proof(Z gcd, Z a, Z b, Z s, Z t) {
                    return gcd == a * s + b * t;
                }
                
                extended(Z a, Z b, Z gcd, Z s, Z t) : GCD{gcd}, S{s}, T{s} {
                    if (!valid_proof(gcd, a, b, s, t)) throw invalid_proof{};
                }
                
            private:
                extended() : GCD{}, division<Z>{} {} 
                 
                struct sequence {
                    division<Z> Div;
                    Z S;
                    Z T;
                };
                
                // must provide prev.Div.Remainder > current.Div.Remainder.
                static sequence loop(const sequence prev, const sequence current) {
                    division<Z> div = prev.Div.Remainder / current.Div.Remainder;
                    if (div.Remainder == 0) return current;
                    return loop(current, {div, {prev.S - current.S * div.Quotient, prev.T - current.T * div.Quotient}});
                }
                
                static sequence run(const Z r0, const Z r1) {
                    return loop(sequence{{0, r0}, 1, 0}, sequence{{0, r1}, 0, 1});
                }
            public:
                static extended algorithm(const Z a, const Z b) {
                    sequence e = a < b ? run(b, a) : run(a, b);
                    return {e.Div.Quotient, e.S, e.T};
                }
            
            };
            
            template <typename Z, typename N> 
            N abs(Z);
            
            template <typename N, typename Z>
            struct extended<N, Z> {
                N GCD;
                Z S;
                Z T;
                
                bool valid() const {
                    return extended<Z>{GCD, S, T}.valid();
                }
                
                extended(N a, N b, N gcd, Z s, Z t) : GCD{gcd}, S{s}, T{s} {
                    if (!extended<Z>::valid_proof(gcd, a, b, s, t)) throw invalid_proof{};
                }
                
                static extended algorithm(const N a, const N b) {
                    extended<Z> e = extended<Z>::algorithm(a, b);
                    return {abs<Z, N>(e.GCD), e.S, e.T};
                }
                
            };
        }
    }
}

#endif
