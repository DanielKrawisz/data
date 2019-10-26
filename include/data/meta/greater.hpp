// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META_MINUS
#define DATA_META_MINUS

#include "require.hpp"

namespace data::meta {
        
    // type that can only be constructed left is greater than right. 
    template <uint32 left, uint32 right> struct greater : require<(left > right)> {};
    
}

#endif

