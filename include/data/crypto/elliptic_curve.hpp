// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ELLIPTIC_CURVE
#define DATA_CRYPTO_ELLIPTIC_CURVE

#include <data/math/algebra/elliptic_curve.hpp>
#include <data/math/algebra/finite_field.hpp>

namespace data::crypto {

    template <auto Curve,
        math::elliptic_curve<Curve>::affine_point base,
        math::elliptic_curve<Curve>::scalar order, // must be prime
        uint32 cofactor> struct elliptic_curve : math::elliptic_curve<Curve> {

        using scalar = math::elliptic_curve<Curve>::scalar;

        // a public key
        using affine_point = math::elliptic_curve<Curve>::affine_point;
        using pubkey = affine_point;

        constexpr static const affine_point Base = base;
        constexpr static const scalar Order = order;

        constexpr static uint32 Cofactor = cofactor;

        // a private key or part of a signature.
        struct factor : math::nonzero<math::prime_field<order>> {
            constexpr factor (const scalar &);

            constexpr affine_point operator * (const affine_point &p) const {
                return p * this->Value.Value;
            }

            constexpr affine_point to_public () const {
                return *this * Base;
            }
        };

        using secret = factor;

        // NOTE: this is the definition of a tweak, but faster algorithms are possible.
        static affine_point inline tweak (const affine_point &p, const factor &x) {
            return p + to_public (x);
        }

    };

    template <auto Curve,
        math::elliptic_curve<Curve>::affine_point base,
        math::elliptic_curve<Curve>::scalar order, // must be prime
        uint32 cofactor> struct ECDSA : elliptic_curve<Curve, base, order, cofactor> {
        using factor = elliptic_curve<Curve, base, order, cofactor>::factor;
        using secret = elliptic_curve<Curve, base, order, cofactor>::factor;
        using pubkey = elliptic_curve<Curve, base, order, cofactor>::factor;

        struct signature {
            factor R;
            factor S;
            bool valid () const;
        };

        // this will fail with a very small probability and
        // then you have to try another ephemeral key.
        signature sign (const factor &message, const secret &key, const factor &ephemeral) const;

        bool verify (const factor &message, const signature &x, const pubkey &key) const;

    };
/*
    template <math::field field, typename N>
    elliptic_curve<field, N>::pubkey inline elliptic_curve<field, N>::to_public (const secret &x) const {
        return Base ^ x;
    }

    template <math::field field, typename N>
    elliptic_curve<field, N>::point inline elliptic_curve<field, N>::R (const N &random_k) const {
        return Base ^ random_k;
    }

    template <math::field field, typename N>
    elliptic_curve<field, N>::encryption_parameters inline elliptic_curve<field, N>::setup_encryption (const pubkey &p, const N &random_k) const {
        return {R (random_k), p ^ random_k};
    }

    template <math::field field, typename N>
    N elliptic_curve<field, N>::invert (const N &n) const {
        // if Order is not prime, then this formula doesn't work.
        return Order - n;
    }

    template <math::field field, typename N>
    elliptic_curve<field, N>::signature inline elliptic_curve<field, N>::sign (const N &message, const secret &key, const N &random_k) const {
        auto r = R (random_k);
        return {r, invert (random_k) * (message + key * r[0]) % Order};
    }

    template <math::field field, typename N>
    bool inline elliptic_curve<field, N>::verify (const N &message, const signature &x, const pubkey &key) const {
        auto s_1 = invert (x.S);
        auto u1 = s_1 * message % Order;
        auto u2 = s_1 * x.R[0] % Order;
        return x.R == (Base ^ u1) * (key ^ u2);
    }

    template <math::field field, typename N>
    elliptic_curve<field, N>::point elliptic_curve<field, N>::point::operator * (const point &p) const {
        return math::Weierstrauss<field>::point (*this) * math::Weierstrauss<field>::point (p);
    }

    template <math::field field, typename N>
    elliptic_curve<field, N>::point elliptic_curve<field, N>::point::operator ^ (const N &n) const {
        return math::Weierstrauss<field>::point (*this) * n;
    }

    template <math::field field, typename N>
    inline elliptic_curve<field, N>::point::operator typename math::Weierstrauss<field>::point () const {
        return math::Weierstrauss<field>::point (Curve, static_cast<math::space::affine<field, 2>::point> (*this));
    }

    template <math::field field, typename N>
    bool inline elliptic_curve<field, N>::point::valid () const {
        return math::Weierstrauss<field>::point (*this).valid ();
    }*/

}

#endif

