// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/valid.hpp>
#include <data/abs.hpp>
#include <data/math/number/division.hpp>
#include <data/arithmetic.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include <sstream>

namespace data::math::number::euclidian {
    template <RingNumber N, typename Z = decltype (data::negate (std::declval<N> ()))>
    struct extended;
}

namespace data::math {
    struct invalid_proof : std::exception {};
}

namespace data::math::number::euclidian {
    template <RingNumber N, typename Z>
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
        
        constexpr extended (Z a, Z b, N gcd, Z s, Z t) : GCD {gcd}, BezoutS {s}, BezoutT {t} {
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
                division<N> div = data::divmod (Div.Remainder, math::nonzero {s.Div.Remainder});
                return {div,
                    static_cast<Z> (BezoutS - s.BezoutS * div.Quotient),
                    static_cast<Z> (BezoutT - s.BezoutT * div.Quotient)};
            }
            
        };
        
        // must provide prev.Div.Remainder > current.Div.Remainder.
        constexpr static extended loop (const sequence prev, const sequence current) {
            sequence next = prev / current;

            if (next.Div.Remainder == 0)
                return extended {current.Div.Remainder, current.BezoutS, current.BezoutT};

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

    // Z either needs to be signed or it needs to be an unsigned integral that can underflow below zero.
    template <RingNumber Z, RingNumber N = Z>
    constexpr auto inv_mod (const Z &x, const nonzero<N> &mod) -> maybe<decltype (data::mod (x, mod))> {

        if (mod.Value == 0) throw division_by_zero {};
        if (mod.Value < 0) throw exception {} << "we don't know how to handle this case yet";
        using result_type = decltype (data::mod (x, mod));

        // the euclid algorithm requires negative numbers. We can work with
        // signed types for Z or with unsigned types that can underflow.
        using input_type = std::conditional_t<Signed<Z> || UnsignedIntegral<Z>, Z, to_signed<Z>>;

        if (x == 0) return {};
        auto proof = number::euclidian::extended<result_type, input_type>::algorithm
            (result_type (mod.Value), data::mod (x, mod));

        if (proof.GCD != 1) return {};

        // for some numbers, mods can be negative.
        auto result = data::mod (proof.BezoutT, mod);
        return is_negative (result) ? result_type (result + mod.Value) : result;
    }
}

// default definition for invert mod and GCD.
namespace data::math::def {

    template <typename Z, typename N>
    struct invert_mod {
        constexpr auto inline operator () (const Z &x, const nonzero<N> &mod) -> maybe<decltype (number::divmod (x, mod.Value).Remainder)> {
            // we have a problem here because we can underflow, and when that happens, we cannot correctly mod
            // the result. We transform to a signed type that is bigger than this and then transform back.
            if constexpr (UnsignedIntegral<Z>) {
                constexpr size_t size = sizeof (Z);
                using bigger =
                    std::conditional_t<size == 1, int16,
                    std::conditional_t<size == 2, int32,
                    std::conditional_t<size == 4, int64,
                        number::bounded<true, endian::little, size / 8 + 1, uint64>>>>;
                using result_type = decltype (data::mod (x, mod));
                auto result = number::inv_mod (math::convert<bigger> (x), nonzero {math::convert<bigger> (mod.Value)});
                if (!result) return {};
                return maybe<result_type> {data::math::convert<result_type> (*result)};
            } else return number::inv_mod (x, mod);
        }
    };

    template <typename N, typename Z>
    struct GCD {
        constexpr N inline operator () (const N &a, const N &b) {
            if (data::is_zero (a)) return data::abs (b);
            if (data::is_zero (b)) return data::abs (a);
            return number::euclidian::extended<N, Z>::algorithm (a, b).GCD;
        }
    };
}
