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
            // TODO this algorithm can be made a lot faster by not using odd and div_2 but
            // instead providing an accessor to each digit without altering the number.
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
