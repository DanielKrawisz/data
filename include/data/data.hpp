// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

#include <data/math/number/bounded.hpp>
#include <data/list/linked.hpp>
#include <data/map/rb.hpp>
#include <data/tools/map_set.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/tree/linked_tree.hpp>
#include <data/slice.hpp>
#include <data/fold.hpp>
#include <data/for_each.hpp>
#include <data/crypto/ripemd160.hpp>
#include <data/crypto/sha256.hpp>
#include <data/crypto/sha512.hpp>
#include <data/crypto/secp256k1.hpp>

namespace data::exported {
    
    template <typename X>
    using list = data::list::linked<X>;
            
    template <typename X>
    using slice = data::slice<X>;
        
    template <typename K, typename V>
    using map = data::rb_map<K, V>;
    
    template <typename X>
    using set = data::map_set<map<X, bool>, X>;
    
    template <typename X>
    using queue = data::functional_queue<list<X>>;
        
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
