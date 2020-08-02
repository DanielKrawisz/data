// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ABS
#define DATA_MATH_NUMBER_ABS

#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/division.hpp>
#include <data/math/nonnegative.hpp>

namespace data::math::number {

    template <typename N, typename Z> 
    struct abs {
        N operator()(const Z& i) const {
            return N{i < Z{0} ? -i : i};
        }
    };

    template <> 
    struct abs<uint64, int64> {
        uint64 operator()(const int64& i) const {
            return static_cast<uint64>(i < 0 ? -i : i);
        }
    };

    template <> 
    struct abs<int64, int64> {
        int64 operator()(const int64& i) const {
            return i < 0 ? -i : i;
        }
    };

    template <typename Z> 
    struct sign {
        math::sign operator()(const Z& i) const {
            return i == Z{0} ? math::zero : i < Z{0} ? math::negative : math::positive;
        }
    };

    template <typename Z> 
    struct arg {
        Z operator()(const Z& i) const {
            if (i == 0) throw division_by_zero{};
            return i / nonzero{abs<Z, Z>{}(i)};
        }
    };
}

#endif
