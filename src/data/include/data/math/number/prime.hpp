// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_PRIME
#define DATA_MATH_NUMBER_PRIME

#include <data/math/nonnegative.hpp>

namespace data::math::number {
    template <typename N> struct eratosthenes;
    template <typename N> struct primes;
    template <typename N> struct AKS;

    template <typename N> struct factorization;
    template <typename N> factorization<N> factorize (N, eratosthenes<N> &);

    // A number that is known to be prime.
    // So far eratosthenes is the only way
    // to construct this type.
    template <typename N>
    struct prime : nonzero<N> {
        enum likelihood {
            certain,
            probable,
            impossible
        };

        N Prime;
        likelihood Likelihood;

        bool valid () const {
            return Likelihood != impossible;
        }

        prime () : Prime {0}, Likelihood {impossible} {}

        bool operator == (const prime &p) const {
            return Prime == p.Prime;
        }

    private:
        prime (N p, likelihood l) : Prime {p}, Likelihood {l} {}

        friend struct eratosthenes<N>;
        friend struct AKS<N>;
        friend factorization<N> factorize<N> (N, eratosthenes<N> &);
    };

    template <typename N> factorization<N> operator * (const prime<N> &, const prime<N> &);
    template <typename N> factorization<N> operator * (factorization<N>, factorization<N>);

    template <typename N>
    std::ostream inline &operator << (std::ostream &o, const prime<N> &p) {
        return o << "prime {" << p.Prime << "}";
    }

}

#endif


