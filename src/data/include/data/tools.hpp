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
#include <data/map.hpp>
#include <data/dispatch.hpp>
#include <data/set.hpp>
#include <data/priority_queue.hpp>

#include <data/tools/map_set.hpp>
#include <data/tools/priority_queue.hpp>

#include <data/fold.hpp>

namespace data {

    template <typename X> using replacements = list<pair<X>>;

    template <typename X> stack<X> replace (stack<X>, replacements<X>);
    template <typename X> list<X> replace (list<X>, replacements<X>);
    template <typename X> tree<X> replace (tree<X>, replacements<X>);
    template <typename K, typename V> map<K, V> replace (map<K, V>, replacements<V>);
    
    // Take a function fun and some lists {x, ...}, {y, ...}, {z, ...} ... and return {f (x, y, z, ...), ...}
    template <typename f, Sequence... Vals, typename f_result = decltype (std::declval<f> () (first (std::declval<Vals> ())...))>
    auto map_thread (f fun, Vals... lists) -> stack<f_result>;

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

}

#endif
