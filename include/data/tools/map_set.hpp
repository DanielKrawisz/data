// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MAP_SET
#define DATA_TOOLS_MAP_SET

#include <data/list/linked.hpp>
#include <data/map.hpp>
    
namespace data::tool {
    
    // turn any map into a set. 
    template <typename M>
    struct map_set {
        using key = typename interface::map<M>::key;
        using value  = typename interface::map<M>::value;
        
        M Map;
        
        bool empty() const {
            return Map.empty();
        }
        
        bool contains(const key& k) const {
            return Map.contains(k);
        }
        
        size_t size() const {
            return Map.size();
        }
        
        map_set insert(const key& k) const {
            return map_set{Map.insert(k, value{})};
        }
        
        map_set remove(const key& k) const {
            return map_set{Map.remove(k, value{})};
        }
        
        map_set insert(functional::stack::linked<key> keys) const {
            if (keys.empty()) return *this;
            return add(keys.first()).add(keys.rest());
        }
        
        map_set operator<<(const key& k) const {
            return insert(k);
        }
        
        functional::stack::linked<const key&> values() const {
            return Map.keys();
        }
        
        map_set() : Map{} {}
        map_set(M m) : Map(m) {}
        map_set(functional::stack::linked<key> keys) : Map{} {
            insert(keys);
        }
    };

}

#endif
