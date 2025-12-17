// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_RANDOM
#define DATA_CRYPTO_RANDOM

#include <data/random.hpp>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

namespace data::crypto {

    template <typename engine> concept DRBG =
        RNG<engine> && requires (byte_slice b) {
            { engine {b} };
        } && requires (engine &e, byte_slice b) {
            { e.reseed (b) } -> Same<e &>;
        };

    template <typename engine> concept StrongRNG = RNG<engine> &&
        std::derived_from<engine, CryptoPP::RandomNumberGenerator>;

    template <std::derived_from<CryptoPP::RandomNumberGenerator> RNG>
    struct crypto_pp_secure_engine : public RNG {
        void read (byte *b, size_t z) override {
            this->GenerateBlock (b, z);
        }

        // forward constructors
        using RNG::RNG;
    };

    using default_secure_random = crypto_pp_secure_engine<CryptoPP::AutoSeededRandomPool>;

    // Direct OS entropy (nonblocking)
    using OS_entropy = crypto_pp_secure_engine<CryptoPP::NonblockingRng>;

    // Direct OS entropy (may block)
    using OS_entropy_strong = crypto_pp_secure_engine<CryptoPP::BlockingRng>;

}

#endif

