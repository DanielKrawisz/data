#ifndef MILEWSKI_DATA_MILEWSKI_MILEWSKI_HPP
#define MILEWSKI_DATA_MILEWSKI_MILEWSKI_HPP

#include <milewski/rb.hpp>
#include <data/list.hpp>
#include <data/tools/iterator_list.hpp>
#include <data/map.hpp>
    
namespace data {
        
    template <typename K, typename V>
    class rb_map {
        RBMap<K, V> Map;
        constexpr static const ::data::map::definition::map<rb_map, K, V> require_is_map{};
            
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
            
        bool empty() {
            return Map.isEmpty();
        }
            
        rb_map() : Map{} {};
            
        template <typename list>
        rb_map(list l) : Map{list::template reduce(map::insert, l)} {}
            
        rb_map(std::initializer_list<std::pair<K, V> > init);
            
    };
    
}

#endif

