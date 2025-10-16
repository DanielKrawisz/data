// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DIVIDE
#define DATA_DIVIDE

#include <data/math/nonnegative.hpp>
#include <data/abs.hpp>
#include <data/arithmetic.hpp>
#include <iostream> // required on windows

namespace data {

    template <typename dividend, typename divisor>
    constexpr auto divide (const dividend &a, const math::nonzero<divisor> &b);

    template <typename dividend, typename divisor>
    constexpr bool divides (const dividend &a, const math::nonzero<divisor> &b);

    template <typename N, typename R = N> struct division {
        N Quotient;
        R Remainder;

        bool valid () const {
            return data::valid (Quotient) && data::valid (Remainder);
        }

        constexpr division (const N &q, const R &r) : Quotient {q}, Remainder {r} {}
        constexpr division () : Quotient {}, Remainder {} {}

        constexpr bool operator == (const division &d) const {
            return Quotient == d.Quotient && Remainder == d.Remainder;
        }
    };
}

namespace data::math {

    struct division_by_zero : std::logic_error {
        division_by_zero () : std::logic_error {"division by zero"} {}
    };
}

namespace data::math::def {

    template <typename dividend, typename divisor = dividend> struct divmod;

    template <std::unsigned_integral X>
    struct divmod<X, X> {
        constexpr auto operator () (X dividend, math::nonzero<X> divisor) ->
            division<decltype (dividend / divisor.Value), decltype (dividend % divisor.Value)> {
            return {dividend / divisor.Value, dividend % divisor.Value};
        }
    };

    template <std::signed_integral X, std::unsigned_integral Y>
    struct divmod<X, Y> {
        constexpr division<Y, Y> operator () (X dividend, math::nonzero<Y> divisor) {
            auto quotient = static_cast<Y> (dividend / divisor.Value);
            auto remainder = static_cast<Y> (dividend % divisor.Value);
            if (remainder == 0) return {quotient, 0};
            if (dividend < 0) return {static_cast<Y> (quotient - 1), static_cast<Y> (divisor.Value - remainder)};
            return {quotient, remainder};
        }
    };

    template <std::signed_integral X, std::signed_integral Y>
    struct divmod<X, Y> {
        constexpr auto operator () (X dividend, math::nonzero<Y> divisor) ->
        division<decltype (dividend / divisor.Value), decltype (dividend % divisor.Value)> {
            auto quotient = dividend / divisor.Value;
            auto remainder = dividend % divisor.Value;
            if (remainder == 0) return {quotient, 0};
            else if (dividend < 0) {
                if (divisor.Value < 0) return {-quotient + 1, -divisor.Value + remainder};
                else return {quotient - 1, divisor.Value + remainder};
            } else return {quotient, remainder};
        }
    };

}

namespace data {
    template <typename dividend, typename divisor>
    constexpr auto inline divmod (const dividend &a, const math::nonzero<divisor> &b) {
        return math::def::divmod<dividend, divisor> {} (a, b);
    }

    template <typename N, typename R>
    std::ostream inline &operator << (std::ostream &o, const division<N, R> x) {
        return o << "division {Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }

    template <typename A, typename B = A> constexpr auto divide (const A &x, const math::nonzero<B> &n) {
        return divmod (x, n).Quotient;
    }
}

#endif
