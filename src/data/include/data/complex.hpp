// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_COMPLEX
#define DATA_COMPLEX

#include <data/norm.hpp>
#include <data/arithmetic.hpp>
#include <data/math/field.hpp>
#include <data/math/module.hpp>

namespace data {

    template <typename A, typename B = A> constexpr auto inner (const A &, const B &);

    // conjugate should turn every imaginary part of
    template <typename A> constexpr auto conjugate (const A &);

    template <typename A> constexpr auto re (const A &);

    // the non-real part.
    template <typename A> constexpr A inline im (const A &x) {
        return x - A {re (x)};
    }

    // refers to the even and odd parts of a composition algebra.
    template <typename A> constexpr auto ev (const A &);

    template <typename A> constexpr auto od (const A &);

    template <typename A> concept quad_algebra =
        math::ring<A> && requires (const A &x, const A &y) {
            { inner (x, y) } -> ImplicitlyConvertible<A>;
        };

    template <typename A, typename B> concept sub_quad_algebra =
        quad_algebra<A> && ImplicitlyConvertible<A, B> &&
        requires (const A &a, const B &b) {
            {a + b} -> ImplicitlyConvertible<B>;
            {b + a} -> ImplicitlyConvertible<B>;
            {a - b} -> ImplicitlyConvertible<B>;
            {b - a} -> ImplicitlyConvertible<B>;
            {a * b} -> ImplicitlyConvertible<B>;
            {b * a} -> ImplicitlyConvertible<B>;
        };

    template <typename A> concept cayley_dickson_algebra =
        quad_algebra<A> && requires (const A &x) {
            requires Same<decltype (ev (x)), decltype (od (x))>;
            //requires Same<decltype (ev (x)), A> ||
            //    (sub_quad_algebra<decltype (ev (x)), A>);
            requires Ordered<decltype (re (x))>;
            requires ImplicitlyConvertible<decltype (abs (re (x))),
                decltype (quadrance (x))>;
        };

    template <typename A> concept Real = cayley_dickson_algebra<A> &&
        requires (const A &x) {
            requires Same<A, decltype (re (x))>;
            requires Same<A, decltype (ev (x))>;
        };

    template <typename A> concept Complex = cayley_dickson_algebra<A> && Real<decltype (ev (std::declval<A> ()))>;

    template <typename A> concept Quaternionic = cayley_dickson_algebra<A> && Complex<decltype (ev (std::declval<A> ()))>;

    template <typename A> concept Octonionic = cayley_dickson_algebra<A> && Quaternionic<decltype (ev (std::declval<A> ()))>;

    template <typename A> concept CompositionAlgebra = cayley_dickson_algebra<A> && math::field<A>;

    template <typename A> constexpr auto re (const A &x) {
        if constexpr (ImplicitlyConvertible<A, decltype (ev (x))>) {
            return ev (x);
        } else {
            return re (ev (x));
        }
    }
}

namespace data::math::def {
    template <typename X> struct conjugate;
    template <typename X, typename Y = X> struct inner;
    template <typename X> struct od;
    template <typename X> struct ev;
}

namespace data {
    template <typename X>
    constexpr auto inline conjugate (const X &x) {
        return math::def::conjugate<X> {} (x);
    }

    template <typename X>
    constexpr auto inline od (const X &x) {
        return math::def::od<X> {} (x);
    }

    template <typename X>
    constexpr auto inline ev (const X &x) {
        return math::def::ev<X> {} (x);
    }

    template <typename A, typename B> constexpr auto inline inner (const A &x, const B &y) {
        return math::def::inner<A, B> {} (x, y);
    }
}

namespace data::math::def {

    template <typename X> struct conjugate {
        constexpr X operator () (const X &x) {
            return data::im (x) - X {data::re (x)};
        }
    };

    template <typename X, typename Y>
    struct inner {
        constexpr auto operator () (const X &x, const Y &y) {
            return data::times (x, data::conjugate (y));
        }
    };

    // if abs and inner are defined, then quadrance x is inner x x.
    template <typename X> requires requires (const X &x) {
        { data::abs (data::re (data::inner (x, x))) };
    } struct quadrance<X> {
        constexpr auto operator () (const X &x) {
            return data::abs (data::re (data::inner (x, x)));
        }
    };

    template <typename X> struct ev {
        constexpr X operator () (const X &x) {
            return x;
        }
    };

    template <typename X> struct od {
        constexpr X operator () (const X &x) {
            return X {};
        }
    };

}

#endif
