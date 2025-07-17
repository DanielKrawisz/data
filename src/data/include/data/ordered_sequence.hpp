// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ORDERED_SEQUENCE
#define DATA_ORDERED_SEQUENCE

#include <data/stack.hpp>

// implementation of List
#include <data/tools/ordered_list.hpp>

namespace data {

    // functional queue built using the list. 
    template <typename X> using ordered_sequence = tool::ordered_stack<stack<X>, X>;

    template <typename elem>
    stack<elem> reverse (const ordered_sequence<elem> x);

    template <typename elem>
    stack<elem> reverse (const ordered_sequence<elem> x) {
        stack<elem> n;
        for (const elem &e : x) n >>= e;
        return n;
    }
    
}

#endif
