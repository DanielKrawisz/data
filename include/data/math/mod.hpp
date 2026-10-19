// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/power.hpp>

namespace data::math::def {

    template <typename X> using doubled =
        std::conditional_t<!Integral<X>, X,
            std::conditional_t<SignedIntegral<X>,
                std::conditional_t<sizeof (X) == 1, int16,
                std::conditional_t<sizeof (X) == 2, int32,
                std::conditional_t<sizeof (X) == 4, int64,
                number::bounded<true, endian::little, (sizeof (X) + 7) * 2 / 8, uint64>>>
                >,
                std::conditional_t<sizeof (X) == 1, uint16,
                std::conditional_t<sizeof (X) == 2, uint32,
                std::conditional_t<sizeof (X) == 4, uint64,
                number::bounded<false, endian::little, (sizeof (X) + 7) * 2 / 8, uint64>>>
                >>>;

    // default square mod
    template <typename Base, group_number Mod> struct square_mod<Base, Mod> {
        constexpr Mod operator () (const Base &x, const nonzero<Mod> &z) {

            // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
            // the result. We transform to a type that is bigger than this and then transform back.

            using innerBase = doubled<Base>;
            using innerMod = doubled<Mod>;

            return math::convert<Mod> (
                data::mod (
                    data::square (math::convert<innerBase> (x)),
                    math::nonzero {math::convert<innerMod> (z.Value)}));
        }
    };

    template <typename Base, group_number Exp, group_number Mod> struct pow_mod<Base, Exp, Mod> {
        constexpr Mod operator () (const Base &x, const Exp &y, const nonzero<Mod> &z) {

            // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
            // the result. We transform to a type that is bigger than this and then transform back.

            using innerBase = doubled<Base>;
            using innerExp = doubled<Exp>;
            using innerMod = doubled<Mod>;

            return math::convert<Mod> (
                math::binary_accumulate_pow_mod (
                    math::convert<innerBase> (x),
                    math::convert<innerExp> (y),
                    math::nonzero {math::convert<innerMod> (z.Value)}));
        }
    };

    template <typename Base, group_number Exp, group_number Mod> struct times_mod<Base, Exp, Mod> {
        constexpr Mod operator () (const Base &x, const Exp &y, const nonzero<Mod> &z) {

            // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
            // the result. We transform to a type that is bigger than this and then transform back.

            using innerBase = doubled<Base>;
            using innerExp = doubled<Exp>;
            using innerMod = doubled<Mod>;

            return math::convert<Mod> (
                math::binary_accumulate_times_mod (
                    math::convert<innerBase> (x),
                    math::convert<innerExp> (y),
                    math::nonzero {math::convert<innerMod> (z.Value)}));
        }
    };


}
