// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP
#define DATA_MAP

#include <data/tools/rb.hpp>
#include <data/tools/binary_search_tree.hpp>
#include <data/tree.hpp>

namespace data {

    // a functional map implemented as a red-black tree
    template <typename K, typename V> using map = binary_search_map<K, V,
        RB::tree<data::entry<const K, V>,
            tree<RB::colored<data::entry<const K, V>>>>>;

    template <typename K, typename V, Proposition<V> F>
    map<K, V> select (map<K, V>, F fun);

    template <typename key, typename value>
    map<key, value> replace_part (map<key, value> m, const key &k, const value &v);

    template <typename K, typename V, Proposition<V> F>
    map<K, V> select (map<K, V> m, F fun) {
        map<K, V> selected;
        for (const auto &[key, value]: m) if (fun (value)) selected = selected.insert (key, value);
        return selected;
    }

    template <typename key, typename value>
    map<key, value> inline replace_part (map<key, value> m, const key &k, const value &v) {
        return m.replace_part (k, v);
    }

}

#endif
