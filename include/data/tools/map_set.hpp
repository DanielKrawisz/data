// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MAP_SET
#define DATA_TOOLS_MAP_SET

#include <data/list/linked.hpp>
#include <data/map.hpp>
    
namespace data {
        
    // turn any map into a set. 
    template <typename M>
    struct map_set {
        using key = typename interface::map<M>::key;
        using value  = typename interface::map<M>::value;
        using entry = data::entry<key, value>;
        
        M Map;
        
        bool empty() const {
            return Map.empty();
        }
        
        bool contains(key k) const {
            return Map.contains(k);
        }
        
        uint32 size() const {
            return Map.size();
        }
        
        map_set insert(key k) const {
            return map_set{Map.insert(k, true)};
        }
        
        map_set insert(list::linked<key> keys) const {
            if (keys.empty()) return *this;
            return add(keys.first()).add(keys.rest());
        }
        
        map_set operator<<(key k) const {
            return insert(k);
        }
        
        list::linked<key> keys() const {
            return Map.keys();
        }
        
        list::linked<entry> values() const {
            return Map.values();
        }
        
        map_set() : Map{} {}
        map_set(M m) : Map(m) {}
        map_set(list::linked<key> keys) : Map{} {
            insert(keys);
        }
    };

}

#endif
