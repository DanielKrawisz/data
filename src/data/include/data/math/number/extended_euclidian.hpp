// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN
#define DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN

#include <data/valid.hpp>
#include <data/math/number/division.hpp>
#include <data/integral.hpp>
#include <sstream>

namespace data::math::number::euclidian {
    template <ring_integral N, ring_integral Z = decltype (data::negate (std::declval<N> ()))>
    requires ring_number_system<Z, N> || Same<Z, N>
    struct extended;
}

namespace data::math {
    struct invalid_proof : std::exception {};
}

namespace data::math::number::euclidian {
    template <ring_integral N, ring_integral Z>
    requires ring_number_system<Z, N> || Same<Z, N>
    struct extended {
        N GCD;
        Z BezoutS;
        Z BezoutT;
        
        bool valid () const {
            return data::valid (GCD) && data::valid (BezoutS) && data::valid (BezoutT);
        }
        
        static bool valid_proof (N gcd, Z a, Z b, Z s, Z t) {
            return gcd == a * s + b * t;
        }
        
        constexpr extended (Z a, Z b, N gcd, Z s, Z t) : GCD {gcd}, BezoutS {s}, BezoutT {s} {
            if (!valid_proof (gcd, a, b, s, t)) throw invalid_proof {};
        }
        
    private:
        constexpr extended () : GCD {}, BezoutS {}, BezoutT {} {}
        
        constexpr extended (const N gcd, const Z s, const Z t) : GCD {gcd}, BezoutS {s}, BezoutT {t} {}
        
        struct sequence {
            division<N> Div;
            Z BezoutS;
            Z BezoutT;

            constexpr sequence (const division<N> d, const Z &s, const Z &t):
                Div {d}, BezoutS {s}, BezoutT {t} {}
            
            constexpr sequence operator / (const sequence &s) const {
                division<N> div = natural_divmod<N> (Div.Remainder, s.Div.Remainder);
                return {div,
                    static_cast<Z> (BezoutS - s.BezoutS * div.Quotient),
                    static_cast<Z> (BezoutT - s.BezoutT * div.Quotient)};
            }
            
        };
        
        // must provide prev.Div.Remainder > current.Div.Remainder.
        constexpr static extended loop (const sequence prev, const sequence current) {
            sequence next = prev / current;
            if (next.Div.Remainder == 0) return extended {current.Div.Remainder, current.BezoutS, current.BezoutT};
            return loop (current, next);
        }
        
        // we know that a >= b
        constexpr static extended run (const N &a, const N &b) {
            return loop (sequence {{0, a}, Z {1}, Z {0}}, sequence {{0, b}, Z {0}, Z {1}});
        }
        
    public:
        constexpr static extended algorithm (const N &a, const N &b) {
            return a < b ? run (b, a) : run (a, b);
        }
    };

}

namespace data::math::number {
    template <ring_integral Z, ring_integral N>
    constexpr auto natural_invert_mod (const Z &x, const nonzero<N> &mod) ->
        maybe<decltype (divmod (x, mod).Remainder)> {
        if (mod.Value < 0) throw exception {} << "mod by negative number";
        using remainder_type = decltype (divmod (x, mod).Remainder);
        auto proof = number::euclidian::extended<remainder_type, Z>::algorithm
            (remainder_type (mod.Value), divmod (x, mod).Remainder);
        if (proof.GCD != 1) return {};
        return def::divmod<decltype (proof.BezoutT), N> {} (proof.BezoutT, mod).Remainder;
    }
}

namespace data::math::def {
    template <typename Z, typename N>
    struct invert_mod {
        constexpr auto operator () (const Z &x, const nonzero<N> &mod) {
            return number::natural_invert_mod<Z, N> (x, mod);
        }
    };
}

namespace data {
    template <typename N, typename Z = N>
    constexpr N inline GCD (const N &a, const N &b) {
        return data::math::number::euclidian::extended<N, Z>::algorithm (a, b).GCD;
    }
}

#endif
