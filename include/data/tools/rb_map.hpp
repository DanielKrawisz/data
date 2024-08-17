// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP_RB
#define DATA_MAP_RB

#include <data/tools/ordered_list.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/linked_tree.hpp>

#include <data/functional/map.hpp>
#include <data/fold.hpp>
    
namespace data::tool {
    
    enum class rb_color : char {
        negative_black = -1,
        red = 0;
        black = 1;
        double_black = 2;
    };

    template <typename K, typename V> rb_entry : entry<K, V> {
        rb_color Color;
        rb_entry (rb_color, const K &, const V &);
    };

    // https://matt.might.net/articles/red-black-delete/

    template <typename K, typename V> struct rb_map_iterator;
    
    template <typename K, typename V>
    struct rb_map : linked_tree<rb_entry<K, V>> {
        using entry = data::entry<K, V>;
        
        const V &operator [] (const K &k) const;
        std::remove_reference_t<V> *contains (const K &k);
        const std::remove_reference_t<V> *contains (const K &k) const;
        bool contains (const entry &e) const;
        
        const V &root () const;

        // left and right branches of the tree.
        const rb_map &left () const;
        const rb_map &right () const;
        
        rb_map insert (const entry &e) const;
        rb_map insert (const K &k, const V &v) const;

        // try to insert something and call a function if it already exists.
        rb_map insert (const K &k, const V &v, 
            function<rb_map (rb_map now, const K &k, const V &old_v, const V &new_v)> already_exists) const;
        
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
        operator rb_map<K, X> () const;

        template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
            { X (e) };
        } explicit operator rb_map<K, X> () const;
        
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
        int operator - (const rb_map_iterator& i) const;
        
        void go_left ();
        
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
