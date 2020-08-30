// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP
#define DATA_MAP

#include <data/functional/set.hpp>
#include <data/indexed.hpp>
    
namespace data {

    template <typename K, typename V> 
    requires std::totally_ordered<K> && requires { K{}; } && requires { V{}; }
    struct entry {
        const K Key;
        V Value;
        
        entry(const K k, V v) : Key(k), Value(v) {}
        
        // an invalid instance must exist for type value.
        entry(const K k) : Key(k), Value{} {}
        
        bool valid() const {
            return Value != V{};
        }
        
        const K key() const {
            return Key;
        }
        
        V value() const {
            return Value;
        }
        
        V operator[](const K k) {
            if (Key == k) return Value;
            return V{};
        }
        
        bool operator==(const entry& e) const {
            return Key == e.Key && Value == e.Value;
        }
        
        bool operator<(const entry& e) const {
            return Key < e.Key;
        }
        
        bool operator>(const entry& e) const {
            return Key > e.Key;
        }
        
        bool operator<=(const entry& e) const {
            return Key < e.Key;
        }
        
        bool operator>=(const entry& e) const {
            return Key >= e.Key;
        }
    };
    
    namespace interface {
    
        template <typename X, typename key, typename value>
        concept has_insert_key_value = requires (X x, const key k, const value v) {
            { x.insert(k, v) } -> std::convertible_to<X>;
        };
        
        template <typename map, typename key>
        concept has_keys_method = requires (map x) {
            { x.keys() } -> sequence<const key&>;
        };
        
    }

    template <typename K, typename V> 
    inline std::ostream& operator<<(std::ostream& o, const entry<K, V>& e) {
        return o << e.Key << " -> " << e.Value;
    }
    
    namespace functional {
    
        template <typename X, 
            typename key = decltype(std::declval<X>().values().first().key()), 
            typename value = decltype(std::declval<X>().values().first().value())>
        concept map = container<const X, key> && 
            indexed<const X, key, value> &&
            ordered_set<const X, const entry<key, value>> &&
            interface::has_insert_key_value<const X, key, value> && 
            interface::has_insert_method<X, const entry<key, value>> && 
            interface::has_keys_method<const X, key> && 
            interface::has_remove_method<const X, const key> && 
            std::default_initializable<X>;

        template <map M> 
        inline std::ostream& operator<<(std::ostream& o, const M& m) {
            return o << m.values();
        }
        
    }

}

#endif
