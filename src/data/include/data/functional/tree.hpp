// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_TREE
#define DATA_FUNCTIONAL_TREE

#include <data/types.hpp>
#include <data/concepts.hpp>
#include <data/functional/list.hpp>
#include <data/stack.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data::interface {
        
    template <typename tree>
    concept has_left_method = requires (tree t) {
        { t.left () } -> ImplicitlyConvertible<tree>;
    };
    
    template <typename tree>
    concept has_right_method  = requires (tree t) {
        { t.right () } -> ImplicitlyConvertible<tree>;
    };
        
    template <typename tree>
    concept has_root_method = requires (tree t) {
        { t.root () } -> Reference<>;
    };

    template <typename tree, typename element>
    concept has_tree_constructor = requires (element e, tree r, tree l) {
        { tree {e, r, l} };
    } && requires (element e) {
        { tree {e} };
    };
    
}

namespace data {

    template <typename X> requires interface::has_root_method<const X>
    decltype (auto) inline root (X &&x) {
        return x.root ();
    }
    
    template <typename X>
    inline X left (const X &x) {
        return x;
    }

    template <typename X>
    inline X right (const X &x) {
        return x;
    }

    template <interface::has_left_method X>
    X inline left (const X &x) {
        return x.left ();
    }

    template <interface::has_right_method X>
    X inline right (const X &x) {
        return x.right ();
    }

    template <typename T, typename element = decltype (std::declval<T> ().root ())>
    concept Tree =
        interface::has_left_method<T> &&
        interface::has_right_method<T> &&
        interface::has_root_method<T> && requires (T t) {
            { t.root () } -> ImplicitlyConvertible<element>;
        };

}
    
namespace data::functional {
    
    template <typename T, typename element = decltype (std::declval<T> ().root ())>
    concept search_tree = Tree<const T, element> && Ordered<element>;

    template <typename T, typename X> requires search_tree<T, X>
    unref<X> inline *contains (const T t, X x) {
        return data::empty (t) ? nullptr: x == root (t) ? &root (t):
            x < root (t) ? contains (left (t), x) : contains (right (t), x);
    }
    
    template <typename T, typename element = decltype (std::declval<T> ().root ())>
    concept buildable_tree = Tree<T, element> &&
        interface::has_tree_constructor<T, element> && 
        std::default_initializable<T>;

    template <typename T, typename X, typename already_exists>
    requires search_tree<T, X> && buildable_tree<T, X>
    T inline insert (const T t, X x, already_exists if_equivalent) {
        return data::empty (t) ? T {x, T {}, T {}}:
            x <=> data::root (t) == 0 ? T {if_equivalent (root (t), x), left (t), right (t)}:
                x < data::root (t) ? T {data::root (t), insert<T, X> (left (t), x, if_equivalent), right (t)}:
                    T {data::root (t), left (t), insert<T, X> (right (t), x, if_equivalent)};
    }

    template <typename value>
    const value &keep_old (const value &old_val, const value &new_val) {
        return old_val;
    }

    template <typename value>
    const value &replace_new (const value &old_val, const value &new_val) {
        return new_val;
    }

    template <typename T, typename X = decltype (std::declval<T> ().root ())>
    requires search_tree<T, X> && buildable_tree<T, X>
    T inline remove (const T t, inserted<X> x) {
        struct combine {
            T operator () (const T left, const T right) {
                if (data::empty (left)) return right;
                if (data::empty (right)) return left;
                return T {data::root (left), data::left (left), combine {} (data::right (left), right)};
            }
        };

        return data::empty (t) ? t:
            x <=> data::root (t) == 0 ? combine {} (left (t), right (t)):
                x < data::root (t) ? T {data::root (t), remove<T, X> (left (t), x), right (t)}:
                    T {data::root (t), left (t), remove<T, X> (right (t), x)};
    }
    
    template <typename tree, typename P> 
    void for_each_infix (P f, const tree &t) {
        if (data::empty (t)) return;
        for_each_infix (f, data::left (t));
        f (t.root ());
        for_each_infix (f, data::right (t));
    }
    
    template <typename tree, typename P> 
    void for_each_prefix (P f, const tree &t) {
        if (data::empty (t)) return;
        f (t.root ());
        for_each_prefix (f, data::left (t));
        for_each_prefix (f, data::right (t));
    }

    template <typename tree, typename P>
    void for_each_infix (P f, tree &t) {
        if (data::empty (t)) return;
        auto l = t.left ();
        for_each_infix (f, l);
        f (t.root ());
        auto r = t.right ();
        for_each_infix (f, r);
    }

    template <typename tree, typename P>
    void for_each_prefix (P f, tree &t) {
        if (data::empty (t)) return;
        f (t.root ());
        auto l = t.left ();
        for_each_prefix (f, l);
        auto r = t.right ();
        for_each_prefix (f, r);
    }
    
    template <Stack out, Tree in>
    out values_infix (const in &t) {
        out o {};
        for_each_infix ([&o] (const auto &x) -> void {
            o = prepend (o, x);
        }, t);
        return reverse (o);
    }
    
    template <Stack out, Tree in>
    out values_prefix (const in &t) {
        out o {};
        for_each_prefix ([&o] (const auto &x) -> void {
            o = prepend (o, x);
        }, t);
        return reverse (o);
    }

    // a node that could be used to construct a tree.
    template <typename value, typename tree>
    struct tree_node final {
        value Value;
        tree Left;
        tree Right;
        tree_node (const value &v, tree l, tree r) : Value {v}, Left {l}, Right {r} {}
    };

    // an iterator for a tree that treats it as a binary search tree
    // since there are other ways that the elements of a tree could
    // be organized, we don't have any natural iterator for a tree.
    template <Ordered X, Tree<X> T> struct binary_search_iterator;

    template <Ordered X, Tree<X> T>
    bool operator == (const binary_search_iterator<X, T> &i, const binary_search_iterator<X, T> &j);

    template <Ordered X, Tree<X> T> struct binary_search_iterator {
        using sentinel = data::sentinel<T>;

        using iterator_category = std::forward_iterator_tag;
        using value_type        = unref<X>;
        using reference         = X &;
        using pointer           = value_type *;
        using difference_type   = int;

        const T *Tree;
        T Next;
        stack<const T> Last;

        // we need this constructor in order to satisfy some
        // std concepts but it's not really good for anything.
        binary_search_iterator ();

        // for the beginning, the second argument should be the tree itself.
        // for the end, the second argument should be the empty tree.
        binary_search_iterator (const T *m, const T &n);

        binary_search_iterator operator ++ (int);
        binary_search_iterator &operator ++ ();

        reference operator * () const;
        pointer operator -> () const;

    private:
        void go_left ();
    };

    template <data::Tree X, data::Tree Y>
    requires std::equality_comparable_with<decltype (std::declval<X> ().root ()), decltype (std::declval<Y> ().root ())>
    bool inline tree_equal (const X &a, const X &b) {
        return &a == &b ? true :
            data::empty (a) && data::empty (b) ? true :
                data::empty (a) || data::empty (b) ? false :
                    data::root (a) != data::root (b) ? false :
                        data::left (a) == data::left (b) && data::right (a) == data::right (b);
    }

    template <Ordered X, Tree<X> T>
    binary_search_iterator<X, T>
    binary_search_iterator<X, T>::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <Ordered X, Tree<X> T>
    binary_search_iterator<X, T>
    &binary_search_iterator<X, T>::operator ++ () {
        if (Next.empty ()) return *this;

        if (!Next.right ().empty ()) {
            Next = Next.right ();
            go_left ();
            return *this;
        }

        if (!data::empty (Last)) {
            Next = Last.first ();
            Last = Last.rest ();
            return *this;
        }

        Next = T {};
        return *this;
    }

    template <Ordered X, Tree<X> T>
    binary_search_iterator<X, T>::reference inline binary_search_iterator<X, T>::operator * () const {
        return Next.root ();
    }

    template <Ordered X, Tree<X> T>
    binary_search_iterator<X, T>::pointer inline binary_search_iterator<X, T>::operator -> () const {
        return &Next.root ();
    }

    template <Ordered X, Tree<X> T>
    bool operator == (const binary_search_iterator<X, T> &i, const binary_search_iterator<X, T> &j) {
        return j.Tree == i.Tree && j.Next == i.Next;
    }

    template <Ordered X, Tree<X> T>
    void binary_search_iterator<X, T>::go_left () {
        if (Next.empty ()) return;
        while (!Next.left ().empty ()) {
            Last >>= Next;
            Next = Next.left ();
        }
    }

    template <Ordered X, Tree<X> T>
    inline binary_search_iterator<X, T>::binary_search_iterator () : Tree {nullptr}, Next {}, Last {} {}

    template <Ordered X, Tree<X> T>
    inline binary_search_iterator<X, T>::binary_search_iterator (const T *m, const T &n) : Tree {m}, Next {n}, Last {} {
        go_left ();
    }
}

#endif
