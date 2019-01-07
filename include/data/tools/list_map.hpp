#ifndef DATA_TOOLS_LIST_MAP_HPP
#define DATA_TOOLS_LIST_MAP_HPP

#include <data/list.hpp>
#include <data/map.hpp>
    
namespace data {
        
    // any list type can be turned into a map type. 
    template <typename key, typename value, typename list, typename it>
    struct list_map {
        list List;
            
        // proof that list map is a map. 
        constexpr static const map::definition::countable<list_map<key, value, list, it>, key, value, list> requirement1{};
        constexpr static const map::definition::removable<list_map<key, value, list, it>, key> requirement2{};
            
        bool empty() const {
            return list::empty(List);
        }
            
        data::map::entry<key, value>& first() const {
            return list::first(List);
        }

        list_map rest() {
            return list_map{list::rest(List)};
        }
            
        value get(key k) const {
            if (empty()) return value{};
                
            value v = first()[k];
                
            if (v != value{}) return v;
                
            return list_map{rest(List)}[k];
        }
            
        value operator[](key k) const {
            return get(k);
        }
            
        bool contains(key k) const {
            return get(k) != value{};
        }
            
        it begin() const {
            return std::begin(List);
        }
            
        it end() const {
            return std::end(List);
        }
            
        list entries() const {
            return List;
        }
            
        list_map(list l) : List(l) {}
        list_map() : List{} {}
    };

}

#endif
