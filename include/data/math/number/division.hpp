// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/increment.hpp>
#include <data/arithmetic.hpp>

#include <data/exception.hpp>

namespace data::math::number {

    // Generic division algorithm.
    // assume both numbers are non-negative.
    template <MultiplicativeNumber N>
    constexpr division<N> natural_divmod (const N &Dividend, const N &Divisor) {

        if (Divisor == 0) throw division_by_zero {};
        if (Divisor > Dividend) return {0, Dividend};
        if (Divisor == 1) return {Dividend, 0u};
        if (Divisor == 2) return {div_2 (Dividend), mod_2 (Dividend)};

        N pow {1u};
        N exp {Divisor};

        // initialization phase
        {
            size_t width_d = bit_width (Dividend);
            size_t width_s = bit_width (exp);

            exp <<= (width_d - width_s);
            pow <<= (width_d - width_s);
            if (exp > Divisor) {
                exp >>= 1;
                pow >>= 1;
            }
        }

        // division phase
        // at this point, pow is the largest power of two such
        // that exp = Divisor * pow is smaller than Dividend.

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

    template <MultiplicativeNumber Z, MultiplicativeNumber N>
    constexpr division<Z, N> integer_natural_divmod (const Z &Dividend, const N &Divisor) {
        division<N> d {natural_divmod<N> (abs (Dividend), Divisor)};

        if (d.Remainder == 0) return {Dividend < 0 ? -Z (d.Quotient) : Z (d.Quotient), d.Remainder};

        if (Dividend < 0) return {static_cast<Z> (-(d.Quotient + 1u)), static_cast<N> (Divisor - d.Remainder)};

        return {Z (d.Quotient), d.Remainder};
    }

    enum modulo_negative_divisor_convention {
        // The remainder is always positive.
        EUCLIDIAN_ALWAYS_POSITIVE,

        // used in Bitcoin, c++, OpenSSL, Python3
        TRUNCATE_TOWARD_ZERO,

        PYTHON_2_FLOOR_DIV
    };

    template <modulo_negative_divisor_convention m, MultiplicativeNumber Z>
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

            if (Dividend < 0) return {
                Divisor < 0 ? d.Quotient + 1 : -(d.Quotient + 1),
                divisor - d.Remainder};

            if (Divisor < 0) return {static_cast<Z> (-d.Quotient), static_cast<N> (d.Remainder)};
        } else if constexpr (m == TRUNCATE_TOWARD_ZERO) {

            if (Dividend < 0) return {Divisor < 0 ? Z (d.Quotient): Z (-(d.Quotient)), Z (-d.Remainder)};

            if (Divisor < 0) return {static_cast<Z> (-d.Quotient), static_cast<N> (d.Remainder)};
        } else if constexpr (m == PYTHON_2_FLOOR_DIV) {
            throw unimplemented {"python 2 division"};
        } else throw exception {} << "Invalid modulo convention";

        return {Z (d.Quotient), d.Remainder};
    }

}
