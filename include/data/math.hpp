// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH
#define DATA_MATH

#include <data/math/permutation.hpp>
#include <data/math/fraction.hpp>

namespace data {
    
    // permutations
    template <typename X>
    using permutation = data::math::permutation<X>;
    
    template <typename Z, typename N = Z>
    using fraction = data::math::fraction<Z, N>;
    
}

#endif
