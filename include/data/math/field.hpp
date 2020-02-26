// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/math/ring.hpp>
#include <data/math/nonnegative.hpp>

namespace data::interface {
    
    // TODO division algebra. 
    
    template <typename elem, typename plus, typename times>
    struct field : ring<elem, plus, times>, group<math::nonzero<elem>, times> {};
    
}

#endif
