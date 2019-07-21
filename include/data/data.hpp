// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

#include <data/types.hpp>

namespace data {
    
    template <typename X, typename Y>
    inline X plus(X a, Y b) {
        return a + b;
    }
    
    template <typename Any>
    bool valid(Any);
    
    template <typename List>
    List reverse(List);

}

#endif
