// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP
#define DATA_MAP

#include <data/concepts.hpp>
#include <data/functional/set.hpp>
#include <data/indexed.hpp>
#include <data/math/ordered.hpp>
    
namespace data {

    template <ordered K, typename V>
    struct entry {
        K Key;
        V Value;
        
        entry (const K &k, const V &v) : Key (k), Value (v) {}
        
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
    template <ordered K, typename V>
    bool operator == (const entry<K, V> &l, const entry<K, V> &r) {
        return l.Key == r.Key;
    }

    template <ordered K, typename V>
    auto operator <=> (const entry<K, V> &l, const entry<K, V> &r) {
        return l.Key <=> r.Key;
    }
    
    namespace interface {
    
        template <typename X, typename key, typename value>
        concept has_insert_key_value = requires (X x, const key k, const value v) {
            { x.insert (k, v) } -> implicitly_convertible_to<X>;
        };
        
        template <typename map, typename key>
        concept has_keys_method = requires (map x) {
            { x.keys () } -> sequence<const key &>;
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
        concept map = container<const X, const key> &&
            indexed<const X, const key, value> &&
            ordered_set<const X, const entry<key, value>> &&
            interface::has_insert_key_value<const X, const key, value> &&
            interface::has_insert_method<X, const entry<key, value>> &&
            interface::has_keys_method<const X, const key> &&
            interface::has_remove_method<const X, const key> && 
            std::default_initializable<X>;

        template <map M> 
        std::ostream inline &write (std::ostream &o, const M &m) {
            return o << m.values ();
        }
        
        // we find that it is useful to have a map that also has other
        // attributes. This type is useful for creating a map that can
        // be extended.

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        struct base_map : M {
            using M::map;

            // the derived type needs to inheret these constructors.
            base_map (M &&rb);
            base_map (const M &rb);

            derived insert (const K &k, const V &v) const;
            derived insert (const entry<K, V> &e) const;
            derived insert (const K &k, const V &v, data::function<V (const V &old_v, const V &new_v)> already_exists) const;

            derived operator << (const entry<K, V> &e) const;

            derived replace (const V &a, const V &b) const;
            derived replace_part (const K &k, const V &v) const;
            derived replace_part (const K &k, data::function<V (const V &)> f) const;

            derived remove (const K &k) const;

        };

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        inline base_map<derived, K, V, M>::base_map (M &&rb) : M {rb} {}

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        inline base_map<derived, K, V, M>::base_map (const M &rb) : M {rb} {}

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::insert (const K &k, const V &v) const {
            return derived {M::insert (k, v)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::insert (const entry<K, V> &e) const {
            return derived {M::insert (e)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::insert (const K &k, const V &v,
            data::function<V (const V &old_v, const V &new_v)> already_exists) const {
            return derived {M::insert (k, v, already_exists)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::operator << (const entry<K, V> &e) const {
            return derived {M::insert (e)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::replace (const V &a, const V &b) const {
            return derived {M::replace (a, b)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::replace_part (const K &k, const V &v) const {
            return derived {M::replace_part (k, v)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::replace_part (const K &k, data::function<V (const V &)> f) const {
            return derived {M::replace_part (k, f)};
        }

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        derived inline base_map<derived, K, V, M>::remove (const K &k) const {
            return derived {M::remove (k)};
        }
    }

}

#endif
