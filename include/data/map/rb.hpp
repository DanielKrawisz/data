// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP_RB
#define DATA_MAP_RB

#include <data/list/linked.hpp>
#include <data/map.hpp>
#include <data/fold.hpp>
#include <milewski/RBMap/RBMap.h>
#include <data/io/unimplemented.hpp>
    
namespace data {
    
    template <typename K, typename V>
    struct rb_map {
        using entry = data::entry<const K, const V>;
        using map = milewski::okasaki::RBMap<const K, const V>;
    private:
        map Map;
        uint32 Size;
        
        rb_map(map m, uint32 x) : Map{m}, Size{x} {}
        
    public:
        const V& operator[](const K& k) const {
            return Map.findWithDefault(V{}, k);
        }
        
        bool contains(const K& k) const {
            return Map.member(k);
        }
        
        bool contains(const entry& e) const {
            return operator[](e.Key) == e.Value;
        }
        
        rb_map insert(const K& k, const V& v) const {
            return contains(k) ? *this : rb_map{Map.inserted(k, v), Size + 1};
        }
        
        rb_map insert(const entry& e) const {
            return insert(e.Key, e.Value);
        }
        
        rb_map remove(const K& k) const {
            throw method::unimplemented{"rb_map::remove"};
        }
        
        rb_map remove(const entry& e) const {
            return operator[](e.Key) == e.Value ? remove(e.Key) : *this;
        }
        
        rb_map operator<<(const entry& e) {
            return Map.inserted(e.Key, e.Value);
        }
        
        bool empty() const {
            return Map.isEmpty();
        }
        
        uint32 size() const {
            return Size;
        }
        
        rb_map() : Map{} {}
        rb_map(std::initializer_list<std::pair<K, V>> init);
        
        list::linked<const K&> keys() const {
            list::linked<const K&> kk{};
            milewski::okasaki::forEach(Map, [&kk](const K& k, V)->void{
                kk = kk << k;
            });
            return kk;
        }
        
        list::linked<entry> values() const {
            throw method::unimplemented{"rb_map::values"};
        }
        
        bool valid() const;
        
    };
    
    template <typename K, typename V>
    inline rb_map<K, V> insert_rb_map(rb_map<K, V> m, entry<K, V> e) {
        m.insert(e);
    }
    
    template <typename K, typename V>
    inline rb_map<K, V>::rb_map(std::initializer_list<std::pair<K, V> > init)
        : Map{reduce(insert_rb_map<K, V>, make_iterator_list(init))} {}
    
}

#endif
