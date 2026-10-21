// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/arithmetic.hpp>

namespace data::math {

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

            int32 i = 0;
            size_t width = bit_width (p);
            while (i < width) {
                if (digit (p, i)) so_far = C (so_far, x2n);
                x2n = D (x2n);
                i++;
            }

            return so_far;
        }
    };

    // we can make a multiplication out of a group number type
    // thus turning them into ring number type.
    template <typename A, group_number B>
    constexpr A inline binary_accumulate_times (const A &x, const B &y) {
        if (y < 0) return -binary_accumulate_times (x, -y);
        return binary_accumulate<A, A, B> {&plus<A, A>, &mul_2<A>} (A {0}, x, y);
    }

    template <typename A, group_number B, group_number Mod>
    constexpr A inline binary_accumulate_times_mod (const A &x, const B &y, const nonzero<Mod> &z) {
        if (z.Value == 0) throw division_by_zero {};
        if (y < 0) return -binary_accumulate_times_mod (x, -y, z);
        return binary_accumulate<Mod, A, B> {[z] (const Mod &a, const A &b) -> A {
            // we don't use plus_mod here in order to avoid expanding the numbers a second time.'
            return mod (plus (a, b), z);
        }, [z] (const A &a) {
            return mod<A, Mod> (mul_2 (a), z);
        }} (Mod {0}, x, y);
    }

    template <typename Base, group_number Exp, group_number_unsigned Mod>
    constexpr Mod inline binary_accumulate_times_mod (const Base &x, const Exp &y, const nonzero<Mod> &z) {
        if (y < 0) return negate_mod (binary_accumulate_times_mod (x, negate_mod (y, z), z), z);
        return binary_accumulate<Mod, Mod, Mod> {[z] (const Mod &a, const Mod &b) -> Mod {
            // we don't use plus_mod here in order to avoid expanding the numbers a second time. '
            return mod (plus (a, b), z);
        }, [z] (const Mod &a) {
            return mod (mul_2 (a), z);
        }} (Mod {0}, mod (x, z), mod (y, z));
    }

}
