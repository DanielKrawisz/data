// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_MAP
#define DATA_FUNCTIONAL_MAP

#include <data/concepts.hpp>
#include <data/functional/set.hpp>
#include <data/container.hpp>
#include <data/indexed.hpp>
#include <data/ordered.hpp>
    
namespace data {

    template <Ordered K, typename V>
    struct entry {
        K Key;
        V Value;
        
        constexpr entry (const K &k, const V &v) : Key (k), Value (v) {}
        
        bool valid () const {
            return data::valid (Key) && data::valid (Value);
        }
        
        const K &key () const {
            return Key;
        }
        
        const V &value () const {
            return Value;
        }
    };

    // equal if the keys are equal.
    // NOTE this totally breaks some things.
    template <Ordered K, typename V>
    bool operator == (const entry<K, V> &l, const entry<K, V> &r) {
        return l.Key == r.Key;
    }

    template <Ordered K, typename V>
    auto operator <=> (const entry<K, V> &l, const entry<K, V> &r) {
        return l.Key <=> r.Key;
    }
    
    namespace interface {
    
        template <typename X, typename key, typename value>
        concept has_insert_key_value = requires (X x, const key k, const value v) {
            { x.insert (k, v) } -> ImplicitlyConvertible<X>;
        };
        
        template <typename map, typename key>
        concept has_keys_method = requires (map x) {
            { x.keys () } -> Sequence<const key &>;
        };
        
    }

    template <typename K, typename V> 
    std::ostream inline &operator << (std::ostream &o, const entry<K, V> &e) {
        return o << e.Key << ": " << e.Value;
    }
    
    namespace functional {
    
        template <typename X, 
            typename key = decltype (std::declval<X> ().values ().first ().Key),
            typename value = decltype (std::declval<X> ().values ().first ().Value)>
        concept map = Container<const X, const key> &&
            indexed<const X, const key, const value> &&
            OrderedSet<const X, const entry<const key, value>> &&
            interface::has_insert_key_value<const X, const key, value> &&
            interface::has_insert_method<X, const entry<const key, value>> &&
            interface::has_keys_method<const X, const key> &&
            interface::has_remove_method<const X, const key> && 
            std::default_initializable<X>;

        template <map M> 
        std::ostream inline &write (std::ostream &o, const M &m) {
            return o << m.values ();
        }

    }

}

#endif
