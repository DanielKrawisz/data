// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/math/number/binary_accumulate.hpp>

// NOTE: these functions would be better if we didn't use >> but
// instead accessed each number digit by digit.

namespace data::math {

    struct negative_power : exception {
        negative_power (): exception {"cannot take a negative power"} {}
    };

    template <typename Base, group_number Exp>
    constexpr Base inline binary_accumulate_pow (const Base &x, const Exp &y) {
        if (y < 0) throw negative_power {};
        return binary_accumulate<Base, Base, Exp> {&times<Base, Base>, &square<Base>} (Base {1}, x, y);
    }

    template <typename Base, group_number Exp, group_number Mod>
    constexpr Mod inline binary_accumulate_pow_mod (const Base &x, const Exp &y, const nonzero<Mod> &z) {
        if (z.Value < 0) throw negative_power {};
        if (y < 0) throw negative_power {};
        return binary_accumulate<Mod, Base, Exp> {[z] (const Mod &a, const Base &b) -> Mod {
            // we don't use times mod here in order to avoid expanding the numbers a second time. '
            return mod (times (a, b), z);
        }, [z] (const Base &a) {
            return static_cast<Base> (mod<Base, Mod> (square (a), z));
        }} (Mod {1}, x, y);
    }

    template <typename Base, group_number Exp, group_number_unsigned Mod>
    constexpr Mod inline binary_accumulate_pow_mod (const Base &x, const Exp &y, const nonzero<Mod> &z) {
        if (y < 0) throw negative_power {};
        return binary_accumulate<Mod, Base, Exp> {[z] (const Mod &a, const Base &b) {
            return mod (times (a, b), z);
        }, [z] (const Base &a) {
            return static_cast<Base> (mod<Base, Mod> (square (a), z));
        }} (Mod {1}, x, y);
    }

}

namespace data::math::def {

    // default implementations for pow.
    template <typename A, group_number Exp>
    struct pow<A, Exp> {
        constexpr A operator () (const A &x, const Exp &y) {
            return math::binary_accumulate_pow (x, y);
        }
    };

}

