// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EMPTY
#define DATA_EMPTY

#include "list.hpp"

namespace data {

    template <typename X>
    inline bool empty(const X x) {
        return container::empty(x);
    }

    template <typename X>
    inline typename list::is_list<X>::returned first(const X x) {
        return list::first(x);
    }

    template <typename X>
    inline X rest(const X x) {
        return list::rest(x);
    }

    template <typename X>
    inline X reverse(const X x) {
        return list::reverse(x);
    }

}

#endif
