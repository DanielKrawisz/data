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
    template <typename K, typename V> bool operator < (const entry<K, V> &a, const entry<K, V> &b);

    template <typename K, typename V> struct map;

    template <typename K, std::equality_comparable V> bool operator == (const map<K, V> &, const map<K, V> &);

    // this map can be iterated over.
    template <typename K, typename V> struct iterator;
    
    // this map will be built on a tree, which we already have.
    template <typename K, typename V>
    struct map : linked_tree<entry<K, V>> {
        using tree = linked_tree<entry<K, V>>;

        static bool balanced (tree);
        static map balance (tree);
        
        const V &operator [] (const K &k) const;
        std::remove_reference_t<V> *contains (const K &k);
        const std::remove_reference_t<V> *contains (const K &k) const;
        bool contains (const data::entry<K, V> &e) const;
        
        // if a key already exists, the default behavior is NOT to replace that key.
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
            return tree::valid () && balanced ();
        }
        
        map () : tree {} {}
        
        map (std::initializer_list<data::entry<K, V>> init);

        ordered_stack<linked_stack<K>> keys () const;
        ordered_stack<linked_stack<entry<K, V>>> values () const;

        // implicit conversion
        template <typename X> requires convertible_to<V, X>
        operator map<K, X> () const;

        // explicit conversion
        template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
            { X (e) };
        } explicit operator map<K, X> () const;

        using iterator = RB::iterator<K, V>;
        using sentinel = data::sentinel<map>;

        iterator begin () const;
        sentinel end () const;

    private:
        map (tree &&t) : tree {t} {}
    };

    template <typename K, typename V> struct iterator : map<K, V>::tree::iterator {
        using map<K, V>::tree::iterator::iterator;

        using difference_type = int;
        using value_type = data::entry<K, V>;

        iterator operator ++ (int);
        iterator &operator ++ ();

        const data::entry<K, V> &operator * () const;
        const data::entry<K, V> *operator -> () const;

        int operator - (const iterator &i) const;
    };

    template <typename K, typename V> bool inline operator < (const entry<K, V> &a, const entry<K, V> &b) {
        return static_cast<data::entry<K, V>> (a) < static_cast<data::entry<K, V>> (b);
    }

    template <typename K, typename V> iterator<K, V> inline iterator<K, V>::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <typename K, typename V> iterator<K, V> inline &iterator<K, V>::operator ++ () {
        ++static_cast<map<K, V>::tree::iterator &> (*this);
        return *this;
    }

    template <typename K, typename V> const data::entry<K, V> inline &iterator<K, V>::operator * () const {
        return static_cast<const data::entry<K, V> &> (*static_cast<const map<K, V>::tree::iterator &> (*this));
    }

    template <typename K, typename V> const data::entry<K, V> inline *iterator<K, V>::operator -> () const {
        return static_cast<const data::entry<K, V> *> (this->operator -> ());
    }

    template <typename K, typename V> int inline iterator<K, V>::operator - (const iterator<K, V> &i) const {
        return static_cast<const map<K, V>::tree::iterator &> (*this) - static_cast<const map<K, V>::tree::iterator &> (i);
    }

    template <typename K, typename V> iterator<K, V> inline &map<K, V>::begin () const {
        return iterator {*this};
    }

    template <typename K, typename V> map<K, V>sentinel; inline &map<K, V>::end () const {
        return sentinel {*this};
    }

    template <typename K, std::equality_comparable V> bool operator == (const map<K, V> &a, const map<K, V> &b) {
        if (a.size () != b.size ()) return false;
        auto bi = b.begin ();
        for (auto ai = a.begin (); a != a.end (); a++) if (*a != *b) return false;
        else bi++;
        return true;
    }

    template <typename K, typename V> inline map<K, V>::map (std::initializer_list<data::entry<K, V>> init) {
        for (const auto &i : init) *this = insert (i);
    }

    template <typename K, typename V> bool inline contains (const data::entry<K, V> &e) const {
        auto v = contains (e.Key);
        if (!bool (v)) return false;
        return *v == e.Value;
    }

    template <typename K, typename V> ordered_stack<linked_stack<K>> map<K, V>::keys () const {

    }

    template <typename K, typename V> ordered_stack<linked_stack<entry<K, V>>> map<K, V>::values () const {

    }

    template <typename K, typename V> template <typename X> requires convertible_to<V, X>
    map<K, V>::operator map<K, X> () const {

    }

    template <typename K, typename V> template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
        { X (e) };
    } map<K, V>::operator map<K, X> () const {

    }

    // put function definitions here.
}

#endif
