// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN
#define DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN

#include <data/math/abs.hpp>
#include <data/math/number/integer.hpp>
#include <data/valid.hpp>

namespace data::math {
    struct invalid_proof : std::exception {};
    namespace number {
        namespace euclidian {
            template <typename ...> struct extended;
            template <typename Z>
            struct extended<Z> {
                Z GCD;
                Z BezoutS;
                Z BezoutT;
                
                bool valid() const {
                    return data::valid(GCD) && data::valid(BezoutS) && data::valid(BezoutT);
                }
                
                static bool valid_proof(Z gcd, Z a, Z b, Z s, Z t) {
                    return gcd == a * s + b * t;
                }
                
                extended(Z a, Z b, Z gcd, Z s, Z t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
                    if (!valid_proof(gcd, a, b, s, t)) throw invalid_proof{};
                }
                
            private:
                extended() : GCD{}, BezoutS{}, BezoutT{} {}
                
                extended(const Z gcd, const Z s, const Z t) : GCD{gcd}, BezoutS{s}, BezoutT{t} {} 
                
                struct sequence {
                    division<Z> Div;
                    Z BezoutS;
                    Z BezoutT;
                    
                    sequence operator/(const sequence& s) const {
                        auto div = integer::divide(Div.Remainder, s.Div.Remainder);
                        return {div, BezoutS - div.Quotient * s.BezoutS, BezoutT - div.Quotient * s.BezoutT};
                    }
                };
                
                // must provide prev.Div.Remainder > current.Div.Remainder.
                static extended loop(const sequence prev, const sequence current) {
                    sequence next = prev / current;
                    if (next.Div.Remainder == 0) return extended{current.Div.Remainder, current.BezoutS, current.BezoutT};
                    return loop(current, next);
                }
                
                // we know that a >= b
                static extended run(const Z a, const Z b) {
                    return loop(sequence{{0, a}, 1, 0}, sequence{{0, b}, 0, 1});
                }
                
            public:
                static extended algorithm(const Z a, const Z b) {
                    return a < b ? run(b, a) : run(a, b);
                }
            };
            
            template <typename N, typename Z>
            struct extended<N, Z> {
                N GCD;
                Z BezoutS;
                Z BezoutT;
                
                bool valid() const {
                    return data::valid(GCD) && data::valid(BezoutS) && data::valid(BezoutT);
                }
                
                extended(N a, N b, N gcd, Z s, Z t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
                    if (!extended<Z>::valid_proof(gcd, a, b, s, t)) throw invalid_proof{};
                }
                
                static extended algorithm(const N a, const N b) {
                    extended<Z> e = extended<Z>::algorithm(Z{a}, Z{b});
                    return extended{abs<Z>{}(e.GCD), e.BezoutS, e.BezoutT};
                }
                
            private:
                extended() : GCD{}, BezoutS{}, BezoutT{} {}
                
                extended(const N gcd, const Z s, const Z t) : GCD{gcd}, BezoutS{s}, BezoutT{t} {} 
            };
            
            template <>
            struct extended<uint64, int64> {
                uint64 GCD;
                int64 BezoutS;
                int64 BezoutT;
                
                bool valid() const {
                    if(GCD > 0x7fffffffffffffff) return false;
                }
                
                extended(uint64 a, uint64 b, uint64 gcd, int64 s, int64 t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
                    if (!extended<int64>::valid_proof(gcd, a, b, s, t)) throw invalid_proof{};
                }
                
                static extended algorithm(const uint64 a, const uint64 b) {
                    if(a > 0x7fffffffffffffff || b > 0x7fffffffffffffff) return {};
                    extended<int64> e = extended<int64>::algorithm(static_cast<int64>(a), static_cast<int64>(b));
                    return extended{abs<int64>{}(e.GCD), e.BezoutS, e.BezoutT};
                }
                
            private:
                extended() : GCD{}, BezoutS{}, BezoutT{} {}
                
                extended(const uint64 gcd, const int64 s, const int64 t) : GCD{gcd}, BezoutS{s}, BezoutT{t} {} 
            };
        }
    }
}

#endif
