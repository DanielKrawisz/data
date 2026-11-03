// Copyright (c) 2023-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/sign.hpp>
#include <data/norm.hpp>
#include <data/math/algebra.hpp>
#include <data/arithmetic/halves.hpp>
#include <data/arithmetic/negativity.hpp>
#include <data/arithmetic/carry.hpp>

#include <iostream>

namespace data::arithmetic {

    // NOTE: there is an inconsistent ordering of variables in this file.
    template <std::input_iterator it, std::sentinel_for<it> sen>
    constexpr bool equal (it i, sen z, it j) {
        while (i != z) {
            if (*i != *j) return false;
            i++;
            j++;
        }
        return true;
    }

    template <std::input_iterator it, std::sentinel_for<it> sen>
    constexpr bool greater (it i, sen z, it j) {
        while (i != z) {
            if (*i > *j) return true;
            i++;
            j++;
        }
        return false;
    }

    template <std::input_iterator it, std::sentinel_for<it> sen>
    constexpr bool less (it i, sen z, it j) {
        while (i != z) {
            if (*i < *j) return true;
            i++;
            j++;
        }
        return false;
    }

    template <std::input_iterator it, std::sentinel_for<it> sen>
    constexpr std::strong_ordering compare (it i, sen z, it j) {

        while (i != z) {
            if (*i < *j) return std::strong_ordering::less;
            if (*i > *j) return std::strong_ordering::greater;
            i++;
            j++;
        }

        return std::strong_ordering::equal;
    }

    template <typename digit,
        std::output_iterator<digit> ito,
        std::sentinel_for<ito> sen,
        std::input_iterator iti>
    constexpr void bit_negate (ito o, sen z, iti i) {
        while (o != z) {
            *o = ~*i;
            o++;
            i++;
        }
    }

    template <typename digit,
        std::output_iterator<digit> ito,
        std::sentinel_for<ito> sen,
        std::input_iterator ita,
        std::input_iterator itb>
    constexpr void bit_and (ito i, sen z, ita a, itb b) {
        while (i != z) {
            *i = *a & *b;
            i++;
            a++;
            b++;
        }
    }

    template <typename digit,
        std::output_iterator<digit> ito,
        std::sentinel_for<ito> sen,
        std::input_iterator ita,
        std::input_iterator itb>
    constexpr void bit_or (ito i, sen z, ita a, itb b) {
        while (i != z) {
            *i = *a | *b;
            i++;
            a++;
            b++;
        }
    }

    template <typename digit,
        std::output_iterator<digit> ito,
        std::sentinel_for<ito> sen,
        std::input_iterator ita,
        std::input_iterator itb>
    constexpr void bit_xor (ito i, sen z, ita a, itb b) {
        while (i != z) {
            *i = *a ^ *b;
            i++;
            a++;
            b++;
        }
    }

    // TODO continue to adjust the parameters to be closer to those above.

    // bit shift operations are defined in terms of big-endian numbers.
    // but really they are operations to powers of 2 ignoring sign,
    // regardless of the way that a number is actually represented.

    // amount should be less than the number of digits base 2 in the word.
    // We should iterate from little to big endian.
    template <typename sen, typename ita, std::input_iterator itb, Word word>
    requires std::output_iterator<ita, word> && std::sentinel_for<sen, itb>
    constexpr std::enable_if_t<has_twice<word>::value, void>
    shift_left (ita &i, sen z, itb b, uint16 amount, word fill) {
        using two_digits = typename twice<word>::type;

        while (b != z) {
            auto bp = b;
            bp++;
            two_digits result = combine<word> (*b, bp != z ? *bp : fill) << amount;
            *i =  greater_half (result);
            i++;
            b = bp;
        }
    }

    // amount should be less than the number of digits base 2 in the word.
    // We should iterate from big to little endian.
    template <typename sen, typename ita, std::input_iterator itb, Word word>
    requires std::output_iterator<ita, word> && std::sentinel_for<sen, itb>
    constexpr std::enable_if_t<has_twice<word>::value, void>
    shift_right (ita &i, sen z, itb b, uint16 amount, word fill) {
        using two_digits = typename twice<word>::type;

        while (b != z) {
            auto bp = b;
            bp++;
            two_digits x =  combine<word> (bp != z ? *bp : fill, *b);
            two_digits result =  combine<word> (bp != z ? *bp : fill, *b) >> amount;
            *i =  lesser_half (result);
            i++;
            b = bp;
        }
    }

    // amount should be less than the number of digits base 2 in the word.
    // We should iterate from little to big endian.
    template <typename sen, typename ita, std::input_iterator itb, Word word>
    requires std::output_iterator<ita, word> && std::sentinel_for<sen, itb>
    constexpr std::enable_if_t<!has_twice<word>::value, void>
    shift_left (ita &i, sen z, itb b, uint16 amount, word fill) {
        constexpr static const uint16 digits = sizeof (word) * CHAR_BIT;
        word prev = fill >> (digits - amount);

        while (b != z) {
            *i = static_cast<word> ((static_cast<uint32> (*b) << amount) | prev);
            prev = *b >> (digits - amount);
            i++;
            b++;
        }
    }

    // amount should be less than the number of digits base 2 in the word.
    // We should iterate from big to little endian.
    template <typename sen, typename ita, std::input_iterator itb, Word word>
    requires std::output_iterator<ita, word> && std::sentinel_for<sen, itb>
    constexpr std::enable_if_t<!has_twice<word>::value, void>
    shift_right (ita &i, sen z, itb b, uint16 amount, word fill) {
        constexpr static const uint16 digits = sizeof (word) * CHAR_BIT;
        word prev = fill << (digits - amount);

        while (b != z) {
            *i = static_cast<word> ((static_cast<uint32> (*b) >> amount) | prev);
            prev = *b << (digits - amount);
            i++;
            b++;
        }
    }

    template <Word word, typename it, typename sen>
    requires std::output_iterator<it, word> && std::sentinel_for<sen, it>
    constexpr void bit_shift_left (it i, sen z, uint32 x, bool fill) {
        size_t words = x / (8 * sizeof (decltype (*i)));
        uint32 bits = x % (8 * sizeof (decltype (*i)));
        word filler = fill ? ~0 : 0;

        size_t size = z - i;
        if (words <= size) arithmetic::shift_left (i, z, i + words, bits, filler);

        while (i != z) {
            *i = filler;
            i++;
        }
    }

    template <Word word, typename it, typename sen>
    requires std::output_iterator<it, word> && std::sentinel_for<sen, it>
    constexpr void bit_shift_right (it i, sen z, uint32 x, bool fill) {
        size_t words = x / (8 * sizeof (decltype (*i)));
        uint32 bits = x % (8 * sizeof (decltype (*i)));
        word filler = fill ? ~0 : 0;

        size_t size = z - i;
        if (words <= size) arithmetic::shift_right (i, z, i + words, bits, filler);

        while (i != z) {
            *i = filler;
            i++;
        }
    }

    template <size_t size, size_t mod8 = size % 8, size_t mod4 = size % 4, size_t mod2 = size % 2>
    struct max_word_size;

    template <size_t size> struct max_word_size<size, 0, 0, 0> {
        using type = uint64;
    };

    template <size_t size> struct max_word_size<size, 4, 0, 0> {
        using type = uint32;
    };

    template <size_t size, size_t mod8> struct max_word_size<size, mod8, 2, 0> {
        using type = uint16;
    };

    template <size_t size, size_t mod8, size_t mod4> struct max_word_size<size, mod8, mod4, 0> {
        using type = byte;
    };

    template <std::unsigned_integral digit> struct get_limit {
        constexpr static const digit max_unsigned = std::numeric_limits<digit>::max ();
        constexpr static const digit max_signed = (static_cast<digit> (max_unsigned << 1)) >> 1;
    };

    template <std::unsigned_integral digit> struct get_sign_bit {
        constexpr static const digit value = ~static_cast<digit> (get_limit<digit>::max_signed);
    };

    template <typename digit> constexpr static const digit sign_bit_of = get_sign_bit<digit>::value;

    template <typename digit> const digit max_unsigned = get_limit<digit>::max_unsigned;
    template <typename digit> const digit max_signed_twos = get_limit<digit>::max_signed;
    template <typename digit> const digit min_signed_twos = get_sign_bit<digit>::value;

    template <typename X> concept range = std::ranges::random_access_range<X>;

    template <typename X> using digit = std::remove_const_t<unref<decltype (std::declval<const X> ()[0])>>;

    template <range X>
    constexpr size_t size (const X x) {
        return x.end () - x.begin ();
    }

    template <std::random_access_iterator X>
    constexpr bool sign_bit (const X begin, const X end) {
        return (begin != end) && (*(end - 1) & get_sign_bit<std::iter_value_t<X>>::value) != 0;
    }

    template <range X>
    constexpr bool sign_bit (X x) {
        return sign_bit (x.begin (), x.end ());
    }

    template <negativity neg, std::random_access_iterator X>
    constexpr bool is_zero (const X begin, const X end) {
        if (begin == end) return true;

        if constexpr (neg == negativity::nones || neg == negativity::twos) {
            auto i = end;
            do {
                --i;
                 if (*i != 0) return false;
            } while (i != begin);

            return true;
        } else if constexpr (neg == negativity::BC) {

            auto i = end - 1;

            // we know there must be at least one digit since the number is not zero.
            auto dig = *i & ~sign_bit_of<std::iter_value_t<X>>;

            while (true) {
                if (dig != 0)
                    return false;

                if (i == begin) return true;

                --i;
                dig = *i;
            }
        }

        throw exception {} << "invalid negativity " << neg;
    }

    template <negativity neg, range X>
    constexpr bool is_zero (const X x) {
        return is_zero<neg> (x.begin (), x.end ());
    }

    template <negativity c, std::random_access_iterator X>
    constexpr size_t minimal_size (const X begin, const X end) {

        const size_t size = end - begin;

        if (size == 0) return 0;

        X rbegin = begin + (size - 1);

        size_t min_size = size;

        if constexpr (c == negativity::nones) {

            auto i = rbegin;

            while (true) {
                if (*i != 0) return min_size;

                else --min_size;
                if (i == begin) return min_size;
                --i;
            }

        } else if constexpr (c == negativity::twos) {
            std::iter_value_t<X> fill = *rbegin;

            if (fill != std::numeric_limits<decltype (fill)>::max () && fill != 0)
                return size;

            const auto sign_bit = sign_bit_of<decltype (fill)>;
            const bool sign = (fill & sign_bit) != 0;

            auto i = rbegin;

            while (true) {

                if (*i != fill) {
                    const bool digit_sign = (*i & sign_bit) != 0;
                    return min_size + (digit_sign != sign ? 1 : 0);
                }

                --min_size;

                if (i == begin) {
                    // we need at least one word for -1
                    return sign ? 1 : 0;
                }

                --i;
            }


        } else if constexpr (c == negativity::BC) {

            // numbers that don't begin with 00 or 80 are minimal.
            auto i = rbegin;

            // remove sign bit from last digit.
            std::iter_value_t<X> digit = *i & ~get_sign_bit<std::iter_value_t<X>>::value;

            while (digit == 0) {
                --min_size;
                if (i == begin) return 0;
                --i;
                digit = *i;
            };

            // if the sign bit is set, we have to add an extra digit.
            return digit < get_sign_bit<std::iter_value_t<X>>::value ? min_size: min_size + 1;

        }

        throw exception {} << "invalid negativity" << c;
    }

    template <negativity neg, range X>
    constexpr size_t minimal_size (X x) {
        return minimal_size<neg> (x.begin (), x.end ());
    }

    template <negativity neg, std::random_access_iterator X>
    constexpr math::sign sign (const X begin, const X end) {
        if constexpr (neg == negativity::nones) {
            return is_zero<neg> (begin, end) ? math::zero : math::positive;
        }

        if (begin == end) return math::zero;

        // we know there must be at least one digit since range isn't empty.
        auto i = end - 1;

        // get the sign bit.
        bool sign_bit = (*i & sign_bit_of<std::iter_value_t<X>>) != 0;

        if constexpr (neg == negativity::twos) {

            while (true) {
                if (*i != 0)
                    return sign_bit ? math::negative : math::positive;

                if (i == begin) return math::zero;

                --i;
            }

        } else if constexpr (neg == negativity::BC) {

            auto dig = *i & ~sign_bit_of<std::iter_value_t<X>>;

            while (true) {
                if (dig != 0)
                    return sign_bit ? math::negative : math::positive;

                if (i == begin) return math::zero;

                --i;
                dig = *i;
            }
        }

        throw exception {} << "invalid negativity";
    }

    template <negativity neg, range X>
    constexpr math::sign sign (X x) {
        return sign<neg> (x.begin (), x.end ());
    }

    namespace BC {
        template <std::random_access_iterator X>
        constexpr bool is_zero (const X begin, const X end) {
            auto i = begin;

            if (i == end) return true;
            if (*i != 0 && *i != get_sign_bit<std::iter_value_t<X>>::value) return false;

            while (true) {
                i++;
                if (i == end) return true;
                if (*i != 0) return false;
            }
        }

        template <range X>
        constexpr bool is_zero (X x) {
            return is_zero (x.begin (), x.end ());
        }

        template <std::random_access_iterator X>
        constexpr bool is_negative_zero (const X begin, const X end) {
            auto i = begin;
            if (i == end) return false;
            if (*i != get_sign_bit<std::iter_value_t<X>>::value) return false;
            while (true) {
                i++;
                if (i == end) return true;
                if (*i != 0) return false;
            }
        }

        template <range X>
        constexpr bool is_positive_zero (X x) {
            return arithmetic::is_zero (x);
        }
    }

    template <negativity neg, std::random_access_iterator X>
    constexpr bool is_negative (const X begin, const X end) {
        if constexpr (neg == negativity::nones) {
            return false;
        } else if constexpr (neg == negativity::twos) {
            return sign_bit (begin, end);
        } else if constexpr (neg == negativity::BC) {
            return sign_bit (begin, end) && !BC::is_negative_zero (begin, end);
        }
    }

    template <negativity neg, range X>
    constexpr bool is_negative (X x) {
        return is_negative<neg> (x.begin (), x.end ());
    }

    template <negativity neg, range X>
    constexpr bool get_digit (X x, int32 i) {
        if (i < 0) return false;
        constexpr size_t digit_size = sizeof (*x.begin ()) * CHAR_BIT;
        size_t range_size = size (x);

        if constexpr (neg == negativity::nones) {
            if (i >= range_size * digit_size) return false;
        } else if constexpr (neg == negativity::twos) {
            if (i >= range_size * digit_size) return is_negative<neg> (x);
        } else if constexpr (neg == negativity::BC) {
            if (i >= range_size * digit_size || i == range_size * digit_size - 1) return false;
        }

        return (*(x.begin () + (i / digit_size)) >> (i % digit_size)) & 1;
    }

    template <negativity neg, std::random_access_iterator X>
    constexpr size_t bit_width (const X begin, const X end) {
        constexpr const size_t word_bits = sizeof (std::iter_value_t<X>) * CHAR_BIT;
        size_t size = end - begin;

        if constexpr (neg == negativity::nones) {
            if (size == 0) return 0;

            size_t width = size * word_bits;

            auto i = end;

            do {
                --i;
                width -= word_bits;
                if (*i != 0) {
                    width += std::bit_width (*i); // note: this is only defined for unsigned integral types.
                    break;
                }
            } while (i != begin);

            return width;
        } else {
            if (size == 0) return 1;

            size_t width = size * word_bits;

                if constexpr (neg == negativity::twos) {

                math::sign sgn = sign<negativity::twos> (begin, end);
                if (sgn == math::zero) return 1;

                bool negative = sgn == math::negative;

                auto i = end;

                do {
                    --i;

                    width -= word_bits;

                    if (!negative && *i != 0) {
                        width += std::bit_width (*i); // note: this is only defined for unsigned integral types.
                        break;
                    }

                    if (negative && *i != std::numeric_limits<std::iter_value_t<X>>::max ()) {
                        width += std::bit_width (static_cast<std::iter_value_t<X>> (~*i));
                        break;
                    }
                } while (i != begin);

                return width + 1;
            } else if constexpr (neg == negativity::BC)  {

                math::sign sgn = sign<negativity::BC> (begin, end);
                if (sgn == math::zero) return 1;

                auto i = end - 1;

                // we know there must be at least one digit since the number is not zero.
                std::iter_value_t<X> dig = *i & ~sign_bit_of<std::iter_value_t<X>>;

                while (true) {
                    width -= word_bits;
                    if (dig != 0) {
                        width += std::bit_width (dig); // note: this is only defined for unsigned integral types.
                        break;
                    }

                    if (i == begin) break;

                    --i;
                    dig = *i;
                }

                return width + 1;
            }
        }

        throw exception {} << "invalid negativity " << neg;
    }

    template <negativity neg, range X>
    requires Word<digit<X>>
    constexpr size_t bit_width (X x) {
        return bit_width<neg> (x.begin (), x.end ());
    }

    template <negativity neg, range X>
    requires Word<digit<X>>
    constexpr bool is_minimal (X x) {

        if constexpr (neg == negativity::nones) {
            return size (x) == 0 || x[-1] != 0;
        } else if constexpr (neg == negativity::twos) {
            // minimal zero.
            if (size (x) == 0) return true;
            // numbers of one byte.
            if (size (x) == 1) return x[1] != 0;
            // numbers without an initial 00 or ff.
            digit<X> biggest = x[-1];
            digit<X> max_value = std::numeric_limits<digit<X>>::max ();
            if (biggest != max_value && biggest != 0) return true;
            // numbers that would be interpreted with the wrong sign if they were shortened.
            digit<X> next_biggest = x[-2];
            return (biggest == 0 && (next_biggest & get_sign_bit<digit<X>>::value)) ||
            (biggest == max_value && !(next_biggest & get_sign_bit<digit<X>>::value));
        } else if constexpr (neg == negativity::BC) {
            // minimal zero.
            return (size (x) == 0) ||
            // numbers without an initial 00 or 80.
            (x[-1] != 0 && x[-1] != sign_bit_of<digit<X>>) ||
            // numbers that would be interpreted as having the wrong sign if they were shortened.
            (size (x) > 1 && (x[-2] & sign_bit_of<digit<X>>));
        }

        throw exception {} << "invalid negativity " << neg;
    }

    template <negativity neg, range X>
    requires Word<digit<X>>
    constexpr bool is_positive (X x) {

        if constexpr (neg == negativity::nones) {
            return !is_zero (x);
        } else if constexpr (neg == negativity::twos) {
            return !is_zero (x) && !sign_bit (x);
        } else if constexpr (neg == negativity::BC) {
            return !sign_bit (x) && !is_positive_zero (x);
        }

        throw exception {} << "invalid negativity " << neg;
    }

}

namespace data::arithmetic::twos {

    template <range X>
    constexpr bool is_negative_one (const X x) {
        if (size (x) == 0) return false;
        for (const auto &d : x) if (d != max_unsigned<digit<X>>) return false;
        return true;
    }

}

namespace data::arithmetic::BC {

    template <range X>
    constexpr bool cast_to_bool (X x) {
        return !is_zero (x);
    }

}

namespace data::arithmetic {

    template <negativity c, range X>
    constexpr std::strong_ordering compare (X a, X b) {
        if constexpr (c == negativity::nones) {

            auto za = size (a);
            auto zb = size (b);
            if (za < zb) return 0 <=> compare<c> (b, a);
            size_t size_difference = za - zb;

            auto ai = a.rbegin ();
            for (int i = 0; i < size_difference; i++) {
                if (0 != *ai) return std::strong_ordering::greater;
                ai++;
            }

            return arithmetic::compare (ai, a.rend (), b.rbegin ());
        } else {

            math::sign sga = sign<c> (a);
            math::sign sgb = sign<c> (b);

            if (sga != sgb) {
                if (sga == math::negative) return std::strong_ordering::less;
                if (sgb == math::negative) return std::strong_ordering::greater;
                if (sga == math::zero) return std::strong_ordering::less;
                if (sgb == math::zero) return std::strong_ordering::greater;

                throw exception {} << "it shouldn't be possible to get here.";
            }

            if (sga == math::zero) return std::strong_ordering::equal;

            auto za = size<c> (a);
            auto zb = size<c> (b);

            X *greater;
            X *lesser;

            size_t size_difference;
            bool reversed;

            if (za >= zb) {
                greater = &a;
                lesser = &b;
                size_difference = za - zb;
                reversed = false;
            } else {
                greater = &b;
                lesser = &a;
                size_difference = zb - za;
                reversed = true;
            }

            using word = decltype (*a.begin ());
            word check_digit;
            std::strong_ordering base_case_result;
            if (sga == math::negative) {
                base_case_result = reversed ? std::strong_ordering::greater : std::strong_ordering::less;
                check_digit = std::numeric_limits<word>::max ();
            } else {
                base_case_result = reversed ? std::strong_ordering::less : std::strong_ordering::greater;
                check_digit = 0;
            }

            auto ai = greater->rbegin ();
            for (int i = 0; i < size_difference; i++) {
                if (check_digit != *ai) return base_case_result;
                ai++;
            }

            std::strong_ordering compare = arithmetic::compare (a.rend (), ai, b.rbegin ());

            return reversed ? 0 <=> compare : compare;
        }
    }
}
