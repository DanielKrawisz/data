// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE
#define DATA_TREE

#include <data/tools/linked_tree.hpp>

namespace data {

    template <typename X> using tree = linked_tree<X>;

    template <typename X, typename E>
    bool contains (const tree<X> &, const E &);

}

#endif
