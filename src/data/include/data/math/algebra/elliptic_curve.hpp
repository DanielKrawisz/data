// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CRYPTO_ELLIPTIC_CURVE
#define DATA_MATH_CRYPTO_ELLIPTIC_CURVE

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>
#include <data/math/point.hpp>
#include <data/math/power.hpp>

namespace data::math {

    template <typename curve> concept EllipticCurve =
        field<typename curve::coordinate> && ring_number<typename curve::scalar> &&
        requires (const curve &q) {
            { q.discriminant () } -> Same<typename curve::coordinate>;
        } && requires (typename space::vector<typename curve::coordinate, 2> &x) {
            // whether the point is actually on the curve.
            { curve::valid (x) } -> Same<bool>;
            { curve::negate (x) } -> Same<typename space::vector<typename curve::coordinate, 2>>;
        } && requires (typename space::vector<typename curve::coordinate, 2> &x,
            typename space::vector<typename curve::coordinate, 2> &y) {
            { curve::plus (x, y) } -> Same<unsigned_limit<typename space::vector<typename curve::coordinate, 2>>>;
        } && requires (typename space::vector<typename curve::coordinate, 2> &x,
            typename curve::scalar &y) {
            { curve::times (x, y) } -> Same<unsigned_limit<typename space::vector<typename curve::coordinate, 2>>>;
        };

    template <auto Curve>
    requires EllipticCurve<decltype (Curve)>
    struct elliptic_curve {
        using curve = decltype (Curve);
        using coordinate = curve::coordinate;
        using scalar = curve::scalar;
        using projective_coordinate = unsigned_limit<coordinate>;

        constexpr static coordinate discriminant () {
            return Curve.discriminant ();
        }

        constexpr static bool valid () {
            return discriminant () != coordinate {};
        }

        struct point;

        // a point not including the infinite point.
        struct affine_point : space::vector<typename curve::coordinate, 2> {

            constexpr affine_point (const coordinate &x, const coordinate &y):
                space::vector<typename curve::coordinate, 2> {x, y} {}

            constexpr affine_point (const space::vector<typename curve::coordinate, 2> &p):
                space::vector<typename curve::coordinate, 2> {p} {}

            constexpr bool valid () const;

            constexpr coordinate x () const;
            constexpr coordinate y () const;

            bool operator == (const affine_point &) const;

            affine_point operator - () const;

            point operator + (const point &) const;
            point operator - (const point &) const;

            point operator + (const affine_point &) const;
            point operator - (const affine_point &) const;

            point operator * (const scalar &) const;
        };

        struct point : unsigned_limit<affine_point> {
            // the infinite point.
            constexpr point ();

            constexpr point (const coordinate &x, const coordinate &y);

            constexpr point (const affine_point &);

            constexpr bool valid ();

            projective_coordinate x () const;
            projective_coordinate y () const;

            bool operator == (const point &) const;

            point operator - () const;
            point operator + (const point &) const;
            point operator - (const point &) const;
            point operator * (const scalar &) const;
        };
    };

    // Not every elliptic curve can be expressed in Weierstrauss form,
    // but those that can't are very much exceptions.
    // NOTE: this particular type is only good for prime field curves ATM.
    template <ring_number N, field field> struct Weierstrauss {

        using coordinate = field;
        using scalar = N;

        coordinate A;
        coordinate B;

        constexpr Weierstrauss (const coordinate &a, const coordinate &b):
            A {a}, B {b} {}

        constexpr bool valid () const;

        constexpr bool operator == (const Weierstrauss &) const;

        constexpr field discriminant () const;

        using affine_point = space::vector<coordinate, 2>;
        using point = unsigned_limit<affine_point>;

        constexpr static bool valid (const affine_point &);

        static affine_point negate (const affine_point &);
        static point negate (const point &);
        static point plus (const affine_point &, const affine_point &);
        static point plus (const point &, const point &);
        static point times (const affine_point &, const scalar &);
        static point times (const point &, const scalar &);

/*
        struct point;
        struct compressed_point;
        struct projective_vector;
        struct Jacobian_point;

        point invert (const point &);
        compressed_point invert (const compressed_point &);
        projective_vector invert (const projective_vector &);
        Jacobian_point invert (const Jacobian_point &);

        point multiply (const point &, const point &);
        compressed_point multiply (const compressed_point &, const compressed_point &);
        projective_vector multiply (const projective_vector &, const projective_vector &);
        Jacobian_point multiply (const Jacobian_point &, const Jacobian_point &);

        struct point : unsigned_limit<typename space::affine<field, 2>::point> {
            const Weierstrauss &Curve;
            bool valid () const;

            bool operator == (const projective_vector &) const;

            point operator * (const point &) const;
            point operator / (const point &) const;

            template <typename N> point operator ^ (const N &) const;

            compressed_point compress () const;

            field x () const;
            field y () const;

        protected:
            point (const Weierstrauss &curve, unsigned_limit<typename space::affine<field, 2>::point>);
        };

        struct compressed {
            field X;
            sign YSign;
        };

        struct compressed_point : unsigned_limit<compressed> {
            const Weierstrauss &Curve;
            bool valid () const;

            bool operator == (const projective_vector &) const;

            compressed_point operator * (const compressed_point &) const;
            compressed_point operator / (const compressed_point &) const;

            template <typename N> compressed_point operator ^ (const N &) const;

            point uncompress () const;

            field x () const;
            field y () const;
        };

        struct projective_vector : space::projective<field, 3>::template exterior<1> {
            const Weierstrauss &Curve;
            bool valid () const;

            bool operator == (const projective_vector &) const;

            projective_vector operator * (const projective_vector &) const;
            projective_vector operator / (const projective_vector &) const;

            template <typename N> projective_vector operator ^ (const N &) const;

            field x () const;
            field y () const;
            field z () const;
        };

        struct Jacobian_point : space::projective<field, 3>::template exterior<1> {
            const Weierstrauss &Curve;
            bool valid () const;

            Jacobian_point operator * (const Jacobian_point &) const;
            Jacobian_point operator / (const Jacobian_point &) const;

            template <typename N> Jacobian_point operator ^ (const N &) const;

            field x () const;
            field y () const;
            field z () const;
        };*/
    };

    template <ring_number N, field field>
    constexpr bool inline Weierstrauss<N, field>::valid () const {
        // make sure the curve is not singular.
        return discriminant () != 0;
    }

    template <ring_number N, field field>
    constexpr field inline Weierstrauss<N, field>::discriminant () const {
        return A * A * A * 4 + B * B * 27;
    }
/*
    template <field field>
    Weierstrauss<field>::point inline Weierstrauss<field>::point::operator * (const point &x) const {
        return Curve.multiply (*this, x);
    }

    template <field field>
    Weierstrauss<field>::point inline Weierstrauss<field>::point::operator / (const point &x) const {
        return Curve.multiply (*this, Curve.invert (x));
    }

    template <field field>
    Weierstrauss<field>::compressed_point Weierstrauss<field>::compressed_point::operator * (const compressed_point &x) const {
        return Curve.multiply (*this, x);
    }

    template <field field>
    Weierstrauss<field>::compressed_point Weierstrauss<field>::compressed_point::operator / (const compressed_point &x) const {
        return Curve.multiply (*this, Curve.invert (x));
    }

    template <field field>
    bool inline Weierstrauss<field>::compressed_point::valid () const {
        return uncompress ().valid ();
    }

    template <field field>
    Weierstrauss<field>::projective_vector inline Weierstrauss<field>::projective_vector::operator * (const projective_vector &x) const {
        return Curve.multiply (*this, x);
    }

    template <field field>
    Weierstrauss<field>::projective_vector inline Weierstrauss<field>::projective_vector::operator / (const projective_vector &x) const {
        return Curve.multiply (*this, Curve.invert (x));
    }

    template <field field>
    Weierstrauss<field>::Jacobian_point inline Weierstrauss<field>::Jacobian_point::operator * (const Jacobian_point &x) const {
        return Curve.multiply (*this, x);
    }

    template <field field>
    Weierstrauss<field>::Jacobian_point inline Weierstrauss<field>::Jacobian_point::operator / (const Jacobian_point &x) const {
        return Curve.multiply (*this, Curve.invert (x));
    }

    template <field field> template <typename N>
    Weierstrauss<field>::compressed_point inline Weierstrauss<field>::compressed_point::operator ^ (const N &n) const {
        return (this->uncompress () ^ n).compress ();
    }

    template <field field>
    Weierstrauss<field>::compressed_point inline Weierstrauss<field>::invert (const compressed_point &u) {
        return invert (u.uncompress ()).compress ();
    }

    template <field field>
    Weierstrauss<field>::compressed_point inline Weierstrauss<field>::multiply (const compressed_point &a, const compressed_point &b) {
        return multiply (a.uncompress (), b.uncompress ()).compress ();
    }

    template <field field> template <typename N>
    Weierstrauss<field>::point inline Weierstrauss<field>::point::operator ^ (const N &n) const {
        return data::pow<field, N> (*this, n);
    }

    template <field field> template <typename N>
    Weierstrauss<field>::projective_vector inline Weierstrauss<field>::projective_vector::operator ^ (const N &n) const {
        return data::pow<field, N> (*this, n);
    }

    template <field field> template <typename N>
    Weierstrauss<field>::Jacobian_point inline Weierstrauss<field>::Jacobian_point::operator ^ (const N &n) const {
        return data::pow<field, N> (*this, n);
    }

    template <field field>
    bool inline Weierstrauss<field>::point::valid () const {
        return is_infinite (*this) || y () * y () == x () * x () * x () + A * x () + B;
    }

    template <field field>
    bool inline Weierstrauss<field>::projective_vector::valid () const {
        return y () * y () * z () == x () * x () * x () + A * x () * z () * z () + B * z () * z () * z ();
    }

    template <field field>
    bool Weierstrauss<field>::Jacobian_point::valid () const {
        auto zz = z () * z ();
        auto zzz = zz * z ();
        auto zzzz = zz * zz;
        auto zzzzzz = zzz * zzz;
        return y () * y () == x () * x () * x () + A * x () * zzzz + B * zzzzzz;
    }

    template <field field>
    Weierstrauss<field>::point multiply (const point &a, const point &b) {
        if (is_infinite (a)) return b;
        if (is_infinite (b)) return a;
        if (a == b) return
    }

    template <field field>
    Weierstrauss<field>::projective_vector Weierstrauss<field>::multiply (const projective_vector &a, const projective_vector &b) {

    }

    template <field field>
    Weierstrauss<field>::Jacobian_point Weierstrauss<field>::multiply (const Jacobian_point &a, const Jacobian_point &b) {

    }

    template <field field>
    Weierstrauss<field>::point invert (const point &p) {
        if (is_infinite (p)) return p;
        return point {p.x (), field {1} / p.y ()};
    }

    template <field field>
    Weierstrauss<field>::compressed_point invert (const compressed_point &p) {
        if (is_infinite (p)) return p;
        compressed z = p.Value.get<compressed> ();
        return compressed {z.X, -z.YSign};
    }

    template <field field>
    Weierstrauss<field>::projective_vector invert (const projective_vector &p) {
        return projective_vector {x (), field {1} / y (), z ()};
    }

    template <field field>
    Weierstrauss<field>::Jacobian_point invert (const Jacobian_point &p) {
        return Jacobian_point {x (), field {1} / y (), z ()};
    }*/
    
}

#endif


