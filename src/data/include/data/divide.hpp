// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DIVIDE
#define DATA_DIVIDE

#include <data/math/nonnegative.hpp>
#include <iostream> // required on windows

namespace data {

    template <typename dividend, typename divisor>
    auto divide (const dividend &a, const math::nonzero<divisor> &b);

    template <typename dividend, typename divisor>
    bool divides (const dividend &a, const math::nonzero<divisor> &b);

    template <typename N, typename R = N> struct division {
        N Quotient;
        R Remainder;

        bool valid () const {
            return data::valid (Quotient) && data::valid (Remainder);
        }

        division (const N &q, const R &r) : Quotient {q}, Remainder {r} {}
        division () : Quotient {}, Remainder {} {}

        bool operator == (const division &d) const {
            return Quotient == d.Quotient && Remainder == d.Remainder;
        }

        bool operator != (const division &d) const {
            return !(*this == d);
        }
    };
}

namespace data::math {

    struct division_by_zero : std::logic_error {
        division_by_zero () : std::logic_error {"division by zero"} {}
    };

    template <typename dividend, typename divisor = dividend> struct divide;

    template <std::unsigned_integral X>
    struct divide<X, X> {
        division<X, X> operator () (X dividend, nonzero<X> divisor) {
            return {static_cast<X> (dividend / static_cast<X> (divisor.Value)), static_cast<X> (dividend % divisor.Value)};
        }
    };

    template <std::signed_integral X, std::unsigned_integral Y>
    struct divide<X, Y> {
        division<X, Y> operator () (X dividend, nonzero<Y> divisor) {
            auto quotient = static_cast<X> (dividend / static_cast<X> (divisor.Value));
            auto remainder = static_cast<X> (dividend % divisor.Value);
            if (remainder == 0) return {quotient, 0};
            if (dividend < 0) return {static_cast<X> (quotient - 1), static_cast<Y> (divisor.Value - static_cast<Y> (remainder))};
            return {quotient, static_cast<Y> (remainder)};
        }
    };

    template <std::signed_integral X>
    struct divide<X, X> {
        division<X, std::make_unsigned_t<X>> operator () (X dividend, nonzero<X> divisor) {
            auto quotient = static_cast<X> (dividend / divisor.Value);
            auto remainder = static_cast<X> (dividend % divisor.Value);
            if (remainder == 0) return {quotient, 0};
            else if (dividend < 0) {
                if (divisor.Value < 0) return {static_cast<X> (-quotient + 1),
                    static_cast<std::make_unsigned_t<X>> (-divisor.Value + remainder)};
                else return {static_cast<X> (quotient - 1), static_cast<std::make_unsigned_t<X>> (divisor.Value + remainder)};
            } else return {static_cast<X> (quotient), static_cast<std::make_unsigned_t<X>> (remainder)};
        }
    };
    
}

namespace data {
    template <typename dividend, typename divisor>
    auto inline divide (const dividend &a, const math::nonzero<divisor> &b) {
        return math::divide<dividend, divisor> {} (a, b);
    }
    
    template <typename dividend, typename divisor>
    bool inline divides (const dividend &a, const math::nonzero<divisor> &b) {
        return b == 0 ? true : math::divide<dividend, divisor> {} (a, b).Remainder == 0;
    }

    template <typename N, typename R>
    std::ostream inline &operator << (std::ostream &o, const division<N, R> x) {
        return o << "division {Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }

}

#endif
