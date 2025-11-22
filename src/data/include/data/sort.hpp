// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SORT
#define DATA_SORT

#include <data/functional/list.hpp>
#include <data/iterable.hpp>

namespace data {

    template <Pendable list> requires Ordered<decltype (std::declval<list> ().first ())>
    list merge_sort (const list &x) {
        size_t z = size (x);
        if (z < 2) return x;

        size_t half = z / 2;
        return functional::merge (merge_sort (take (x, half)), merge_sort (drop (x, half)));
    }

    template <Pendable L> requires Ordered<decltype (std::declval<L> ().first ())>
    L inline sort (const L &x) {
        return merge_sort (x);
    }

    template <Iterable X>
    X sort (const X &x) {
        auto z = x;
        std::sort (z.begin (), z.end ());
        return z;
    }

    template <Iterable X>
    bool inline sorted (const X &x) {
        return std::is_sorted (x.begin (), x.end ());
    }

}

#endif

