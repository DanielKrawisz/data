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
        constexpr division<X, X> operator () (X dividend, math::nonzero<X> divisor) {
            return {static_cast<X> (dividend / static_cast<X> (divisor.Value)), static_cast<X> (dividend % divisor.Value)};
        }
    };

    template <std::signed_integral X, std::unsigned_integral Y>
    struct divmod<X, Y> {
        constexpr division<X, Y> operator () (X dividend, math::nonzero<Y> divisor) {
            auto quotient = static_cast<X> (dividend / static_cast<X> (divisor.Value));
            auto remainder = static_cast<X> (dividend % divisor.Value);
            if (remainder == 0) return {quotient, 0};
            if (dividend < 0) return {static_cast<X> (quotient - 1), static_cast<Y> (divisor.Value - static_cast<Y> (remainder))};
            return {quotient, static_cast<Y> (remainder)};
        }
    };

    template <std::signed_integral X>
    struct divmod<X, X> {
        constexpr division<X, std::make_unsigned_t<X>> operator () (X dividend, math::nonzero<X> divisor) {
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

namespace data::math::def {

    // default definition of mod in terms of divide.
    template <typename A, typename Mod> struct mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::divmod (x, n).Remainder;
        }
    };

    template <typename A, typename Mod> struct negate_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return x < n.Value ?
            data::mod (n.Value - x, n):
            data::mod (n.Value - data::mod (x, n), n);
        }
    };

    template <typename A, typename Mod> struct mul_2_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::mul_2 (x), n);
        }
    };

    template <typename A, typename B, typename Mod> struct plus_mod {
        constexpr auto operator () (const A &x, const B &y, const math::nonzero<Mod> &n) const {
            return data::mod (data::plus (x, y), n);
        }
    };

    template <typename A, typename B, typename Mod> struct minus_mod {
        constexpr auto operator () (const A &x, const B &y, const nonzero<Mod> &n) const {
            return data::mod (data::minus (x, y), n);
        }
    };

    template <typename A, typename Mod> struct square_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::square (x), n);
        }
    };

    // note: should have a special case for integral that uses twice.
    template <typename A, typename B, typename Mod> struct times_mod {
        constexpr auto operator () (A a, B b, nonzero<Mod> n) {
            return data::mod ((data::abs (a) * data::abs (b)), n);
        }
    };
}

#endif
