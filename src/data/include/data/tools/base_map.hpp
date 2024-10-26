// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_BASE_MAP
#define DATA_TOOLS_BASE_MAP

#include <data/tools.hpp>

namespace data {

    namespace tool {
        // we find that it is useful to have a map that also has other
        // attributes. This type is useful for creating a map that can
        // be extended.

        template <typename derived, typename K, typename V, functional::map<K, V> M>
        struct base_map : M {
            using M::M;

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

    template <typename K, typename V, typename derived>
    struct base_map : map<K, V> {
        using map<K, V>::map;

        // the derived type needs to inheret these constructors.
        base_map (map<K, V> &&rb);
        base_map (const map<K, V> &rb);

        derived insert (const K &k, const V &v) const;
        derived insert (const entry<K, V> &e) const;
        derived insert (const K &k, const V &v, data::function<V (const V &old_v, const V &new_v)> already_exists) const;

        derived operator << (const entry<K, V> &e) const;

        derived replace (const V &a, const V &b) const;
        derived replace_part (const K &k, const V &v) const;
        derived replace_part (const K &k, data::function<V (const V &)> f) const;

        derived remove (const K &k) const;

    };

    template <typename K, typename V, typename derived>
    inline base_map<K, V, derived>::base_map (map<K, V> &&rb) : map<K, V> {rb} {}

    template <typename K, typename V, typename derived>
    inline base_map<K, V, derived>::base_map (const map<K, V> &rb) : map<K, V> {rb} {}

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::insert (const K &k, const V &v) const {
        return derived {map<K, V>::insert (k, v)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::insert (const entry<K, V> &e) const {
        return derived {map<K, V>::insert (e)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::insert (const K &k, const V &v,
        data::function<V (const V &old_v, const V &new_v)> already_exists) const {
        return derived {map<K, V>::insert (k, v, already_exists)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::operator << (const entry<K, V> &e) const {
        return derived {map<K, V>::insert (e)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::replace (const V &a, const V &b) const {
        return derived {map<K, V>::replace (a, b)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::replace_part (const K &k, const V &v) const {
        return derived {map<K, V>::replace_part (k, v)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::replace_part (const K &k, data::function<V (const V &)> f) const {
        return derived {map<K, V>::replace_part (k, f)};
    }

    template <typename K, typename V, typename derived>
    derived inline base_map<K, V, derived>::remove (const K &k) const {
        return derived {map<K, V>::remove (k)};
    }

}

#endif
