// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ABS
#define DATA_ABS

#include <data/types.hpp>
#include <data/ordered.hpp>
#include <data/io/exception.hpp>

namespace data {

    template <typename A> constexpr auto negate (const A &);

    template <typename A> constexpr auto abs (const A &);

    template <typename X> struct make_signed;
    template <typename X> struct make_unsigned;

    template <typename A> using to_unsigned = make_unsigned<A>::type;
    template <typename A> using to_signed = make_signed<A>::type;

    template <std::integral X> struct make_signed<X> {
        using type = std::make_signed<X>::type;
    };

    template <std::integral X> struct make_unsigned<X> {
        using type = std::make_unsigned<X>::type;
    };

}

namespace data::math::def {
    template <typename A> struct negate;
    template <typename A> struct abs;
}

// these can be template specialized to make the corresponding functions behave as you wish.
namespace data {

    template <typename A> constexpr auto inline negate (const A &x) {
        return math::def::negate<A> {} (x);
    }

    // abs should match with negate
    template <typename A> constexpr auto inline abs (const A &x) {
        return math::def::abs<A> {} (x);
    }
}

namespace data::math {
    // Check if negate (A) is well-formed on A and returns A
    template <typename A>
    concept has_negate = requires (const A &a) {
            { def::negate<A> {} (a) };
        };

    // Check if negate (A) is well-formed on A and returns A
    template <typename A, typename = void>
    concept has_negate_signed = requires (const A &a) {
        { def::negate<A> {} (a) } -> Same<A>;
    };

    // Check if negate (A) is well-formed on A and does not return A
    template <typename A, typename = void>
    concept has_negate_unsigned = has_negate<A> && !has_negate_signed<A>;

    // Check if abs (A) is well-formed on A and returns A
    template <typename A, typename = void>
    struct has_abs_unsigned : std::false_type {};

    template <typename A>
    struct has_abs_unsigned<A, std::void_t<decltype (abs (std::declval<A> ()))>>
        : std::is_same<decltype (abs (std::declval<A> ())), A> {};

    // Check if abs (A) is well-formed on A and does not return A
    template <typename A, typename = void>
    struct has_abs_signed : std::false_type {};

    template <typename A>
    struct has_abs_signed<A, std::void_t<decltype (abs (std::declval<A> ()))>>
        : std::bool_constant<!has_abs_unsigned<A>::value> {};

    template <typename A>
    concept abs_and_negate_signed =
        has_abs_signed<A>::value && has_negate_signed<A> &&
        requires (const A &a) {
            { negate (abs (a)) } -> ImplicitlyConvertible<A>;
        };

    template <typename A>
    concept abs_and_negate_unsigned =
        has_abs_unsigned<A>::value && has_negate_unsigned<A> &&
        requires (const A &a) {
            { abs (negate (a)) } -> ImplicitlyConvertible<A>;
        };

    template <typename A>
    concept abs_unsigned =
        has_abs_unsigned<A>::value && !has_negate<A>;

    template <typename A, typename = void>
    struct has_make_signed : std::false_type {};

    template <typename A>
    struct has_make_signed<A, std::void_t<typename make_signed<A>::type>> : std::true_type {};

    template <typename A, typename = void>
    struct has_make_unsigned : std::false_type {};

    template <typename A>
    struct has_make_unsigned<A, std::void_t<typename make_unsigned<A>::type>> : std::true_type {};
}

namespace data {

    template <typename A> concept Signed =
        Same<A, to_signed<A>> && (!Same<A, to_unsigned<A>> || !math::has_make_unsigned<A>::value);

    template <typename A> concept Unsigned =
        Same<A, to_unsigned<A>> && (!Same<A, to_signed<A>> || !math::has_make_signed<A>::value);

    // this won't cover all possibilities. For some kinds of numbers, we will simply have to set these.
    template <math::abs_and_negate_signed X>
    struct make_signed<X> {
        using type = X;
    };

    template <math::abs_and_negate_signed X>
    struct make_unsigned<X> {
        using type = decltype (abs (std::declval<X> ()));
    };

    template <math::abs_and_negate_unsigned X>
    struct make_signed<X> {
        using type = decltype (negate (std::declval<X> ()));
    };

    template <math::abs_and_negate_unsigned X>
    struct make_unsigned<X> {
        using type = X;
    };

    template <math::abs_unsigned X>
    struct make_unsigned<X> {
        using type = X;
    };

}

// default behavior is to use the operators.
namespace data::math::def {

    template <std::floating_point A> struct abs<A> {
        constexpr A operator () (const A &x) const {
            return x < 0 ? -x : x;
        }
    };

    template <std::unsigned_integral A> struct abs<A> {
        constexpr A operator () (const A &x) const {
            return x;
        }
    };

    template <std::signed_integral A> struct abs<A> {
        constexpr A operator () (const A &x) const {
            return x < 0 ? -x : x;
        }
    };

    template <std::signed_integral A> struct negate<A> {
        constexpr A operator () (const A &x) const {
            if (x == std::numeric_limits<A>::min ()) throw exception {} << "invalid negate value " << x;
            return -x;
        }
    };

    template <typename A>
    requires (!std::signed_integral<A>) &&
    requires (const A &x) {
        { -x };
    } struct negate<A> {
        constexpr auto operator () (const A &x) {
            return -x;
        }
    };
}

static_assert (!data::math::abs_and_negate_signed<data::int64>);
static_assert (!data::math::abs_and_negate_unsigned<data::int64>);
static_assert (!data::math::abs_unsigned<data::int64>);

static_assert (!data::math::abs_and_negate_signed<data::uint64>);
static_assert (!data::math::abs_and_negate_unsigned<data::uint64>);
static_assert (!data::math::abs_unsigned<data::uint64>);

#endif
