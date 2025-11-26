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

    template <typename K, typename V, typename E>
    requires std::equality_comparable_with<K, E>
    decltype (auto) inline contains (const map<K, V> &m, E &&v) {
        return m.contains (v);
    }

    template <typename K, typename V>
    size_t size (const map<K, V> &);

    template <typename K, typename V, typename E>
    map<K, V> insert (const map<K, V> &m, const K &k, E &&v);

    template <typename K, typename V, typename F>
    map<K, V> select (const map<K, V> &, F &&fun);

    template <typename K, typename V, typename E>
    map<K, V> erase (const map<K, V> &, E &&v);

    template <typename K, typename V>
    size_t inline size (const map<K, V> &m) {
        return m.size ();
    }

    template <typename K, typename V, typename F>
    map<K, V> select (const map<K, V> &m, F &&fun) {
        map<K, V> selected;
        for (const auto &[key, value]: m) if (fun (value)) selected = selected.insert (key, value);
        return selected;
    }

    template <typename K, typename V, typename E>
    map<K, V> inline erase (const map<K, V> &m, E &&e) {
        return select (m, [&e] (const V &v) -> bool {
            return e != v;
        });
    }

    template <typename K, typename V, typename E>
    map<K, V> inline insert (const map<K, V> &m, const K &k, E &&v) {
        return m.insert (k, v);
    }

}

#endif
