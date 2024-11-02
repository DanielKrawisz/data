// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMMUTATIVE
#define DATA_MATH_COMMUTATIVE

#include <data/math/algebra.hpp>

namespace data::math {
    
    template <typename f, typename x> struct is_commutative;

    template <typename f, typename x> concept commutative = requires {
        is_commutative<f, x> {};
    };
    
    template <typename x> concept commutative_plus = commutative<plus<x>, x>;
    template <typename x> concept commutative_times = commutative<times<x>, x>;

    template <std::integral num> struct is_commutative<times<num>, num> {};
    template <std::integral num> struct is_commutative<plus<num>, num> {};
    
}

#endif 

