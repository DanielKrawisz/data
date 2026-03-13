// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DISPATCH
#define DATA_DISPATCH

#include <data/list.hpp>
#include <data/map.hpp>

namespace data {

    template <typename K, typename V> using dispatch = list<entry<K, V>>;

    template <typename K, typename V>
    map<K, list<V>> dispatch_to_map (dispatch<K, V> d) {
        map<K, list<V>> map;
        for (const auto &[key, val] : d)
            map = map.insert (key, {val}, [] (list<V> old_value, list<V> new_value) {
                return old_value + new_value;
            });
        return map;
    }

}

#endif

