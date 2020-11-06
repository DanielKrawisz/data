// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MAP_SET
#define DATA_TOOLS_MAP_SET

#include <data/tools/linked_stack.hpp>
#include <data/tools/functional_queue.hpp>
    
namespace data::tool {
    
    // turn any map into a set. 
    template <typename M, 
        typename key = typename std::remove_const<typename std::remove_reference<decltype(std::declval<M>().keys().first())>::type>::type, 
        typename value = typename std::remove_const<typename std::remove_reference<decltype(std::declval<M>()[std::declval<key>()])>::type>::type>
    struct map_set {
        
        // functional queue built using the list. 
        template <typename X> using list = tool::functional_queue<tool::linked_stack<X>>;
        
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
        
        map_set insert(list<key> keys) const {
            if (keys.empty()) return *this;
            return insert(keys.first()).insert(keys.rest());
        }
        
        map_set insert(map_set<M> m) const {
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
            return map_set{Map.remove(k, value{})};
        }
        
        // TODO make this a list<const key&> 
        list<key> values() const {
            return Map.keys();
        }
        
        map_set() : Map{} {}
        map_set(M m) : Map(m) {}
        map_set(list<key> keys) : Map{} {
            insert(keys);
        }
        
        bool operator==(const map_set& m) const {
            if (size() != m.size()) return false;
            if (size() == 0) return true;
            auto left_entry = values().first();
            auto right_entry = m.values().first();
            auto left = values().rest();
            auto right = m.values().rest();
            while(true) {
                if (left_entry == right_entry) {
                    if (left.empty()) return true;
                    left_entry = left.first();
                    right_entry = right.first();
                    left = left.rest();
                    right = right.rest();
                } else {
                    auto left_look_ahead = left;
                    auto left_look_ahead_entry = left_entry;
                    while (true) {
                        if (left_look_ahead_entry > right_entry) return false;
                        if (left_look_ahead.empty()) return false;
                        left_look_ahead = left_look_ahead.rest();
                        left_look_ahead_entry = left_look_ahead.first();
                        if (left_look_ahead_entry == right_entry) goto loop;
                    }
                    return false;
                } 
                loop:;
            }
            
        }
        
        bool operator!=(const map_set& m) const {
            return !operator==(m);
        }
        
        map_set operator&(const map_set& m) const {
            return map_set{}.insert(*this).insert(m);
        }
        
        map_set operator|(const map_set& m) const {
            throw method::unimplemented{""};
        }
        
        map_set operator-(const map_set& m) const {
            throw method::unimplemented{""};
        }
    };

}

#endif
