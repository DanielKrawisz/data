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
    struct map : linked_tree<RB::entry<const K, V>> {
        using tree = linked_tree<RB::entry<const K, V>>;
        using entry = data::entry<const K, V>;

        static bool balanced (tree);
        static map balance (tree);
        
        const V &operator [] (const K &k) const;
        std::remove_reference_t<V> *contains (const K &k);
        const std::remove_reference_t<V> *contains (const K &k) const;
        bool contains (const entry &e) const;
        
        // if a key already exists, the default behavior is NOT to replace that key.
        map insert (const entry &e) const;
        map insert (const K &k, const V &v) const;

        // try to insert something and call a function if it already exists.
        // (so that the user can decide whether he wants to replace it or not or combine them or whatever)
        map insert (const K &k, const V &v,
            function<V (const K &k, const V &old_v, const V &new_v)> already_exists) const;
        
        map operator << (const entry &e) const;
        
        // this is the difficult one. Follow
        // https://matt.might.net/articles/red-black-delete/
        map remove (const K &k) const;
        
        bool valid () const;
        
        map () : tree {} {}
        
        map (std::initializer_list<entry> init);

        using key_list = ordered_stack<linked_stack<const K>>;
        using value_list = ordered_stack<linked_stack<entry>>;

        key_list keys () const;
        value_list values () const;

        // for any allowed implicit conversion V -> X,
        // an implicit conversion map<K, V> -> map<K, X> is provided
        template <typename X> requires convertible_to<V, X>
        operator map<K, X> () const;

        // for any allowed explicit conversion V -> X,
        // an explicit conversion map<K, V> -> map<K, X> is provided
        template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
            { X (e) };
        } explicit operator map<K, X> () const;

        using iterator = RB::iterator<K, V>;
        using sentinel = data::sentinel<tree>;

        iterator begin () const;
        sentinel end () const;

    private:
        map (tree &&t) : tree {t} {}
    };

    template <typename K, typename V> struct iterator : map<K, V>::tree::iterator {
        using map<K, V>::tree::iterator::iterator;
        using sentinel = map<K, V>::sentinel;
        using entry = map<K, V>::entry;

        using difference_type = int;
        using value_type = entry;

        iterator operator ++ (int);
        iterator &operator ++ ();

        const entry &operator * () const;
        const entry *operator -> () const;

        bool operator == (const iterator i) const;

        bool operator == (const sentinel i) const;

        int operator - (const iterator &i) const;
    };

    template <typename K, typename V> bool inline operator < (const entry<K, V> &a, const entry<K, V> &b) {
        return static_cast<data::entry<K, V>> (a) < static_cast<data::entry<K, V>> (b);
    }

    template <typename K, typename V> bool inline map<K, V>::valid () const {
        return tree::valid () && balanced ();
    }

    template <typename K, typename V> map<K, V> inline map<K, V>::operator << (const data::entry<const K, V> &e) const {
        return insert (e);
    }

    template <typename K, typename V> template <typename X> requires convertible_to<V, X>
    inline map<K, V>::operator map<K, X> () const {
        return map<K, X> {static_cast<linked_tree<RB::entry<const K, X>>> (static_cast<const tree &> (*this))};
    }

    template <typename K, typename V> template <typename X> requires (!is_convertible_v<V, X>) && requires (const V &e) {
        { X (e) };
    } inline map<K, V>::operator map<K, X> () const {
        return map<K, X> {static_cast<linked_tree<RB::entry<const K, X>>> (static_cast<const tree &> (*this))};
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

    template <typename K, typename V> const iterator<K, V>::entry inline &iterator<K, V>::operator * () const {
        return static_cast<const data::entry<const K, V> &> (*static_cast<const map<K, V>::tree::iterator &> (*this));
    }

    template <typename K, typename V> const iterator<K, V>::entry inline *iterator<K, V>::operator -> () const {
        return static_cast<const data::entry<const K, V> *> (this->operator -> ());
    }

    template <typename K, typename V> int inline iterator<K, V>::operator - (const iterator<K, V> &i) const {
        return static_cast<const map<K, V>::tree::iterator &> (*this) - static_cast<const map<K, V>::tree::iterator &> (i);
    }

    template <typename K, typename V> bool inline iterator<K, V>::operator == (const iterator<K, V> i) const {
        return static_cast<const map<K, V>::tree::iterator &> (*this) == static_cast<const map<K, V>::tree::iterator &> (i);
    }

    template <typename K, typename V> bool inline iterator<K, V>::operator == (const map<K, V>::sentinel i) const {
        return static_cast<const map<K, V>::tree::iterator &> (*this).operator == (i);
    }

    template <typename K, typename V> iterator<K, V> inline map<K, V>::begin () const {
        return iterator {*this};
    }

    template <typename K, typename V> map<K, V>::sentinel inline map<K, V>::end () const {
        return sentinel {*this};
    }

    template <typename K, std::equality_comparable V> bool operator == (const map<K, V> &a, const map<K, V> &b) {
        if (a.size () != b.size ()) return false;
        auto bi = b.begin ();
        for (auto ai = a.begin (); ai != a.end (); ai++) if (*ai != *bi) return false;
        else bi++;
        return true;
    }

    template <typename K, typename V> inline map<K, V>::map (std::initializer_list<data::entry<const K, V>> init) {
        for (const auto &i : init) *this = insert (i);
    }

    template <typename K, typename V> bool inline map<K, V>::contains (const data::entry<const K, V> &e) const {
        auto v = contains (e.Key);
        if (!bool (v)) return false;
        return *v == e.Value;
    }

    template <typename K, typename V> map<K, V>::key_list map<K, V>::keys () const {
        linked_stack<const K> kk {};

        for (const auto &e : *this) kk <<= e.Key;

        key_list x {};
        for (const auto& k : data::reverse (kk)) x = x << k;
        return x;
    }

    template <typename K, typename V> map<K, V>::value_list map<K, V>::values () const {
        linked_stack<data::entry<const K, V>> kk {};

        for (const auto &e : *this) kk <<= e;

        value_list x {};
        for (const auto& e : data::reverse (kk)) x = x << e;
        return x;
    }

    // put function definitions here.
}

#endif
