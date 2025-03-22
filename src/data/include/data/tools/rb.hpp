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

    template <ordered V> struct colored {
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

    template <ordered V> auto inline operator <=> (const colored<V> &a, const colored<V> &b) {
        return a.Value <=> b.Value;
    }

    template <ordered V> bool inline operator == (const colored<V> &a, const colored<V> &b) {
        return a.Value == b.Value;
    }

    template <sortable V> std::ostream inline &operator << (std::ostream &o, const colored<V> &c) {
        return o << c.Value;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    const unref<V> inline &root (T t) {
        return data::root (t).Value;
    }

    // an RB tree is balanced if no red node has a red child and if the sum
    // of all black nodes from all leaves to the root is the same.
    template <sortable V, functional::buildable_tree<colored<V>> T> bool balanced (T t);

    template <sortable V, functional::buildable_tree<colored<V>> T> bool inline valid (T t) {
        return data::valid (t) && balanced (t);
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> T balance (T t);

    // insert a node into an RB tree
    template <sortable V, functional::buildable_tree<colored<V>> T>
    T insert (T t, inserted<V> v);

    template <sortable V, functional::buildable_tree<colored<V>> T> T remove (T t, const V &v);

    template <sortable V, functional::buildable_tree<colored<V>> T>
    color inline root_color (T t) {
        return empty (t) ? color::black : root (t).Color;
    }

    // a requirement for being balanced.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    bool inline red_nodes_have_no_red_children (T t) {
        if (empty (t)) return true;
        if (root_color<V> (t) == color::red && (root_color<V> (left (t)) == color::red || root_color<V> (right (t)) == color::red)) return false;
        return red_nodes_have_no_red_children<V> (left (t)) && red_nodes_have_no_red_children<V> (right (t));
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

    template <sortable V, functional::buildable_tree<colored<V>> T> T balance (inserted<V> v, T l, T r);

    // if the tree is balanced when this method is used on
    // it, it will be balanced when the method is done.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    T inline insert (T t, inserted<V> v) {
        return data::empty (t) ? T {colored<V> {color::red, v}, T {}, T {}}:
            v == root<V> (t) ? t: root_color<V> (t) != color::red ?
                (v < root<V> (t) ?
                    balance<V> (data::root (t), insert (left (t), v), right (t)):
                    balance<V> (data::root (t), left (t), insert (right (t), v))):
                (v < root<V> (t) ?
                    T {root<V> (t), insert (left (t), v), right (t)}:
                    T {root<V> (t), left (t), insert (right (t), v)});
    }

    // now let's talk about how to balance an RB tree.
    template <sortable V, functional::buildable_tree<colored<V>> T> bool inline doubled_left (T t) {
        return !empty (t) && root_color<V> (t) == color::red &&
            !empty (left (t)) && root_color<V> (left (t)) == color::red;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> bool inline doubled_right (T t) {
        return !empty (t) && root_color<V> (t) == color::red &&
            !empty (right (t)) && root_color<V> (right (t)) == color::red;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> T inline blacken (T t) {
        return empty (t) ? T {} : T {colored<V> {color::red, root<V> (t)}, left (t), right (t)};
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> T balance (inserted<V> v, T l, T r) {
        if (doubled_left<V> (l))
            return T {colored<V> {color::red, root<V> (l)},
                blacken<V> (left (l)),
                T {colored<V> {color::black, v}, right (l), r}};

        if (doubled_right<V> (l))
            return T {colored<V> {color::red, root<V> (right (l))},
                T {colored<V> {color::black, root<V> (l)}, left (l), left (right (l))},
                T {colored<V> {color::black, v}, right (right (l)), r}};

        if (doubled_left<V> (r))
            return T {colored<V> {color::red, root<V> (left (r))},
                T {colored<V> {color::black, v}, left (l), left (left (r))},
                T {colored<V> {color::black, root<V> (r)}, right (left (r)), right (r)}};

        if (doubled_right<V> (r))
            return T {colored<V> {color::red, root<V> (r)},
                T {colored<V> {color::black, v}, l, left (r)},
                blacken<V> (right (r))};

        return T (colored<V> {color::black, v}, l, r);
    }

    // finally, how to remove an RB tree.
    // See matt.might.net/articles/red-black-delete/

}

#endif
