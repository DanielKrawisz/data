// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_FACTOR
#define DATA_MATH_NUMBER_FACTOR

#include <data/math/power.hpp>
#include <data/math/number/eratosthenes.hpp>
#include <data/math/number/natural.hpp>

namespace data::math::number {

    template <typename N> struct factorization : list<power<prime<N>, N>> {};

    template <typename N> factorization<N> factor (N n, eratosthenes<N> &e) {

        factorization<N> factors;

        if (n == 0) return factors;
        // create an infinite list of primes.
        primes<N> P {e};

        N x = n;

        // successively try all primes until the number is exhausted.
        while (true) {
            if (x == 1) goto out;

            division<N> d;
            prime<N> p;

            // search for the next prime factor.
            while (true) {
                p = P.first ();
                d = math::number::natural::divide (x, p.Prime);

                // in this case, the number itself must be prime.
                if (d.Quotient < p.Prime) {
                    p = prime<N> {x, prime<N>::certain};
                    x = 1;
                    break;
                }

                if (d.Remainder == 0) {
                    x = d.Quotient;
                    break;
                }

                P = P.rest ();
            }

            // next we try that prime again until it no longer divides the result.
            power<prime<N>, N> factor {p, 1};

            while (true) {
                division<N> d = math::number::natural::divide (x, factor.Base.Prime);
                if (d.Remainder != 0) break;
                factor.Exponent++;
                x = d.Quotient;
            }

            factors <<= factor;
        }

        out:
        // update the sieve we were using in case we generated more primes with it.
        e = P.Eratosthenes;
        return factors;
    }

}

#endif

