// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_PRIME
#define DATA_MATH_NUMBER_PRIME

#include <data/integral.hpp>
#include <data/random.hpp>
#include <iostream> // required by windows.

namespace data::math::number {
    template <integral N> struct prime;

    template <typename N> struct eratosthenes;
    template <typename N> struct primes;
    template <typename N> struct AKS;

    template <typename N> struct factorization;
    template <typename N> factorization<N> factorize (nonzero<N>, eratosthenes<N> &);

    template <typename N> factorization<N> operator * (const prime<N> &, const prime<N> &);
    template <typename N> factorization<N> operator * (factorization<N>, factorization<N>);

    // test primality with Miller-Rabin + trial division. (This will rely on cryptopp or GMP)
    template <typename N> prime<N> is_prime (entropy &, const N &, int rounds);

    // Slower than Miller-Rabin but with 100% chance of success.
    template <typename N> prime<N> generate_Maurer (entropy &);

    // A number that is known to be prime.
    // So far eratosthenes is the only way
    // to construct this type.
    template <integral N>
    struct prime : nonzero<N> {
        enum likelihood {
            certain,
            probable,
            impossible
        };

        nonzero<N> Prime;
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
        friend factorization<N> factorize<N> (nonzero<N>, eratosthenes<N> &);
        friend prime<N> is_prime<N> (entropy &, const N &, int rounds);
        friend prime<N> generate_Maurer<N> (entropy &);
    };

    template <typename N>
    std::ostream inline &operator << (std::ostream &o, const prime<N> &p) {
        return o << "prime {" << p.Prime << "}";
    }

}

#endif


