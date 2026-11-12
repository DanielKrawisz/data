// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/point.hpp>
#include <data/arithmetic.hpp>

namespace data::math::algebra {

    template <typename curve> concept EllipticCurveAlgebra =
        Field<typename curve::coordinate> &&
        RingNumber<typename curve::scalar> &&
        requires (const curve &q) {
            { q.discriminant () } -> Same<typename curve::coordinate>;
        // unsigned_limit<space::vector<curve::coordinate, 2>> is a natural
        // representation of the general point including the infinite point.
        } && requires (unsigned_limit<space::vector<typename curve::coordinate, 2>> point) {
            { curve::valid (point) } -> Same<bool>;
            { curve::negate (point) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
        } && requires (unsigned_limit<space::vector<typename curve::coordinate, 2>> point_x, unsigned_limit<space::vector<typename curve::coordinate, 2>> point_y) {
            { curve::plus (point_x, point_y) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
        } && requires (unsigned_limit<space::vector<typename curve::coordinate, 2>> point, typename curve::scalar scalar) {
            { curve::times (point, scalar) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
            { curve::times (scalar, point) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
        // space::vector<curve::coordinate, 2> is a natural representation
        // of an affine point (a non-infinite point)
        } && requires (space::vector<typename curve::coordinate, 2> affine_point) {
            { curve::negate (affine_point) } -> Same<space::vector<typename curve::coordinate, 2>>;
            { curve::valid (affine_point) } -> Same<bool>;
        } && requires (space::vector<typename curve::coordinate, 2> affine_point_x, space::vector<typename curve::coordinate, 2> affine_point_y) {
            { curve::plus (affine_point_x, affine_point_y) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
        } && requires (space::vector<typename curve::coordinate, 2> affine_point, typename curve::scalar scalar) {
            { curve::times (affine_point, scalar) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
            { curve::times (scalar, affine_point) } -> Same<unsigned_limit<space::vector<typename curve::coordinate, 2>>>;
        // we also have projective representations of points for which we can
        // use space::projective<curve::coordinate, 2>::point
        } && requires (typename space::projective<typename curve::coordinate, 2>::point point) {
            { curve::negate (point) } -> Same<typename space::projective<typename curve::coordinate, 2>::point>;
            { curve::valid (point) } -> Same<bool>;
        } && requires (typename space::projective<typename curve::coordinate, 2>::point point_x, typename space::projective<typename curve::coordinate, 2>::point point_y) {
            { curve::plus (point_x, point_y) } -> Same<typename space::projective<typename curve::coordinate, 2>::point >;
        } && requires (typename space::projective<typename curve::coordinate, 2>::point point, typename curve::scalar scalar) {
            { curve::times (point, scalar) } -> Same<typename space::projective<typename curve::coordinate, 2>::point>;
            { curve::times (scalar, point) } -> Same<typename space::projective<typename curve::coordinate, 2>::point>;
        // Because we can have compressed points for various curve representations,
        // the curve is expected to have its own native point representation
        } && requires {
            { typename curve::point {} };
        } && requires (typename curve::coordinate x, typename curve::coordinate y) {
            { typename curve::point {x, y} };
        } && requires (typename curve::point x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename curve::affine_point>;
            { -x } -> Same<typename curve::affine_point>;
            { unsigned_limit<space::vector<typename curve::coordinate, 2>> (x) };
        } && requires (typename curve::point x, typename curve::point y) {
            { curve::plus (x, y) } -> Same<typename curve::point>;
            { x + y } -> Same<typename curve::point>;
        } && requires (typename curve::point &x, const typename curve::scalar y) {
            { curve::times (x, y) } -> Same<typename curve::point>;
            { curve::times (y, x) } -> Same<typename curve::point>;
            { x * y } -> Same<typename curve::point>;
            { y * x } -> Same<typename curve::point>;
        // we have affine points to represent non-infinite points.
        } && requires (typename curve::coordinate x, typename curve::coordinate y) {
            { typename curve::affine_point {x, y} };
        } && requires (typename curve::affine_point x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename curve::affine_point>;
            { typename curve::point {x} };
            { space::vector<typename curve::coordinate, 2> (x) };
        } && requires (typename curve::affine_point x, typename curve::affine_point y) {
            { curve::plus (x, y) } -> Same<typename curve::point>;
            { x + y } -> Same<typename curve::point>;
        } && requires (typename curve::affine_point &x, const typename curve::scalar y) {
            { curve::times (x, y) } -> Same<typename curve::point>;
            { curve::times (y, x) } -> Same<typename curve::point>;
            { x * y } -> Same<typename curve::point>;
            { y * x } -> Same<typename curve::point>;
        } && requires {
            { typename curve::projective_point {} };
        } && requires (typename curve::coordinate x, typename curve::coordinate y, typename curve::coordinate z) {
            { typename curve::projective_point {x, y, z} };
        } && requires (typename curve::projective_point x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename curve::affine_point>;
            { -x } -> Same<typename curve::affine_point>;
        } && requires (typename curve::projective_point x, typename curve::projective_point y) {
            { curve::plus (x, y) } -> Same<typename curve::projective_point>;
            { x + y } -> Same<typename curve::projective_point>;
        } && requires (typename curve::projective_point x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename curve::affine_point>;
            { typename curve::projective_point {x} };
        } && requires (const typename space::vector<typename curve::coordinate, 2> &x) {
            { typename curve::affine_point {x} };
        } && requires (const typename curve::projective_point &x, const typename curve::scalar &y) {
            { curve::times (x, y) } -> Same<typename curve::projective_point>;
            { curve::times (y, x) } -> Same<typename curve::projective_point>;
        } && requires (space::vector<typename curve::coordinate, 2> x) {
            { typename curve::affine_point (x) };
            { typename curve::point (x) };
        } && requires (unsigned_limit<space::vector<typename curve::coordinate, 2>> x) {
            { typename curve::point (x) };
        } && requires (space::projective<typename curve::coordinate, 2>::point x) {
            { typename curve::projective_point (x) };
        };

    enum sign : byte {
        zero = 0x00,
        even = 0x02,
        odd = 0x03
    };

    // Next we have four representations of curves.
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

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;
        using projective_point = space::projective<coordinate, 2>::point;

        constexpr bool valid (const affine_point &);
        constexpr bool valid (const point &);
        constexpr bool valid (const projective_point &);

        constexpr affine_point negate (const affine_point &);
        constexpr point negate (const point &);
        constexpr projective_point negate (const projective_point &);

        constexpr point plus (const point &, const point &);
        constexpr point plus (const affine_point &, const affine_point &);
        constexpr projective_point plus (const projective_point &, const projective_point &);

        constexpr point times (const point &, const scalar &);
        constexpr point times (const affine_point &, const scalar &);
        constexpr projective_point times (const projective_point &, const scalar &);

        constexpr point times (const scalar &, const point &);
        constexpr point times (const scalar &, const affine_point &);
        constexpr projective_point times (const scalar &, const projective_point &);

        struct compressed_point {
            coordinate X;
            sign Y;
        };

        constexpr compressed_point negate (const compressed_point &);
        constexpr compressed_point plus (const compressed_point &, const compressed_point &);
        constexpr compressed_point times (const scalar &, const compressed_point &);
        constexpr compressed_point times (const compressed_point &, const scalar &);

        constexpr projective_point Jacobian_negate (const projective_point &);
        constexpr projective_point Jacobian_plus (const projective_point &, const projective_point &);
        constexpr projective_point Jacobian_times (const projective_point &, const scalar &);
        constexpr projective_point Jacobian_times (const scalar &, const projective_point &);
    };

    template <Field field, RingNumber N> struct Edwards {
        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate D;

        constexpr Edwards (const coordinate &a, const coordinate &d):
            A {a}, D {d} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Edwards &) const;

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;
        using projective_point = space::projective<coordinate, 2>::point;

        constexpr bool valid (const affine_point &);
        constexpr bool valid (const point &);
        constexpr bool valid (const projective_point &);

        constexpr affine_point negate (const affine_point &);
        constexpr point negate (const point &);
        constexpr projective_point negate (const projective_point &);

        constexpr point plus (const point &, const point &);
        constexpr point plus (const affine_point &, const affine_point &);
        constexpr projective_point plus (const projective_point &, const projective_point &);

        constexpr point times (const point &, const scalar &);
        constexpr point times (const affine_point &, const scalar &);
        constexpr projective_point times (const projective_point &, const scalar &);

        constexpr point times (const scalar &, const point &);
        constexpr point times (const scalar &, const affine_point &);
        constexpr projective_point times (const scalar &, const projective_point &);
    };

    template <Field field, RingNumber N> struct twisted_Edwards {
        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate D;

        constexpr twisted_Edwards (
            const coordinate &a,
            const coordinate &d
        ): A {a}, D {d} {}

        constexpr bool valid () const;

        constexpr bool operator == (const twisted_Edwards &) const;

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;
        using projective_point = space::projective<coordinate, 2>::point;

        constexpr bool valid (const affine_point &);
        constexpr bool valid (const point &);
        constexpr bool valid (const projective_point &);

        constexpr affine_point negate (const affine_point &);
        constexpr point negate (const point &);
        constexpr projective_point negate (const projective_point &);

        constexpr point plus (const point &, const point &);
        constexpr point plus (const affine_point &, const affine_point &);
        constexpr projective_point plus (const projective_point &, const projective_point &);

        constexpr point times (const point &, const scalar &);
        constexpr point times (const affine_point &, const scalar &);
        constexpr projective_point times (const projective_point &, const scalar &);

        constexpr point times (const scalar &, const point &);
        constexpr point times (const scalar &, const affine_point &);
        constexpr projective_point times (const scalar &, const projective_point &);

    };

    template <Field field, RingNumber N> struct Montgomery {
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

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;
        using projective_point = space::projective<coordinate, 2>::point;

        constexpr bool valid (const affine_point &);
        constexpr bool valid (const point &);
        constexpr bool valid (const projective_point &);

        constexpr affine_point negate (const affine_point &);
        constexpr point negate (const point &);
        constexpr projective_point negate (const projective_point &);

        constexpr point plus (const point &, const point &);
        constexpr point plus (const affine_point &, const affine_point &);
        constexpr projective_point plus (const projective_point &, const projective_point &);

        constexpr point times (const point &, const scalar &);
        constexpr point times (const affine_point &, const scalar &);
        constexpr projective_point times (const projective_point &, const scalar &);

        constexpr point times (const scalar &, const point &);
        constexpr point times (const scalar &, const affine_point &);
        constexpr projective_point times (const scalar &, const projective_point &);
    };

    template <Field field, RingNumber N> struct Hessian {
        using coordinate = field;
        using scalar = N;

        coordinate D;

        constexpr explicit Hessian (const coordinate &d):
            D {d} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Hessian &) const;

        constexpr field discriminant () const;

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;
        using projective_point = space::projective<coordinate, 2>::point;

        constexpr bool valid (const affine_point &);
        constexpr bool valid (const point &);
        constexpr bool valid (const projective_point &);

        constexpr affine_point negate (const affine_point &);
        constexpr point negate (const point &);
        constexpr projective_point negate (const projective_point &);

        constexpr point plus (const point &, const point &);
        constexpr point plus (const affine_point &, const affine_point &);
        constexpr projective_point plus (const projective_point &, const projective_point &);

        constexpr point times (const point &, const scalar &);
        constexpr point times (const affine_point &, const scalar &);
        constexpr projective_point times (const projective_point &, const scalar &);

        constexpr point times (const scalar &, const point &);
        constexpr point times (const scalar &, const affine_point &);
        constexpr projective_point times (const scalar &, const projective_point &);

    };

    template <typename>
    struct is_elliptic_curve_representation : std::false_type {};

    template <Field field, RingNumber N>
    struct is_elliptic_curve_representation<Weierstrass<field, N>> : std::true_type {};

    template <Field field, RingNumber N>
    struct is_elliptic_curve_representation<Edwards<field, N>> : std::true_type {};

    template <Field field, RingNumber N>
    struct is_elliptic_curve_representation<twisted_Edwards<field, N>> : std::true_type {};

    template <Field field, RingNumber N>
    struct is_elliptic_curve_representation<Montgomery<field, N>> : std::true_type {};

    template <Field field, RingNumber N>
    struct is_elliptic_curve_representation<Hessian<field, N>> : std::true_type {};

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    struct elliptic_curve_base {
        using coordinate = decltype (curve)::coordinate;
        using scalar = decltype (curve)::scalar;

        constexpr static coordinate discriminant ();

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;
        using projective_point = space::projective<coordinate, 2>::point;

        // whether the point is on the curve.
        constexpr static bool valid (const affine_point &);
        constexpr static bool valid (const point &);
        constexpr static bool valid (const projective_point &);

        constexpr static affine_point negate (const affine_point &);
        constexpr static point negate (const point &);
        constexpr static projective_point negate (const projective_point &);

        constexpr static point plus (const point &, const point &);
        constexpr static point plus (const affine_point &, const affine_point &);
        constexpr static projective_point plus (const projective_point &, const projective_point &);

        constexpr static point times (const point &, const scalar &);
        constexpr static point times (const affine_point &, const scalar &);
        constexpr static projective_point times (const projective_point &, const scalar &);

        constexpr static point times (const scalar &, const point &);
        constexpr static point times (const scalar &, const affine_point &);
        constexpr static projective_point times (const scalar &, const projective_point &);

    };

    template <auto curve, typename X = decltype (curve)> struct elliptic_curve;

    template <auto curve, Field field, RingNumber N> struct elliptic_curve<curve, Weierstrass<field, N>> : elliptic_curve_base<curve> {
        using coordinate = field;
        using scalar = N;

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

        struct projective_point : space::projective<coordinate, 2>::point {
            using space::projective<coordinate, 2>::point::point;
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

        struct Jacobian_point : space::projective<coordinate, 2>::point {
            using space::projective<coordinate, 2>::point::point;
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

    template <auto curve> requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::discriminant () -> coordinate {
        return curve.discriminant ();
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr bool elliptic_curve_base<curve>::valid (const affine_point &p) {
        return curve.valid (p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr bool elliptic_curve_base<curve>::valid (const point &p) {
        return curve.valid (p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr bool elliptic_curve_base<curve>::valid (const projective_point &p) {
        return curve.valid (p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::negate (const affine_point &p) -> affine_point {
        return curve.negate (p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::negate (const point &p) -> point {
        return curve.negate (p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::negate (const projective_point &p) -> projective_point {
        return curve.negate (p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::plus (const point &p, const point &q) -> point {
        return curve.plus (p, q);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::plus (const affine_point &p, const affine_point &q) -> point {
        return curve.plus (p, q);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::plus (const projective_point &p, const projective_point &q) -> projective_point {
        return curve.plus (p, q);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::times (const point &p, const scalar &n) -> point {
        return curve.times (p, n);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::times (const affine_point &p, const scalar &n) -> point {
        return curve.times (p, n);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::times (const projective_point &p, const scalar &n) -> projective_point {
        return curve.times (p, n);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::times (const scalar &n, const point &p) -> point {
        return curve.times (n, p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::times (const scalar &n, const affine_point &p) -> point {
        return curve.times (n, p);
    }

    template <auto curve>
    requires (is_elliptic_curve_representation<decltype (curve)>::value)
    constexpr auto elliptic_curve_base<curve>::times (const scalar &n, const projective_point &p) -> projective_point {
        return curve.times (n, p);
    }
    
}

