// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_ALGEBRA
#define DATA_MATH_ALGEBRA_ALGEBRA
#include <data/math/linear/space.hpp>

namespace data::math {
    template <typename F, typename V>
    concept algebra = linear::space<F, V> && ring<V>;

    template <typename F, typename V>
    concept division_algebra = algebra<F, V> && requires (const V &a, const V &b) {
        {a / b} -> std::same_as<V>;
    };

    template <typename F, typename V>
    concept normed_division_algebra = division_algebra<F, V> && normed<F, V>;

}

#endif
