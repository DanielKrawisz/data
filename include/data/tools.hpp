// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS
#define DATA_TOOLS

// basic types
#include <data/types.hpp>

// This library uses a valid() method and a Valid member in a stereotyped way. 
#include <data/valid.hpp>

// Interfaces related to data structures. 
#include <data/interface.hpp> 
#include <data/set.hpp>
#include <data/stack.hpp>
#include <data/map.hpp>
#include <data/tree.hpp>

// A implementations of data structures. 
#include <data/tools/linked_stack.hpp>
#include <data/tools/rb_map.hpp>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_tree.hpp>
#include <data/tools/map_set.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/tools/ordered_list.hpp>
#include <data/tools/cycle.hpp>

#include <data/cross.hpp>

#include <data/fold.hpp>

namespace data {
    
    template <typename X> using stack = tool::linked_stack<X>;
    
    // functional queue built using the list. 
    template <typename X> using list = tool::functional_queue<stack<X>>;
    
    // tree. 
    template <typename X> using tree = tool::linked_tree<X>;
    
    // a functional map implemented as a red-black tree
    // wrapper of Milewski's implementation of Okasaki.
    template <typename K, typename V> using map = tool::rb_map<K, V>;
    
    // set implemented as a map. 
    template <typename X> using set = tool::map_set<map<X, tool::unit>>;
    
    // priority queue. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using priority_queue = tool::priority_queue<X, stack<X>>;
    
    // ordered_list. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using ordered_list = tool::ordered_list<X>;
    
    // get all values from a map with the given keys. 
    template <typename key, typename value, typename map>
    list<value> get_all(map m, list<key> k) {
        struct inner {
            map M;
                    
            inner(map m) : M{m} {}
                    
            list<value> operator()(key k, list<value> l) {
                value v = M[k];
                if (v == value{}) return l;
                return l + v;
            }
        };
                
        return reduce(inner{m}, k);
    }

}

#endif
