
#pragma once

#include <CryptoPP/Z.hpp>

#include <data/math/number/prime.hpp>
#include <CryptoPP/nbtheory.h>

namespace data::math::number {

    template <>
    prime<CryptoPP::Z> generate_Maurer<CryptoPP::Z> (
        random::source &source,
        uint32 digits
    ) {
        CryptoPP::to_crypto_pp_RNG rng {source};

        return CryptoPP::MaurerProvablePrime (rng, digits);
    }

    template <Number N>
    prime<N> generate_Maurer (
        random::source &source,
        uint32 digits
    ) {
        auto p = generate_Maurer<CryptoPP::Z> (
            source, digits, rounds
        );

        return convert<N> (p);
    }
}

