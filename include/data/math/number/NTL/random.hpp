
// data/random/NTL.hpp

#pragma once

#include <data/math/number/NTL/Z.hpp>
#include <data/math/number/prime.hpp>
#include <NTL/random.hpp>

namespace data::math::number {

    template <> prime<N> inline is_prime<N> (random::source &x, const N &n, int rounds) {
        if (NTL::random_function (
            x,
            static_cast<long (*) (const NTL::ZZ &, long)> (NTL::ProbPrime),
            n.Value,
            static_cast<long> (rounds)))
            return prime<N> (n, probable);
        else return prime<N> {};
    }

    template <> prime<N> inline generate_random_prime<N> (random::source &x, uint32 bits, generate_prime_parameters params) {
        if (params.safe) return prime<N> {N {NTL::random_function (
                x, NTL::GenGermainPrime_ZZ,
                static_cast<long> (bits) - 1u,
                static_cast<long> (params.rounds))} * 2u + 1u, probable};
        else return prime<N> {N {NTL::random_function (
            x, NTL::GenPrime_ZZ,
            static_cast<long> (bits),
            static_cast<long> (params.rounds)
        )}, probable};
    }

    template <WholeNumber NN> prime<NN> inline is_prime (random::source &x, const NN &n, int rounds) {
        return prime<NN> (is_prime<N> (x, convert<N> (n)));
    }

    template <WholeNumber NN> prime<NN> inline generate_random_prime (random::source &x, uint32 bits, generate_prime_parameters p) {
        return prime<NN> (generate_random_prime<N> (x, bits, p));
    }

}

