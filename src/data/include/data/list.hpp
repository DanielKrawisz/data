// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <data/stack.hpp>

// implementation of List
#include <data/tools/functional_queue.hpp>

namespace data {

    // functional queue built using the list. 
    template <typename X> using list = functional_queue<data::stack<X>, X>;

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

}

#endif
