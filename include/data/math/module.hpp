// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_MODULE
#define DATA_MATH_MODULE

#include <data/math/ring.hpp>

namespace data::interface {
    
    template <
        typename ring_plus, 
        typename ring_times, 
        typename ring_elem, 
        typename group_plus, 
        typename group_elem>
    struct module : 
        ring<ring_plus, ring_times, ring_elem>, 
        group<group_plus, group_elem> {
        // TODO
    };
    
}

#endif 
