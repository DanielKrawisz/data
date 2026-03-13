// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_PRIORITY_QUEUE
#define DATA_PRIORITY_QUEUE

#include <data/tools/priority_queue.hpp>
#include <data/tree.hpp>

namespace data {

    // priority queue.
    template <typename X> using priority_queue = tool::priority_queue<tree<X>, X>;

}

#endif
