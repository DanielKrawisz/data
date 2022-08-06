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
        const V &operator[](const K& k) const;
        V *contains(const K& k);
        const V *contains(const K& k) const;
        bool contains(const entry& e) const;
        
        const V &root() const;
        const rb_map &left() const;
        const rb_map &right() const;
        
        rb_map insert(const K& k, const V& v) const;
        rb_map insert(const entry& e) const;
        
        rb_map operator<<(const entry& e) const;
        
        rb_map remove(const K& k) const;
        rb_map remove(const entry& e) const;
        
        bool valid() const {
            try {
                milewski::okasaki::forEach(Map, [](const K &k, const V &v) -> void {
                    if (!data::valid(k) || !data::valid(v)) throw 0;
                });
            } catch (int) {
                return false;
            }
            return true;
        }
        
        bool empty() const;
        size_t size() const;
        
        rb_map() : Map{}, Size{0} {}
        rb_map(const entry& e) : rb_map{rb_map{} << e} {}
        rb_map(const K& k, const V& v) : rb_map{entry{k, v}} {}
        
        rb_map(std::initializer_list<std::pair<K, V>> init);
        
        const ordered_stack<linked_stack<K>> keys() const;
        
        const ordered_stack<linked_stack<entry>> values() const;
        
        bool operator==(const rb_map& map) const;
        
        bool operator!=(const rb_map& map) const {
            return !(*this == map);
        }
        
    };
    
    template <typename K, typename V>
    std::ostream inline &operator<<(std::ostream& o, const rb_map<K, V>& x) {
        return functional::write(o << "map", x.values());
    }
    
    template <typename K, typename V>
    inline rb_map<K, V>::rb_map(std::initializer_list<std::pair<K, V>> init) : Map{}, Size{0} {
        for (auto p : init) *this = insert(p.first, p.second);
    }
    
    template <typename K, typename V>
    bool inline rb_map<K, V>::operator==(const rb_map& map) const {
        return values() == map.values();
    }
    
    template <typename K, typename V>
    const ordered_stack<linked_stack<K>> rb_map<K, V>::keys() const {
        linked_stack<K> kk{};
        milewski::okasaki::forEach(Map, [&kk](const K& k, V) -> void {
            kk = kk << k;
        });
        ordered_stack<linked_stack<K>> x{};
        for (const auto& k : data::reverse(kk)) x = x << k;
        return x;
    }
    
    template <typename K, typename V>
    const ordered_stack<linked_stack<entry<K, V>>> rb_map<K, V>::values() const {
        linked_stack<entry> kk{};
        milewski::okasaki::forEach(Map, [&kk](const K& k, V v) -> void {
            kk = kk << entry{k, v};
        });
        ordered_stack<linked_stack<entry>> x{};
        for (const auto& e : data::reverse(kk)) x = x << e;
        return x;
    }
    
    template <typename K, typename V>
    const V inline &rb_map<K, V>::operator[](const K& k) const {
        V *x = Map.find(k);
        if (x == nullptr) throw "not found";
        return *x;
    }
    
    template <typename K, typename V>
    V inline *rb_map<K, V>::contains(const K& k) {
        return Map.find(k);
    }
    
    template <typename K, typename V>
    const V inline *rb_map<K, V>::contains(const K& k) const {
        return Map.find(k);
    }
    
    template <typename K, typename V>
    bool inline rb_map<K, V>::contains(const entry& e) const {
        return operator[](e.Key) == e.Value;
    }
    
    template <typename K, typename V>
    rb_map<K, V> rb_map<K, V>::insert(const K& k, const V& v) const {
        const V *already = contains(k);
        if (already == nullptr) return rb_map{Map.inserted(k, v), Size + 1};
        if (*already == v) return *this;
        rb_map removed = this->remove(k);
        return rb_map{removed.Map.inserted(k, v), removed.Size + 1};
    }
    
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::insert(const entry& e) const {
        return insert(e.Key, e.Value);
    }
    
    // there's a more efficient way of doing this. 
    // See matt.might.net/articles/red-black-delete/ 
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::remove(const K& k) const {
        rb_map m{};
        auto v = values();
        for (auto x = v.begin(); x != v.end(); ++x) if ((*x).Key != k) m = m.insert(*x);
        return m;
    }
    
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::remove(const entry& e) const {
        return operator[](e.Key) == e.Value ? remove(e.Key) : *this;
    }
    
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::operator<<(const entry& e) const {
        return insert(e.Key, e.Value);
    }
    
    template <typename K, typename V>
    bool inline rb_map<K, V>::empty() const {
        return Map.isEmpty();
    }
    
    template <typename K, typename V>
    size_t inline rb_map<K, V>::size() const {
        return Size;
    }
    
}

#endif
