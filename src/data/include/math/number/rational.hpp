// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_RATIONAL
#define DATA_MATH_NUMBER_RATIONAL

#include <data/math/field.hpp>
#include <data/math/ordered.hpp>

namespace data::math {
    template <typename Q> concept rational = ordered<Q> && field<Q, plus<Q>, times<Q>>;
}

#endif
