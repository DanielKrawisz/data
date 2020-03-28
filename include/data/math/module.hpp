// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_MODULE
#define DATA_MATH_MODULE

#include <data/math/ring.hpp>

namespace data::interface {
    
    template <
        typename ring_elem, 
        typename group_elem,
        typename ring_plus = data::plus<ring_elem>, 
        typename ring_times = data::times<ring_elem>, 
        typename group_plus = data::plus<group_elem>>
    struct module : 
        ring<ring_elem, ring_plus, ring_times>, 
        group<group_plus, group_elem> {
        // TODO
    };
    
}

#endif 
