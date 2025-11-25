// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MAP_SET
#define DATA_TOOLS_MAP_SET

#include <data/stack.hpp>
#include <data/tools/functional_queue.hpp>
    
namespace data::tool {
    
    struct unit {
        unit () {}
        
        bool operator == (unit x) const {
            return true;
        }
    };
    
    inline std::ostream &operator << (std::ostream &o, unit) {
        return o << "unit" << std::endl;
    }

    template <typename it> struct map_set_iterator;
    
    // turn any map into a set. 
    template <typename M, typename K = decltype (std::declval<const M> ().values ().first ().Key)>
    requires Map<M, std::remove_const_t<K>, unit>
    struct map_set {
        using key = K;
        
        M Map;
        
        bool empty () const {
            return Map.empty ();
        }
        
        bool contains (const key &k) const {
            return Map.contains (k);
        }
        
        size_t size () const {
            return Map.size ();
        }
        
        map_set insert (const key &k) const {
            return map_set {Map.insert (k, unit {}, [] (const unit &a, const unit &b) {return a;})};
        }
        
        template <typename list> requires Sequence<list, key>
        map_set insert (list keys) const {
            if (keys.empty ()) return *this;
            return insert (keys.first ()).insert (keys.rest ());
        }
        
        template <typename ... P>
        map_set insert (K k, P... p) const {
            return insert (k).insert (p...);
        }
        
        map_set operator << (const key &k) const {
            return insert (k);
        }
        
        map_set remove (const key &k) const {
            return map_set {Map.remove (k)};
        }
        
        ordered_sequence<const key &> values () const {
            return Map.keys ();
        }
        
        map_set () : Map {} {}
        
        explicit map_set (M m) : Map (m) {}
        
        template <typename list> requires Sequence<list, key>
        explicit map_set (list keys) : Map {} {
            *this = insert (keys);
        }
        
        template <typename ... P>
        map_set (K k, P... p) : map_set {map_set {}.insert (k, p...)} {}
        
        bool operator == (const map_set &m) const {
            return values () == m.values ();
        }
        
        map_set operator & (const map_set &m) const {
            return map_set {}.insert (*this).insert (m);
        }
        
        map_set operator | (const map_set &m) const {
            auto a = values ();
            auto b = values ();
            map_set x {};
            
            while (!a.empty () && !b.empty ()) {
                auto i = a.first ();
                auto j = b.first ();
                if (i == j) {
                    x = x.insert (i);
                    a = a.rest ();
                    b = b.rest ();
                } else if (i < j) a = a.rest ();
                else b = b.rest ();
            }
            
            return x;
        }
        
        map_set operator - (const map_set &m) const {
            auto a = values ();
            auto b = values ();
            map_set x {};
            
            while (!a.empty () && !b.empty ()) {
                auto i = a.first ();
                auto j = b.first ();
                if (i == j) {
                    a = a.rest ();
                    b = b.rest ();
                } if (i < j) {
                    x = x.insert (i);
                    a = a.rest ();
                } else b = b.rest ();
            }
            
            return x;
        }
        
        bool valid () const {
            return values ().valid ();
        }

        auto begin () const {
            return map_set_iterator<decltype (std::declval<const M> ().begin ())> {Map.begin ()};
        }

        auto end () const {
            return map_set_iterator<decltype (std::declval<const M> ().end ())> {Map.end ()};
        }
    };
    
    template <typename M, typename K>
    inline std::ostream &operator << (std::ostream &o, const map_set<M, K> &m) {
        return functional::write (o << "set", m.values ());
    }

    template <typename it> struct map_set_iterator {
        it It;

        map_set_iterator ();
        map_set_iterator (it i): It {i} {}

        map_set_iterator &operator ++ () {
            ++It;
            return *this;
        }

        map_set_iterator operator ++ (int) {
            auto x = *this;
            ++(*this);
            return x;
        }

        const auto &operator * () const {
            return It->Key;
        }

        const auto *operator -> () const {
            return &It->Key;
        }

        template <typename I> // in case there's a sentinel at the end.
        bool operator == (const map_set_iterator<I> i) const {
            return It == i.It;
        }

        int operator - (const map_set_iterator &i) const {
            return It - i.It;
        }
    };

}

#endif
