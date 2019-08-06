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
#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/math/number/rational.hpp>
#include <data/crypto/ripemd160.hpp>
#include <data/crypto/sha256.hpp>
#include <data/crypto/sha512.hpp>
#include <data/crypto/secp256k1.hpp>

// using namespace data::exported;
// to get all complete features of this library conveniently. 
namespace data::exported {
    
    // a functional list. 
    template <typename X> using list = data::list::linked<X>;
    
    // functional queue built using the list. 
    template <typename X> using queue = data::functional_queue<list<X>>;
    
    // a functional map implemented as a red-black tree
    template <typename K, typename V> using map = data::rb_map<K, V>;
    
    // set implemented as a map. 
    template <typename X> using set = data::map_set<map<X, bool>, X>;
    
    // Wrapper for a vector that allows for easy creation of subslices, similar to golang.  
    template <typename X> using slice = data::slice<X>;
    
    // Natural numbers.
    using N = math::number::gmp::N;
    
    // Integers. 
    using Z = math::number::gmp::Z;
    
    // Rationals. 
    using Q = math::number::fraction<Z, N>;
    
    // get all values from a map with the given keys. 
    template <typename key, typename value, typename map>
    list<value> get_all(map m, list<key> k);
}

#endif
