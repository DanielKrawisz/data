
#pragma once

#include <data/math/number/NTL/Z.hpp>

#include <data/math/number/prime.hpp>

namespace data::math::number::NTL {
    using namespace ::NTL;
}

namespace data::math::number::NTL {

    bool AKS_is_prime (const N &);

}

namespace data::math::number {

    template <> struct AKS<N> {
        prime<N> is_prime (const N &n) {
            if (NTL::AKS_is_prime (n))
                return prime<N> {n, prime<N>::certain};
            return prime<N> {};
        }
    };

    template struct AKS<N>;

    template <WholeNumber X> struct AKS {
        prime<X> is_prime (const X &n) {
            if (AKS<N>::is_prime (convert<N> (abs (n))))
                return prime<X> {n, prime<N>::certain};
            else return prime<X> {};
        }
    };

    template <> prime<N> inline next_prime (const N &n, uint32 trials) {
        return prime<N> {N (NTL::NextPrime (n.Value, trials)), prime<N>::probable};
    }

    template <WholeNumber NN> prime<NN> inline next_prime (const NN &n) {
        return convert<NN> (next_prime (convert<N> (n)));
    }
}
