// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_MODULE
#define DATA_MATH_MODULE

#include <data/math/ring.hpp>

namespace data::math {
    
    // note: the commented conditions need to be uncommented.
    template <
        typename ring_elem, 
        typename group_elem,
        typename ring_plus = def::plus<ring_elem>,
        typename ring_times = def::times<ring_elem>,
        typename group_plus = def::plus<group_elem>>
    concept algebraic_module =
        ring<ring_elem, ring_plus, ring_times> && group<group_elem, group_plus> /*&&
        requires (const group_elem &g, const ring_elem &r) {
            { g * r } -> ImplicitlyConvertible<ring_elem>;
            { r * g } -> ImplicitlyConvertible<ring_elem>;
        }*/;
    
}

#endif 
