// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/integral.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::number {

    // Generic division algorithm.
    template <ring_integral N>
    constexpr division<N> natural_divmod (const N &Dividend, const N &Divisor) {

        if (Divisor == 0) throw division_by_zero {};
        if (Divisor == 1) return {Dividend, 0u};
        if (Divisor == 2) return {div_2 (Dividend), mod_2 (Dividend)};

        N pow {1u};
        N exp {Divisor};

        // initialization phase
        // NOTE: should be able to use digits_base_2 here.
        {
            uint64 digits_per_round {1};

            // we increase exp by increasing powers of 2 until it is bigger than the divisor.
            // NOTE: this step should not be necessary. There ought to be a function that
            // tells us how many digits a number has.
            while (exp <= Dividend) {
                exp <<= digits_per_round;
                pow <<= digits_per_round;
                digits_per_round <<= 1;
            }

            // we change exp (either increase or decrease) by decreasing powers of 2 until
            // it is the maximum power of 2 that is smaller than the divisor.
            while (true) {
                digits_per_round >>= 1;
                if (digits_per_round == 0) break;
                if (exp > Dividend) {
                    exp >>= digits_per_round;
                    pow >>= digits_per_round;
                } else {
                    exp <<= digits_per_round;
                    pow <<= digits_per_round;
                }
            }
        }

        // division phase
        division<N> result {0, Dividend};
        while (pow > 0) {
            while (exp > result.Remainder) {
                exp >>= 1;
                pow >>= 1;
                if (pow == 0) return result;
            }

            result.Quotient += pow;
            result.Remainder -= exp;
        }

        return result;
    }

    template <ring_integral Z, ring_integral N>
    constexpr division<Z, N> integer_natural_divmod (const Z &Dividend, const N &Divisor) {
        division<N> d {natural_divmod<N> (abs (Dividend), Divisor)};

        if (d.Remainder == 0) return {Dividend < 0 ? -Z (d.Quotient) : Z (d.Quotient), d.Remainder};

        if (Dividend < 0) return {static_cast<Z> (-(d.Quotient + 1)), static_cast<N> (Divisor - d.Remainder)};

        return {Z (d.Quotient), d.Remainder};
    }

    enum modulo_negative_divisor_convention {
        // The remainder is always positive.
        EUCLIDIAN_ALWAYS_POSITIVE,

        // used in Bitcoin, c++, OpenSSL, Python3
        TRUNCATE_TOWARD_ZERO,

        PYTHON_2_FLOOR_DIV
    };

    template <modulo_negative_divisor_convention m, ring_integral Z>
    constexpr division<Z, decltype (abs (std::declval<Z> ()))> integer_divmod (const Z &Dividend, const Z &Divisor) {
        using N = decltype (abs (std::declval<Z> ()));

        // first we divide the absolute values.
        N divisor = abs (Divisor);
        division<N> d {natural_divmod<N> (abs (Dividend), divisor)};

        if (d.Remainder == 0)
            return {data::sign (Divisor) * data::sign (Dividend) == negative ?
                data::negate (Z (d.Quotient)):
                Z (d.Quotient), 0};

        if constexpr (m == EUCLIDIAN_ALWAYS_POSITIVE) {

            // given x == q y + r,
            // if x -> -x, then x == -q y - r
            // if y -> -y, then x == -q y + r
            // if x -> -x and y -> -y, then x = q y - r;

            if (Dividend < 0) return {Divisor < 0 ? Z (d.Quotient + 1): Z (-(d.Quotient + 1)), divisor - d.Remainder};

            if (Divisor < 0) return {static_cast<Z> (-d.Quotient), static_cast<N> (d.Remainder)};
        } else if constexpr (m == TRUNCATE_TOWARD_ZERO) {

            if (Dividend < 0) return {Divisor < 0 ? Z (d.Quotient): Z (-(d.Quotient)), Z (-d.Remainder)};

            if (Divisor < 0) return {static_cast<Z> (-d.Quotient), static_cast<N> (d.Remainder)};
        } else if constexpr (m == PYTHON_2_FLOOR_DIV) {
            throw method::unimplemented {"python 2 division"};
        } else throw exception {} << "Invalid modulo convention";

        return {Z (d.Quotient), d.Remainder};
    }

}

#endif
