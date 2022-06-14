// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMMUTATIVE
#define DATA_MATH_COMMUTATIVE

#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename f, typename x> struct commutative;
    
    template <typename x> using commutative_plus = commutative<plus<x>, x>;
    template <typename x> using commutative_times = commutative<times<x>, x>;
    
    template <std::integral X> struct commutative<plus<X>, X> {};
    template <std::integral X> struct commutative<times<X>, X> {};
    
}

#endif 

