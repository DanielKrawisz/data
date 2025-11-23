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

    template <typename L>
    L sort (const L &x) {
        // TODO we need to uncomment that, but we have a problem because if a
        // type does not have a ::first method then there is a compilation error.
        if constexpr (Pendable<L>/* && Ordered<decltype (std::declval<L> ().first ())>*/) {
            return merge_sort (x);
        } else if constexpr (Iterable<L>) {
            auto z = x;
            std::sort (z.begin (), z.end ());
            return z;
        }
    }

    template <typename L>
    bool sorted (const L &x) {
        if constexpr (Sequence<L>/* && Ordered<decltype (std::declval<L> ().first ())>*/) {
            return data::size (x) < 2 ? true : first (x) <= first (rest (x)) && sorted (rest (x));
        } else if constexpr (Iterable<L>) {
            return std::is_sorted (x.begin (), x.end ());
        }
    }

}

#endif

