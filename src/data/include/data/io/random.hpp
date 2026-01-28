// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_RANDOM
#define DATA_IO_RANDOM

#include <data/crypto/random.hpp>

namespace data::random {

    struct options {
        // if this is disabled, only the casual
        // random generator will be available.
        bool secure = true;

        // if a seed is not provided, we will
        // read our entropy from the OS, which is
        // the preferred option.
        maybe<bytes> seed = {};

        // if a nonce is not provided, we will
        // read it from the entropy source (either
        // from the OS or from a seed.)
        bytes nonce = {};

        // If this is provided, it is used by the
        // secure random generator each time we
        // read from it.
        source *additional = nullptr;

        // used to select the secure RNG we will use.
        // ignored if secure == false
        // allowed values are 112, 128, 192, 256
        size_t strength = 256;

    };

    // after this function is called, random::get () and crypto::random::get ()
    void init (options = {});

    // user must provide this to initialize the secure random generator.
    extern bytes Personalization;

}

#endif
