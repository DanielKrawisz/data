// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_REPLACE
#define DATA_REPLACE

// Implementations of data structures.
#include <data/list.hpp>
#include <data/tree.hpp>
#include <data/map.hpp>
#include <data/cycle.hpp>
#include <data/cross.hpp>
#include <data/array.hpp>
#include <data/for_each.hpp>

namespace data {

    // we can't use a map because we don't know if X is ordered.
    template <typename X> using replacements = const list<pair<X>>;

    template <typename X>
    stack<X> replace (const stack<X> &, replacements<X>);

    template <typename X>
    list<X> replace (const list<X> &, replacements<X>);

    template <typename X>
    tree<X> replace (const tree<X> &, replacements<X>);

    template <typename X>
    cycle<X> replace (const cycle<X> &,   replacements<X>);

    template <typename K, typename V>
    map<K, V> replace (const map<K, V> &, replacements<V>);

    template <typename X>
    cross<X> replace (const cross<X> &, replacements<X>);

    template <typename X, size_t ...sizes>
    array<X, sizes...> replace (const array<X, sizes...> &, replacements<X>);

    template <typename key, typename value, typename E>
    map<key, value> replace_at (const map<key, value> &m, const key &k, E &&v);

    template <typename X, typename E>
    stack<X> replace_at (const stack<X> &, size_t ind, E &&val);

    template <typename X, typename E>
    list<X> replace_at (const list<X> &, size_t ind, E &&val);

    template <typename X, typename E>
    cross<X> replace_at (const cross<X> &, size_t ind, E &&val);

    template <typename X, size_t ...sizes>
    array<X, sizes...> replace_at (const array<X, sizes...> &, size_t ind, X &&val);

    template <typename key, typename value>
    map<key, value> apply_at (const map<key, value> &m, const key &k, function<value (const value &)> f);

    template <typename X> cross<X> replace (const cross<X> &x, replacements<X> r) {
        cross<X> result (x.size ());
        for (size_t i = 0; i < x.size (); i++) {
            bool replaced = false;
            for (const pair<X> &rr : r) if (rr.first == x[i]) {
                result[i] = rr.second;
                replaced = true;
                break;
            }
            if (!replaced) result[i] = x[i];
        }
        return result;
    }

    template <typename X, size_t ...sizes> array<X, sizes...> replace (const array<X, sizes...> &x, replacements<X> r) {
        array<X, sizes...> result;
        for (size_t i = 0; i < array<X, sizes...>::Size; i++) {
            bool replaced = false;
            for (const pair<X> &rr : r) if (rr.first == x.Values[i]) {
                result.Values[i] = rr.second;
                replaced = true;
                break;
            }
            if (!replaced) result.Values[i] = x.Values[i];
        }
        return result;
    }

    template <typename X> tree<X> replace (const tree<X> &x, replacements<X> r) {
        if (empty (x)) return x;
        for (const pair<X> &rr : r) if (root (x) == rr.first)
            return tree<X> {rr.second, replace (left (x), r), replace (right (x), r)};
        return tree<X> {root (x), replace (left (x), r), replace (right (x), r)};
    }

    template <typename X> stack<X> replace (const stack<X> &x, replacements<X> r) {
        stack<X> result = x;
        for (auto &z : result) for (const pair<X> &rr : r) if (z == rr.first) z = rr.second;
        return result;
    }

    template <typename X> list<X> replace (const list<X> &x, replacements<X> r) {
        list<X> result;
        for (const X &z : x) {
            for (const pair<X> &rr : r) if (z == rr.first) {
                result <<= rr.second;
                goto cont;
            }
            result <<= z;
            cont:
        }
        return result;
    }

    template <typename X> cycle<X> replace (const cycle<X> &x, replacements<X> r) {
        return cycle<X> {replace (x.Cycle, r)};
    }

    template <typename K, typename V> map<K, V> replace (const map<K, V> &x, replacements<V> r) {
        map<K, V> result = x;
        for_each ([&r] (V &v) {
            for (const pair<V> &rr : r) if (v == rr.first) {
                v = rr.second;
                return;
            }
        }, result);
        return result;
    }

    template <typename key, typename value, typename E>
    map<key, value> inline replace_at (const map<key, value> &m, const key &k, E &&v) {
        if (empty (m)) return m;
        using node = RB::colored<data::entry<const key, value>>;
        using tree = linked_tree<node>;
        const node &r = root (static_cast<const tree &> (m));
        if (k == r.Value.Key) return tree {
            node {r.Color, data::entry<const key, value> {k, v}},
            m.left (), m.right ()};
        if (k > r.Value.Key) return tree {r, m.left (), replace_at (m.right (), k, v)};
        else return tree {r, replace_at (m.left (), k, v), m.right ()};
    }

    template <typename key, typename value>
    map<key, value> apply_at (const map<key, value> &m, const key &k, function<value (const value &)> f) {
        if (empty (m)) return m;
        using node = RB::colored<data::entry<const key, value>>;
        using tree = linked_tree<node>;
        const node &r = root (static_cast<const tree &> (m));
        if (k == r.Value.Key) return tree {
            node {r.Color, data::entry<const key, value> {k, f (r.Value.Value)}},
            m.left (), m.right ()};
        if (k > r.Value.Key) return tree {r, m.left (), apply_at (m.right (), k, f)};
        else return tree {r, apply_at (m.left (), k, f), m.right ()};
    }
}

#endif
