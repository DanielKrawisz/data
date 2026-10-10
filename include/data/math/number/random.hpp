// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/random.hpp>
#include <data/math/number/bytes.hpp>

// generate a random natural number with a given number of digits.
namespace data::math::number {
    template <Natural N> N generate_random (random::source &x, uint32 bits) {
        if (bits == 0) return N {};

        size_t num_bytes = (bits + 7) / 8;

        auto nb = data::math::N_bytes<endian::big>::zero (num_bytes);

        x >> nb;

        if (bits % 8 != 0) {
            *nb.begin () &= ((1u << (bits % 8)) - 1u);
            *nb.begin () |= (1u << (bits % 8 - 1));
        } else {
            *nb.begin () |= (1u << 7);
        }

        return convert<N> (nb);
    }
}
