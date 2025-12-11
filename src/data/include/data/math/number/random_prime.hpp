// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_RANDOM_PRIME
#define DATA_MATH_NUMBER_RANDOM_PRIME

#include <data/math/number/prime.hpp>
#include <data/random/cryptopp.hpp>
#include <data/random/numbers.hpp>

#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>

// We use crypto++.
namespace data::math::number {
    // test primality with Miller-Rabin + trial division.
    template <typename N> prime<N> is_prime (entropy &, const N &, uint32 rounds);

    // a safe prime is of the form 2 p + 1 where p is also prime.
    template <typename N> prime<N> is_safe_prime (entropy &, const prime<N> &, int rounds);

    // use miller rabin + trial division, fails with a vanishingly small probability
    template <typename N> prime<N> generate_random (entropy &, uint32 digits, int rounds);

    template <typename N> prime<N> generate_random_safe (entropy &, uint32 digits, int rounds);

    // use Maurer’s algorithm. Slower but 100% chance of success.
    template <typename N> prime<N> generate_Maurer (entropy &, uint32 digits, int rounds);
}

namespace data::math::number::cryptopp {
    using namespace cryptopp;

    // convert to crypto++ from N.
    template <typename N> struct to;

    // convert from crypto++ Integer to N.
    template <typename N> struct from;
}

namespace data::math::number {

    template <typename N> prime<N> inline is_safe_prime (entropy &e, const prime<N> &p, int rounds) {
        return is_prime (e, (p.Value - 1) / 2, rounds);
    }

}

#endif

