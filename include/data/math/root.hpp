// Copyright (c) 2020 - 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/set.hpp>

namespace data::math::def {

    // root{}(number) => set<number>
    template <typename number, uint64 power> struct root;
    template <typename number, uint64 power, typename mod> struct root_mod;

    template <typename field, typename number> struct radical {
        field Value;
        number InversePower;
    };
}

namespace data {
    
    template <uint64 pow, typename X> 
    set<X> root (const X &x) {
        return math::def::root<X, pow> {} (x);
    }
    
    template <typename X> 
    set<X> sqrt (const X &x) {
        return math::def::root<X, 2> {} (x);
    }

    template <uint64 pow, typename X, typename mod>
    set<X> root_mod (const X &x, const mod &y) {
        return math::def::root_mod<X, pow, mod> {} (x, y);
    }

    template <typename X, typename mod>
    set<X> sqrt_mod (const X &x, const mod &y) {
        return math::def::root_mod<X, 2, mod> {} (x, y);
    }
    
}
