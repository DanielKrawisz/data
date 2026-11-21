// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

#include <data/concepts.hpp>
#include <data/random.hpp>
#include <data/array.hpp>
#include <data/slice.hpp>

namespace CryptoPP {

    // turn a cryptopp RNG into a random::source.
    template <std::derived_from<RandomNumberGenerator> rng>
    struct crypto_pp_secure_source : data::random::source {
        void read (byte *b, size_t z) override {
            RNG.GenerateBlock (b, z);
        }

        rng RNG;
    };

    // turn a DRBG into a crypto pp RNG.
    struct to_crypto_pp_RNG : RandomNumberGenerator {
        data::random::source &Rand;

        to_crypto_pp_RNG (data::random::source &rand): Rand {rand} {}

        void GenerateBlock (byte *output, size_t size) final override {
            return Rand.read (output, size);
        }
    };

}
