#ifndef DATA_MAP_RB
#define DATA_MAP_RB

#include <data/list.hpp>
#include <data/tools/iterator_list.hpp>
#include <data/map.hpp>
#include <data/fold.hpp>
#include <milewski/RBMap/RBMap.h>
    
namespace data {
        
    template <typename K, typename V>
    class rb_map {
        RBMap<K, V> Map;
        using list = linked_list<map::entry<K, V>>;
        constexpr static const data::map::definition::countable<rb_map, K, V, list> require_is_map{};
            
        rb_map(RBMap<K, V> m) : Map{m} {}
            
    public:
        V operator[](K k) const {
            return Map.findWithDefault(V{}, k);
        }
            
        rb_map insert(K k, V v) const {
            return RBMap<K, V>{Map.inserted(k, v)};
        }
            
        rb_map operator+(map::entry<K, V> e) {
            return RBMap<K, V>{Map.inserted(e.Key, e.Value)};
        }
            
        bool contains(K k) const {
            return Map.member(k);
        }
            
        bool empty() const {
            return Map.isEmpty();
        }
            
        rb_map() : Map{} {}
        rb_map(std::initializer_list<std::pair<K, V>> init);
        
        list entries() const;
        
        bool valid() const;
            
    };
    
}

#endif
