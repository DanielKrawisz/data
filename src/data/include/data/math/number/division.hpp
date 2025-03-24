// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/integral.hpp>

namespace data::math::number {

    // Generic division algorithm.
    template <ring_integral N>
    division<N> natural_divide (const N &Dividend, const N &Divisor) {

        if (Divisor == 0u) throw division_by_zero {};
        if (Divisor == 1u) return {Dividend, 0u};
        if (Divisor == 2u) return {Dividend >> 1, Dividend & 1u};

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

    template <typename Z, typename N>
    requires unsigned_ring_integral<Z> && unsigned_ring_integral<N>
    division<Z, N> inline integer_divide (const Z &Dividend, const N &Divisor) {
        return natural_divide<Z, N> (Dividend, Divisor);
    }

    template <typename Z, typename N>
    requires ring_integral_system<Z, N> && unsigned_ring_integral<N>
    division<Z, N> integer_divide (const Z &Dividend, const N &Divisor) {
        division<N> d {natural_divide<N> (data::abs (Dividend), Divisor)};

        if (d.Remainder == 0) return {Dividend < 0 ? -Z (d.Quotient) : Z (d.Quotient), d.Remainder};

        if (Dividend < 0) return {static_cast<Z> (-(d.Quotient + 1)), static_cast<N> (Divisor - d.Remainder)};

        return {Z (d.Quotient), d.Remainder};
    }

    template <typename Z, typename N = decltype (data::abs (std::declval<Z> ()))>
    requires ring_integral_system<Z, N>
    division<Z, N> integer_divide (const Z &Dividend, const Z &Divisor) {
        // first we divide the absolute values.
        N divisor = data::abs (Divisor);
        division<N> d {natural_divide<N> (data::abs (Dividend), divisor)};

        // given x == q y + r,
        // if x -> -x, then x == -q y - r
        // if y -> -y, then x == -q y + r
        // if x -> -x and y -> -y, then x = q y - r;

        if (d.Remainder == 0) return {data::sign (Divisor) * data::sign (Dividend) == negative ? -Z (d.Quotient) : Z (d.Quotient), d.Remainder};

        if (Dividend < 0) return {Divisor < 0 ? Z (d.Quotient + 1): Z (-(d.Quotient + 1)), divisor - d.Remainder};

        if (Divisor < 0) return {static_cast<Z> (-d.Quotient), static_cast<N> (d.Remainder)};

        return {Z (d.Quotient), d.Remainder};
    }

}

#endif
