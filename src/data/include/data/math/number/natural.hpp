// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/types.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/ordered.hpp>
#include <data/math/ring.hpp>
#include <data/math/division.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/abs.hpp>
#include <data/math/number/increment.hpp>
#include <data/io/wait_for_enter.hpp>

namespace data::math::number {

    template <typename N> concept natural = ordered<N> && requires (const N &n) {
        { data::is_zero (n) } -> std::same_as<bool>;
        { data::is_positive (n) } -> std::same_as<bool>;
        { data::is_negative (n) } -> std::same_as<bool>;
        { data::sign (n) } -> std::same_as<math::signature>;
        { data::abs (n) } -> std::same_as<N>;
        { data::increment (n) } -> std::same_as<nonzero<N>>;
    } && requires (const nonzero<N> &n) {
        { decrement<N> {} (n) } -> std::same_as<N>;
    } && requires (const N &a, const N &b) {
        { a + b } -> std::same_as<N>;
        { a - b } -> std::same_as<N>;
        { a * b } -> std::same_as<N>;
        { a / b } -> std::same_as<N>;
        { a % b } -> std::same_as<N>;
    } && requires (N &n) {
        { ++n } -> std::same_as<N &>;
        { n++ } -> std::same_as<N>;
        { --n } -> std::same_as<N &>;
        { n++ } -> std::same_as<N>;
    } && requires (N &a, const N &b) {
        { a += b } -> std::same_as<N &>;
        { a -= b } -> std::same_as<N &>;
        { a *= b } -> std::same_as<N &>;
    };
    
    // Generic division algorithm. 
    template <typename N> division<N> natural_divide (const N &Dividend, const N &Divisor);
}

namespace data::math {

    template <number::natural N> struct divide<N, N> {
        division<N> operator () (const N &Dividend, const nonzero<N> &Divisor) {
            return number::natural_divide (Dividend, Divisor.Value);
        }
    };
    
}

namespace data {
    
    template <size_t size> struct decimal {
        char Value[size] = {};
        bool Valid {false};
        
        constexpr decimal (const char (&input)[size]) noexcept {
            if (size <= 1) return;
            if (input[0] < '1' || input[0] > '9') return;

            for (size_t i {1}; i < size - 1; i++)
                if (input[i] < '0' || input[i] > '9') return;

            if (input[size - 1] != '\0') return;

            Valid = true;
            for (size_t i {0}; i < size; i++) Value[i] = input[i];
        }
        
        constexpr operator uint64 () const {
            if (size > 20) return 0;
            uint64 x {0};
            uint64 digit {1};
            for (int i {size-2}; i >= 0; --i) {
                x += digit * static_cast<uint64> (Value[i] - '0');
                digit *= 10;
            }
            return x;
        }
    };

    template <size_t size>
    std::ostream inline &operator << (std::ostream &o, const decimal<size> d) {
        return o << "decimal["<< size <<"]" <<'"' << d.Value << '"';
    }

    // make sure these two won't work.
    template <> struct decimal<0>;
    template <> struct decimal<1>;
    
    template <size_t N> decimal (const char (&)[N]) -> decimal<N>;
    template <size_t N> decimal (decimal<N>) -> decimal<N>;
}

namespace data::math::number {
    template <std::unsigned_integral N> division<N> inline natural_divide (const N &Dividend, const N &Divisor) {
        if (Divisor == 0) throw division_by_zero {};
        return division<N> {Dividend / Divisor, Dividend % Divisor};
    }

    // Generic division algorithm.
    template <typename N> division<N> natural_divide (const N &Dividend, const N &Divisor) {

        if (Divisor == 0u) throw division_by_zero {};
        if (Divisor == 1u) return {Dividend, 0u};
        if (Divisor == 2u) return {Dividend >> 1, Dividend & 1u};

        N pow {1u};
        N exp {Divisor};

        // initialization phase
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
                if (pow == 0) goto out;
            }

            result.Quotient += pow;
            result.Remainder -= exp;
        }

        out:
        return result;
    }
}

#endif
