// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_RANDOM
#define DATA_CRYPTO_RANDOM

#include <data/concepts.hpp>
#include <data/random.hpp>
#include <data/array.hpp>
#include <data/slice.hpp>
#include <data/crypto/NIST_DRBG.hpp>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

namespace data::crypto::random {
    using namespace data::random;

    template <typename engine> concept DRBG =
        data::random::RNG<engine> &&
        std::derived_from<engine, source> &&
        requires (engine &e, byte_slice b) {
            { e.reseed (b) };
        } && requires {
            engine::MaxStrength;
        };

    template <typename engine> concept RNG = RNG<engine> &&
        std::derived_from<engine, CryptoPP::RandomNumberGenerator>;

    // turn a cryptopp RNG into a random::source.
    template <std::derived_from<CryptoPP::RandomNumberGenerator> rng>
    struct crypto_pp_secure_source : source {
        void read (byte *b, size_t z) override {
            RNG.GenerateBlock (b, z);
        }

        rng RNG;
    };

    // turn a DRBG into a crypto pp RNG.
    struct to_crypto_pp_RNG : CryptoPP::RandomNumberGenerator {
        random::source &Rand;

        to_crypto_pp_RNG (random::source &rand): Rand {rand} {}

        void GenerateBlock (byte *output, size_t size) final override {
            return Rand.read (output, size);
        }
    };

    // Direct OS entropy (nonblocking)
    using OS_entropy = crypto_pp_secure_source<CryptoPP::NonblockingRng>;

    // Direct OS entropy (may block)
    using OS_entropy_strong = crypto_pp_secure_source<CryptoPP::BlockingRng>;

    using default_secure_random = automatic_reseed<NIST::auto_generate_with_additional_entropy<NIST::HMAC_DRBG<hash::SHA2_256>>>;

    // NOTE: we do not actually implement this function.
    source &get ();

}

#endif

