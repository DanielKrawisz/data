// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <data/stack.hpp>

namespace data::interface {
    
    template <typename Q>
    struct list : stack<Q> {
        using element = typename std::enable_if<
            meta::has_append_method<Q, typename sequence<Q>::element>::value, typename sequence<Q>::element>::type;
    }; 
}

#endif

