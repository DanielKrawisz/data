// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS
#define DATA_TOOLS

#include <data/maybe.hpp>

// Interfaces related to data structures. 
#include <data/functional/set.hpp>
#include <data/functional/map.hpp>
#include <data/functional/tree.hpp>

// Implementations of data structures. 
#include <data/list.hpp>
#include <data/tree.hpp>
#include <data/ordered_sequence.hpp>

#include <data/tools/cycle.hpp>
#include <data/tools/rb.hpp>
#include <data/tools/binary_search_tree.hpp>
#include <data/tools/map_set.hpp>
#include <data/tools/priority_queue.hpp>

#include <data/fold.hpp>

namespace data {
    
    template <typename X> using cycle = tool::cycle<list<X>, X>;
    
    // a functional map implemented as a red-black tree
    template <typename K, typename V> using map = binary_search_map<K, V,
        RB::tree<data::entry<const K, V>,
            tree<RB::colored<data::entry<const K, V>>>>>;
    
    // set implemented as a map. 
    template <typename X> using set = RB::tree<X, tree<RB::colored<X>>>;

    set<int> get_numbers ();
    
    template <typename X> set<X> merge (set<X>, set<X>);
    template <typename X> set<X> remove (set<X>, const X &);
    
    // priority queue. 
    template <typename X> using priority_queue = tool::priority_queue<tree<X>, X>;

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

    template <typename X, Proposition<X> F>
    ordered_sequence<X> select (const ordered_sequence<X> &, F fun);

    template <typename X, Proposition<X> F>
    priority_queue<X> select (const priority_queue<X> &, F fun);

    template <typename K, typename V, Proposition<V> F>
    map<K, V> select (map<K, V>, F fun);

    template <typename X> using replacements = list<pair<X>>;

    template <typename X> stack<X> replace (stack<X>, replacements<X>);
    template <typename X> list<X> replace (list<X>, replacements<X>);
    template <typename X> tree<X> replace (tree<X>, replacements<X>);
    template <typename K, typename V> map<K, V> replace (map<K, V>, replacements<V>);
    
    // Take a function fun and some lists {x, ...}, {y, ...}, {z, ...} ... and return {f (x, y, z, ...), ...}
    template <typename f, Sequence... Vals, typename f_result = decltype (std::declval<f> () (first (std::declval<Vals> ())...))>
    auto map_thread (f fun, Vals... lists) -> stack<f_result>;

    template <typename map, typename key, typename value>
    map replace_part (map X, const key &k, const value &v);

    template <typename elem>
    ordered_sequence<elem> select (const ordered_sequence<elem> x, function<bool (const elem &)> satisfies);

    namespace {
        template <typename given> struct flat {
            using type = given;
            type operator () (given g) {
                return g;
            }
        };

        template <typename given> struct flat<list<list<given>>> {
            using type = flat<list<given>>::type;
            type operator () (list<list<given>> g) {
                return fold ([] (type t, list<given> g) -> type {
                    return t + flat<list<given>> {} (g);
                }, type {}, g);
            }
        };
    }

    template <typename given> using flattened = flat<given>::type;

    template <typename given> flattened<given> inline flatten (given g) {
        return flat<given> {} (g);
    }

    template <typename f, Sequence... Vals, typename f_result>
    auto map_thread (f fun, Vals... lists) -> stack<f_result> {
        if constexpr (sizeof... (lists) > 0) {
            if (!(data::size (lists) == ...)) throw exception {} << "map thread provided lists of different sizes";
            function<stack<f_result> (f, Vals...)> inner = [&inner] (f fun, Vals... lists) {
                if ((data::empty (lists) && ...)) return stack<f_result> {};
                f_result x = fun (data::first (lists)...);
                return data::prepend (inner (fun, data::rest (lists)...), x);
            };
            return inner (fun, lists...);
        } else throw exception {} << "map thread with no inputs";
    }

    template <typename map, typename key, typename value>
    map inline replace_part (map m, const key &k, const value &v) {
        return m.replace_part (k, v);
    }

    template <typename elem>
    ordered_sequence<elem> select (const ordered_sequence<elem> x, function<bool (const elem &)> satisfies) {
        stack<const elem &> n;
        for (const elem &e : x) if (satisfies (e)) n >>= e;
        ordered_sequence<elem> r;
        for (const elem &e : n) r = r.insert (e);
        return r;
    }

}

#endif
