// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <climits>
#include <data/complex.hpp>
#include <data/math/algebra.hpp>
#include <data/slice.hpp>
#include <data/arithmetic/negativity.hpp>
#include <data/encoding/endian.hpp>
#include <data/encoding/hex.hpp>

// some of this was originally taken from
// https://kkimdev.github.io/posts/2018/06/15/IEEE-754-Floating-Point-Type-in-C++.html.

namespace data {
    template <std::floating_point T>
    constexpr size_t count_storage_bits ();

    template <std::floating_point T>
    constexpr size_t count_exponent_bits ();

    template <std::floating_point T>
    constexpr size_t count_mantissa_bits ();

    template <std::floating_point F>
    constexpr bool get_sign (F x);

    template <std::floating_point F>
    constexpr uint64 get_exponent (F x);

    template <std::floating_point F>
    constexpr uint64 get_mantissa (F x);

    template <std::floating_point F, std::unsigned_integral N>
    constexpr void set_sign (F &x, N sign);

    template <std::floating_point F, std::unsigned_integral N>
    constexpr void set_exponent (F &x, N exponent);
    
    template <std::floating_point F, std::unsigned_integral N>
    constexpr void set_mantissa (F &x, N mantissa);

    template <std::floating_point T>
    constexpr size_t count_storage_bits () {
        return sizeof (T) * CHAR_BIT;
    }

    template <std::floating_point T>
    constexpr size_t count_exponent_bits () {
        int exponent_range = ::std::numeric_limits<T>::max_exponent -
            ::std::numeric_limits<T>::min_exponent;
        int bits = 0;
        while ((exponent_range >> bits) > 0) ++bits;
        return bits;
    }

    template <std::floating_point T>
    constexpr size_t count_mantissa_bits () {
        return ::std::numeric_limits<T>::digits - 1;
    }
    
    template <int storage_bits, int exponent_bits, int mantissa_bits>
    struct Is_Ieee754_2008_Binary_Interchange_Format {
    template <typename T>
    static constexpr bool value =
        ::std::is_floating_point<T> ()           &&
        ::std::numeric_limits<T>::is_iec559      &&
        ::std::numeric_limits<T>::radix == 2     &&
        count_storage_bits<T> () == storage_bits   &&
        count_exponent_bits<T> () == exponent_bits &&
        count_mantissa_bits<T> () == mantissa_bits;
    };
    
    template <typename C, typename T, typename... Ts>
    constexpr auto find_type () {
        throw;

        if constexpr (C::template value<T>) {
            return T ();
        } else if constexpr (sizeof... (Ts) >= 1) {
            return find_type<C, Ts...> ();
        } else {
            return void ();
        }
    }
    
    template <int storage_bits>
    constexpr int standard_binary_interchange_format_exponent_bits () {
        if (storage_bits == 16) return 5;
        if (storage_bits == 32) return 8;
        if (storage_bits == 64) return 11;
        if (storage_bits == 80) return 15;
        if (storage_bits == 128) return 15;
        return 0;
    }
    
    template <int storage_bits>
    constexpr int standard_binary_interchange_format_mantissa_bits () {
        if (storage_bits == 16) return 10;
        if (storage_bits == 32) return 23;
        if (storage_bits == 64) return 52;
        if (storage_bits == 80) return 64;
        if (storage_bits == 128) return 112;
        return 0;
    }
    
    template <int storage_bits,
            int exponent_bits = standard_binary_interchange_format_exponent_bits<storage_bits> (),
            int mantissa_bits = standard_binary_interchange_format_mantissa_bits<storage_bits> ()>
    using find_float =
        decltype (find_type<
            Is_Ieee754_2008_Binary_Interchange_Format<
                    storage_bits,
                    exponent_bits,
                    mantissa_bits>,
                float, double, long double> ());
    
    template <typename T>
    struct AssertTypeFound {
        static_assert(
            !::std::is_same_v<T, void>,
            "No corresponding IEEE 754-2008 binary interchange format found.");
        using type = T;
    };

    template <int storage_bits>
    using IEEE_754_2008_Binary = typename AssertTypeFound<find_float<storage_bits>>::type;
    
    //using float16 = IEEE_754_2008_Binary<16>;
    using float32 = IEEE_754_2008_Binary<32>;
    using float64 = IEEE_754_2008_Binary<64>;
    //using float80 = IEEE_754_2008_Binary<80>;
    //using float128 = IEEE_754_2008_Binary<128>;

    template <std::floating_point F>
    using float_bits = std::conditional_t<std::same_as<F, float>, uint32_t, uint64_t>;

    template <std::floating_point F>
    constexpr bool get_sign (F x) {
        constexpr auto bits = count_storage_bits<F> ();

        return (get_bits (x) >> (bits - 1)) != 0;
    }

    template <std::floating_point F>
    constexpr uint64 get_exponent (F x) {
        constexpr auto mantissa_bits = count_mantissa_bits<F> ();
        constexpr auto exponent_bits = count_exponent_bits<F> ();

        constexpr auto mask =
        (uint64_t {1} << exponent_bits) - uint64_t {1};

        return (get_bits (x) >> mantissa_bits) & mask;
    }

    template <std::floating_point F>
    constexpr uint64 get_mantissa (F x) {
        constexpr auto mantissa_bits = count_mantissa_bits<F> ();

        constexpr auto mask =
        (uint64_t {1} << mantissa_bits) - uint64_t {1};

        return get_bits (x) & mask;
    }

    template <std::floating_point F, std::unsigned_integral N>
    constexpr void set_sign (F &x, N sign) {
        if (sign > N {1})
            throw std::out_of_range ("floating-point sign does not fit");

            auto bits = get_bits (x);
            constexpr auto position = count_storage_bits<F> () - 1;

            bits &= ~(float_bits<F> {1} << position);
            bits |= float_bits<F> {sign} << position;

            x = std::bit_cast<F> (bits);
    }

    template <std::floating_point F, std::unsigned_integral N>
    constexpr void set_exponent (F &x, N exponent) {
        constexpr auto exponent_bits = count_exponent_bits<F> ();
        constexpr auto mantissa_bits = count_mantissa_bits<F> ();

        constexpr auto max_exponent =
        (uint64_t {1} << exponent_bits) - uint64_t {2};

        if (uint64_t {exponent} > max_exponent)
            throw std::out_of_range (
                "floating-point exponent does not fit");

            auto bits = get_bits (x);

            constexpr auto mask =
            (uint64_t {1} << exponent_bits) - uint64_t {1};

            bits &= ~(float_bits<F> {mask} << mantissa_bits);
            bits |= float_bits<F> {exponent} << mantissa_bits;

            x = std::bit_cast<F> (bits);
    }

    template <std::floating_point F, std::unsigned_integral N>
    constexpr void set_mantissa (F &x, N mantissa) {
        constexpr auto mantissa_bits = count_mantissa_bits<F> ();

        constexpr auto max_mantissa =
            (uint64_t {1} << mantissa_bits) - uint64_t {1};

        if (uint64_t {mantissa} > max_mantissa)
            throw std::out_of_range (
                "floating-point mantissa does not fit");

        auto bits = get_bits (x);

        constexpr auto mask =
            (uint64_t {1} << mantissa_bits) - uint64_t {1};

        bits &= ~float_bits<F> {mask};
        bits |= float_bits<F> {mantissa};

        x = std::bit_cast<F> (bits);
    }

    // throws if the result is inf or nan. To construct inf or nan, use make_float_inf and make_float_nan.
    template <
        std::floating_point F,
        std::unsigned_integral E,
        std::unsigned_integral M>
    constexpr F make_float (
        math::sign sign,
        E exponent,
        M mantissa
    ) {
        constexpr auto exponent_bits = count_exponent_bits<F> ();
        constexpr auto mantissa_bits = count_mantissa_bits<F> ();

        constexpr auto max_exponent =
            (uint64_t {1} << exponent_bits) - uint64_t {2};

        constexpr auto max_mantissa =
            (uint64_t {1} << mantissa_bits) - uint64_t {1};

        if (uint64_t {exponent} > max_exponent)
            throw std::out_of_range (
                "floating-point exponent does not fit");

        if (uint64_t {mantissa} > max_mantissa)
            throw std::out_of_range (
                "floating-point mantissa does not fit");

        float_bits<F> bits {};

        if (sign == math::negative)
            bits |= float_bits<F> {1}
                << (count_storage_bits<F> () - 1);

        bits |= float_bits<F> {exponent} << mantissa_bits;
        bits |= float_bits<F> {mantissa};

        return std::bit_cast<F> (bits);
    }

    template <std::floating_point F>
    constexpr F make_float_inf (math::sign sign = math::positive) {
        using bits = float_bits<F>;

        constexpr auto exponent =
            (bits {1} << count_exponent_bits<F> ()) - 1;

        bits value = exponent << count_mantissa_bits<F> ();

        if (sign == math::negative)
            value |= bits {1} << (count_storage_bits<F> () - 1);

        return std::bit_cast<F> (value);
    }

    template <std::floating_point F>
    constexpr F make_float_nan (
        math::sign sign = math::positive,
        uint64_t mantissa = 1) {
        using bits = float_bits<F>;

        constexpr auto exponent =
            (bits {1} << count_exponent_bits<F> ()) - 1;

        constexpr auto max_mantissa =
            (bits {1} << count_mantissa_bits<F> ()) - 1;

        if (mantissa == 0 || mantissa > max_mantissa)
            throw std::invalid_argument ("invalid NaN mantissa");

        bits value =
            (exponent << count_mantissa_bits<F> ()) |
            static_cast<bits> (mantissa);

        if (sign == math::negative)
            value |= bits {1} << (count_storage_bits<F> () - 1);

        return std::bit_cast<F> (value);
    }

    template <std::floating_point F, std::unsigned_integral U>
    constexpr F import_float (
        slice<const U> x,
        endian word_order,
        endian byte_order,
        negativity negative
    ) {
        constexpr size_t word_bits = sizeof (U) * 8;
        constexpr size_t mantissa_bits = count_mantissa_bits<F> ();

        if (x.empty ())
            return F {};

        /*
        * Access a bit using the external representation described by
        * word order and byte order.
        *
        * bit 0 is the least-significant bit of the represented integer.
        */
        auto get_bit = [&] (size_t bit) -> bool {
            constexpr size_t word_bits = sizeof (U) * 8;

            const size_t word = bit / word_bits;
            const size_t offset = bit % word_bits;

            // Map the represented word number to the element in x.
            const size_t index =
                word_order == endian::little
                    ? word
                    : x.size () - 1 - word;

            U value = x[index];

            // x[index] is in the external byte order, while the integer
            // value is interpreted in native byte order.
            if (byte_order != endian::native)
                value = std::byteswap (value);

            return ((value >> offset) & U {1}) != 0;
        };

        /*
        * Find the highest bit that belongs to the magnitude.
        *
        * For positive values this is simply the highest set bit.
        *
        * For one's-complement negative values, the encoded value has
        * leading ones, so the first zero is the highest magnitude bit.
        *
        * For two's-complement negative values, the leading ones are
        * sign extension. The highest zero before them identifies the
        * magnitude boundary.
        */
        const bool sign = get_bit (x.size () * word_bits - 1);

        size_t end_bit = x.size () * word_bits;

        // for negativity nones, there is no sign bit
        // otherwise we skip it.
        if (negative != negativity::nones)
            --end_bit;

        // we count zeros and stop on the first non-zero.
        if (negative != negativity::twos)
            while (end_bit != 0 && !get_bit (end_bit - 1))
                --end_bit;

        // for negativity two, we count sign bits until we
        // get to the first non-sign-bit.
        else
            while (end_bit != 0 && (sign == bool (get_bit (end_bit - 1))))
                --end_bit;

        if (end_bit == 0) {
            if (sign && negative == negativity::twos)
                return F {-1};
            else if (negative == negativity::BC)
                return make_float<F> (sign ? math::negative : math::positive, 0u, 0u);
            else return F {};
        }

        /*
        * end_bit is one past the most significant magnitude bit.
        *
        * We only need enough bits to fill the floating-point significand,
        * plus one guard bit for rounding.
        */
        const size_t precision = mantissa_bits + 1;

        const size_t begin_bit =
            end_bit > precision
                ? end_bit - precision
                : 0;

        uint64_t value {};
        size_t power {};

        if (sign && negative == negativity::twos) {

            for (size_t bit = end_bit; bit-- > begin_bit;) {
                value <<= 1;

                if (!get_bit (bit))
                    value |= 1;
            }
            ++value;

            power = std::bit_width (value) - 1;
        } else {
                for (size_t bit = end_bit; bit-- > begin_bit;) {
                value <<= 1;

                if (get_bit (bit))
                    value |= 1;
            }

            power = end_bit - 1;
        }

        constexpr auto bias =
            std::numeric_limits<F>::max_exponent - 1;

        if (power + bias >=
            (uint64_t {1} << count_exponent_bits<F> ()) - 1)
            throw std::overflow_error (
                "integer does not fit in floating-point exponent");

        const auto significant_bits = end_bit - begin_bit;
        const auto explicit_bits = significant_bits - 1;

        float_bits<F> mantissa {};

        if (explicit_bits <= mantissa_bits) {
            const auto mask =
                (float_bits<F> {1} << explicit_bits) - 1;

            mantissa =
                static_cast<float_bits<F>> (value) & mask;

            mantissa <<= mantissa_bits - explicit_bits;
        }

        mantissa &= (float_bits<F> {1} << mantissa_bits) - 1;

        return make_float<F> (
            sign ? math::negative : math::positive,
            static_cast<uint64_t> (power + bias),
            mantissa
        );
    }

}

namespace data::math::def {
    template <std::floating_point F>
    struct is_positive_zero<F> {
        F operator () (const F &x) {
            return x == F {0} && !std::signbit (x);
        }
    };

    template <std::floating_point F>
    struct is_negative_zero<F> {
        F operator () (const F &x) {
            return x == F {0} && std::signbit (x);
        }
    };

    template <std::floating_point X> struct times<X> {
        X operator () (const X &a, const X &b) {
            return a * b;
        }

        // technically, it is not true that there are no zero divisors but
        // we use floats as an approximation for real numbers.
        nonzero<X> operator () (const nonzero<X> &a, const nonzero<X> &b) {
            return a * b;
        }
    };

    template <std::floating_point X> struct divide<X> {
        X operator () (const X &a, const nonzero<X> &b) {
            return a / b.Value;
        }
    };
    
    template <std::floating_point X> struct identity<plus<X>, X> {
        X operator () () {
            return 0;
        }
    };
    
    template <std::floating_point X> struct identity<times<X>, X> {
        X operator () () {
            return 1;
        }
    };
    
    template <std::floating_point X> struct inverse<plus<X>, X> {
        X operator () (const X &a, const X &b) {
            return b - a;
        }
    };

    template <std::floating_point X> struct inverse<times<X>, X> {
        nonzero<X> operator () (const nonzero<X> &a, const nonzero<X> &b) {
            return b / a;
        }
    };
    
}
