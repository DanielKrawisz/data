// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_RB
#define DATA_TOOLS_RB

#include <data/tools/ordered_list.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/linked_tree.hpp>
#include <data/tools/binary_search_tree.hpp>

#include <data/functional/map.hpp>
#include <data/fold.hpp>

// Implement a functional map according to Okasaki's book Functional Data Structures.
// Unfortunately, this book left a crucial method as an exercise for the reader.
// A delete method was later provided by https://matt.might.net/articles/red-black-delete/

namespace data::RB {

    // In the course of deleting an entry, some entries may take on non-standard colors.
    // However, all maps that are provided to the user will only use standard red and black.
    enum class color : int16 {
        negative_black = -1,
        red = 0,
        black = 1,
        double_black = 2
    };

    color inline operator + (const color a, const color b) {
        return color (int16 (a) + int16 (b));
    }

    template <sortable V> struct colored {
        color Color;
        V Value;
        colored (color c, const V &v) : Color {c}, Value {v} {}

        // automatic conversions
        template <typename X> requires implicitly_convertible_to<V, X>
        operator colored<X> () const;

        // explicit conversions
        template <typename X> requires explicitly_convertible_to<V, X>
        explicit operator colored<X> () const;
    };

    template <sortable V> bool inline operator < (const colored<V> &a, const colored<V> &b) {
        return a.Value < b.Value;
    }

    template <sortable V> bool inline operator == (const colored<V> &a, const colored<V> &b) {
        return a.Value == b.Value;
    }

    template <sortable V> std::ostream inline &operator << (std::ostream &o, const colored<V> &c) {
        return o << c.Value;
    }

    // an RB tree is balanced if no red node has a red child and if the sum
    // of all black nodes from all leaves to the root is the same.
    template <sortable V, functional::buildable_tree<colored<V>> T> bool balanced (T t);

    template <sortable V, functional::buildable_tree<colored<V>> T> bool inline valid (T t) {
        return data::valid (t) && balanced (t);
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> bool balance (T t);

    // insert a node into an RB tree
    template <sortable V, functional::buildable_tree<colored<V>> T>
    T inline insert (T t, const V &v) {
        return balance (functional::insert (t, colored<V> {color::black, v}));
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> T remove (T t, const V &v);

    // now let's talk about how to check whether a tree is balanced.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    color inline root_color (T t) {
        return empty (t) ? color::black : root (t).Color;
    }

    // a requirement for being balanced.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    bool inline red_nodes_have_no_red_children (T t) {
        return empty (t) ? true :
            root (t).Color != color::red ? true:
                root_color<V> (left (t)) != color::red && root_color<V> (right (t)) != color::red;
    }

    // The depth of black paths need to be compared to
    // check if a tree is balanced.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    maybe<color> blackness (T t) {
        if (t.empty ()) return color::red;
        maybe<color> right_blackness = blackness<V> (right (t));
        maybe<color> left_blackness = blackness<V> (left (t));
        if (!bool (right_blackness) || !bool (left_blackness) || *right_blackness != *left_blackness) return {};
        return *right_blackness + root (t).Color;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    bool inline balanced (T t) {
        return bool (blackness<V> (t)) && red_nodes_have_no_red_children<V> (t);
    }

    // now let's talk about how to balance an RB tree.

    // finally, how to remove an RB tree.

}

#endif
