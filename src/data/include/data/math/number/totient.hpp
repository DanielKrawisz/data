// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_TOTIENT
#define DATA_MATH_NUMBER_TOTIENT

#include <data/fold.hpp>
#include <data/math/number/factor.hpp>

namespace data::math::number {

    template <typename N> N inline totient (N n, eratosthenes<N> &e) {
        return fold ([] (const N &n, const power<prime<N>, N> &p) -> N {
            return n * data::pow<N, N> (p.Base.Prime, p.Exponent - 1) * (p.Base.Prime - 1);
        }, N {1}, factorize<N> (n, e));
    }

    template <typename N> N inline totient (prime<N> p, eratosthenes<N> &e) {
        return p.Prime - 1;
    }

}

#endif
