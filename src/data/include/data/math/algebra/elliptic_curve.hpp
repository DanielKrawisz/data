// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CRYPTO_ELLIPTIC_CURVE
#define DATA_MATH_CRYPTO_ELLIPTIC_CURVE

#include <data/math/field.hpp>
#include <data/math/point.hpp>
#include <data/power.hpp>

namespace data::math::elliptic {

    // Not every elliptic curve can be expressed in Weierstrauss form, but those that can't are very much exceptions.
    template <field F> struct Weierstrass {
        using field = F;

        F A;
        F B;

        constexpr Weierstrass (const F &a, const F &b): A {a}, B {b} {}

        constexpr bool valid ();

        constexpr field discriminant ();

        using point = typename space::projective<field, 2>::simplex<1>;
        using projective_point = typename space::projective<field, 2>::exterior<0>;

        using signature = byte;
        using coordinate = typename space::projective<field, 2>::coordinate;

        struct compressed_point {
            coordinate X;
            signature Sign;
        };

        using Jacobian_point = typename space::affine<field, 3>::simplex<1>;

        static bool valid (const point &);
        static bool valid (const compressed_point &);
        static bool valid (const projective_point &);
        static bool valid (const Jacobian_point &);

        static point identity ();
        static compressed_point identity_compressed ();
        static projective_point identity_projective ();
        static Jacobian_point identity_Jacobian ();

        static point invert (const point &);
        static compressed_point invert (const compressed_point &);
        static projective_point invert (const projective_point &);
        static Jacobian_point invert (const Jacobian_point &);

        static point plus (const point &, const point &);
        static compressed_point plus (const compressed_point &, const compressed_point &);
        static projective_point plus (const projective_point &, const projective_point &);
        static Jacobian_point plus (const Jacobian_point &, const Jacobian_point &);

        static point minus (const point &, const point &);
        static compressed_point minus (const compressed_point &, const compressed_point &);
        static projective_point minus (const projective_point &, const projective_point &);
        static Jacobian_point minus (const Jacobian_point &, const Jacobian_point &);
    };

    template <auto curve> struct compressed_point;

    template <auto curve>
    struct point : unsigned_limit<typename space::affine<typename decltype (curve)::field, 2>::point> {
        bool valid () const;

        bool operator == (const point &) const;

        point operator - () const;
        point operator + (const point &) const;
        point operator - (const point &) const;

        template <typename N> point operator * (const N &) const;

        compressed_point<curve> compress () const;

        typename decltype (curve)::coordinate x () const;
        typename decltype (curve)::coordinate y () const;
    };

    template <field F>
    struct compressed {
        F X;
        signature YSign;
    };

    template <auto curve>
    struct compressed_point : unsigned_limit<compressed<typename decltype (curve)::field>> {
        bool valid () const;

        bool operator == (const compressed_point &) const;

        compressed_point operator - () const;
        compressed_point operator + (const compressed_point &) const;
        compressed_point operator - (const compressed_point &) const;

        template <typename N> compressed_point operator * (const N &) const;

        point<curve> uncompress () const;

        decltype (curve)::coordinate x () const;
        decltype (curve)::coordinate y () const;
    };

    template <auto curve>
    struct projective_point : space::affine<typename decltype (curve)::field, 3>::exterior<1> {
        bool valid () const;

        bool operator == (const projective_point &) const;

        projective_point operator - () const;
        projective_point operator + (const projective_point &) const;
        projective_point operator - (const projective_point &) const;

        template <typename N> projective_point operator * (const N &) const;

    private:
        decltype (curve)::field u () const;
        decltype (curve)::field v () const;
        decltype (curve)::field w () const;
    };

    template <auto curve> struct Jacobian_point : space::affine<typename decltype (curve)::field, 3>::exterior<1> {
        bool valid () const;

        Jacobian_point operator - () const;
        Jacobian_point operator + (const Jacobian_point &) const;
        Jacobian_point operator - (const Jacobian_point &) const;

        template <typename N> Jacobian_point operator * (const N &) const;

    private:
        decltype (curve)::field u () const;
        decltype (curve)::field v () const;
        decltype (curve)::field w () const;
    };

    template <field F> constexpr bool inline Weierstrass<F>::valid () {
        // make sure the curve is not singular.
        return discriminant () != 0;
    }

    template <field F> F constexpr inline Weierstrass<F>::discriminant () {
        return A * A * A * 4 + B * B * 27;
    }
/*
    template <auto A, auto B, field F>
    requires same_as<decltype (A), decltype (B)>
    bool inline Weierstrauss<A, B, F>::valid (const point &p) const {
        return is_infinite (*this) || y () * y () == x () * x () * x () + A * x () + B;
    }

    template <auto A, auto B, field F>
    requires same_as<decltype (A), decltype (B)>
    bool inline Weierstrauss<A, B, F>::valid (const projective_point &p) const {
        return y () * y () * z () == x () * x () * x () + A * x () * z () * z () + B * z () * z () * z ();
    }

    template <auto A, auto B, field F>
    requires same_as<decltype (A), decltype (B)>
    bool Weierstrauss<field>::valid (const Jacobian_point &j) const {
        auto zz = z () * z ();
        auto zzz = zz * z ();
        auto zzzz = zz * zz;
        auto zzzzzz = zzz * zzz;
        return y () * y () == x () * x () * x () + A * x () * zzzz + B * zzzzzz;
    }*/
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
    }*/
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


