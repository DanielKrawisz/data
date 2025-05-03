// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_FACTOR
#define DATA_MATH_NUMBER_FACTOR

#include <data/tools.hpp>
#include <data/math/polynomial.hpp>
#include <data/math/number/eratosthenes.hpp>

namespace data::math::number {

    template <typename N> struct factorization : list<power<prime<N>, N>> {
        using list<power<prime<N>, N>>::list;
        factorization (list<power<prime<N>, N>> &&x): list<power<prime<N>, N>> {x} {}

        bool operator == (const factorization &x) const {
            return static_cast<const list<power<prime<N>, N>> &> (*this) == static_cast<const list<power<prime<N>, N>> &> (x);
        }

        factorization rest () const {
            return static_cast<const list<power<prime<N>, N>> &> (*this).rest ();
        }

        operator N () const;
    };

    template <typename N> factorization<N> factorize (nonzero<N> n, eratosthenes<N> &e) {

        factorization<N> factors {};

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

                d = data::divide<N> (x, p.Prime);

                // in this case, the number itself must be prime.
                if (d.Quotient < p.Prime.Value) {
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
                division<N> d = math::number::natural_divide (x, factor.Base.Prime.Value);
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

