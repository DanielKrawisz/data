#ifndef DATA_TOOLS_MAP_SET_HPP
#define DATA_TOOLS_MAP_SET_HPP

#include <data/list.hpp>
#include <data/map.hpp>
    
namespace data {
        
    // turn any map into a set. 
    template <typename M, typename key>
    struct map_set {
        M Map;
            
        // proof that map_set is a set. 
        static const set::definition::insertable<map_set<key, key>, key> r1{};
        static const map::definition::map<M, key, bool> r2{};
            
        bool empty() const {
            return map::empty(Map);
        }
            
        bool contains(key k) const {
            return map::contains(Map, k);
        }
        
        map_set add(key k) const {
            return map_set{map::insert(Map, k, true)};
        }
            
        map_set operator+(key k) const {
            return add(k);
        }
            
        map_set(M m) : Map(m) {}
        map_set() : Map{} {}
    };

}

#endif
