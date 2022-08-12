// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN
#define DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN

#include <data/math/abs.hpp>
#include <data/math/number/integer.hpp>
#include <data/valid.hpp>

namespace data::math::number::euclidian {
    template <typename Z, typename N = decltype(data::abs(std::declval<Z>()))> struct extended;
}

namespace data::math {
    struct invalid_proof : std::exception {};
}


namespace data::math::number::euclidian {
    template <typename Z, typename N>
    struct extended {
        N GCD;
        Z BezoutS;
        Z BezoutT;
        
        bool valid() const {
            return data::valid(GCD) && data::valid(BezoutS) && data::valid(BezoutT);
        }
        
        static bool valid_proof(N gcd, Z a, Z b, Z s, Z t) {
            return gcd == a * s + b * t;
        }
        
        extended(Z a, Z b, N gcd, Z s, Z t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
            if (!valid_proof(gcd, a, b, s, t)) throw invalid_proof{};
        }
        
    private:
        extended() : GCD{}, BezoutS{}, BezoutT{} {}
        
        extended(const N gcd, const Z s, const Z t) : GCD{gcd}, BezoutS{s}, BezoutT{t} {} 
        
        struct sequence {
            division<N> Div;
            Z BezoutS;
            Z BezoutT;
            
            sequence operator/(const sequence &s) const {
                division<N> div = natural::divide<N>(Div.Remainder, s.Div.Remainder);
                return {div, BezoutS - s.BezoutS * div.Quotient , BezoutT - s.BezoutT * div.Quotient};
            }
            
        };
        
        // must provide prev.Div.Remainder > current.Div.Remainder.
        static extended loop(const sequence prev, const sequence current) {
            sequence next = prev / current;
            if (next.Div.Remainder == 0) return extended{current.Div.Remainder, current.BezoutS, current.BezoutT};
            return loop(current, next);
        }
        
        // we know that a >= b
        static extended run(const N &a, const N &b) {
            return loop(sequence{{0, a}, Z{1}, Z{0}}, sequence{{0, b}, Z{0}, Z{1}});
        }
        
    public:
        static extended algorithm(const N &a, const N &b) {
            return a < b ? run(b, a) : run(a, b);
        }
    };
}

#endif
