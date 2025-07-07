// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CRYPTO_ELLIPTIC_CURVE
#define DATA_MATH_CRYPTO_ELLIPTIC_CURVE

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>
#include <data/math/point.hpp>
#include <data/power.hpp>

namespace data::math {

    // Not every elliptic curve can be expressed in Weierstrauss form, but those that can't are very much exceptions.
    template <field field> struct Weierstrauss {
        field A;
        field B;

        bool valid () const;

        bool operator == (const Weierstrauss &) const;

        field discriminant () const;

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
            signature YSign;
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
        };
    };

    template <field field>
    bool inline Weierstrauss<field>::valid () const {
        // make sure the curve is not singular.
        return discriminant () != 0;
    }

    template <field field>
    field inline Weierstrauss<field>::discriminant () const {
        return A * A * A * 4 + B * B * 27;
    }

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
/*
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


