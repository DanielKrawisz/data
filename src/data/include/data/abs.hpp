// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ABS
#define DATA_ABS

#include <data/concepts.hpp>
#include <data/ordered.hpp>
#include <data/io/exception.hpp>

namespace data {

    // we determine whether a type is siged or not based on whether
    // the negate function returns the same or different type.
    template <typename A> auto negate (const A &);

    // abs should match with negate
    template <typename A> auto abs (const A &);

}

// these can be template specialized to make the corresponding functions behave as you wish.
namespace data::math {
    template <typename A> struct negate;
    template <typename A> struct abs;

    // Check if negate<A> {} is well-formed on A and returns A
    template <typename A, typename = void>
    struct has_negate_signed : std::false_type {};

    template <typename A>
    struct has_negate_signed<A, std::void_t<decltype (negate<A> {} (std::declval<A> ()))>>
        : std::is_same<decltype (negate<A> {} (std::declval<A> ())), A> {};

    // this will be true for any type for which negate is defined and returns the same type, otherwise false.
    template <typename A>
    inline constexpr bool is_signed_type = has_negate_signed<A>::value;

    // Check if abs<A> {} is well-formed on A and returns X
    template <typename A, typename X, typename = void>
    struct has_abs_unsigned : std::false_type {};

    template <typename A, typename X>
    struct has_abs_unsigned<A, X, std::void_t<decltype (abs<A> {} (std::declval<A> ()))>>
        : std::is_same<decltype (abs<A> {} (std::declval<A> ())), X> {};

    // check if abs<X> {} (negate<X> {} (X)) is the same as X if negate<X> {} (X) is defined
    template <typename X, typename = void>
    struct satisfies_unsigned_condition : std::true_type {};

    template <typename X>
    struct satisfies_unsigned_condition<X, std::void_t<decltype (negate<X> {} (std::declval<X> ()))>> :
        has_abs_unsigned<decltype (negate<X> {} (std::declval<X> ())), X> {};

    // A type is unsigned if it is not a signed type and if negate is defined on
    // it, then abs must be defined on its negation and return the same type.
    template <typename A>
    inline constexpr bool is_unsigned_type = !is_signed_type<A> && satisfies_unsigned_condition<A>::value;

}

namespace data {

    // a signed type is the same type as that which is returned when we negate it.
    template <typename A> concept signed_type = math::is_signed_type<A>;

    // unsigned types are not siged types.
    // Negate is not required to be defined
    // but abs must return the same type if it is defined.
    template <typename A> concept unsigned_type = math::is_unsigned_type<A>;

    template <typename X> struct make_signed;
    template <typename X> struct make_unsigned;

    template <signed_type A> using to_unsigned = make_unsigned<A>::type;
    template <unsigned_type A> using to_signed = make_signed<A>::type;

    template <typename A> auto inline negate (const A &x) {
        return math::negate<A> {} (x);
    }

    // abs should match with negate
    template <typename A> auto inline abs (const A &x) {
        return math::abs<A> {} (x);
    }

    // default definitions of make_signed and make_unsigned.
    template <signed_type X> struct make_signed<X> {
        using type = X;
    };

    template <unsigned_type X> struct make_unsigned<X> {
        using type = X;
    };

    template <unsigned_type X> struct make_signed<X> {
        using type = decltype (negate (std::declval<X> ()));
    };

    // in some cases, we work with a signed type only with no
    // corresponding unsigned type. Then make_unsigned should
    // provide the same type.
    template <signed_type X> struct make_unsigned<X> {
        using type = decltype (abs (std::declval<X> ()));
    };

}

// default behavior is to use the operators.
namespace data::math {

    template <std::floating_point A> struct negate<A> {
        A operator () (const A &x) const {
            return -x;
        }
    };

    template <std::floating_point A> struct abs<A> {
        A operator () (const A &x) const {
            return x < 0 ? -x : x;
        }
    };

    template <std::unsigned_integral A> struct abs<A> {
        A operator () (const A &x) const {
            return x;
        }
    };

    template <std::signed_integral A> struct abs<A> {
        A operator () (const A &x) const {
            return x < 0 ? -x : x;
        }
    };

    template <std::unsigned_integral A> struct negate<A> {
        A operator () (const A &x) const {
            return -x;
        }
    };

    template <std::signed_integral A> struct negate<A> {
        A operator () (const A &x) const {
            if (x == std::numeric_limits<A>::min ()) throw exception {} << "invalid negate value " << x;
            return -x;
        }
    };
}

#endif
