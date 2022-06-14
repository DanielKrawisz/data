// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COUNTABLE
#define DATA_MATH_COUNTABLE

#include <type_traits>

#include <data/types.hpp>

namespace data::math {
    
    template <typename X> struct first;
    
    template <typename L>
    concept countable = requires() {
        { first<L>{}() } -> std::same_as<L>;
    } && requires (const L &n) {
        { next(n) } -> std::same_as<L>;
    };

}

#endif
