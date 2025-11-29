// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_RB
#define DATA_TOOLS_RB

#include <data/tools/ordered_list.hpp>
#include <data/stack.hpp>
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
    enum class color : byte {
        red = 0,
        black = 1,
    };
    
    std::ostream inline &operator << (std::ostream &o, const color &c) {
        return o << (c == color::red ? "RED" : "BLACK");
    }

    color inline operator + (const color a, const color b) {
        return color (byte (a) + byte (b));
    }

    template <Ordered V> struct colored {
        color Color;
        V Value;
        colored (color c, const V &v) : Color {c}, Value {v} {}

        // automatic conversions
        template <typename X> requires ImplicitlyConvertible<V, X>
        operator colored<X> () const;

        // explicit conversions
        template <typename X> requires ExplicitlyConvertible<V, X>
        explicit operator colored<X> () const;
    };

    // Need to provide an ordering for map entries.
    template <Ordered V> auto inline operator <=> (const colored<V> &a, const colored<V> &b) {
        return a.Value <=> b.Value;
    }

    template <Ordered V> bool inline operator == (const colored<V> &a, const colored<V> &b) {
        return a.Value == b.Value;
    }

    template <Sortable V> std::ostream inline &operator << (std::ostream &o, const colored<V> &c) {
        return o << "(" << c.Color << " " << c.Value << ")";
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    const unref<V> inline &root (T t) {
        return data::root (t).Value;
    }

    // an RB tree is balanced if no red node has a red child and if the sum
    // of all black nodes from all leaves to the root is the same.
    template <Sortable V, functional::buildable_tree<colored<V>> T> bool balanced (T t);

    template <Sortable V, functional::buildable_tree<colored<V>> T> bool inline valid (T t) {
        return data::valid (t) && balanced (t);
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T> T balance (T t);

    // insert a node into an RB tree
    template <Sortable V, functional::buildable_tree<colored<V>> T, typename already_equivalent>
    T insert (const T t, inserted<V> v, already_equivalent);

    template <Sortable V, functional::buildable_tree<colored<V>> T> T remove (T t, const V &v);
    template <Sortable V, functional::buildable_tree<colored<V>> T> T erase (T t, const V &v);

    template <Sortable V, functional::buildable_tree<colored<V>> T> const unref<V> *contains (const T t, inserted<V> v);
    
    template <Sortable V, functional::buildable_tree<colored<V>> T> struct tree;

    template <typename V, typename T> std::ostream &operator << (std::ostream &, const tree<V, T> &t);

    template <typename V, typename T> bool empty (const tree<V, T> &t);
    template <typename V, typename T> size_t size (const tree<V, T> &t);
    template <typename V, typename T> tree<V, T> insert (const tree<V, T> &, inserted<V>);

    template <typename V, typename T, typename E> tree<V, T> remove (const tree<V, T> &, const E &);
    template <typename V, typename T, typename E> tree<V, T> erase (const tree<V, T> &, const E &);

    template <typename V, typename T, typename already_exists>
    requires requires (already_exists f, const V &old_v, const V &new_v) {
        { f (old_v, new_v) } -> ImplicitlyConvertible<V>;
    } tree<V, T> merge (const tree<V, T> &, const tree<V, T> &, already_exists f);

    template <typename V, typename T, typename already_exists>
    requires requires (already_exists f, const V &old_v, const V &new_v) {
        { f (old_v, new_v) } -> ImplicitlyConvertible<V>;
    } tree<V, T> intersect (const tree<V, T> &, const tree<V, T> &, already_exists f);

    template <Sortable V, functional::buildable_tree<colored<V>> T> struct tree;

    template <typename V, typename T> tree<V, T> operator & (const tree<V, T> &, const tree<V, T> &);
    template <typename V, typename T> tree<V, T> operator | (const tree<V, T> &, const tree<V, T> &);
    template <typename V, typename T> tree<V, T> operator ^ (const tree<V, T> &, const tree<V, T> &);

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    struct tree : binary_search_tree<colored<V>, T> {
        tree ();
        tree (const T &t);

        tree (std::initializer_list<wrapped<V>> x);

        const V &root () const;

        tree left () const;
        tree right () const;

        const unref<V> *contains (inserted<V> x) const {
            return RB::contains<V, T> (*this, x);
        }

        tree insert (inserted<V> v) const {
            return RB::insert<V, T> (*this, v, &functional::keep_old<V>);
        }

        template <typename already_equivalent> requires requires (already_equivalent f, const V &old_v, const V &new_v) {
            { f (old_v, new_v) } -> ImplicitlyConvertible<V>;
        } tree insert (inserted<V> v, already_equivalent f) const {
            return RB::insert<V, T> (*this, v, f);
        }

        template <typename ...P>
        tree insert (inserted<V> a, inserted<V> b, P... p);

        ordered_sequence<const V &> values () const;

        tree remove (const V &v) const {
            tree t;
            for (const auto &x : *this) if (x != v) t = t.insert (x);
            return t;
        }

        template <typename X, typename U> requires ImplicitlyConvertible<V, X>
        operator tree<X, U> () const {
            tree<X, U> u {};
            for (const V &v : *this) u = u.insert (X (v));
            return u;
        }

        template <typename X, typename U> requires ExplicitlyConvertible<V, X>
        explicit operator tree<X, U> () const {
            tree<X, U> u {};
            for (const V &v : *this) u = u.insert (X (v));
            return u;
        }

        struct iterator : binary_search_tree<colored<V>, T>::const_iterator {
            using parent = binary_search_tree<colored<V>, T>::const_iterator;
            
            // all constructors of the parent class are available to the 
            // base class. We cannot use the standard method because we 
            // don't really know the name of the base class. In gcc, 
            // using parent::iterator works but not in windows. 
            template <typename ...Args>
            iterator (Args &&...args): parent {std::forward<Args> (args)...} {}

            using iterator_category = std::forward_iterator_tag;
            using value_type        = unref<V>;
            using reference         = const V &;
            using pointer           = const value_type *;
            using difference_type   = int;

            iterator operator ++ (int) {
                auto x = *this;
                ++(*this);
                return x;
            }

            iterator &operator ++ () {
                ++static_cast<parent &> (*this);
                return *this;
            }

            reference operator * () const;
            pointer operator -> () const;

            bool operator == (const iterator i) const;
        };

        iterator begin () const;
        iterator end () const;

    };

    template <typename V, typename T>
    std::ostream inline &operator << (std::ostream &o, const tree<V, T> &t) {
        o << "{";
        auto i = t.begin ();
        if (i != t.end ()) {
            o << *i;
            while (true) {
                i++;
                if (i == t.end ()) break;
                o << ", " << *i;
            }
        }
        return o << "}";
    }
    
    template <typename V, typename T> bool inline empty (const tree<V, T> &t) {
        return t.empty ();
    }

    template <typename V, typename T> size_t inline size (const tree<V, T> &t) {
        return t.size ();
    }

    template <typename V, typename T> tree<V, T> inline insert (const tree<V, T> &t, inserted<V> j) {
        return t.insert (j);
    }

    template <typename V, typename T, typename E> tree<V, T> inline remove (const tree<V, T> &t, const E &j) {
        return t.remove (V {j});
    }

    template <typename V, typename T, typename E> tree<V, T> inline erase (const tree<V, T> &t, const E &j) {
        return t.remove (V {j});
    }
    
    template <typename V, typename T> tree<V, T> operator | (const tree<V, T> &a, const tree<V, T> &b) {
        return merge (a, b, [] (const V &old_v, const V &new_v) -> V {
            if (old_v == new_v) return old_v;
            throw exception {} << "cannot merge because of inequivalent values";
        });
    }

    template <typename V, typename T> tree<V, T> operator & (const tree<V, T> &a, const tree<V, T> &b) {
        return intersect (a, b, [] (const V &old_v, const V &new_v) -> V {
            if (old_v == new_v) return old_v;
            throw exception {} << "cannot intersect because of inequivalent values";
        });
    }

    template <typename V, typename T> tree<V, T> operator ^ (const tree<V, T> &a, const tree<V, T> &b) {
        auto i = a.begin ();
        auto j = b.begin ();
        tree<V, T> result;
        while (true) {
            if (i == a.end ()) {
                while (j != b.end ()) {
                    result = result.insert (*j);
                    j++;
                }
                break;
            }

            if (j == b.end ()) {
                while (i != a.end ()) {
                    result = result.insert (*i);
                    i++;
                }
                break;
            }

            if (*i < *j) {
                result = result.insert (*i);
                i++;
            } else if (*j < *i) {
                result = result.insert (*j);
                j++;
            } else {
                i++;
                j++;
            }
        }

        return result;
    }


    template <typename V, typename T, typename already_exists>
    requires requires (already_exists f, const V &old_v, const V &new_v) {
        { f (old_v, new_v) } -> ImplicitlyConvertible<V>;
    } tree<V, T> intersect (const tree<V, T> &a, const tree<V, T> &b, already_exists f) {
        auto i = a.begin ();
        auto j = b.begin ();
        tree<V, T> result;
        while (true) {
            if (i == a.end ()) {
                break;
            }

            if (j == b.end ()) {
                break;
            }

            if (*i < *j) {
                i++;
            } else if (*j < *i) {
                j++;
            } else {
                result = result.insert (f (*i, *j));
                i++;
                j++;
            }
        }

        return result;
    }

    template <typename V, typename T, typename already_exists> 
    requires requires (already_exists f, const V &old_v, const V &new_v) {
        { f (old_v, new_v) } -> ImplicitlyConvertible<V>;
    } tree<V, T> merge (const tree<V, T> &a, const tree<V, T> &b, already_exists f) {
        // note: a faster version of this function is possible. 
        auto n = a;
        for (const auto &v : b) n = n.insert (v, f);
        return n;
    }

    // now let's talk about how to check whether a tree is balanced.
    template <Sortable V, functional::buildable_tree<colored<V>> T>
    color inline root_color (T t) {
        return data::empty (t) ? color::black : root (t).Color;
    }

    // a requirement for being balanced.
    template <Sortable V, functional::buildable_tree<colored<V>> T>
    bool inline red_nodes_have_no_red_children (T t) {
        if (data::empty (t)) return true;
        if (root_color<V> (t) == color::red && (root_color<V> (left (t)) == color::red || root_color<V> (right (t)) == color::red)) return false;
        return red_nodes_have_no_red_children<V> (left (t)) && red_nodes_have_no_red_children<V> (right (t));
    }

    // The depth of black paths need to be compared to
    // check if a tree is balanced.
    template <Sortable V, functional::buildable_tree<colored<V>> T>
    maybe<color> blackness (T t) {
        if (t.empty ()) return color::red;
        maybe<color> right_blackness = blackness<V> (right (t));
        maybe<color> left_blackness = blackness<V> (left (t));
        if (!bool (right_blackness) || !bool (left_blackness) || *right_blackness != *left_blackness) return {};
        return *right_blackness + root (t).Color;
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    bool inline balanced (T t) {
        return bool (blackness<V> (t)) && red_nodes_have_no_red_children<V> (t);
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T> T balance (inserted<V> v, T l, T r);

    // if the tree is balanced when this method is used on
    // it, it will be balanced when the method is done.
    template <Sortable V, functional::buildable_tree<colored<V>> T, typename already_equivalent>
    T inline insert (const T t, inserted<V> v, already_equivalent f) {
        return data::empty (t) ? T {colored<V> {color::red, v}, T {}, T {}}:
            v <=> root<V> (t) == 0 ? T {colored<V> {root_color<V> (t), f (root<V> (t), v)}, left (t), right (t)}:
                root_color<V> (t) != color::red ?
                    (v < root<V> (t) ?
                        balance<V, T> (root<V> (t), insert<V, T> (left (t), v, f), right (t)):
                        balance<V, T> (root<V> (t), left (t), insert<V, T> (right (t), v, f))):
                    (v < root<V> (t) ?
                        T {data::root (t), insert<V, T> (left (t), v, f), right (t)}:
                        T {data::root (t), left (t), insert<V, T> (right (t), v, f)});
    }

    // now let's talk about how to balance an RB tree.
    template <Sortable V, functional::buildable_tree<colored<V>> T> bool inline doubled_left (T t) {
        return !data::empty (t) && root_color<V> (t) == color::red &&
            !data::empty (left (t)) && root_color<V> (left (t)) == color::red;
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T> bool inline doubled_right (T t) {
        return !data::empty (t) && root_color<V> (t) == color::red &&
            !data::empty (right (t)) && root_color<V> (right (t)) == color::red;
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T> T inline blacken (T t) {
        return data::empty (t) ? T {} : T {colored<V> {color::red, root<V> (t)}, left (t), right (t)};
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T> T balance (inserted<V> v, T l, T r) {
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

    template <Sortable V, functional::buildable_tree<colored<V>> T>
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

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    inline tree<V, T>::tree (): binary_search_tree<colored<V>, T> {} {}

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    inline tree<V, T>::tree (const T &t): binary_search_tree<colored<V>, T> {t} {}

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    inline tree<V, T>::tree (std::initializer_list<wrapped<V>> x) {
        for (auto &v : x) *this = insert (v);
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    const V inline &tree<V, T>::root () const {
        return binary_search_tree<colored<V>, T>::root ().Value;
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T> inline tree<V, T>::left () const {
        return binary_search_tree<colored<V>, T>::left ();
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T> inline tree<V, T>::right () const {
        return binary_search_tree<colored<V>, T>::right ();
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    template <typename ...P>
    tree<V, T> inline tree<V, T>::insert (inserted<V> a, inserted<V> b, P... p) {
        return insert (a).insert (b, p...);
    }

    // member functions for the iterator
    template <Sortable V, functional::buildable_tree<colored<V>> T>
    typename tree<V, T>::iterator::reference inline tree<V, T>::iterator::operator * () const {
        return static_cast<const parent &> (*this)->Value;
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    typename tree<V, T>::iterator::pointer inline tree<V, T>::iterator::operator -> () const {
        return &static_cast<const parent &> (*this)->Value;
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    bool inline tree<V, T>::iterator::operator == (const iterator i) const {
        return static_cast<const parent &> (*this) == static_cast<const parent &> (i);
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T>::iterator inline tree<V, T>::begin () const {
        return iterator {this, *this};
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    tree<V, T>::iterator inline tree<V, T>::end () const {
        return iterator {this, tree {}};
    }

    template <Sortable V, functional::buildable_tree<colored<V>> T>
    ordered_sequence<const V &> tree<V, T>::values () const {
        stack<const V &> st;
        for (const V &v : *this) st >>= v;
        return ordered_sequence<const V &> {reverse (st)};
    }

}

#endif
