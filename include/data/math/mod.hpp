// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/power.hpp>

namespace data::math::def {

    template <std::integral X> struct mod<X> {
        constexpr auto operator () (const X &x, const nonzero<X> &n) const {
            if (n.Value == 0) throw math::division_by_zero {};
            X result = x % n.Value;
            return result < 0 ? result + n.Value : result;
        }
    };

    template <MultiplicativeNumber A, MultiplicativeNumber B> struct mod<A, B> {
        constexpr B operator () (const A &a, const nonzero<B> &b) const {
            if (b.Value == 0) throw math::division_by_zero {};
            if (a >= 0 && a < b.Value) return B (a);
            auto result = divmod<A, B> {} (a, b).Remainder;
            return result < 0 ? B (result + b.Value) : result;
        }
    };

    template <typename A, typename Mod> struct negate_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return x == 0 ? Mod (0):
                x > 0 && x < n.Value ?
                // We need to cast to A here because of the endian arithmetic numbers.
                // We should be able to get rid of that. TODO
                Mod (n.Value - x):
                Mod (n.Value - data::mod (x, n));
        }
    };

    template <typename A, typename B, typename Mod> struct plus_mod {
        constexpr auto operator () (const A &x, const B &y, const math::nonzero<Mod> &z) const {
            return data::mod (data::plus (data::mod (x, z), data::mod (y, z)), z);
        }
    };

    template <typename A, typename B, typename Mod> struct minus_mod {
        constexpr auto operator () (const A &x, const B &y, const nonzero<Mod> &z) const {
            return data::mod (data::minus (data::mod (x, z), data::mod (y, z)), z);
        }
    };

    template <typename A, typename Mod> struct mul_2_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::mul_2 (data::mod (x, n)), n);
        }
    };

    // double the size of the numbers in case of overflows.
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

    template <typename X> using extended =
        std::conditional_t<!Integral<X>, X,
            std::conditional_t<SignedIntegral<X>,
                std::conditional_t<sizeof (X) == 1, int16,
                std::conditional_t<sizeof (X) == 2, int32,
                std::conditional_t<sizeof (X) == 4, int64,
                number::bounded<true, endian::little, sizeof (X) / 8 + 1, uint64>>>
                >,
                std::conditional_t<sizeof (X) == 1, uint16,
                std::conditional_t<sizeof (X) == 2, uint32,
                std::conditional_t<sizeof (X) == 4, uint64,
                number::bounded<false, endian::little, sizeof (X) / 8 + 1, uint64>>>
                >>>;

    template <typename Base, group_number Exp, group_number Mod> struct plus_mod<Base, Exp, Mod> {
        constexpr Mod operator () (const Base &x, const Exp &y, const nonzero<Mod> &z) {

            // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
            // the result. We transform to a type that is bigger than this and then transform back.

            using innerBase = extended<Base>;
            using innerExp = extended<Exp>;
            using innerMod = extended<Mod>;

            return math::convert<Mod> (
                data::mod (
                    data::plus (math::convert<innerMod> (data::mod (x, z)), math::convert<innerMod> (data::mod (y, z))),
                    math::nonzero {math::convert<innerMod> (z.Value)}));
        }
    };

    template <typename Base, group_number Exp, group_number Mod> struct minus_mod<Base, Exp, Mod> {
        constexpr Mod operator () (const Base &a, const Exp &b, const nonzero<Mod> &z) {

            if (data::is_negative (a) && data::is_negative (b))
                return data::minus_mod (
                    data::negate_mod (b, z),
                    data::negate_mod (a, z), z);

            if (data::is_negative (a))
                return data::plus_mod (data::negate_mod (a, z), b, z);

            if (data::is_negative (b))
                return data::plus_mod (a, data::negate_mod (b, z), z);

            if (b > a) return data::negate_mod (data::minus_mod (b, a, z), z);

            // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
            // the result. We transform to a type that is bigger than this and then transform back.

            using innerBase = extended<Base>;
            using innerExp = extended<Exp>;
            using innerMod = extended<Mod>;

            return math::convert<Mod> (
                data::mod (
                    data::minus (math::convert<innerMod> (data::mod (a, z)), math::convert<innerMod> (data::mod (b, z))),
                           math::nonzero {math::convert<innerMod> (z.Value)}));
        }
    };

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
