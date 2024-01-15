// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP
#define DATA_MAP

#include <data/functional/set.hpp>
#include <data/indexed.hpp>
    
namespace data {

    template <std::totally_ordered K, typename V>
    struct entry {
        const K Key;
        V Value;
        
        entry (const K &k, const V &v) : Key (k), Value (v) {}
        
        bool valid () const {
            return data::valid (Key) && data::valid (Value);
        }
        
        const K key () const {
            return Key;
        }
        
        const V value () const {
            return Value;
        }
        
        bool operator < (const entry &e) const {
            return Key < e.Key;
        }
        
        bool operator > (const entry &e) const {
            return Key > e.Key;
        }
        
        bool operator <= (const entry &e) const {
            return Key < e.Key;
        }
        
        bool operator >= (const entry &e) const {
            return Key >= e.Key;
        }
    };


    template <std::totally_ordered K, typename V>
    bool operator == (const entry<K, V> &l, const entry<K, V> &r) {
        return l.Key == r.Key && r.Value == l.Value;
    }
    
    namespace interface {
    
        template <typename X, typename key, typename value>
        concept has_insert_key_value = requires (X x, const key k, const value v) {
            { x.insert (k, v) } -> std::convertible_to<X>;
        };
        
        template <typename map, typename key>
        concept has_keys_method = requires (map x) {
            { x.keys () } -> sequence<const key&>;
        };
        
    }

    template <typename K, typename V> 
    std::ostream inline &operator << (std::ostream &o, const entry<K, V> &e) {
        return o << e.Key << ": " << e.Value;
    }
    
    namespace functional {
    
        template <typename X, 
            typename key = decltype (std::declval<X> ().values ().first ().key ()),
            typename value = decltype (std::declval<X> ().values ().first ().value ())>
        concept map = container<const X, key> && 
            indexed<const X, key, value> &&
            ordered_set<const X, const entry<key, value>> &&
            interface::has_insert_key_value<const X, key, value> && 
            interface::has_insert_method<X, const entry<key, value>> && 
            interface::has_keys_method<const X, key> && 
            interface::has_remove_method<const X, const key> && 
            std::default_initializable<X>;

        template <map M> 
        std::ostream inline &write (std::ostream &o, const M &m) {
            return o << m.values ();
        }
        
    }

}

#endif
