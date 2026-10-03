
// data/random/NTL.hpp

#pragma once

#include <data/math/number/NTL/Z.hpp>
#include <data/math/number/prime.hpp>
#include <NTL/random.hpp>

namespace data::math::number {

    template <> prime<N> inline is_prime (random::source &x, const N &n, int rounds) {
        if (NTL::random_function (
            x,
            static_cast<long (*) (const NTL::ZZ &, long)> (NTL::ProbPrime),
            n.Value,
            static_cast<long> (rounds)))
            return prime<N> (n, prime<N>::probable);
        else return prime<N> {};
    }

    template <> prime<N> inline generate_random_prime (random::source &x, uint32 bits, int rounds, bool safe) {
        if (safe) return prime<N> {N {NTL::random_function (
                x, NTL::GenGermainPrime_ZZ,
                static_cast<long> (bits) - 1u,
                static_cast<long> (rounds))} * 2u + 1u, prime<N>::probable};
        else return prime<N> {N {NTL::random_function (
            x, NTL::GenPrime_ZZ,
            static_cast<long> (bits),
            static_cast<long> (rounds)
        )}, prime<N>::probable};
    }

    template <WholeNumber NN> prime<NN> inline is_prime (random::source &x, const NN &n, int rounds) {
        return convert<NN> (is_prime<N> (x, convert<N> (n)));
    }

    template <WholeNumber NN> prime<NN> inline generate_random_prime (random::source &x, uint32 bits, int rounds, bool safe) {
        return convert<NN> (generate_random_prime<N> (x, bits, rounds, safe));
    }

}

