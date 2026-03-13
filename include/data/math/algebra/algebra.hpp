// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_ALGEBRA
#define DATA_MATH_ALGEBRA_ALGEBRA
#include <data/math/linear/space.hpp>

namespace data::math {
    template <typename F, typename V>
    concept algebra = linear::space<F, V> && ring<V>;

}

#endif
