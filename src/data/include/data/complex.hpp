// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_COMPLEX
#define DATA_COMPLEX

#include <data/abs.hpp>
#include <data/arithmetic.hpp>
#include <data/math/ring.hpp>

namespace data {

    template <typename A> constexpr auto conjugate (const A &);

    template <typename A> constexpr auto re (const A &);

    template <typename A> constexpr auto im (const A &);

    template <typename A, typename B = A> constexpr auto inner (const A &, const B &);
}

namespace data::math {
    template <typename X> struct conjugate;
    template <typename X> struct re;
    template <typename X> struct im;
    template <typename X, typename Y = X> struct inner;
}

namespace data {
    template <typename X>
    constexpr auto inline conjugate (const X &x) {
        return math::conjugate<X> {} (x);
    }

    template <typename X>
    constexpr auto inline re (const X &x) {
        return math::re<X> {} (x);
    }

    template <typename X>
    constexpr auto inline im (const X &x) {
        return math::im<X> {} (x);
    }

    template <typename A, typename B> constexpr auto inline inner (const A &x, const B &y) {
        return math::inner<A, B> {} (x, y);
    }

    template <typename A> concept real = ordered<A> && math::ring<A> && requires (const A &x) {
        { inner (x, x) } -> implicitly_convertible_to<A>;
        { re (x) } -> implicitly_convertible_to<A>;
    };

    template <typename A> concept caylay_dickson = math::ring<A> && requires (const A &x) {
        { inner (x, x) };
    } && same_as<decltype (re (std::declval<A> ())), decltype (im (std::declval<A> ()))> &&
        same_as<decltype (inner (std::declval<A> (), std::declval<A> ())),
            decltype (inner (re (std::declval<A> ()), re (std::declval<A> ())))>;

    template <typename A> concept complex = caylay_dickson<A> && real<decltype (re (std::declval<A> ()))>;

    template <typename A> concept quaternionic = caylay_dickson<A> && complex<decltype (re (std::declval<A> ()))>;

    template <typename A> concept octonionic = caylay_dickson<A> && quaternionic<decltype (re (std::declval<A> ()))>;
}

namespace data::math {

    // default definitions
    template <typename X> struct conjugate {
        constexpr X operator () (const X &x) {
            return x;
        }
    };

    template <typename X> struct re {
        constexpr X operator () (const X &x) {
            return x;
        }
    };

    template <typename X> struct im {
        constexpr X operator () (const X &x) {
            return X {0};
        }
    };

    template <typename X, typename Y>
    struct inner {
        constexpr auto operator () (const X &x, const Y &y) {
            return data::times (x, y);
        }
    };

}

#endif
