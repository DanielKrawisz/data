// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/random.hpp>
#include <data/math/number/bytes.hpp>

namespace data::math::number {
    // generate a random natural number with a given number of digits (exactly).
    template <Natural N> N generate_random_by_digits (random::source &oo, uint32 bits) {
        if (bits == 0) return N {};

        size_t num_bytes = (bits + 7) / 8;

        auto nb = data::math::N_bytes<endian::big>::zero (num_bytes);

        oo >> nb;

        if (bits % 8 != 0) {
            *nb.begin () &= ((1u << (bits % 8)) - 1u);
            *nb.begin () |= (1u << (bits % 8 - 1u));
        } else *nb.begin () |= (1u << 7);

        return convert<N> (nb);
    }

    template <Natural N> N generate_random_by_max_digits (random::source &oo, uint32 bits) {
        if (bits == 0) return N {};

        size_t num_bytes = (bits + 7) / 8;

        auto nb = data::math::N_bytes<endian::big>::zero (num_bytes);

        oo >> nb;

        if (bits % 8 != 0) *nb.begin () &= ((1u << (bits % 8 + 1)) - 1u);

        return convert<N> (nb);
    }

    template <Natural N> N generate_random_below (random::source &oo, N modulus) {
        if (modulus == 0) throw exception {} << "invalid modulus";

        auto width = bit_width (modulus);

        while (true) {
            N rand = generate_random_by_max_digits<N> (oo, width);
            if (rand < modulus) return rand;
        }
    }
}
