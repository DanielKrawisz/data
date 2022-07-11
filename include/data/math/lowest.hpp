// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LOWEST
#define DATA_MATH_LOWEST

#include <data/math/ordered.hpp>

namespace data::math {
    
    template <typename> struct lowest;
    
    template <std::unsigned_integral elem> struct lowest<elem> {
        elem operator()() {
            return 0;
        }
    };
    
    template <typename elem> concept bounded_from_below = ordered<elem> && requires() {
        {lowest<elem>{}()} -> std::same_as<elem>;
    };
    
}

#endif

