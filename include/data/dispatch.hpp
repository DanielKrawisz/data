// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DISPATCH
#define DATA_DISPATCH

#include <data/list.hpp>
#include <data/map.hpp>
#include <data/set.hpp>

namespace data {

    template <typename K, typename V> using dispatch = list<entry<const K, V>>;

    // get all values for a given key
    template <typename K, typename V>
    list<const V &> get_values (const dispatch<K, V>, const K &);

    // expect only one value; if more than one is present, return nothing.
    template <typename K, typename V>
    maybe<const V &> get_value (const dispatch<K, V>, const K &);

    template <typename K, typename V>
    map<K, list<const V &>> to_map (const dispatch<K, V> d);

    template <typename K, typename V>
    set<const K &> get_keys (const dispatch<K, V>);

    // expect only one value; if more than one is present, return nothing.
    template <typename K, typename V>
    maybe<const V &> get_value (const dispatch<K, V> d, const K &k) {
        list<const V &> vals = get_values (d, k);
        if (vals.size () == 1) return vals[0];
        return {};
    }

    // get all values
    template <typename K, typename V>
    list<const V &> get_values (const dispatch<K, V> d, const K &k) {
        list<const V &> v;
        for (const auto &[key, value]: d) if (key == k) v <<= value;
        return v;
    }

    template <typename K, typename V>
    map<K, list<const V &>> to_map (const dispatch<K, V> d) {
        map<K, list<const V &>> map;
        for (const auto &[key, val] : d)
            map = map.insert (key, {val}, [] (list<const V &> old_value, list<const V &> new_value) {
                return old_value + new_value;
            });
        return map;
    }

    template <typename K, typename V>
    set<const K &> get_keys (const dispatch<K, V> d) {
        set<const K &> keys {};
        for (const auto &[key, _] : d) keys = keys.insert (key);
        return keys;
    }

}

#endif

