// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_QUEUE
#define DATA_QUEUE

#include <data/list.hpp>
#include <data/iterable.hpp>

namespace data::interface {
    
    template <typename Q>
    struct queue : list<Q> {
        using element = typename std::enable_if<
            meta::has_append_method<Q, typename sequence<Q>::element>::value, typename sequence<Q>::element>::type;
    }; 
}

#endif

