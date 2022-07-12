// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_MODULE
#define DATA_MATH_MODULE

#include <data/math/ring.hpp>

namespace data::math {
    
    template <
        typename ring_elem, 
        typename group_elem,
        typename ring_plus = plus<ring_elem>, 
        typename ring_times = times<ring_elem>, 
        typename group_plus = plus<group_elem>>
    concept algebraic_module = ring<ring_elem, ring_plus, ring_times> && abelian<group_elem, group_plus>;
    
}

#endif 
