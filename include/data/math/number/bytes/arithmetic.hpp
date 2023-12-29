// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_TWOS_COMPLEMENT
#define DATA_MATH_NUMBER_TWOS_COMPLEMENT

#include <data/math/sign.hpp>
#include <data/encoding/halves.hpp>
#include <data/math/number/complement.hpp>

#include <iostream>

namespace data::math::number::arithmetic {

    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool equal (sen z, it i, it j) {
        while (i != z) {
            if (*i != *j) return false;
            i++;
            j++;
        }
        return true;
    }

    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool greater (sen z, it i, it j) {
        while (i != z) {
            if (*i > *j) return true;
            i++;
            j++;
        }
        return false;
    }

    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool less (sen z, it i, it j) {
        while (i != z) {
            if (*i < *j) return true;
            i++;
            j++;
        }
        return false;
    }

    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    std::weak_ordering compare (sen z, it i, it j) {
        while (i != z) {
            if (*i < *j) return std::weak_ordering::less;
            if (*i > *j) return std::weak_ordering::greater;
            i++;
            j++;
        }
        return std::weak_ordering::equivalent;
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    void bit_negate (sen z, ito i, iti j) {
        while (i != z) {
            *i = ~ *j;
            i++;
            j++;
        }
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    void bit_and (sen z, ito i, iti a, iti b) {
        while (i != z) {
            *i = *a & *b;
            i++;
            a++;
            b++;
        }
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    void bit_or (sen z, ito i, iti a, iti b) {
        while (i != z) {
            *i = *a | *b;
            i++;
            a++;
            b++;
        }
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    void bit_xor (sen z, ito i, iti a, iti b) {
        while (i != z) {
            *i = *a ^ *b;
            i++;
            a++;
            b++;
        }
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit plus (sen z, ito &a, digit d, iti &b) {
        using two_digits = typename encoding::twice<digit>::type;

        digit remainder = d;
        while (a != z) {
            two_digits result = encoding::add<digit> (*b, 0, remainder);
            *a = encoding::lesser_half (result);
            remainder = encoding::greater_half (result);

            a++;
            b++;
        }

        return remainder;
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit minus (sen z, ito &a, digit d, iti &b) {
        digit remainder = d;
        while (a != z) {
            if (*b >= remainder) {
                *a = *b - remainder;
                remainder = 0;
            } else {
                *a = encoding::lesser_half<typename encoding::twice<digit>::type> (encoding::combine<digit> (1, *b) - remainder);
                remainder = 1;
            }

            a++;
            b++;
        }

        return remainder;
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit plus (sen z, ito &i, iti &a, iti &b) {
        using two_digits = typename encoding::twice<digit>::type;

        digit remainder = 0;

        while (i != z) {
            two_digits result = encoding::add<digit> (*a, *b, remainder);
            remainder = encoding::greater_half (result);
            *i = encoding::lesser_half (result);
            i++;
            a++;
            b++;
        }

        return remainder;
    }

    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit minus (sen z, ito &i, iti &a, iti &b) {
        using two_digits = typename encoding::twice<digit>::type;

        two_digits remainder = 0;

        while (i != z) {

            auto step = encoding::combine<digit> (1, *a) - *b - remainder;

            *i = encoding::lesser_half<typename encoding::twice<digit>::type> (step);

            remainder = encoding::greater_half<typename encoding::twice<digit>::type> (step) == 0 ? 1 : 0;

            i++;
            a++;
            b++;
        }

        return remainder;
    }

    // bit shift operations are defined in terms of big-endian numbers.
    // but really they are operations to powers of 2 ignoring sign,
    // regardless of the way that a number is actually represented.
    // we can only do this with bytes and not bigger numbers because of
    // little-endian encoding.
    template <typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, byte> && std::sentinel_for<sen, itb>
    void shift_left (ita &i, sen z, itb b, byte amount, byte fill) {
        using two_digits = typename encoding::twice<byte>::type;

        while (b != z) {
            auto bp = b;
            bp++;
            two_digits result = encoding::combine<byte> (*b, bp != z ? *bp : fill) << amount;
            *i = encoding::greater_half (result);
            i++;
            b = bp;
        }
    }

    // you have to use reverse iterators for this function.
    template <typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, byte> && std::sentinel_for<sen, itb>
    void shift_right (ita &i, sen z, itb b, byte amount, byte fill) {
        using two_digits = typename encoding::twice<byte>::type;

        while (b != z) {
            auto bp = b;
            bp++;
            two_digits x = encoding::combine<byte> (bp != z ? *bp : fill, *b);
            two_digits result = encoding::combine<byte> (bp != z ? *bp : fill, *b) >> amount;
            *i = encoding::lesser_half (result);
            i++;
            b = bp;
        }
    }

    template <typename it, typename sen>
    void bit_shift_left (it i, sen z, uint32 x, bool fill) {
        auto bytes = x / 8;
        auto bits = x % 8;
        byte filler = fill ? ~0 : 0;

        size_t size = z - i;
        if (bytes <= size) arithmetic::shift_left (i, z, i + bytes, bits, filler);

        while (i != z) {
            *i = filler;
            i++;
        }
    }

    template <typename it, typename sen>
    void bit_shift_right (it i, sen z, uint32 x, bool fill) {
        auto bytes = x / 8;
        auto bits = x % 8;
        byte filler = fill ? ~0 : 0;

        size_t size = z - i;
        if (bytes <= size) arithmetic::shift_right (i, z, i + bytes, bits, filler);

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

    template <std::integral digit> struct get_sign_bit;

    template <> struct get_sign_bit<byte> {
        static const byte value = 0x80;
    };

    template <> struct get_sign_bit<uint16> {
        static const uint16 value = 0x8000;
    };

    template <> struct get_sign_bit<uint32> {
        static const uint32 value = 0x80000000;
    };

    template <> struct get_sign_bit<uint64> {
        static const uint64 value = 0x8000000000000000;
    };

    template <typename X> using sign_bit_of = get_sign_bit<X>::value;

    template <typename X> concept range = std::ranges::random_access_range<X>;

    template <typename X> using digit = std::remove_const_t<std::remove_reference_t<decltype (std::declval<const X> ()[0])>>;

    template <range X>
    size_t inline size (const X x) {
        return x.end () - x.begin ();
    }

    template <range X>
    bool inline sign_bit (X x) {
        return (size (x) != 0) && (x[-1] & get_sign_bit<digit<X>>::value) != 0;
    }

    template <range X>
    bool inline is_zero (const X x) {
        for (const auto &d : x) if (d != 0) return false;
        return true;
    }

    template <complement c, range X>
    size_t minimal_size (X x) {
        if constexpr (c == complement::nones) {
            int xsize = size (x);
            for (auto i = x.rbegin (); i != x.rend (); i++) {
                if (*i != 0) break;
                else xsize--;
            }

            return xsize;
        } else if constexpr (c == complement::ones) {
            if (size (x) == 0) return 0;
            digit<X> d = *x.rbegin ();
            if (d != std::numeric_limits<digit<X>>::max () && d != 0) return size (x);
            int repeated_bytes = 0;
            auto i = x.rbegin () + 1;
            while (true) {
                // if we reach the end, then the number is either 0 or -1.
                if (i == x.rend ()) return d == 0 ? 0 : 1;
                if (*i == d) repeated_bytes ++;
                else return size (x) - repeated_bytes -
                    (*i < get_sign_bit<digit<X>>::value && d == 0 ||
                        *i >= get_sign_bit<digit<X>>::value && d == std::numeric_limits<digit<X>>::max () ? 1 : 0);
                i++;
            }
        } else if constexpr (c == complement::twos) {
            if (size (x) == 0) return 0;
            // numbers that don't begin with 00 or 80 are minimal.
            auto i = x.rbegin ();
            digit<X> d = *i;
            if (d != get_sign_bit<digit<X>>::value && d != 0) return size (x);

            // count the number of zero bytes after the first.
            int extra_zero_bytes = 0;
            while (true) {
                i++;
                // if we reach the end then this number is zero.
                if (i == x.rend ()) return 0;
                if (*i == 0) extra_zero_bytes ++;
                // if the first non-zero digit does not have the
                // sign bit set then we can remove an extra digit.
                else return size (x) - extra_zero_bytes - (*i < get_sign_bit<digit<X>>::value ? 1 : 0);
            }
        }
    }

    template <complement c, range X>
    std::weak_ordering compare (X a, X b) {
        if constexpr (c == complement::nones) {
            auto za = size (a);
            auto zb = size (b);
            if (za < zb) return 0 <=> compare<c> (b, a);
            size_t size_difference = za - zb;

            auto ai = a.rbegin ();
            for (int i = 0; i < size_difference; i++) {
                if (0 != *ai) return std::weak_ordering::greater;
                ai++;
            }

            return arithmetic::compare (a.rend (), ai, b.rbegin ());
        } else if constexpr (c == complement::ones) {
        } else if constexpr (c == complement::twos) {}
    }

}

namespace data::math::number::arithmetic::nones {

    template <range X>
    bool inline is_minimal (X x) {
        return size (x) == 0 || x[-1] != 0;
    }

    template <range X>
    bool inline is_positive (X x) {
        return !is_zero (x);
    }

    template <range X>
    bool inline is_negative (X x) {
        return false;
    }

    template <range X>
    math::sign inline sign (X x) {
        for (auto i = x.rbegin (); i != x.rend (); i++) if (*i != 0) return math::positive;
        return math::zero;
    }

}

namespace data::math::number::arithmetic::ones {

    template <range X>
    bool is_minimal (X x) {
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
    }

    template <range X>
    bool inline is_positive (X x) {
        return !is_zero (x) && !sign_bit (x);
    }

    template <range X>
    bool inline is_negative (X x) {
        return sign_bit (x);
    }

    template <range X>
    math::sign sign (X x) {
        auto i = x.rbegin ();
        if (i == x.rend ()) return math::zero;
        if (*i & get_sign_bit<digit<X>>::value) return math::negative;
        while (i != x.rend ()) {
            if (*i != 0) return math::positive;
            i++;
        }
        return math::zero;
    }

}

namespace data::math::number::arithmetic::twos {

    template <range X>
    bool is_zero (X x) {
        auto i = x.rbegin ();
        auto e = x.rend ();
        if (i == e) return true;
        if (*i != 0 && *i != get_sign_bit<digit<X>>::value) return false;
        while (true) {
            i++;
            if (i == e) return true;
            if (*i != 0) return false;
        }
    }

    template <range X>
    bool is_negative_zero (X x) {
        auto i = x.rbegin ();
        auto e = x.rend ();
        if (i == e) return false;
        if (*i != get_sign_bit<digit<X>>::value) return false;
        while (true) {
            i++;
            if (i == e) return true;
            if (*i != 0) return false;
        }
    }

    template <range X>
    bool inline is_positive_zero (X x) {
        return arithmetic::is_zero (x);
    }

    template <range X>
    bool is_minimal (X x) {
        // minimal zero.
        return (size (x) == 0) ||
            // numbers without an initial 00 or 80.
            (x[-1] != 0 && x[-1] != get_sign_bit<digit<X>>::value) ||
            // numbers that would be interpreted as having the wrong sign if they were shortened.
            (size (x) > 1 && (x[-2] & get_sign_bit<digit<X>>::value));
    }

    template <range X>
    bool inline is_positive (X x) {
        return !sign_bit (x) && !is_positive_zero (x);
    }

    template <range X>
    bool inline is_negative (X x) {
        return sign_bit (x) && !is_negative_zero (x);
    }

    template <range X>
    math::sign sign (X x) {
        auto i = x.rbegin ();
        if (i == x.rend ()) return math::zero;
        math::sign nonzero = (*i & get_sign_bit<digit<X>>::value) ? math::negative : math::positive;
        if (*i & ~get_sign_bit<digit<X>>::value) return nonzero;
        while (true) {
            i++;
            if (i == x.rend ()) return math::zero;
            if (*i != 0) return nonzero;
        }
    }

    template <range X>
    bool inline cast_to_bool (X x) {
        return !is_zero (x);
    }

}

#endif
