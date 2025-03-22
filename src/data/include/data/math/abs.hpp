// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ABS
#define DATA_MATH_ABS

#include <data/concepts.hpp>
#include <data/math/ordered.hpp>

namespace data {

    // we determine whether a type is siged or not based on whether
    // the negate function returns the same or different type.
    template <typename A> auto negate (const A &);

    // abs should match with negate
    template <typename A> auto abs (const A &);

    // a signed type is the same type as that which is returned when we negate it.
    // abs is defined but not required to return the same type, although it may.
    template <typename A> concept signed_type = same_as<A, decltype (negate (std::declval<const A &> ()))> &&
    requires (const A &a) {
        { abs (a) };
    };

    // unsigned types are not siged types. Negate is not required to be defined
    // but abs must return the same type.
    template <typename A> concept unsigned_type = !signed_type<A> &&
    requires (const A &a) {
        { abs (a) } -> same_as<A>;
    };

    template <typename X> struct make_signed;
    template <typename X> struct make_unsigned;

    template <signed_type A> using to_unsigned = make_unsigned<A>::type;
    template <unsigned_type A> using to_signed = make_signed<A>::type;

}

// these can be template specialized to make the corresponding functions behave as you wish.
namespace data::math {
    template <typename A> struct negate;
    template <typename A> struct abs;
}

namespace data {
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

    template <std::unsigned_integral X> struct make_signed<X> {
        using type = std::make_signed_t<X>;
    };

    template <std::signed_integral X> struct make_unsigned<X> {
        using type = std::make_unsigned_t<X>;
    };

}

// default behavior is to use the operators.
namespace data::math {
    template <typename A> struct negate {
        auto operator () (const A &x) const {
            return -x;
        }
    };

    template <typename A> struct abs {
        A operator () (const A &x) const {
            return x < 0 ? -x : x;
        }
    };
}

#endif
