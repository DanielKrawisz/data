// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_OVERFLOW
#define DATA_OVERFLOW

#include <data/types.hpp>
#include <data/arithmetic/halves.hpp>
#include <iostream>

namespace data::arithmetic {

    // add two numbers and return whether the carry bit was set.
    template <std::unsigned_integral x> bool add_with_carry (x &result, x a, x b);

    // add two numbers and return
    template <std::unsigned_integral x> bool subtract_with_carry (x &result, x a, x b);
    #if defined(FORCE_CARRY_FALLBACK) || !(defined(__linux__) && (defined(__clang__) || defined(__GNUC__)))

    template <> bool inline add_with_carry<unsigned int> (unsigned int &result, unsigned int a, unsigned int b) {
        result = a + b;
        return result < a;
    }

    template <> bool inline subtract_with_carry<unsigned int> (unsigned int &result, unsigned int a, unsigned int b) {
        result = a - b;
        return a < b;
    }

    template <> bool inline add_with_carry<unsigned long int> (unsigned long int &result, unsigned long int a, unsigned long int b) {
        result = a + b;
        return result < a;
    }

    template <> bool inline subtract_with_carry<unsigned long int> (unsigned long int &result, unsigned long int a, unsigned long int b) {
        result = a - b;
        return a < b;
    }

    template <> bool inline add_with_carry<unsigned long long int>
    (unsigned long long int &result, unsigned long long int a, unsigned long long int b) {
        result = a + b;
        return result < a;
    }

    template <> bool inline subtract_with_carry<unsigned long long int>
    (unsigned long long int &result, unsigned long long int a, unsigned long long int b) {
        result = a - b;
        return a < b;
    }
#else
    template <> bool inline add_with_carry<unsigned int> (unsigned int &result, unsigned int a, unsigned int b) {
        return __builtin_uadd_overflow (a, b, &result);
    }

    template <> bool inline subtract_with_carry<unsigned int> (unsigned int &result, unsigned int a, unsigned int b) {
        return __builtin_usub_overflow (a, b, &result);
    }

    template <> bool inline add_with_carry<unsigned long int> (unsigned long int &result, unsigned long int a, unsigned long int b) {
        return __builtin_uaddl_overflow (a, b, &result);
    }

    template <> bool inline subtract_with_carry<unsigned long int> (unsigned long int &result, unsigned long int a, unsigned long int b) {
        return __builtin_usubl_overflow (a, b, &result);
    }

    template <> bool inline add_with_carry<unsigned long long int>
    (unsigned long long int &result, unsigned long long int a, unsigned long long int b) {
        return __builtin_uaddll_overflow (a, b, &result);
    }

    template <> bool inline subtract_with_carry<unsigned long long int>
    (unsigned long long int &result, unsigned long long int a, unsigned long long int b) {
        return __builtin_usubll_overflow (a, b, &result);
    }
#endif
    template <> bool inline add_with_carry<unsigned short int> (unsigned short int &result, unsigned short int a, unsigned short int b) {
        using twice = typename encoding::twice<unsigned short int>::type;
        twice r = static_cast<twice> (a) + static_cast<twice> (b);
        result = encoding::lesser_half (r);
        return encoding::greater_half (r) != 0;
    }

    template <> bool inline
    subtract_with_carry<unsigned short int> (unsigned short int &result, unsigned short int a, unsigned short int b) {
        using twice = typename encoding::twice<unsigned short int>::type;
        twice r = static_cast<twice> (a) - static_cast<twice> (b);
        result = encoding::lesser_half (r);
        return encoding::greater_half (r) != 0;
    }

    template <> bool inline add_with_carry<unsigned char> (unsigned char &result, unsigned char a, unsigned char b) {
        using twice = typename encoding::twice<unsigned char>::type;
        twice r = static_cast<twice> (a) + static_cast<twice> (b);
        result = encoding::lesser_half (r);
        return encoding::greater_half (r) != 0;
    }

    template <> bool inline subtract_with_carry<unsigned char> (unsigned char &result, unsigned char a, unsigned char b) {
        using twice = typename encoding::twice<unsigned char>::type;
        twice r = static_cast<twice> (a) - static_cast<twice> (b);
        result = encoding::lesser_half (r);
        return encoding::greater_half (r) != 0;
    }

    template <std::unsigned_integral x> struct multiply_result {
        x Lesser;
        x Greater;
    };

    template <std::unsigned_integral digit>
    requires requires {
        typename encoding::twice<digit>::type;
    } multiply_result<digit> inline
    multiply_with_carry (digit a, digit b) {
        using twice = typename encoding::twice<digit>::type;
        twice r = static_cast<twice> (a) * static_cast<twice> (b);
        return {encoding::lesser_half (r), encoding::greater_half (r)};
    }

    template <std::unsigned_integral digit>
    multiply_result<digit> multiply_with_carry (digit a, digit b) {
        digit d1 = static_cast<digit> (encoding::lesser_half (a)) * static_cast<digit> (encoding::lesser_half (b));

        digit d3;
        bool carry = add_with_carry<digit> (d3,
            static_cast<digit> (encoding::greater_half (a)) * static_cast<digit> (encoding::lesser_half (b)),
            static_cast<digit> (encoding::lesser_half (a)) * static_cast<digit> (encoding::greater_half (b)));

        digit d4 = static_cast<digit> (encoding::greater_half (a)) * static_cast<digit> (encoding::greater_half (b));

        return {d1 + static_cast<digit> (encoding::lesser_half (d3)) << 32,
            d4 + encoding::greater_half (d3) + (carry ? (digit (1) << 32) : 0)};

    }

    // make sure a and b can be iterated at least as far as i.
    template <std::unsigned_integral digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit add_with_carry (sen z, ito &i, iti &a, digit b) {

        digit remainder = b;

        while (i != z) {
            remainder = add_with_carry<digit> (*i, *a, remainder) ? 1 : 0;
            i++;
            a++;
        }

        return remainder;
    }

    template <std::unsigned_integral digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit subtract_with_carry (sen z, ito &i, iti &a, digit b) {

        digit remainder = b;

        while (i != z) {
            remainder = subtract_with_carry<digit> (*i, *a, remainder) ? 1 : 0;
            i++;
            a++;
        }

        return remainder;
    }

    // make sure a and b can be iterated at least as far as i.
    template <std::unsigned_integral digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit add_with_carry (sen z, ito &i, iti &a, iti &b) {

        digit remainder = 0;

        while (i != z) {
            bool carry = add_with_carry<digit> (*i, *a, *b);
            if (remainder != 0) carry |= add_with_carry<digit> (*i, *i, remainder);
            i++;
            a++;
            b++;
            remainder = carry ? 1 : 0;
        }

        return remainder;
    }

    template <std::unsigned_integral digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti>
    digit subtract_with_carry (sen z, ito &i, iti &a, iti &b) {

        digit remainder = 0;

        while (i != z) {
            bool carry = subtract_with_carry<digit> (*i, *a, *b);
            if (remainder != 0) carry |= subtract_with_carry<digit> (*i, *i, remainder);
            i++;
            a++;
            b++;
            remainder = carry ? 1 : 0;
        }

        return remainder;
    }

}

#endif
