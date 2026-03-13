// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_TOTIENT
#define DATA_MATH_NUMBER_TOTIENT

#include <data/fold.hpp>
#include <data/math/number/factor.hpp>

namespace data::math::number {

    template <typename N> nonzero<N> inline totient (nonzero<N> n, eratosthenes<N> &e) {
        return nonzero<N> {fold ([] (const N &n, const power<prime<N>, N> &p) -> N {
            return n * data::pow<N, N> (p.Base.Prime.Value, p.Exponent - 1u) * (p.Base.Prime.Value - 1u);
        }, N {1}, factorize<N> (n, e))};
    }

    template <typename N> nonzero<N> inline totient (prime<N> p, eratosthenes<N> &e) {
        return nonzero<N> {p.Prime.Value - 1};
    }

}

#endif
