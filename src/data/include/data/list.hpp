// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <data/stack.hpp>

// implementation of List
#include <data/tools/functional_queue.hpp>

namespace data {

    // functional queue built using the list. 
    template <typename X> using list = functional_queue<data::stack<X>, X>;
}

#endif
