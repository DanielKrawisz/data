// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DIVIDE
#define DATA_DIVIDE

#include <data/math/nonzero.hpp>
#include <data/abs.hpp>
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

        // implicit conversions
        template <typename X, typename Y = X>
        requires ImplicitlyConvertible<N, X> && ImplicitlyConvertible<R, Y>
        constexpr operator division<X, Y> () const;

        // explicit conversions
        template <typename X, typename Y = X>
        requires Convertible<N, X> && Convertible<R, Y> && (
            ExplicitlyConvertible<N, X> && ExplicitlyConvertible<R, Y>)
        constexpr explicit operator division<X, Y> () const;
    };

    template <typename Z, typename N> division (const Z &, const N &) -> division<Z, N>;
}

namespace data::math {

    struct division_by_zero : exception {
        division_by_zero () : exception {"division by zero"} {}
    };
}

namespace data::math::def {

    template <typename dividend, typename divisor = dividend> struct divmod;

    template <std::integral X, std::integral Y>
    struct divmod<X, Y> {
        constexpr auto inline operator () (X dividend, math::nonzero<Y> divisor) ->
        division<decltype (dividend / divisor.Value), decltype (dividend % divisor.Value)> {
            if (divisor.Value == 0) throw division_by_zero {};
            return {dividend / divisor.Value, dividend % divisor.Value};
        }
    };

    template <typename dividend, typename divisor = dividend> struct divides {
        constexpr bool inline operator () (const dividend &v, const math::nonzero<divisor> &z) {
            return divmod<dividend, divisor> {} (v, z).Remainder == 0;
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

    template <typename A, typename B> constexpr auto divide (const A &x, const math::nonzero<B> &n) {
        return divmod (x, n).Quotient;
    }

    template <typename N, typename R>
    template <typename X, typename Y>
    requires ImplicitlyConvertible<N, X> && ImplicitlyConvertible<R, Y>
    constexpr division<N, R>::operator division<X, Y> () const {
        return division<X, Y> {X (Quotient), Y (Remainder)};
    }

    template <typename N, typename R>
    template <typename X, typename Y>
    requires Convertible<N, X> && Convertible<R, Y> && (
        ExplicitlyConvertible<N, X> && ExplicitlyConvertible<R, Y>)
    constexpr division<N, R>::operator division<X, Y> () const {
        return division<X, Y> {X (Quotient), Y (Remainder)};
    }
}

#endif
