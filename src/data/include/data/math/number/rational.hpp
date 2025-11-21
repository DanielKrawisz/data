// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_RATIONAL
#define DATA_MATH_NUMBER_RATIONAL

#include <data/math/field.hpp>
#include <data/ordered.hpp>
#include <data/arithmetic.hpp>

namespace data::math {
    template <typename Q> concept rational =
        Ordered<Q> && field<Q, def::plus<Q>, def::times<Q>> &&
        BigNumberConstructable<Q> &&
        NumberComparable<Q> &&
        div_algebraic_big<Q>;
}

#endif
