// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/point.hpp>
#include <data/arithmetic.hpp>

namespace data::math::algebra {

    template <typename curve> concept EllipticCurve =
        Field<typename curve::coordinate> && RingNumber<typename curve::scalar> &&
        requires (const curve &q) {
            { q.discriminant () } -> Same<typename curve::coordinate>;
        } && requires {
            { typename curve::point {} };
        } && requires (const typename curve::point &x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename curve::affine_point>;
            { -x } -> Same<typename curve::affine_point>;
        } && requires (const typename curve::point &x, const typename curve::point &y) {
            { curve::plus (x, y) } -> Same<typename curve::point>;
            { x + y } -> Same<typename curve::point>;
        } && requires (const typename curve::affine_point &x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename curve::affine_point>;
            { typename curve::point {x} };
        } && requires (const typename space::vector<typename curve::coordinate, 2> &x) {
            { typename curve::affine_point {x} };
        } && requires (const typename curve::point &x, const typename curve::scalar &y) {
            { curve::times (x, y) } -> Same<typename curve::point>;
            { curve::times (y, x) } -> Same<typename curve::point>;
        };

    enum sign : byte {
        zero = 0x00,
        even = 0x02,
        odd = 0x03
    };

    // Next we have four representations of curves (which is not the same as the curve itself)

    template <Field field, RingNumber N> struct Weierstrass {
        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate B;

        constexpr Weierstrass (const coordinate &a, const coordinate &b):
            A {a}, B {b} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Weierstrass &) const;

        constexpr field discriminant () const;
    };

    template <Field field, RingNumber N>
    struct Edwards {
        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate D;

        constexpr Edwards (const coordinate &a, const coordinate &d):
            A {a}, D {d} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Edwards &) const;
    };


    template <Field field, RingNumber N>
    struct TwistedEdwards {
        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate D;

        constexpr TwistedEdwards (
            const coordinate &a,
            const coordinate &d
        ): A {a}, D {d} {}

        constexpr bool valid () const;

        constexpr bool operator == (const TwistedEdwards &) const;
    };


    template <Field field, RingNumber N>
    struct Montgomery {
        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate B;

        constexpr Montgomery (
            const coordinate &a,
            const coordinate &b
        ): A {a}, B {b} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Montgomery &) const;

        constexpr field discriminant () const;
    };


    template <Field field, RingNumber N>
    struct Hessian {
        using coordinate = field;
        using scalar = N;

        coordinate D;

        constexpr explicit Hessian (const coordinate &d):
            D {d} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Hessian &) const;

        constexpr field discriminant () const;
    };

    // this is the curve itself.
    template <auto curve, typename type = decltype (curve)> struct elliptic_curve;

    template <auto curve, Field field, RingNumber N> struct elliptic_curve<curve, Weierstrass<field, N>> {
        using coordinate = field;
        using scalar = N;

        constexpr bool valid () const;

        constexpr field discriminant () const;

        struct point;
        struct projective_point;
        struct Jacobian_point;

        // a point not including the infinite point.
        struct affine_point {
            // construct an uncompressed point.
            constexpr affine_point (const coordinate &x, const coordinate &y): X {x}, Y {y} {};
            // construct a compressed point.
            constexpr affine_point (const coordinate &x, sign y): X {x}, Y {y} {};

            constexpr affine_point (const space::vector<coordinate, 2> &);
            constexpr operator space::vector<coordinate, 2> () const;

            point operator + (const affine_point &) const;
            point operator * (const scalar &) const;
            friend point operator * (const scalar &, const affine_point &);

            bool compressed () const;
            bool uncompressed () const;
            point compress () const;
            point decompress () const;

            coordinate X;
            either<coordinate, sign> Y;
        };

        constexpr static bool valid (const affine_point &);

        static affine_point negate (const affine_point &);
        static point plus (const affine_point &, const affine_point &);
        static point times (const affine_point &, const scalar &);
        static point times (const scalar &, const affine_point &);

        static bool compressed (const affine_point &);
        static bool uncompressed (const affine_point &);
        static affine_point compress (const affine_point &);
        static affine_point decompress (const affine_point &);

        struct point : unsigned_limit<affine_point> {

            // defines point () as the infinite point, which is also the group identity.
            // defines a constructor for every constructor of affine_point.
            using unsigned_limit<affine_point>::unsigned_limit;

            constexpr point (unsigned_limit<affine_point> &&);
            constexpr point (const unsigned_limit<affine_point> &);

            constexpr point (const projective_point &);
            constexpr point (const Jacobian_point &);

            point operator + (const point &) const;
            point operator * (const scalar &) const;
            friend point operator * (const scalar &, const point &);

            bool compressed () const;
            bool uncompressed () const;
            point compress () const;
            point decompress () const;
        };

        constexpr static bool valid (const point &);

        static point negate (const point &);
        static point plus (const point &, const point &);
        static point times (const point &, const scalar &);
        static point times (const scalar &, const point &);

        struct projective_point : space::projective<field, 2>::point {
            using space::projective<field, 2>::point::point;
            constexpr bool valid () const;

            constexpr projective_point (const projective_point &);
            constexpr projective_point (const Jacobian_point &);

            projective_point operator + (const projective_point &) const;
            projective_point operator * (const scalar &) const;
            friend projective_point operator * (const scalar &, const projective_point &);
        };

        constexpr static bool valid (const projective_point &);

        static projective_point negate (const projective_point &);
        static projective_point plus (const projective_point &, const projective_point &);
        static projective_point times (const projective_point &, const scalar &);
        static projective_point times (const scalar &, const projective_point &);

        struct Jacobian_point : space::projective<field, 2>::point {
            using space::projective<field, 2>::point::point;
            constexpr bool valid () const;

            constexpr Jacobian_point (const projective_point &);
            constexpr Jacobian_point (const point &);

            Jacobian_point operator + (const Jacobian_point &) const;
            Jacobian_point operator * (const scalar &) const;
            friend Jacobian_point operator * (const scalar &, const Jacobian_point &);
        };

        constexpr static bool valid (const Jacobian_point &);

        static Jacobian_point negate (const Jacobian_point &);
        static Jacobian_point plus (const Jacobian_point &, const Jacobian_point &);
        static Jacobian_point times (const Jacobian_point &, const scalar &);
        static Jacobian_point times (const scalar &, const Jacobian_point &);

    };
    
}

