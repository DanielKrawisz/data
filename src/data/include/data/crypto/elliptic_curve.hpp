// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ELLIPTIC_CURVE
#define DATA_CRYPTO_ELLIPTIC_CURVE

#include <data/math/algebra/elliptic_curve.hpp>

namespace data::crypto {

    template <math::skew_field field, typename N> struct elliptic_curve : math::Weierstrauss<field> {
        using point = typename math::space::affine<field, 2>::point;
        point Base;

        using secret = N;
        using pubkey = point;

        pubkey to_public (const secret &) const;

        // the point R is used in both encryption and signatures
        // (but you should use a different set of keys for each)
        point R (const N &random_k) const;

        struct signature {
            point R;
            N S;
        };

        signature sign (const N &message, const secret &key) const;
        bool verify (const N &message, const signature &x, const pubkey &key) const;

        struct encryption_parameters {
            point R;
            point S;
        };

    };


}

#endif

