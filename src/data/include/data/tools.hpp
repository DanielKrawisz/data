// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS
#define DATA_TOOLS

// basic types
#include <data/types.hpp>

// This library uses a valid () method and a Valid member in a stereotyped way.
#include <data/valid.hpp>

#include <data/maybe.hpp>

// Interfaces related to data structures. 
#include <data/functional/set.hpp>
#include <data/functional/list.hpp>
#include <data/functional/map.hpp>
#include <data/functional/tree.hpp>

#include <data/reverse.hpp>
#include <data/take.hpp>
#include <data/sort.hpp>

// A implementations of data structures. 
#include <data/tools/linked_stack.hpp>
#include <data/tools/rb.hpp>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_tree.hpp>
#include <data/tools/binary_search_tree.hpp>
#include <data/tools/map_set.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/tools/ordered_list.hpp>
#include <data/tools/cycle.hpp>

#include <data/bytes.hpp>
#include <data/fold.hpp>

namespace data {
    
    template <typename X> using stack = linked_stack<X>;
    
    // functional queue built using the list. 
    template <typename X> using list = functional_queue<stack<X>>;
    
    template <typename X> using cycle = tool::cycle<list<X>, X>;
    
    // tree. 
    template <typename X> using tree = linked_tree<X>;
    
    // a functional map implemented as a red-black tree
    template <typename K, typename V> using map = binary_search_map<K, V,
        RB::tree<data::entry<const K, V>,
            tree<RB::colored<data::entry<const K, V>>>>>;
    
    // set implemented as a map. 
    template <typename X> using set = RB::tree<X, tree<RB::colored<X>>>;
    
    // priority queue. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using priority_queue = tool::priority_queue<tree<X>, X>;
    
    // ordered_list. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using ordered_list = tool::ordered_stack<stack<X>>;

    template <typename K, typename V> using dispatch = list<entry<K, V>>;

    template <typename K, typename V>
    map<K, list<V>> dispatch_to_map (dispatch<K, V> d) {
        map<K, list<V>> map;
        for (const auto &[key, val] : d)
            map = map.insert (key, {val}, [] (list<V> old_value, list<V> new_value) {
                return old_value + new_value;
            });
        map;
    }
    
    // Take a function fun and some lists {x, ...}, {y, ...}, {z, ...} ... and return {f (x, y, z, ...), ...}
    template <typename f, sequence... Vals, typename f_result = decltype (std::declval<f> () (data::first (std::declval<Vals> ())...))>
    auto map_thread (f fun, Vals... lists) -> stack<f_result>;

    template <typename map, typename key, typename value>
    map replace_part (map X, const key &k, const value &v);

    template <typename elem>
    stack<elem> reverse (const ordered_list<elem> x);

    template <typename elem>
    ordered_list<elem> select (const ordered_list<elem> x, function<bool (const elem &)> satisfies);

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

    template <typename f, sequence... Vals, typename f_result>
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
    stack<elem> reverse (const ordered_list<elem> x) {
        stack<elem> n;
        for (const elem &e : x) n <<= e;
        return n;
    }

    template <typename elem>
    ordered_list<elem> select (const ordered_list<elem> x, function<bool (const elem &)> satisfies) {
        stack<const elem &> n;
        for (const elem &e : x) if (satisfies (e)) n <<= e;
        ordered_list<elem> r;
        for (const elem &e : n) r = r.insert (e);
        return r;
    }

    // lazy writer can be used without knowing the size
    // of the data to be written beforehand.
    template <std::integral word> struct lazy_writer : message_writer<bytestring<word>, word> {
        list<bytestring<word>> Bytes;

        void write (const word* b, size_t size) final override {
            Bytes = Bytes << bytestring<word> (view<word> {b, size});
        }

        operator bytestring<word> () const {
            size_t size = 0;
            for (const bytestring<word> &b : Bytes) size += b.size ();
            bytestring<word> z (size);
            iterator_writer w {z.begin (), z.end ()};
            for (const bytestring<word> &b : Bytes) w << b;
            return z;
        }

        bytestring<word> complete () final override {
            return bytestring<word> (*this);
        }
    };

    using lazy_bytes_writer = lazy_writer<byte>;

}

#endif
