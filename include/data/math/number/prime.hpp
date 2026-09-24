// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/arithmetic.hpp>
#include <data/random.hpp>
#include <iostream> // required by windows.

namespace data::math::number {
    // TODO: this constraint is too narrow
    // because it requires bit operations.
    template <WholeNumber N> struct prime;

    template <WholeNumber N> struct eratosthenes;
    template <WholeNumber N> struct primes;
    template <WholeNumber N> struct AKS;

    template <WholeNumber N> struct factorization;
    template <WholeNumber N> factorization<N> factorize (nonzero<N>, eratosthenes<N> &);

    template <WholeNumber N> factorization<N> operator * (const prime<N> &, const prime<N> &);
    template <WholeNumber N> factorization<N> operator * (factorization<N>, factorization<N>);

    // test primality with Miller-Rabin + trial division.
    template <WholeNumber N> prime<N> is_prime (random::source &, const N &, int rounds);

    // a safe prime is of the form 2 p + 1 where p is also prime.
    template <WholeNumber N> prime<N> is_safe (random::source &, const prime<N> &, int rounds);

    // use miller rabin + trial division, fails with a vanishingly small probability
    template <WholeNumber N> prime<N> generate_random_prime (random::source &, uint32 digits, int rounds, bool safe = false);

    // use Maurer’s algorithm. Slower but 100% chance of success.
    template <WholeNumber N> prime<N> generate_Maurer (random::source &, uint32 digits, int rounds);

    template <WholeNumber N> prime<N> next_prime (const N &, uint32 trials = 100);

    // A number that is known to be prime.
    template <WholeNumber N>
    struct prime : nonzero<N> {
        enum likelihood {
            certain,
            probable,
            impossible
        };

        likelihood Likelihood;

        bool valid () const {
            return Likelihood != impossible;
        }

        prime () : nonzero<N> {0}, Likelihood {impossible} {}

        operator bool () const {
            return valid ();
        }

    private:
        prime (N p, likelihood l) : nonzero<N> {p}, Likelihood {l} {}

        friend struct eratosthenes<N>;
        friend struct AKS<N>;
        friend factorization<N> factorize<N> (nonzero<N>, eratosthenes<N> &);
        friend prime<N> is_prime<N> (random::source &, const N &, int rounds);
        friend prime<N> generate_random_prime<N> (random::source &, uint32 digits, int rounds, bool safe);
        friend prime<N> generate_Maurer<N> (random::source &, uint32 digits, int rounds);
        friend prime<N> next_prime<N> (const N &, uint32 trials);
    };

    template <WholeNumber N>
    std::ostream inline &operator << (std::ostream &o, const prime<N> &p) {
        return o << "prime {" << p.Value << "}";
    }

    template <WholeNumber N> prime<N> inline is_safe (random::source &e, const prime<N> &p, int rounds) {
        return is_prime<N> (e, (p.Value - 1u) / 2u, rounds);
    }

}
