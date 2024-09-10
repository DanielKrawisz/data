// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_PROBABILITY_TRIANGULAR_DISTRIBUTION
#define DATA_MATH_PROBABILITY_TRIANGULAR_DISTRIBUTION

#include <data/types.hpp>
#include <cmath>

namespace data::math {

    template <typename real = double>
    struct triangular_distribution {
        real Min; // minimum
        real Mode; // the apex of the triangle, therefore A < C < B;
        real Max; // maximum

        template <std::uniform_random_bit_generator engine>
        real operator () (engine &e) const {
            std::uniform_real_distribution<real> dis (0.0, 1.0);
            real ca = Mode - Min;
            real ba = Max - Min;
            real f = ca / ba;
            real x = dis (e);
            return x < f ? Min + std::sqrt (x * ba * ca) : Max - std::sqrt ((1 - x) * ba * (Max - Mode));
        }

        real mean () const {
            return (Min + Max + Mode) / 3;
        }

    };

}

#endif

