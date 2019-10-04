// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MAP_SET
#define DATA_TOOLS_MAP_SET

#include <data/list/linked.hpp>
#include <data/map.hpp>
    
namespace data {
        
    // turn any map into a set. 
    template <typename M, typename key>
    struct map_set {
        M Map;
            
        // proof that map_set is a set. 
        constexpr static const set::definition::insertable<map_set<key, key>, key> r1{};
        constexpr static const map::definition::map<M, key, bool> r2{};
            
        bool empty() const {
            return map::empty(Map);
        }
            
        bool contains(key k) const {
            return map::contains(Map, k);
        }
        
        map_set add(key k) const {
            return map_set{map::insert(Map, k, true)};
        }
        
        map_set add(list::linked<key> keys) const {
            if (keys.empty()) return *this;
            return add(keys.first()).add(keys.rest());
        }
            
        map_set operator+(key k) const {
            return add(k);
        }
        
        list::linked<key> entries() const {
            return Map.keys();
        }
            
        map_set(M m) : Map(m) {}
        map_set() : Map{} {}
        map_set(list::linked<key> keys) : Map{} {
            add(keys);
        }
    };

}

#endif
