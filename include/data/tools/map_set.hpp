// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MAP_SET
#define DATA_TOOLS_MAP_SET

#include <data/tools/linked_stack.hpp>
#include <data/tools/functional_queue.hpp>
    
namespace data::tool {
    
    struct unit {
        bool Valid;
        
        unit(bool b) : Valid{b} {}
        unit() : Valid{false} {}
        
        bool operator==(unit x) {
            return Valid == x.Valid;
        }
        
        bool operator!=(unit x) {
            return Valid != x.Valid;
        }
    };
    
    inline std::ostream& operator<<(std::ostream& o, unit) {
        return o << "unit" << std::endl;
    }
    
    // turn any map into a set. 
    template <typename M, 
        typename K = typename std::remove_const<typename std::remove_reference<decltype(std::declval<M>().keys().first())>::type>::type, 
        typename V = typename std::remove_const<typename std::remove_reference<decltype(std::declval<M>()[std::declval<K>()])>::type>::type>
    requires functional::map<M, K, V> && std::same_as<unit, V>
    struct map_set {
        using key = K;
        using value = V;
        
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
            return map_set{Map.insert(k, value{true})};
        }
        
        template <typename list> requires sequence<list, key>
        map_set insert(list keys) const {
            if (keys.empty()) return *this;
            return insert(keys.first()).insert(keys.rest());
        }
        
        template <typename X> requires interface::has_values_method<X, key>
        map_set insert(X m) const {
            auto v = m.values();
            auto q = *this;
            while (!v.empty()) {
                q = q.insert(v.first());
                v = v.rest();
            }
            return q;
        }
        
        map_set operator<<(const key& k) const {
            return insert(k);
        }
        
        map_set remove(const key& k) const {
            return map_set{Map.remove(k, value{true})};
        }
        
        const ordered_list<key> values() const {
            return Map.keys();
        }
        
        map_set() : Map{} {}
        map_set(M m) : Map(m) {}
        template <typename list> requires sequence<list, key>
        map_set(list keys) : Map{} {
            insert(keys);
        }
        
        bool operator==(const map_set& m) const {
            return values() == m.values();
        }
        
        bool operator!=(const map_set& m) const {
            return !operator==(m);
        }
        
        map_set operator&(const map_set& m) const {
            return map_set{}.insert(*this).insert(m);
        }
        
        map_set operator|(const map_set& m) const {
            auto a = values();
            auto b = values();
            map_set x{};
            
            while(!a.empty() && !b.empty()) {
                auto i = a.first();
                auto j = b.first();
                if (i == j) {
                    x = x.insert(i);
                    a = a.rest();
                    b = b.rest();
                } else if(i < j) a = a.rest();
                else b = b.rest();
            }
            
            return x;
        }
        
        map_set operator-(const map_set& m) const {
            auto a = values();
            auto b = values();
            map_set x{};
            
            while(!a.empty() && !b.empty()) {
                auto i = a.first();
                auto j = b.first();
                if (i == j) {
                    a = a.rest();
                    b = b.rest();
                } if(i < j) {
                    x = x.insert(i);
                    a = a.rest();
                } else b = b.rest();
            }
            
            return x;
        }
    };
    
    template <typename M, typename K, typename V>
    inline std::ostream& operator<<(std::ostream& o, const map_set<M, K, V>& m) {
        return functional::write(o << "set", m.values());
    }

}

#endif
