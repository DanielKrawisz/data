// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

// This file implements conversions for integral, endian_integral, bounded, and Z_bytes types.

#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/arithmetic/arithmetic.hpp>

namespace data::math::def {
    namespace {

        template <endian ToEndian, negativity ToNeg, Word ToWord, negativity FromNeg, typename X>
        number::Z_bytes<ToEndian, ToNeg, ToWord> convert_to_Z_bytes (const X &val);

        template <bool IsSigned, endian ToEndian, size_t ToSize, Word ToWord, negativity FromNeg, typename X>
        constexpr number::bounded<IsSigned, ToEndian, ToSize, ToWord> convert_to_bounded (const X &val);

        template <endian ToEndian, std::integral T, std::size_t to_n_bits, negativity FromNeg, typename X>
        constexpr endian_arithmetic<ToEndian, T, to_n_bits, alignment::no> convert_to_endian_arithmetic (const X &val);

        template <std::integral T, negativity FromNeg,typename X>
        constexpr T convert_to_integral (const X &);

    }

    // convert between any two types of Z_bytes
    template <endian ToEndian, negativity ToNeg, Word ToWord, endian FromEndian, negativity FromNeg, Word FromWord>
    requires Unsame<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>>
    number::Z_bytes<ToEndian, ToNeg, ToWord> inline
    convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>>::operator ()
    (const number::Z_bytes<FromEndian, FromNeg, FromWord> &from) const {
        return convert_to_Z_bytes<ToEndian, ToNeg, ToWord, FromNeg> (from);
    }

    // convert between any bounded types.
    template <bool a, endian r, size_t x, Word w, bool b, endian o, size_t y, Word u>
    requires Unsame<number::bounded<a, r, x, w>, number::bounded<b, o, y, u>>
    constexpr number::bounded<a, r, x, w>
    convert<number::bounded<a, r, x, w>, number::bounded<b, o, y, u>>::operator ()
    (const number::bounded<b, o, y, u> &n) const {
        return convert_to_bounded<a, r, x, w, b ? negativity::twos : negativity::nones> (n);
    }

    // Z_bytes <--> bounded
    template <bool is_signed, endian to_r, size_t to_size, Word to_word, endian from_r, negativity neg, Word from_word>
    number::bounded<is_signed, to_r, to_size, to_word>
    convert<number::bounded<is_signed, to_r, to_size, to_word>, number::Z_bytes<from_r, neg, from_word>>::operator ()
    (const number::Z_bytes<from_r, neg, from_word> &z) const {
        return convert_to_bounded<is_signed, to_r, to_size, to_word, neg> (z);
    }

    template <endian to_r, negativity neg, Word to_word, bool is_signed, endian from_r, size_t size, Word from_word>
    number::Z_bytes<to_r, neg, to_word>
    convert<number::Z_bytes<to_r, neg, to_word>, number::bounded<is_signed, from_r, size, from_word>>::operator ()
    (const number::bounded<is_signed, from_r, size, from_word> &u) const {
        // special case, if the bounded number is similar enough to the result, we just copy it directly.
        if constexpr (Same<to_word, from_word> && ((is_signed && neg == negativity::twos) || (!is_signed && neg == negativity::nones))) {

            number::Z_bytes<to_r, neg, to_word> result;

            result.resize (size);

            std::copy (u.words ().begin (), u.words ().end (), result.words ().begin ());

            return result;
        } else return convert_to_Z_bytes<to_r, neg, to_word, is_signed ? negativity::twos : negativity::nones> (u);
    }

    // bounded <--> endian arithmetic
    template <bool is_signed, endian r, size_t size, Word word, endian o, std::integral T, std::size_t n_bits>
    constexpr endian_arithmetic<o, T, n_bits, alignment::no>
    convert<endian_arithmetic<o, T, n_bits, alignment::no>, number::bounded<is_signed, r, size, word>>::operator ()
    (const number::bounded<is_signed, r, size, word> &u) const {
        return convert_to_endian_arithmetic<o, T, n_bits, is_signed ? negativity::twos : negativity::nones> (u);
    }

    template <bool is_signed, endian r, size_t size, Word word, endian o, std::integral T, std::size_t n_bits>
    constexpr number::bounded<is_signed, r, size, word>
    convert<number::bounded<is_signed, r, size, word>, endian_arithmetic<o, T, n_bits, alignment::no>>::operator ()
    (const endian_arithmetic<o, T, n_bits, alignment::no> &ear) const {
        return math::convert<number::bounded<is_signed, r, size, word>> (T (ear));
    }

    // Z_bytes <--> endian arithmetic
    template <endian to_r, negativity neg, Word to_word, endian o, std::integral T, std::size_t n_bits>
    number::Z_bytes<to_r, neg, to_word>
    convert<number::Z_bytes<to_r, neg, to_word>, endian_arithmetic<o, T, n_bits, alignment::no>>::operator ()
    (const endian_arithmetic<o, T, n_bits, alignment::no> &ear) const {
        return convert_to_Z_bytes<to_r, neg, to_word, std::signed_integral<T> ? negativity::twos : negativity::nones> (ear);
    }

    template <endian o, std::integral T, std::size_t n_bits, endian FromEndian, negativity FromNeg, Word FromWord>
    endian_arithmetic<o, T, n_bits, alignment::no>
    convert<endian_arithmetic<o, T, n_bits, alignment::no>, number::Z_bytes<FromEndian, FromNeg, FromWord>>::operator ()
    (const number::Z_bytes<FromEndian, FromNeg, FromWord> &from) const {
        return convert_to_endian_arithmetic<o, T, n_bits, std::signed_integral<T> ? negativity::twos : negativity::nones> (from);
    }

    // conversions to integral types.
    template <std::integral I, bool is_signed, endian r, size_t size, Word word>
    constexpr I convert<I, number::bounded<is_signed, r, size, word>>::operator () (const number::bounded<is_signed, r, size, word> &x) const {
        return convert_to_integral<I, is_signed ? negativity::twos : negativity::nones> (x);
    }

    template <std::integral I, endian r, negativity neg, Word word>
    I convert<I, number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &u) const {
        return convert_to_integral<I, neg> (u);
    }

    namespace {

        template <endian ToEndian, negativity ToNeg, Word ToWord, negativity FromNeg, std::random_access_iterator In>
        number::Z_bytes<ToEndian, ToNeg, ToWord> range_to_Z_bytes (In in_begin, In in_end);

        template <bool IsSigned, endian ToEndian, size_t ToSize, Word ToWord, negativity FromNeg, std::random_access_iterator In>
        constexpr number::bounded<IsSigned, ToEndian, ToSize, ToWord> range_to_bounded (In in_begin, In in_end);

        template <endian ToEndian, std::integral T, std::size_t to_n_bits, negativity FromNeg, std::random_access_iterator In>
        constexpr endian_arithmetic<ToEndian, T, to_n_bits, alignment::no> range_to_endian_arithmetic (In in_begin, In in_end);

        template <std::integral T, negativity FromNeg, std::random_access_iterator In>
        constexpr T range_to_integral (In in_begin, In in_end);

        struct out_of_bounds_err {};
        struct negative_to_unsigned {};
    }

    template <bool is_signed, endian r, size_t size, Word word, std::integral I>
    constexpr number::bounded<is_signed, r, size, word> convert<number::bounded<is_signed, r, size, word>, I>::operator () (const I &x) const {
        std::make_unsigned_t<I> y = static_cast<std::make_unsigned_t<I>> (x);
        try {
            return range_to_bounded<is_signed, r, size, word, std::signed_integral<I> ? negativity::twos : negativity::nones> (&y, &y + 1);
        } catch (negative_to_unsigned) {
            throw exception {} << "try to convert negative value " << x << " to unsigned type";
        } catch (out_of_bounds_err) {
            throw exception {} << "try to convert value " << x << " that is too big for a type of size " << (size * sizeof (word));
        }
    }

    template <endian r, negativity neg, Word word, std::integral I>
    number::Z_bytes<r, neg, word> convert<number::Z_bytes<r, neg, word>, I>::operator () (const I &x) const {
        const std::make_unsigned_t<I> *z = (const std::make_unsigned_t<I> *) (&x);
        try {
            return range_to_Z_bytes<r, neg, word, std::signed_integral<I> ? negativity::twos : negativity::nones> (z, z + 1);
        } catch (negative_to_unsigned) {
            throw exception {} << "try to convert negative value " << x << " to unsigned type";
        }
    }

    namespace {

        template <endian ToEndian, negativity ToNeg, Word ToWord, negativity FromNeg, typename X>
        number::Z_bytes<ToEndian, ToNeg, ToWord> convert_to_Z_bytes (const X &val) {
            try {
                return range_to_Z_bytes<ToEndian, ToNeg, ToWord, FromNeg> (val.words ().begin (), val.words ().end ());
            } catch (negative_to_unsigned) {
                throw exception {} << "try to convert negative value " << val << " to unsigned type";
            }
        }

        template <bool IsSigned, endian ToEndian, size_t ToSize, Word ToWord, negativity FromNeg, typename X>
        constexpr number::bounded<IsSigned, ToEndian, ToSize, ToWord> convert_to_bounded (const X &val) {
            try {
                return range_to_bounded<IsSigned, ToEndian, ToSize, ToWord, FromNeg> (val.words ().begin (), val.words ().end ());
            } catch (negative_to_unsigned) {
                throw exception {} << "try to convert negative value " << val << " to unsigned type";
            } catch (out_of_bounds_err) {
                throw exception {} << "try to convert value " << val << " that is too big for a type of size " << (ToSize * sizeof (ToWord));
            }
        }

        template <endian ToEndian, std::integral T, std::size_t to_n_bits, negativity FromNeg, typename X>
        constexpr endian_arithmetic<ToEndian, T, to_n_bits, alignment::no> convert_to_endian_arithmetic (const X &val) {
            try {
                return range_to_endian_arithmetic<ToEndian, T, to_n_bits, FromNeg> (val.words ().begin (), val.words ().end ());
            } catch (negative_to_unsigned) {
                throw exception {} << "try to convert negative value " << val << " to unsigned type";
            } catch (out_of_bounds_err) {
                throw exception {} << "try to convert value " << val << " that is too big for a type of size " << (to_n_bits / CHAR_BIT);
            }
        }

        template <std::integral T, negativity FromNeg, typename X>
        constexpr T convert_to_integral (const X &val) {
            try {
                return range_to_integral<T, FromNeg> (val.words ().begin (), val.words ().end ());
            } catch (negative_to_unsigned) {
                throw exception {} << "try to convert negative value " << val << " to unsigned type";
            } catch (out_of_bounds_err) {
                throw exception {} << "try to convert value " << val << " that is too big for a type of size " << sizeof (T);
            }
        }

        // the minimum size of the result.
        template <
            Word ToWord,
            negativity ToNeg,
            negativity FromNeg,
            std::random_access_iterator X>
        constexpr size_t result_min_size (const X begin, const X end);

        template <
            negativity ToNeg,
            negativity FromNeg,
            std::random_access_iterator Out,
            std::random_access_iterator In>
        constexpr void port_bits (
            Out out_begin,
            Out out_end,
            In in_begin,
            In in_end);

        template <endian ToEndian, negativity ToNeg, Word ToWord, negativity FromNeg, std::random_access_iterator In>
        number::Z_bytes<ToEndian, ToNeg, ToWord> range_to_Z_bytes (In in_begin, In in_end) {

            number::Z_bytes<ToEndian, ToNeg, ToWord> result;
            auto min_size = result_min_size<ToWord, ToNeg, FromNeg> (in_begin, in_end);
            result.resize (min_size);

            port_bits<ToNeg, FromNeg> (
                result.words ().begin (),
                result.words ().end (),
                in_begin,
                in_end);

            return result;
        }

        template <bool IsSigned, endian ToEndian, size_t ToSize, Word ToWord, negativity FromNeg, std::random_access_iterator In>
        constexpr number::bounded<IsSigned, ToEndian, ToSize, ToWord> range_to_bounded (In in_begin, In in_end) {
            if (result_min_size<ToWord, IsSigned ? negativity::twos : negativity::nones, FromNeg> (in_begin, in_end) > ToSize)
                throw out_of_bounds_err {};

            number::bounded<IsSigned, ToEndian, ToSize, ToWord> ub;
            port_bits<IsSigned ? negativity::twos : negativity::nones, FromNeg> (ub.words ().begin (), ub.words ().end (), in_begin, in_end);
            return ub;
        }

        template <endian ToEndian, std::integral T, std::size_t to_n_bits, negativity FromNeg, std::random_access_iterator In>
        constexpr endian_arithmetic<ToEndian, T, to_n_bits, alignment::no> range_to_endian_arithmetic (In in_begin, In in_end) {

            if (result_min_size<byte, std::signed_integral<T> ? negativity::twos : negativity::nones, FromNeg> (in_begin, in_end) > sizeof (T))
                throw out_of_bounds_err {};

            std::make_unsigned_t<T> z;
            port_bits<std::signed_integral<T> ? negativity::twos : negativity::nones, FromNeg> (&z, &z + 1, in_begin, in_end);
            return endian_arithmetic<ToEndian, T, to_n_bits, alignment::no> (z);
        }

        template <std::integral T, negativity FromNeg, std::random_access_iterator In>
        constexpr T range_to_integral (In in_begin, In in_end) {

            if (result_min_size<byte, std::signed_integral<T> ? negativity::twos : negativity::nones, FromNeg> (in_begin, in_end) > sizeof (T))
                throw out_of_bounds_err {};

            std::make_unsigned_t<T> z;
            port_bits<std::signed_integral<T> ? negativity::twos : negativity::nones, FromNeg> (&z, &z + 1, in_begin, in_end);
            return z;
        }

        // Returns true if the sequence represents a minimum negative two's-complement
        // value, allowing arbitrary leading zero words and redundant sign extension.
        //
        // This is the one negative value whose absolute value cannot be
        // represented using the same number of bits in two's complement.
        // Converting it to sign-and-magnitude therefore requires an extra bit.
        template <std::random_access_iterator X>
        constexpr bool is_minimum_negative_twos (X begin, X end);

        // Returns true if a negative sign-and-magnitude (BC) number has a
        // magnitude that is a power of two.
        //
        // Such a number can be represented using one fewer bit in two's
        // complement, because the sign bit of BC is separate from its magnitude,
        // whereas two's complement can use its sign position as part of the
        // representation of the negative value.
        template <std::random_access_iterator X>
        constexpr bool is_negative_power_of_two_BC (X begin, X end);

        template <
            Word ToWord,
            negativity ToNeg,
            negativity FromNeg,
            std::random_access_iterator X>
        constexpr size_t result_min_size (const X begin, const X end) {

            math::sign sgn = arithmetic::sign<FromNeg> (begin, end);

            if (sgn == math::zero) return 0;

            // Start with the bit width of the input value. For signed representations,
            // this includes the sign bit.
            size_t bits = arithmetic::bit_width<FromNeg> (begin, end);

            // Determine whether the input represents a negative value.
            bool negative = false;
            if constexpr (FromNeg != negativity::nones)
                negative = sgn == math::negative;

            // Converting from unsigned to signed requires an additional sign bit.
            if constexpr (FromNeg == negativity::nones && ToNeg != negativity::nones) {
                ++bits;
            }

            // An unsigned representation cannot represent negative values.
            // For positive values, we can discard the source's sign bit.
            else if constexpr (FromNeg != negativity::nones && ToNeg == negativity::nones) {
                if (negative) throw negative_to_unsigned {};
                --bits;
            }

            // Two's complement -> sign-and-magnitude.
            // The minimum negative value has a magnitude that needs an extra bit.
            else if constexpr (FromNeg == negativity::twos && ToNeg == negativity::BC) {
                if (is_minimum_negative_twos (begin, end))
                    ++bits;
            }

            // Sign-and-magnitude -> two's complement.
            // A power-of-two magnitude can be represented with one fewer bit.
            else if constexpr (FromNeg == negativity::BC && ToNeg == negativity::twos) {
                if (is_negative_power_of_two_BC (begin, end))
                    --bits;
            }

            constexpr size_t to_bits = sizeof (ToWord) * CHAR_BIT;

            // Calculate the number of destination words needed to hold the value.
            return (bits + to_bits - 1) / to_bits;
        }

        enum word_transformation {
            identity_transformation,
            twos_negate
        };

        template <
            word_transformation Transformation,
            bool LastDigitSpecial,
            std::random_access_iterator In>
        class word_reader {
        public:
            using FromWord = std::iter_value_t<In>;

        private:
            In begin;
            In end;
            bool carry;
            size_t Count;

        public:
            constexpr word_reader (In begin, In end)
                : begin (begin), end (end),
                carry (Transformation == twos_negate), Count {0} {}

            constexpr bool completed () {
                return begin == end && !carry;
            }

            constexpr size_t count () const {
                return Count;
            }

            // set the argument to the next word value,
            // increment the reader, and return
            // whether we are at the end.
            constexpr FromWord get () {
                FromWord digit;
                Count++;

                if (begin == end) {
                    digit = carry ? 1 : 0;
                    carry = false;
                    return digit;
                }

                digit = *begin;
                begin++;

                // The final BC word contains a sign bit which is not part
                // of the magnitude. Remove it before any transformation.
                if constexpr (LastDigitSpecial) {
                    if (begin == end) {
                        constexpr FromWord sign_mask = FromWord (1) << (sizeof (FromWord) * CHAR_BIT - 1);

                        digit &= ~sign_mask;
                    }
                }

                if constexpr (Transformation == twos_negate) {
                    digit = ~digit;

                    if (carry) {
                        ++digit;
                        carry = digit == 0;
                    }
                }

                return digit;
            }
        };

        // we remove optimization because of a weird problem that happened with a similar function on -O2 and -O3
        #pragma GCC push_options
        #pragma GCC optimize("O1")
        // this helper function exists because we use a word writer differently depending on the input parameters.
        // it returns the number of words read from the word writer, which we need later.
        template <
            word_transformation Transformation,
            bool LastDigitSpecial,
            std::random_access_iterator In,
            std::random_access_iterator Out
        >
        constexpr size_t transfer_words (
            In in_begin,
            In in_stop,
            Out & out_digit,
            Out out_end
        ) {
            using ToWord = std::iter_value_t<Out>;
            using FromWord = std::iter_value_t<In>;

            constexpr std::size_t from_bits = sizeof (FromWord) * CHAR_BIT;
            constexpr std::size_t to_bits   = sizeof (ToWord) * CHAR_BIT;

            word_reader<Transformation, LastDigitSpecial, In> wr {in_begin, in_stop};

            if constexpr (to_bits == from_bits) {
                while (out_digit != out_end && !wr.completed ()) {
                    *out_digit = wr.get ();
                    out_digit++;
                }
            } else if constexpr (to_bits < from_bits) {
                constexpr std::size_t digits_per_word = from_bits / to_bits;
                constexpr ToWord mask = std::numeric_limits<ToWord>::max ();

                while (out_digit != out_end && !wr.completed ()) {
                    FromWord source_word = wr.get ();

                    for (std::size_t i = 0; i < digits_per_word; ++i) {
                        // TODO we don't really need to return the count here.
                        // the count only matters in case three
                        if (out_digit == out_end) return wr.count ();

                        *out_digit++ = static_cast<ToWord> (source_word & mask);
                        source_word >>= to_bits;
                    }
                }
            } else {
                // case 3
                constexpr std::size_t digits_per_output_word = to_bits / from_bits;

                while (out_digit != out_end && !wr.completed ()) {
                    ToWord destination_word = 0;

                    std::size_t i = 0;

                    while (true) {
                        const std::size_t shift = i * from_bits;

                        destination_word |= static_cast<ToWord> (wr.get ()) << shift;

                        i++;

                        if (i == digits_per_output_word) break;

                        if (wr.completed ()) {
                            *out_digit = destination_word;
                            return wr.count ();
                        }
                    }

                    *out_digit++ = destination_word;
                }
            }

            return wr.count ();
        }

        // the output range is required to be sufficient to hold the given number.
        // we remove optimization because of a weird problem that happened with a similar function on -O2 and -O3
        #pragma GCC push_options
        #pragma GCC optimize("O1")
        template <
            negativity ToNeg,
            negativity FromNeg,
            std::random_access_iterator Out,
            std::random_access_iterator In>
        constexpr void port_bits (
            Out out_begin,
            Out out_end,
            In in_begin,
            In in_end
        ) {
            using ToWord = std::iter_value_t<Out>;
            using FromWord = std::iter_value_t<In>;

            constexpr std::size_t from_bits = sizeof (FromWord) * CHAR_BIT;
            constexpr std::size_t to_bits   = sizeof (ToWord) * CHAR_BIT;

            constexpr bool from_signed = FromNeg != negativity::nones;
            constexpr bool to_signed = ToNeg != negativity::nones;

            bool negative = arithmetic::is_negative<FromNeg> (in_begin, in_end);

            if constexpr (from_signed && !to_signed)
                if (negative) negative_to_unsigned {};

            ToWord fill = 0;
            if constexpr (ToNeg == negativity::twos) {
                if (negative) fill = numeric_limits<ToWord>::max ();
            }

            // the minimum size required to represent the input number. In the case of
            // negativity::BC, there may be a sign bit that is beyond the range
            // [in_begin, in_stop]. However, we will know that this sign bit exists
            // because we would see that negative == true. On the other hand, there
            // may be a sign bit that is in this range if the given range is minimal.
            auto in_stop = in_begin + arithmetic::minimal_size<FromNeg> (in_begin, in_end);

            auto out_digit = out_begin;

            if (in_begin != in_stop) {

                size_t words_read;

                if constexpr (
                    FromNeg != ToNeg && from_signed && to_signed
                ) {
                    if (negative) {
                        words_read = transfer_words<
                            twos_negate,
                            FromNeg == negativity::BC> (in_begin, in_stop, out_digit, out_end);
                    } else {
                        words_read = transfer_words<
                            identity_transformation,
                            FromNeg == negativity::BC> (in_begin, in_stop, out_digit, out_end);
                    }
                } else {
                    words_read = transfer_words<
                        identity_transformation,
                        FromNeg == negativity::BC> (in_begin, in_stop, out_digit, out_end);
                }

                // handle the case that there are unused bits in the
                // current output word.
                if constexpr (to_bits > from_bits) {

                    const std::size_t input_bits = words_read * from_bits;
                    const std::size_t used_bits = input_bits % to_bits;

                    if (used_bits != 0 && out_digit != out_end) {

                        if constexpr (ToNeg == negativity::twos) {
                            if (negative)
                                *out_digit |= std::numeric_limits<ToWord>::max () << used_bits;
                        }

                        out_digit++;
                    }
                }
            }

            // fill in all remaining out digits
            while (out_digit != out_end) *out_digit++ = fill;

            // attach sign bit if necessary.
            if constexpr (ToNeg == negativity::BC) {
                if (out_begin != out_end && negative)
                    *(out_end - 1) |= arithmetic::sign_bit_of<ToWord>;
            }
        }

        // Returns true if the sequence represents a minimum negative two's-complement
        // value, allowing arbitrary leading zero words and redundant sign extension.
        // Returns true if the sequence represents the minimum negative
        // two's-complement value at some bit width, allowing redundant
        // zero padding and sign extension.
        template <std::random_access_iterator X>
        constexpr bool is_minimum_negative_twos (X begin, X end) {
            using Word = std::iter_value_t<X>;

            if (begin == end) return false;

            constexpr size_t bytes = sizeof (Word);
            constexpr Word byte_mask = 0xff;

            bool found_sign = false;

            for (auto i = begin; i != end; ++i) {
                Word word = *i;

                for (size_t b = 0; b < bytes; ++b) {
                    const auto byte = (word >> (b * CHAR_BIT)) & byte_mask;

                    if (!found_sign) {
                        if (byte == 0) continue;
                        if (byte != 0x80) return false;
                        found_sign = true;
                    }
                    else {
                        if (byte != 0xff) return false;
                    }
                }
            }

            return found_sign;
        }

        // Returns true if a negative sign-and-magnitude (BC) number has a
        // magnitude that is a power of two.
        //
        // Such a number can be represented using one fewer bit in two's
        // complement, because the sign bit of BC is separate from its magnitude,
        // whereas two's complement can use its sign position as part of the
        // representation of the negative value.
        template <std::random_access_iterator X>
        constexpr bool is_negative_power_of_two_BC (X begin, X end) {
            using Word = std::iter_value_t<X>;

            if (begin == end) return false;

            constexpr size_t bits = sizeof (Word) * CHAR_BIT;
            constexpr Word sign_mask = Word (1) << (bits - 1);

            // The BC sign bit must indicate a negative value.
            if (!(*(end - 1) & sign_mask)) return false;

            // Ignore the sign bit and count the set bits in the magnitude.
            size_t set_bits = 0;

            for (auto i = begin; i != end; ++i) {
                Word word = *i;

                if (i == end - 1)
                    word &= ~sign_mask;

                if (word != 0) {
                    // A power of two has exactly one set bit across the range.
                    if (set_bits != 0 || !std::has_single_bit (word))
                        return false;

                    set_bits = 1;
                }
            }

            return set_bits == 1;
        }

    }

}
