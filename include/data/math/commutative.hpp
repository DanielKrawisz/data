// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMMUTATIVE
#define DATA_MATH_COMMUTATIVE

#include <data/math/algebra.hpp>

namespace data::math {
    
    template <typename f, typename x> struct commutative;
    
    template <typename x> using commutative_plus = commutative<plus<x>, x>;
    template <typename x> using commutative_times = commutative<times<x>, x>;
    
}

#endif 

