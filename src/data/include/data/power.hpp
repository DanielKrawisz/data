// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POWER
#define DATA_POWER

#include <data/integral.hpp>
#include <data/math/ring.hpp>

namespace data::math {

    // essentially a fold expression in which the bits of
    // the number become the sequence of inputs for each round.
    template <typename N, typename A, bit_integral B>
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
            return operator () ((p & B {1}) == 1 ? C (so_far, x2n) : so_far, D (x2n), p >> 1);
        }
    };

    // we can make a multiplication out of a group_integral type
    // thus turning them into ring integral type.
    template <typename A, bit_integral B>
    constexpr A inline binary_accumulate_times (const A &x, const B &y) {
        if (y < 0) return -binary_accumulate_times (x, -y);
        return binary_accumulate<A, A, B> {&data::plus<A, A>, &data::mul_2<A>} (A {0}, x, y);
    }

    template <typename A, bit_integral B>
    constexpr A inline binary_accumulate_pow (const A &x, const B &y) {
        if (y < 0) throw exception {} << "cannot take a negative power";
        return binary_accumulate<A, A, B> {&data::times<A, A>, &data::square<A>} (A {1}, x, y);
    }

    template <typename A, unsigned_bit_integral B>
    constexpr A inline binary_accumulate_times (const A &x, const B &y) {
        return binary_accumulate<A, A, B> {&data::plus<A, A>, &data::mul_2<A>} (A {0}, x, y);
    }

    template <typename A, unsigned_bit_integral B>
    constexpr A inline binary_accumulate_pow (const A &x, const B &y) {
        return binary_accumulate<A, A, B> {&data::times<A, A>, &data::square<A>} (A {1}, x, y);
    }

    template <typename A, bit_integral B, group_integral Mod>
    constexpr A inline binary_accumulate_times_mod (const A &x, const B &y, const nonzero<Mod> &z) {
        if (z.Value < 0) throw exception {} << "cannot mod by a negative number";
        if (y < 0) return -binary_accumulate_times_mod (x, -y, z);
        return binary_accumulate<Mod, A, B> {[z] (const Mod &a, const A &b) -> A {
            return data::plus_mod<Mod, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return data::mul_2_mod<A, Mod> (a, z);
        }} (Mod {0}, x, y);
    }

    template <typename A, bit_integral B, unsigned_group_integral Mod>
    constexpr A inline binary_accumulate_times_mod (const A &x, const B &y, const nonzero<Mod> &z) {
        if (y < 0) return -binary_accumulate_times_mod (x, -y, z);
        return binary_accumulate<Mod, A, B> {[z] (const A &a, const A &b) -> A {
            return data::plus_mod<A, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return data::mul_2_mod<A, Mod> (a, z);
        }} (Mod {0}, x, y);
    }

    template <typename A, unsigned_bit_integral B, unsigned_group_integral Mod>
    constexpr A inline binary_accumulate_times_mod (const A &x, const B &y, const nonzero<Mod> &z) {
        return binary_accumulate<Mod, A, B> {[z] (const A &a, const A &b) -> A {
            return data::plus_mod<A, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return data::mul_2_mod<A, Mod> (a, z);
        }} (Mod {0}, x, y);
    }

    template <typename A, bit_integral B, group_integral Mod>
    constexpr auto inline binary_accumulate_pow_mod (const A &x, const B &y, const nonzero<Mod> &z) -> decltype (data::abs (z.Value)) {
        if (z.Value < 0) throw exception {} << "cannot mod by a negative number";
        if (y < 0) throw exception {} << "cannot take a negative power";
        return binary_accumulate<Mod, A, B> {[z] (const Mod &a, const A &b) {
            return data::times_mod<Mod, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return static_cast<A> (data::square_mod<A, Mod> (a, z));
        }} (Mod {1}, x, y);
    }

    template <typename A, bit_integral B, unsigned_group_integral Mod>
    constexpr auto inline binary_accumulate_pow_mod (const A &x, const B &y, const nonzero<Mod> &z) -> decltype (data::abs (z.Value)) {
        if (y < 0) throw exception {} << "cannot take a negative power";
        return binary_accumulate<Mod, A, B> {[z] (const Mod &a, const A &b) {
            return data::times_mod<Mod, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return static_cast<A> (data::square_mod<A, Mod> (a, z));
        }} (Mod {1}, x, y);
    }

    template <typename A, unsigned_bit_integral B, unsigned_group_integral Mod>
    constexpr auto inline binary_accumulate_pow_mod (const A &x, const B &y, const nonzero<Mod> &z) -> decltype (data::abs (z.Value)) {
        return binary_accumulate<Mod, A, B> {[z] (const Mod &a, const A &b) {
            return data::times_mod<Mod, A, Mod> (a, b, z);
        }, [z] (const A &a) {
            return static_cast<A> (data::square_mod<A, Mod> (a, z));
        }} (Mod {1}, x, y);
    }

    // default implementations for pow and pow mod.
    template <typename A, bit_integral Exp>
    struct pow<A, Exp> {
        constexpr A operator () (const A &x, const Exp &y) {
            return binary_accumulate_pow (x, y);
        }
    };

    template <typename A, bit_integral Exp, group_integral Mod> struct pow_mod<A, Exp, Mod> {
        constexpr auto operator () (const A &x, const Exp &y, const nonzero<Mod> &z) -> decltype (data::abs (z.Value)) {
            return binary_accumulate_pow_mod (x, y, z);
        }
    };

}

#endif

