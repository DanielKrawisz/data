// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META_UNSIGNED_MINUS
#define DATA_META_UNSIGNED_MINUS

#include <data/types.hpp>

namespace data::meta {
    
    template <uint32 left, uint32 right> struct unsigned_minus {
        constexpr static uint32 result = left > right ? left - right : 0;
    };
    
}

#endif

        
