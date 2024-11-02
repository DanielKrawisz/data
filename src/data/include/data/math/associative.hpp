// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ASSOCIATIVE
#define DATA_MATH_ASSOCIATIVE

#include <data/math/algebra.hpp>

namespace data::math {
    
    template <typename f, typename x> struct is_associative;

    template <typename f, typename x> concept associative = requires {
        is_associative<f, x> {};
    };
    
    template <typename x> concept associative_plus = associative<plus<x>, x>;
    template <typename x> concept associative_times = associative<times<x>, x>;

    template <std::integral num> struct is_associative<times<num>, num> {};
    template <std::integral num> struct is_associative<plus<num>, num> {};
    
}

#endif 

