// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP_RB
#define DATA_MAP_RB

#include <data/tools/ordered_list.hpp>
#include <data/functional/map.hpp>
#include <data/fold.hpp>
#include <milewski/RBMap/RBMap.h>
    
namespace data::tool {
    
    template <typename K, typename V>
    struct rb_map {
        using entry = data::entry<K, V>;
        using map = milewski::okasaki::RBMap<K, V>;
    private:
        map Map;
        size_t Size;
        
        rb_map(map m, size_t x) : Map{m}, Size{x} {}
        
    public:
        const V& operator[](const K& k) const;
        bool contains(const K& k) const;
        bool contains(const entry& e) const;
        
        rb_map insert(const K& k, const V& v) const;
        rb_map insert(const entry& e) const;
        
        rb_map operator<<(const entry& e) const;
        
        rb_map remove(const K& k) const;
        rb_map remove(const entry& e) const;
        
        bool valid() const {
            return values().valid();
        }
        
        bool empty() const;
        size_t size() const;
        
        rb_map() : Map{}, Size{0} {}
        rb_map(const entry& e) : rb_map{rb_map{} << e} {}
        rb_map(const K& k, const V& v) : rb_map{entry{k, v}} {}
        
        rb_map(std::initializer_list<std::pair<K, V>> init);
        
        const ordered_list<K> keys() const;
        
        const ordered_list<entry> values() const;
        
        bool operator==(const rb_map& map) const;
        
        bool operator!=(const rb_map& map) const {
            return !(*this == map);
        }
        
        using const_iterator = ordered_list<entry>::const_iterator;
        
        const_iterator begin() const;
        const_iterator end() const;
        
    };
    
    template <typename K, typename V>
    inline std::ostream& operator<<(std::ostream& o, const rb_map<K, V>& x) {
        return functional::write(o << "map", x.values());
    }
    
    template <typename K, typename V>
    inline rb_map<K, V>::rb_map(std::initializer_list<std::pair<K, V> > init) : Map{}, Size{0} {
        for (auto p : init) *this = insert(p.first, p.second);
    }
    
    template <typename K, typename V>
    bool rb_map<K, V>::operator==(const rb_map& map) const {
        if (this == &map) return true;
        if (size() != map.size()) return false;
        auto a = map.begin();
        for (auto b = begin(); b != end(); ++b) {
            if (*a != *b) return false;
            ++a;
        }
        return true;
    }
    
    template <typename K, typename V>
    const ordered_list<K> rb_map<K, V>::keys() const {
        linked_stack<K> kk{};
        milewski::okasaki::forEach(Map, [&kk](const K& k, V) -> void {
            kk = kk << k;
        });
        ordered_list<K> x{};
        for (const auto& k : data::reverse(kk)) x = x << k;
        return x;
    }
    
    template <typename K, typename V>
    const ordered_list<entry<K, V>> rb_map<K, V>::values() const {
        linked_stack<entry> kk{};
        milewski::okasaki::forEach(Map, [&kk](const K& k, V v) -> void {
            kk = kk << entry{k, v};
        });
        ordered_list<entry> x{};
        for (const auto& e : data::reverse(kk)) x = x << e;
        return x;
    }
    
    template <typename K, typename V>
    inline const V& rb_map<K, V>::operator[](const K& k) const {
        static V Default{};
        return Map.findWithDefault(Default, k);
    }
    
    template <typename K, typename V>
    inline bool rb_map<K, V>::contains(const K& k) const {
        return Map.member(k);
    }
    
    template <typename K, typename V>
    inline bool rb_map<K, V>::contains(const entry& e) const {
        return operator[](e.Key) == e.Value;
    }
    
    template <typename K, typename V>
    inline rb_map<K, V> rb_map<K, V>::insert(const K& k, const V& v) const {
        V already = operator[](k);
        if (already == V{}) return rb_map{Map.inserted(k, v), Size + 1};
        if (already == v) return *this;
        rb_map removed = this->remove(k);
        return rb_map{removed.Map.inserted(k, v), removed.Size + 1};
    }
    
    template <typename K, typename V>
    inline rb_map<K, V> rb_map<K, V>::insert(const entry& e) const {
        return insert(e.Key, e.Value);
    }
    
    // there's a more efficient way of doing this. 
    // See matt.might.net/articles/red-black-delete/ 
    template <typename K, typename V>
    inline rb_map<K, V> rb_map<K, V>::remove(const K& k) const {
        rb_map m{};
        for (auto x = begin(); x != end(); ++x) if ((*x).Key != k) m = m.insert(*x);
        return m;
    }
    
    template <typename K, typename V>
    inline rb_map<K, V> rb_map<K, V>::remove(const entry& e) const {
        return operator[](e.Key) == e.Value ? remove(e.Key) : *this;
    }
    
    template <typename K, typename V>
    inline rb_map<K, V> rb_map<K, V>::operator<<(const entry& e) const {
        return insert(e.Key, e.Value);
    }
    
    template <typename K, typename V>
    inline bool rb_map<K, V>::empty() const {
        return Map.isEmpty();
    }
    
    template <typename K, typename V>
    inline size_t rb_map<K, V>::size() const {
        return Size;
    }
    
    template <typename K, typename V>
    inline rb_map<K, V>::const_iterator rb_map<K, V>::begin() const {
        return values().begin();
    } 
    
    template <typename K, typename V>
    inline typename rb_map<K, V>::const_iterator rb_map<K, V>::end() const {
        return values().end();
    }
    
}

#endif
