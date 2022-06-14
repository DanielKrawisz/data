// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ASSOCIATIVE
#define DATA_MATH_ASSOCIATIVE

#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename f, typename x> struct associative;
    
    template <typename x> using associative_plus = associative<plus<x>, x>;
    template <typename x> using associative_times = associative<times<x>, x>;
    
    template <std::integral X> struct associative<plus<X>, X> {};
    template <std::integral X> struct associative<times<X>, X> {};
    
}

#endif 

