// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_PRIMITIVE_ROOT
#define DATA_MATH_NUMBER_PRIMITIVE_ROOT

#include <data/math/power.hpp>
#include <data/math/number/totient.hpp>

namespace data::math::number {

    template <typename N> bool is_primitive_root (N p, N a, eratosthenes<N> &e);
    template <typename N> bool is_primitive_root (prime<N> p, N a, eratosthenes<N> &e);

    template <typename N> maybe<N> primitive_root (N p, eratosthenes<N> &e);
    template <typename N> maybe<N> primitive_root (prime<N> p, eratosthenes<N> &e);

    template <typename N>
    struct primitive_roots {
        N P;
        N Totient;
        list<prime<N>> PrimeFactors;
        primitive_roots (const N &p, eratosthenes<N> &e);
        primitive_roots (const prime<N> &p, eratosthenes<N> &e);
        bool is (N a) const;
    };

    template <typename N> bool inline is_primitive_root (N p, N a, eratosthenes<N> &e) {
        return primitive_roots<N> {p, e}.is (a);
    }

    template <typename N> maybe<N> primitive_root (prime<N> p, eratosthenes<N> &e) {
        if (p.Prime < 3) return {};
        primitive_roots<N> roots {p, e};
        // note: we can skip all powers, for example 4, 8, 9, 16, 25, 27...
        N a {2};
        // note: we may set limits on this procedure to stop and fail
        // if it takes too long. Thus, failure to return a value does
        // not mean that a primitive root does not exist.
        while (!roots.is (a)) a++;
        return a;
    }

    template <typename N> maybe<N> primitive_root (N p, eratosthenes<N> &e) {
        if (p < 3) return {};
        primitive_roots<N> roots {p, e};
        // note: we can skip all powers, for example 4, 8, 9, 16, 25, 27...
        N a {2};
        // note: we may set limits on this procedure to stop and fail
        // if it takes too long. Thus, failure to return a value does
        // not mean that a primitive root does not exist.
        while (!roots.is (a)) a++;
        return a;
    }

    template <typename N> primitive_roots<N>::primitive_roots (const N &p, eratosthenes<N> &e):
    P {p}, Totient {totient<N> (p, e)}, PrimeFactors {for_each ([] (const power<prime<N>, N> &p) {
        return p.Base;
    }, factorize<N> (Totient, e))} {}

    template <typename N> primitive_roots<N>::primitive_roots (const prime<N> &p, eratosthenes<N> &e):
    P {p.Prime}, Totient {totient<N> (p, e)}, PrimeFactors {for_each ([] (const power<prime<N>, N> &p) {
        return p.Base;
    }, factorize<N> (Totient, e))} {}

    template <typename N> bool primitive_roots<N>::is (N a) const {
        // for exach prime factor p, test that a^(s/p) != 1.
        for (const prime<N> &p : PrimeFactors)
            if (data::pow_mod (P, a, Totient / p.Prime) == N {1}) return false;
        return true;
    }

}

#endif

