// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META_REQUIRE
#define DATA_META_REQUIRE

#include <data/types.hpp>

namespace data::meta {
        
    // type that can only be constructed if the assertian is true. 
    template <bool value> struct require;
    
    template <> struct require<true> {};
    
}

#endif

        
