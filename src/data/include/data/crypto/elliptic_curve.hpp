// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ELLIPTIC_CURVE
#define DATA_CRYPTO_ELLIPTIC_CURVE

#include <data/math/algebra/elliptic_curve.hpp>

namespace data::crypto {

    template <math::field field, typename N> struct elliptic_curve : math::Weierstrauss<field> {
        // this representation does not have the infinite point.
        // The infinite point is not available as a public key in cryptography.
        // It only exists internally.
        struct point : math::space::affine<field, 2>::point {
            const elliptic_curve &Curve;
            bool valid () const;

            point operator * (const point &) const;
            point operator ^ (const N &) const;

            operator math::Weierstrauss<field>::point () const;

            field x () const;
            field y () const;

        private:
            point (const math::Weierstrauss<field>::point &);
        };

        point Base;

        // order of Base. Must be prime.
        N Order;

        // Cofactor * Order = size of the whole group
        uint32 Cofactor;

        using secret = N;
        using pubkey = point;

        pubkey to_public (const secret &) const;

        N invert (const N &) const;

        // the point R is used in both encryption and signatures
        // (but you should use a different set of keys for each)
        point R (const N &random_k) const;

        struct signature {
            point R;
            N S;
        };

        signature sign (const N &message, const secret &key, const N &random_k) const;
        bool verify (const N &message, const signature &x, const pubkey &key) const;

        struct encryption_parameters {
            point R;
            point Z;
        };

        encryption_parameters setup_encryption (const pubkey &p, const N &random_k) const;

    };

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
        return R == (Base ^ u1) * (key ^ u2);
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
    inline elliptic_curve<field, N>::point::operator math::Weierstrauss<field>::point () const {
        return math::Weierstrauss<field>::point (Curve, static_cast<math::space::affine<field, 2>::point> (*this));
    }

    template <math::field field, typename N>
    bool inline elliptic_curve<field, N>::point::valid () const {
        return math::Weierstrauss<field>::point (*this).valid ();
    }

}

#endif

