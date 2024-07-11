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
        { data::abs (z) };
        { increment<Z> {} (z) } -> std::same_as<Z>;
        { decrement<Z> {} (z) } -> std::same_as<Z>;
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

    template <std::signed_integral Z> struct increment<Z> {
        Z operator () (const Z &);
    };

    template <std::signed_integral Z> struct decrement<Z> {
        Z operator () (const Z &);
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

namespace data::math {

    template <number::integer Z>
    requires number::natural<decltype (data::abs (std::declval<Z> ()))> || number::integer<decltype (data::abs (std::declval<Z> ()))>
    struct divide<Z> {
        using N = decltype (data::abs (std::declval<Z> ()));
        division<Z, N> operator () (const Z &Dividend, const Z &Divisor) {
            return number::integer_divide (Dividend, Divisor);
        }
    };

    template <number::integer Z, number::natural N>
    struct divide<Z, N> {
        division<Z, N> operator () (const Z &Dividend, const N &Divisor) {
            return number::integer_natural_divide (Dividend, Divisor);
        }
    };

}

namespace data::math::number {

    template <std::signed_integral Z> Z inline increment<Z>::operator () (const Z &z) {
        return z + 1;
    }

    template <std::signed_integral Z> Z inline decrement<Z>::operator () (const Z &z) {
        return z - 1;
    }

}

#endif
