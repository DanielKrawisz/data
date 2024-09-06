// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP_RB
#define DATA_MAP_RB

#include <data/tools/ordered_list.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/functional/map.hpp>
#include <data/fold.hpp>
#include <milewski/RBMap/RBMap.h>
    
namespace data::tool {
    
    template <typename K, typename V> struct rb_map_iterator;
    
    template <typename K, typename V>
    struct rb_map {
        using entry = data::entry<K, V>;
        using map = milewski::okasaki::RBMap<K, V>;
        
    private:
        map Map;
        size_t Size;
        
        rb_map (map m, size_t x) : Map {m}, Size {x} {}
        
    public:
        const V &operator [] (const K &k) const;
        std::remove_reference_t<V> *contains (const K &k);
        const std::remove_reference_t<V> *contains (const K &k) const;
        bool contains (const entry &e) const;
        
        const V &root () const;
        const rb_map &left () const;
        const rb_map &right () const;
        
        rb_map insert (const entry &e) const;
        rb_map insert (const K &k, const V &v) const;
        rb_map insert (const K &k, const V &v, 
            function<V (const V &old_v, const V &new_v)> already_exists) const;

        rb_map replace_part (const K &k, const V& v) const;
        
        rb_map operator << (const entry &e) const;
        
        rb_map remove (const K &k) const;
        
        bool valid () const {
            struct not_valid {};

            try {
                milewski::okasaki::forEach (Map, [] (const K &k, const V &v) -> void {
                    if (!data::valid (k) || !data::valid (v)) throw not_valid {};
                });
            } catch (not_valid) {
                return false;
            }

            return true;
        }
        
        bool empty () const;
        size_t size () const;
        
        rb_map () : Map {}, Size {0} {}
        
        rb_map (std::initializer_list<entry> init);
        
        ordered_stack<linked_stack<K>> keys () const;
        
        ordered_stack<linked_stack<entry>> values () const;
        
        rb_map_iterator<K, V> begin () const;
        
        rb_map_iterator<K, V> end () const;

        template <typename X> requires convertible_to<V, X>
        operator rb_map<K, X> () const {
            rb_map<K, X> m;
            for (const auto &e : *this) m = m.insert (e);
            return m;
        }

        template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
            { X (e) };
        } explicit operator rb_map<K, X> () const {
            rb_map<K, X> m;
            for (const auto &e : *this) m = m.insert (e);
            return m;
        }
        
    };

    template <typename K, std::equality_comparable V>
    bool operator == (const rb_map<K, V> &, const rb_map<K, V> &);
    
    template <typename K, typename V> 
    struct rb_map_iterator {
        using map = milewski::okasaki::RBMap<K, V>;
        using node = milewski::okasaki::Node<K, V>;
        
        ptr<const node> Map;
        ptr<const node> Next;
        linked_stack<ptr<const node>> Last;
        int Index;
        
        // we need this constructor in order to satisfy some 
        // std concepts but it's not really good for anything. 
        rb_map_iterator () : Map {nullptr}, Next {}, Last {}, Index {0} {}
        
        // constructor for the end of a map. 
        rb_map_iterator (const map *m, int size) : Map {m->_root}, Next {}, Last {}, Index {size} {}
        
        // constructor for the beginning of a map. 
        rb_map_iterator (const map *m) : Map {m->_root}, Next {m->_root}, Last {}, Index {0} {
            go_left ();
        }
        
        rb_map_iterator operator ++ (int);
        rb_map_iterator &operator ++ ();
        
        const data::entry<K, V> &operator * () const;
        const data::entry<K, V> *operator -> () const;
        
        bool operator == (const rb_map_iterator i) const;
        int operator - (const rb_map_iterator &i) const;
        
        void go_left ();
        
    };

    // a map that is good for deriving from.
    // NOTE: it seems that using this type leads to segmentation faults, not sure why.
    template <typename K, typename V, typename derived> struct base_rb_map : rb_map<K, V> {
        using rb_map<K, V>::rb_map;

        // the derived type needs to inheret these constructors.
        base_rb_map (rb_map<K, V> &&rb) : rb_map<K, V> {rb} {}
        base_rb_map (const rb_map<K, V> &rb) : rb_map<K, V> {rb} {}

        derived insert (const K& k, const V& v) const {
            derived {rb_map<K, V>::insert (k, v)};
        }

        derived insert (const entry<K, V> &e) const {
            derived {rb_map<K, V>::insert (e)};
        }

        derived operator << (const entry<K, V> &e) const {
            derived {rb_map<K, V>::insert (e)};
        }

        derived remove (const K &k) const {
            derived {rb_map<K, V>::remove (k)};
        }

    };

}

namespace std {
    template <typename K, typename V> 
    struct iterator_traits<data::tool::rb_map_iterator<K, V>> {
        using value_type = remove_const_t<data::entry<K, V>>;
        using difference_type = int;
        using pointer = const remove_reference_t<data::entry<K, V>> *;
        using reference = const data::entry<K, V> &;
        using iterator_concept = input_iterator_tag;
    };
}
    
namespace data::tool {
    
    template <typename K, typename V>
    std::ostream inline &operator << (std::ostream &o, const rb_map<K, V> &x) {
        return functional::write (o << "map", x.values ());
    }
    
    template <typename K, typename V>
    inline rb_map<K, V>::rb_map (std::initializer_list<entry> init) : Map {}, Size {0} {
        for (auto p : init) *this = insert (p);
    }
    
    template <typename K, std::equality_comparable V>
    bool inline operator == (const rb_map<K, V> &l, const rb_map<K, V> &r) {
        return l.values () == r.values ();
    }
    
    template <typename K, typename V>
    ordered_stack<linked_stack<K>> rb_map<K, V>::keys () const {
        linked_stack<K> kk {};

        milewski::okasaki::forEach (Map, [&kk] (const K &k, V) -> void {
            kk = kk << k;
        });

        ordered_stack<linked_stack<K>> x {};
        for (const auto& k : data::reverse (kk)) x = x << k;
        return x;
    }
    
    template <typename K, typename V>
    ordered_stack<linked_stack<entry<K, V>>> rb_map<K, V>::values () const {
        linked_stack<entry> kk {};

        milewski::okasaki::forEach (Map, [&kk] (const K &k, V v) -> void {
            kk = kk << entry {k, v};
        });

        ordered_stack<linked_stack<entry>> x {};
        for (const auto& e : data::reverse (kk)) x = x << e;
        return x;
    }
    
    template <typename K, typename V>
    const V inline &rb_map<K, V>::operator [] (const K &k) const {
        std::remove_reference_t<V> *x = Map.find (k);
        if (x == nullptr) throw "not found";
        return *x;
    }
    
    template <typename K, typename V>
    std::remove_reference_t<V> inline *rb_map<K, V>::contains (const K &k) {
        return Map.find (k);
    }
    
    template <typename K, typename V>
    const std::remove_reference_t<V> inline *rb_map<K, V>::contains (const K &k) const {
        return Map.find (k);
    }
    
    template <typename K, typename V>
    bool inline rb_map<K, V>::contains (const entry &e) const {
        return operator [] (e.Key) == e.Value;
    }
    
    template <typename K, typename V>
    rb_map<K, V> rb_map<K, V>::insert (const K &k, const V &v) const {
        const std::remove_reference_t<V> *already = contains (k);
        if (already == nullptr) return rb_map {Map.inserted (k, v), Size + 1};
        throw exception {} << "key already exists";/*
        if (*already == v) return *this;
        rb_map removed = this->remove (k);
        return rb_map {removed.Map.inserted (k, v), removed.Size + 1};*/
    }

    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::insert
    (const K &k, const V &v, function<V (const V &old_v, const V &new_v)> already_exists) const {
        const V *already = contains (k);
        return already == nullptr ? rb_map {Map.inserted (k, v), Size + 1} : this->replace_part (k, already_exists (*already, v));
    }

    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::replace_part (const K &k, const V &v) const {
        rb_map r {};
        for (const auto &e : *this) r = r.insert (e.Key, e.Key == k ? v : e.Value);
        return r;
    }
    
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::insert (const entry &e) const {
        return insert (e.Key, e.Value);
    }
    
    // there's a more efficient way of doing this. 
    // See matt.might.net/articles/red-black-delete/ 
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::remove (const K &k) const {
        rb_map m {};
        auto v = values ();
        for (auto x = v.begin (); x != v.end (); ++x) if ((*x).Key != k) m = m.insert (*x);
        return m;
    }
    
    template <typename K, typename V>
    rb_map<K, V> inline rb_map<K, V>::operator << (const entry &e) const {
        return insert (e.Key, e.Value);
    }
    
    template <typename K, typename V>
    bool inline rb_map<K, V>::empty () const {
        return Map.isEmpty ();
    }
    
    template <typename K, typename V>
    size_t inline rb_map<K, V>::size () const {
        return Size;
    }
    
    template <typename K, typename V>
    rb_map_iterator<K, V> inline rb_map<K, V>::begin () const {
        return rb_map_iterator<K, V> {&Map};
    }
    
    template <typename K, typename V>
    rb_map_iterator<K, V> inline rb_map<K, V>::end () const {
        return rb_map_iterator<K, V> {&Map, static_cast<int> (Size)};
    }
    
    template <typename K, typename V>
    rb_map_iterator<K, V> rb_map_iterator<K, V>::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }
    
    template <typename K, typename V>
    rb_map_iterator<K, V> &rb_map_iterator<K, V>::operator ++ () {
        if (Next == nullptr) return *this;
        Index++;
            
        if (Next->_rgt != nullptr) {
            Next = Next->_rgt;
            go_left ();
            return *this;
        } 
            
        if (!data::empty (Last)) {
            Next = Last.first ();
            Last = Last.rest ();
            return *this;
        }
        
        Next = nullptr;
        return *this;
    }
    
    template <typename K, typename V>
    const data::entry<K, V> inline &rb_map_iterator<K, V>::operator * () const {
        return Next->_entry;
    }

    template <typename K, typename V>
    const data::entry<K, V> inline *rb_map_iterator<K, V>::operator -> () const {
        return &Next->_entry;
    }
    
    template <typename K, typename V>
    bool inline rb_map_iterator<K, V>::operator == (const rb_map_iterator i) const {
        return Map == i.Map && Next == i.Next;
    }
    
    template <typename K, typename V>
    int inline rb_map_iterator<K, V>::operator - (const rb_map_iterator& i) const {
        if (Map == i.Map) return Index - i.Index;
        return 0;
    }
    
    template <typename K, typename V>
    void rb_map_iterator<K, V>::go_left () {
        if (Next == nullptr) return;
        while (Next->_lft != nullptr) {
            Last = Last << Next;
            Next = Next->_lft;
        }
    }
    
}

#endif
