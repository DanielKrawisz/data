// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CRYPTO_ELLIPTIC_CURVE
#define DATA_MATH_CRYPTO_ELLIPTIC_CURVE

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>
#include <data/math/point.hpp>

namespace data::math {

    // Not every elliptic curve can be expressed in Weierstrauss form, but those that can't are very much exceptions.
    template <skew_field field> struct Weierstrauss {
        field A;
        field B;

        field discriminant () const;

        // make sure the curve is not singular.
        bool valid () const {
            return discriminant () != 0;
        }

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

            point operator * (const point &) const;
            point operator / (const point &) const;

            template <typename N> point operator ^ (const N &) const;

            compressed_point compress () const;
        };

        struct compressed {
            field X;
            signature YSign;
        };

        struct compressed_point : unsigned_limit<compressed> {
            const Weierstrauss &Curve;
            bool valid () const;

            compressed_point operator * (const compressed_point &) const;
            compressed_point operator / (const compressed_point &) const;

            template <typename N> point operator ^ (const N &) const;

            point uncompress () const;
        };

        struct projective_vector : space::projective<field, 3>::exterior<1> {
            const Weierstrauss &Curve;
            bool valid () const;

            projective_vector operator * (const projective_vector &) const;

            template <typename N> projective_vector operator ^ (const N &) const;
        };

        struct Jacobian_point : space::projective<field, 3>::exterior<1> {
            const Weierstrauss &Curve;
            bool valid () const;

            projective_vector operator * (const projective_vector &) const;

            template <typename N> projective_vector operator ^ (const N &) const;
        };
    };
    
}

#endif


