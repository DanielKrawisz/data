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

// Implement a functional map according to Okasaki's book Functional Data Structures.
// Unfortunately, this book left a crucial method as an exercise for the reader.
// A delete method was later provided by https://matt.might.net/articles/red-black-delete/
    
namespace data::tool::RB {
    
    // In the course of deleting an entry, some entries may take on non-standard colors.
    // However, all maps that are provided to the user will only use standard red and black.
    enum class color : char {
        negative_black = -1,
        red = 0,
        black = 1,
        double_black = 2
    };

    template <typename K, typename V> struct entry : data::entry<K, V> {
        color Color;
        entry (color, const K &, const V &);
    };

    // Need to provide an ordering for map entries.
    template <typename K, typename V> bool inline operator < (const entry<K, V> &a, const entry<K, V> &b) {
        return static_cast<data::entry<K, V>> (a) < static_cast<data::entry<K, V>> (b);
    }

    template <typename K, typename V> struct map;

    template <typename K, std::equality_comparable V> bool operator == (const map<K, V> &, const map<K, V> &);

    // this map can be iterated over.
    template <typename K, typename V> struct iterator;
    
    // this map will be built on a tree, which we already have.
    template <typename K, typename V>
    struct map : linked_tree<entry<K, V>> {
        using tree = linked_tree<entry<K, V>>;
        
        const V &operator [] (const K &k) const;
        std::remove_reference_t<V> *contains (const K &k);
        const std::remove_reference_t<V> *contains (const K &k) const;
        bool contains (const data::entry<K, V> &e) const;
        
        const V &root () const;

        // left and right branches of the tree.
        const map &left () const;
        const map &right () const;
        
        map insert (const data::entry<K, V> &e) const;
        map insert (const K &k, const V &v) const;

        // try to insert something and call a function if it already exists.
        map insert (const K &k, const V &v,
            function<map (map now, const K &k, const V &old_v, const V &new_v)> already_exists) const;
        
        map operator << (const data::entry<K, V> &e) const;
        
        // this is the difficult one. Follow
        // https://matt.might.net/articles/red-black-delete/
        map remove (const K &k) const;
        
        bool valid () const {
            return tree::empty () && balanced ();
        }
        
        map () : tree {} {}
        
        map (std::initializer_list<data::entry<K, V>> init) {
            for (const auto &i : init) *this = insert (i);
        }
        
        ordered_stack<linked_stack<K>> keys () const;
        
        ordered_stack<linked_stack<entry<K, V>>> values () const;

        using iterator = RB::iterator<K, V>;

        iterator begin () const;
        iterator end () const;

        // implicit conversion
        template <typename X> requires convertible_to<V, X>
        operator map<K, X> () const;

        // explicit conversion
        template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
            { X (e) };
        } explicit operator map<K, X> () const;

    private:
        bool balanced () const;
    };

    template <typename K, typename V> struct iterator : map<K, V>::tree::iterator {
        using map<K, V>::tree::iterator::iterator;

        iterator operator ++ (int);
        iterator &operator ++ ();

        const data::entry<K, V> &operator * () const;
        const data::entry<K, V> *operator -> () const;

        int operator - (const iterator &i) const;
    };

}

namespace std {
    // standard definitons for an iterator according to the standard library.
    template <typename K, typename V> 
    struct iterator_traits<data::tool::RB::iterator<K, V>> {
        using value_type = remove_const_t<data::entry<K, V>>;
        using difference_type = int;
        using pointer = const remove_reference_t<data::entry<K, V>> *;
        using reference = const data::entry<K, V> &;
        using iterator_concept = input_iterator_tag;
    };
}
    
namespace data::tool::RB {
    // put function definitions here.
}

#endif
