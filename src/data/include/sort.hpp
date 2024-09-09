// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SORT
#define DATA_SORT

#include <data/tools.hpp>

namespace data {

    template <functional::pendable L> requires ordered<element_of<L>>
    L inline sort (const L &x) {
        return merge_sort (x);
    }

    template <iterable X> 
    X sort (const X &x) {
        auto z = x;
        std::sort (z.begin (), z.end ());
        return z;
    }

    template <iterable X> 
    bool inline sorted (const X &x) {
        return std::is_sorted (x.begin (), x.end ());
    }

}

#endif

