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

    // Need to provide an ordering for map entries.
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
    T insert (const T t, inserted<V> v, function<inserted<V> (inserted<V>, inserted<V>)> already_equivalent = &functional::keep_old<V>);

    template <sortable V, functional::buildable_tree<colored<V>> T> T remove (T t, const V &v);

    template <sortable V, functional::buildable_tree<colored<V>> T> const unref<V> *contains (const T t, inserted<V> v);

    template <sortable V, functional::buildable_tree<colored<V>> T>
    struct tree : binary_search_tree<colored<V>, T> {
        tree ();
        tree (const T &t);

        tree (std::initializer_list<wrapped<V>> x);

        const unref<V> &root () const;

        tree left () const;
        tree right () const;

        const unref<V> *contains (inserted<V> x) const {
            return RB::contains<V, T> (*this, x);
        }

        tree insert (inserted<V> v, function<const V & (const V &, const V &)> already_equivalent = &functional::keep_old<V>) const {
            return RB::insert<V, T> (*this, v, already_equivalent);
        }

        template <typename ...P>
        tree insert (inserted<V> a, inserted<V> b, P... p);

        tree remove (inserted<V> v) const {
            tree t;
            for (const auto &x : *this) if (x != v) t = t.insert (x);
            return t;
        }

        template <typename X, typename U> requires implicitly_convertible_to<V, X>
        operator tree<X, U> () const {
            tree<X, U> u {};
            for (const V &v : *this) u = u.insert (X (v));
            return u;
        }

        template <typename X, typename U> requires explicitly_convertible_to<V, X>
        explicit operator tree<X, U> () const {
            tree<X, U> u {};
            for (const V &v : *this) u = u.insert (X (v));
            return u;
        }

        struct iterator : binary_search_tree<colored<V>, T>::iterator {
            using parent = binary_search_tree<colored<V>, T>::iterator;
            using parent::iterator;

            using value_type = const unref<V>;

            iterator operator ++ (int);
            iterator &operator ++ ();

            const unref<V> &operator * () const;
            const unref<V> *operator -> () const;

            bool operator == (const iterator i) const;
        };

        iterator begin () const;
        iterator end () const;

        tool::ordered_stack<linked_stack<inserted<V>>> values () const;

    protected:
        // note: this function has quite a bad design. The point of it is to enable
        // functions in RB map such as replace and replace_part. It also assumes
        // that we are using linked_tree.
        T for_each (function<V (inserted<V>)> f) const {
            static_cast<const T &> (*this).for_each ([f] (inserted<colored<V>> cx) -> colored<V> {
                return colored<V> {cx.Color, f (cx.Value)};
            });
        }

    };

    // now let's talk about how to check whether a tree is balanced.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    color inline root_color (T t) {
        return data::empty (t) ? color::black : root (t).Color;
    }

    // a requirement for being balanced.
    template <sortable V, functional::buildable_tree<colored<V>> T>
    bool inline red_nodes_have_no_red_children (T t) {
        if (data::empty (t)) return true;
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
    T inline insert (const T t, inserted<V> v, function<inserted<V> (inserted<V>, inserted<V>)> already_equivalent) {
        return data::empty (t) ? T {colored<V> {color::red, v}, T {}, T {}}:
            v <=> root<V> (t) == 0 ? T {colored<V> {root_color<V> (t), already_equivalent (root<V> (t), v)}, left (t), right (t)}:
                root_color<V> (t) != color::red ?
                    (v < root<V> (t) ?
                        balance<V, T> (root<V> (t), insert<V, T> (left (t), v), right (t)):
                        balance<V, T> (root<V> (t), left (t), insert<V, T> (right (t), v))):
                    (v < root<V> (t) ?
                        T {data::root (t), insert<V, T> (left (t), v), right (t)}:
                        T {data::root (t), left (t), insert<V, T> (right (t), v)});
    }

    // now let's talk about how to balance an RB tree.
    template <sortable V, functional::buildable_tree<colored<V>> T> bool inline doubled_left (T t) {
        return !data::empty (t) && root_color<V> (t) == color::red &&
            !data::empty (left (t)) && root_color<V> (left (t)) == color::red;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> bool inline doubled_right (T t) {
        return !data::empty (t) && root_color<V> (t) == color::red &&
            !data::empty (right (t)) && root_color<V> (right (t)) == color::red;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T> T inline blacken (T t) {
        return data::empty (t) ? T {} : T {colored<V> {color::red, root<V> (t)}, left (t), right (t)};
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

    template <sortable V, functional::buildable_tree<colored<V>> T>
    const unref<V> *contains (const T t, inserted<V> x) {
        if (data::empty (t)) return nullptr;
        const auto &e = data::root (t);
        return e.Value == x ? &e.Value: x < e.Value ?
        contains<V, T> (data::left (t), x) :
        contains<V, T> (data::right (t), x);
    }

    // finally, how to remove an RB tree.
    // See matt.might.net/articles/red-black-delete/

    // member functions of the tree

    template <sortable V, functional::buildable_tree<colored<V>> T>
    inline tree<V, T>::tree (): binary_search_tree<colored<V>, T> {} {}

    template <sortable V, functional::buildable_tree<colored<V>> T>
    inline tree<V, T>::tree (const T &t): binary_search_tree<colored<V>, T> {t} {}

    template <sortable V, functional::buildable_tree<colored<V>> T>
    inline tree<V, T>::tree (std::initializer_list<wrapped<V>> x) {
        for (auto &v : x) *this = insert (v);
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    const unref<V> inline &tree<V, T>::root () const {
        return binary_search_tree<colored<V>, T>::root ().Value;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T> inline tree<V, T>::left () const {
        return binary_search_tree<colored<V>, T>::left ();
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T> inline tree<V, T>::right () const {
        return binary_search_tree<colored<V>, T>::right ();
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    template <typename ...P>
    tree<V, T> inline tree<V, T>::insert (inserted<V> a, inserted<V> b, P... p) {
        return insert (a).insert (b, p...);
    }

    // member functions for the iterator

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T>::iterator inline tree<V, T>::iterator::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T>::iterator inline &tree<V, T>::iterator::operator ++ () {
        ++static_cast<parent &> (*this);
        return *this;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    const unref<V> inline &tree<V, T>::iterator::operator * () const {
        return static_cast<const parent &> (*this)->Value;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    const unref<V> inline *tree<V, T>::iterator::operator -> () const {
        return &static_cast<const parent &> (*this)->Value;
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    bool inline tree<V, T>::iterator::operator == (const iterator i) const {
        return static_cast<const parent &> (*this) == static_cast<const parent &> (i);
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T>::iterator inline tree<V, T>::begin () const {
        return iterator {this, *this};
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T>::iterator inline tree<V, T>::end () const {
        return iterator {this, tree {}};
    }

    template <sortable V, functional::buildable_tree<colored<V>> T>
    tool::ordered_stack<linked_stack<inserted<V>>> tree<V, T>::values () const {
        linked_stack<inserted<V>> st;
        for (inserted<V> v : *this) st <<= v;
        linked_stack<inserted<V>> ts;
        for (inserted<V> v : st) ts <<= v;
        return tool::ordered_stack<linked_stack<inserted<V>>> {st};
    }

}

#endif
