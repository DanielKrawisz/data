// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/math/number/natural.hpp>

namespace data::math::number {

    template <typename Z> concept integer = ordered<Z> && integral_domain<Z> && requires (const Z &z) {
        { data::is_zero (z) } -> std::same_as<bool>;
        { data::is_positive (z) } -> std::same_as<bool>;
        { data::is_negative (z) } -> std::same_as<bool>;
        { data::sign (z) } -> std::same_as<math::signature>;
        { data::increment (z) } -> std::same_as<Z>;
        { data::decrement (z) } -> std::same_as<Z>;
    } && requires (const Z &a, const Z &b) {
        { a + b } -> std::same_as<Z>;
        { a - b } -> std::same_as<Z>;
        { a * b } -> std::same_as<Z>;
        { a / b } -> std::same_as<Z>;
    } && requires (Z &z) {
        { ++z } -> std::same_as<Z &>;
        { z++ } -> std::same_as<Z>;
        { --z } -> std::same_as<Z &>;
        { z++ } -> std::same_as<Z>;
    } && requires (Z &a, const Z &b) {
        { a += b } -> std::same_as<Z &>;
        { a -= b } -> std::same_as<Z &>;
        { a *= b } -> std::same_as<Z &>;
    };

    template <typename Z, typename N>
    division<Z, N> integer_natural_divide (const Z &Dividend, const N &Divisor);

    template <typename Z, typename N = decltype (data::abs (std::declval<Z> ()))>
    division<Z, N> integer_divide (const Z &Dividend, const Z &Divisor);

}

namespace data::math {

    template <number::integer Z>
    requires number::natural<decltype (data::abs (std::declval<Z> ()))> || number::integer<decltype (data::abs (std::declval<Z> ()))>
    struct divide<Z> {
        using N = decltype (data::abs (std::declval<Z> ()));
        division<Z, N> operator () (const Z &Dividend, const nonzero<Z> &Divisor) {
            return number::integer_divide (Dividend, Divisor.Value);
        }
    };

    template <number::integer Z, number::natural N>
    struct divide<Z, N> {
        division<Z, N> operator () (const Z &Dividend, const nonzero<N> &Divisor) {
            return number::integer_natural_divide (Dividend, Divisor.Value);
        }
    };

    template <std::unsigned_integral dividend> requires number::natural<dividend>
    struct divide<dividend, dividend> {
        division<dividend> operator () (dividend, nonzero<dividend>);
    };

    template <std::signed_integral dividend> requires number::integer<dividend>
    struct divide<dividend, dividend> {
        division<dividend, std::make_unsigned_t<dividend>> operator () (dividend, nonzero<dividend>);
    };

    template <std::signed_integral dividend, std::unsigned_integral divisor>
    requires std::same_as<std::make_unsigned_t<dividend>, divisor> && number::integer<dividend> && number::natural<divisor>
    struct divide<dividend, divisor> {
        division<dividend, divisor> operator () (dividend, nonzero<std::make_unsigned_t<dividend>>);
    };

}

namespace data::math::number {

    // implementation of naturals given an implementation of integers.
    template <integer Z> struct N;

    template <integer Z> bool operator == (const N<Z> &, const N<Z> &);

    template <integer Z> auto operator <=> (const N<Z> &, const N<Z> &) -> decltype (std::declval<Z> () <=> std::declval<Z> ());

    template <integer Z> N<Z> operator + (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator - (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator * (const N<Z> &, const N<Z> &);

    template <integer Z> N<Z> operator + (const N<Z> &, uint64);
    template <integer Z> N<Z> operator - (const N<Z> &, uint64);
    template <integer Z> N<Z> operator * (const N<Z> &, uint64);

    // divided by
    template <integer Z> N<Z> operator / (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator / (const N<Z> &, uint64);

    // mod
    template <integer Z> N<Z> operator % (const N<Z> &, const N<Z> &);

    // bit operations
    template <integer Z> N<Z> operator | (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator & (const N<Z> &, const N<Z> &);

    // bit shift, which really just means
    // powers of two.
    template <integer Z> N<Z> operator << (const N<Z> &, int);
    template <integer Z> N<Z> operator >> (const N<Z> &, int);

    // pre increment
    template <integer Z> N<Z> &operator ++ (N<Z> &);
    template <integer Z> N<Z> &operator -- (N<Z> &);

    // post increment
    template <integer Z> N<Z> operator ++ (N<Z> &, int);
    template <integer Z> N<Z> operator -- (N<Z> &, int);

    template <integer Z> std::ostream& operator << (std::ostream& o, const N<Z> &n);

    // implementation of naturals given an implementation of integers.
    template <integer Z> struct N {
        Z Value;

        explicit N (const Z &z) : Value {z} {}
        explicit N (Z &&z) : Value {z} {}

        operator Z () const {
            return Value;
        }
    };

}

namespace data::math {

    template <number::integer Z>
    struct divide<number::N<Z>> {
        division<number::N<Z>> operator () (const number::N<Z> &Dividend, const nonzero<number::N<Z>> &Divisor);
    };

}

namespace data::math::number {

    template <integer Z> struct increment<N<Z>> {
        nonzero<N<Z>> operator () (const N<Z> &);
    };

    template <integer Z> struct decrement<N<Z>> {
        N<Z> operator () (const nonzero<N<Z>> &);
        N<Z> operator () (const N<Z> &);
    };

    template <typename Z, typename N>
    division<Z, N> integer_natural_divide (const Z &Dividend, const N &Divisor) {
        division<N> d {natural_divide<N> (data::abs (Dividend), Divisor)};

        if (Dividend < 0) return {-(d.Quotient + 1), Divisor - d.Remainder};

        return {Z (d.Quotient), d.Remainder};
    }

    template <typename Z, typename N = decltype (data::abs (std::declval<Z> ()))>
    division<Z, N> integer_divide (const Z &Dividend, const Z &Divisor) {
        N divisor = data::abs (Divisor);
        division<N> d {natural_divide<N> (data::abs (Dividend), divisor)};

        if (Dividend < 0) {
            if (Divisor < 0) return {Z (d.Quotient + 1), divisor - d.Remainder};

            return {-(d.Quotient + 1), divisor - d.Remainder};
        }

        if (Divisor < 0) return {-d.Quotient, d.Remainder};

        return {Z (d.Quotient), d.Remainder};
    }

}

#endif
