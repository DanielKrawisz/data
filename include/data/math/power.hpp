// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/arithmetic.hpp>
#include <data/math/ring.hpp>
#include <data/math/number/bounded/bounded.hpp>

// NOTE: these functions would be better if we didn't use >> but
// instead accessed each number digit by digit.

namespace data::math {

    struct negative_power : exception {
        negative_power (): exception {"cannot take a negative power"} {}
    };

    // essentially a fold expression in which the bits of
    // the number become the sequence of inputs for each round.
    template <typename N, typename A, group_number B>
    class binary_accumulate {
        // use plus for times and times for pow.
        function<N (const N &, const A &)> C;

        // use mul_2 for times and square for pow.
        function<A (const A &)> D;

    public:
        constexpr binary_accumulate (
            function<N (const N &, const A &)> c,
            function<A (const A &)> d): C {c}, D {d} {}

        constexpr N operator () (N so_far, A x2n, B p) {
            if (p == 0) return so_far;
            return operator () (odd (p) ? C (so_far, x2n) : so_far, D (x2n), div_2 (p));
        }
    };

    // we can make a multiplication out of a group number type
    // thus turning them into ring number type.
    template <typename A, group_number B>
    constexpr A inline binary_accumulate_times (const A &x, const B &y) {
        if (y < 0) return -binary_accumulate_times (x, -y);
        return binary_accumulate<A, A, B> {&plus<A, A>, &mul_2<A>} (A {0}, x, y);
    }

    template <typename Base, group_number Exp>
    constexpr Base inline binary_accumulate_pow (const Base &x, const Exp &y) {
        if (y < 0) throw negative_power {};
        return binary_accumulate<Base, Base, Exp> {&times<Base, Base>, &square<Base>} (Base {1}, x, y);
    }

    template <typename A, group_number B, group_number Mod>
    constexpr A inline binary_accumulate_times_mod (const A &x, const B &y, const nonzero<Mod> &z) {
        if (z.Value == 0) throw division_by_zero {};
        if (y < 0) return -binary_accumulate_times_mod (x, -y, z);
        return binary_accumulate<Mod, A, B> {[z] (const Mod &a, const A &b) -> A {
            return plus_mod<Mod, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return mul_2_mod<A, Mod> (a, z);
        }} (Mod {0}, x, y);
    }

    template <typename A, group_number B, group_number_unsigned Mod>
    constexpr A inline binary_accumulate_times_mod (const A &x, const B &y, const nonzero<Mod> &z) {
        if (y < 0) return -binary_accumulate_times_mod (x, -y, z);
        return binary_accumulate<Mod, A, B> {[z] (const A &a, const A &b) -> A {
            return plus_mod<A, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return mul_2_mod<A, Mod> (a, z);
        }} (Mod {0}, x, y);
    }

    template <typename Base, group_number Exp, group_number Mod>
    constexpr Mod inline binary_accumulate_pow_mod (const Base &x, const Exp &y, const nonzero<Mod> &z) {
        if (z.Value < 0) throw negative_power {};
        if (y < 0) throw negative_power {};
        return binary_accumulate<Mod, Base, Exp> {[z] (const Mod &a, const Base &b) {
            return data::times_mod<Mod, Base, Mod> (a, b, z);
        }, [z] (const Base &a) {
            return static_cast<Base> (data::square_mod<Base, Mod> (a, z));
        }} (Mod {1}, x, y);
    }

    template <typename Base, group_number Exp, group_number_unsigned Mod>
    constexpr Mod inline binary_accumulate_pow_mod (const Base &x, const Exp &y, const nonzero<Mod> &z) {
        if (y < 0) throw negative_power {};
        return binary_accumulate<Mod, Base, Exp> {[z] (const Mod &a, const Base &b) {
            return times_mod<Mod, Base, Mod> (a, b, z);
        }, [z] (const Base &a) {
            return static_cast<Base> (square_mod<Base, Mod> (a, z));
        }} (Mod {1}, x, y);
    }
}

namespace data::math::def {

    // default implementations for pow and pow mod.
    template <typename A, group_number Exp>
    struct pow<A, Exp> {
        constexpr A operator () (const A &x, const Exp &y) {
            return math::binary_accumulate_pow (x, y);
        }
    };

    template <typename X> using accumulate_inner =
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

    // default pow_mod
    template <typename Base, group_number Exp, group_number Mod> struct pow_mod<Base, Exp, Mod> {
        constexpr Mod operator () (const Base &x, const Exp &y, const nonzero<Mod> &z) {

            // we have a problem here because we can overflow, and when that happens, we cannot correctly mod
            // the result. We transform to a type that is bigger than this and then transform back.

            using innerBase = accumulate_inner<Base>;
            using innerExp = accumulate_inner<Exp>;
            using innerMod = accumulate_inner<Mod>;

            return math::convert<Mod> (
                math::binary_accumulate_pow_mod (
                    math::convert<innerBase> (x),
                    math::convert<innerExp> (y),
                    math::nonzero {math::convert<innerMod> (z.Value)}));
        }
    };

}

