
// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BUILT_IN
#define DATA_MATH_NUMBER_BUILT_IN

#include <data/math/abs.hpp>
#include <data/arithmetic/arithmetic.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {

    template <std::unsigned_integral A> struct negate<A> {
        std::make_signed_t<A> operator () (const A &x) const {
            if (x & data::arithmetic::get_sign_bit<A>::value) throw exception {} << "unsigned integral too big to negate";
            return -static_cast<std::make_signed_t<A>> (x);
        }
    };

    template <std::unsigned_integral A> struct abs<A> {
        A operator () (const A &x) const {
            return x;
        }
    };

    template <std::signed_integral A> struct negate<A> {
        A operator () (const A &x) const {
            if (x == std::numeric_limits<A>::min ()) throw exception {} << "signed integral too low to negate";
            return -x;
        }
    };

    template <std::signed_integral A> struct abs<A> {
        std::make_unsigned_t<A> operator () (const A &x) const {
            return static_cast<std::make_unsigned_t<A>> (x == std::numeric_limits<A>::min () || x > 0 ? x : -x);
        }
    };

    template <std::unsigned_integral A> struct digits_base_2<A> {
        size_t operator () (const A &);
    };
}

// ensure that built-in types are now signed or unsigned types as appropriate.
namespace data {

    static_assert (signed_type<float>);
    static_assert (signed_type<double>);

    static_assert (signed_type<signed char>);
    static_assert (unsigned_type<unsigned char>);

    static_assert (signed_type<signed short int>);
    static_assert (unsigned_type<unsigned short int>);

    static_assert (signed_type<signed int>);
    static_assert (unsigned_type<unsigned int>);

    static_assert (signed_type<signed long int>);
    static_assert (unsigned_type<unsigned long int>);

    static_assert (signed_type<signed long long int>);
    static_assert (unsigned_type<unsigned long long int>);

}

#endif
