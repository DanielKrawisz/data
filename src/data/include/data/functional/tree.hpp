// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_TREE
#define DATA_FUNCTIONAL_TREE

#include <data/concepts.hpp>
#include <data/functional/list.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data::interface {
        
    template <typename tree>
    concept has_left_method = requires (tree t) {
        { t.left () } -> implicitly_convertible_to<tree>;
    };
    
    template <typename tree>
    concept has_right_method  = requires (tree t) {
        { t.right () } -> implicitly_convertible_to<tree>;
    };
        
    template <typename tree>
    concept has_root_method = requires (tree t) {
        { t.root () };
    };

    template <typename tree, typename element>
    concept has_tree_constructor = requires (element e, tree r, tree l) {
        { tree {e, r, l} };
    } && requires (element e) {
        { tree {e} };
    };
    
}

namespace data {

    template <interface::has_root_method X> 
    inline const decltype (std::declval<const X> ().root ()) root (const X &x) {
        return x.root ();
    }
    
    template <typename X>
    inline X left (const X &x) {
        return x;
    }
    
    template <interface::has_left_method X>
    inline X left (const X &x) {
        return x.left ();
    }
    
    template <typename X>
    inline X right (const X &x) {
        return x;
    }
    
    template <interface::has_right_method X>
    inline X right (const X &x) {
        return x.right ();
    }
}
    
namespace data::functional {
    
    template <typename T, typename element = decltype (std::declval<T> ().root ())>
    concept tree =
        interface::has_left_method<const T> && 
        interface::has_right_method<const T> && 
        interface::has_root_method<const T>;
    
    template <typename T, typename element = element_of<T>>
    concept search_tree = tree<T, element> && ordered<element>;

    template <typename T, typename X> requires search_tree<T, X>
    unref<X> inline *contains (T t, X x) {
        return empty (t) ? nullptr: x == root (t) ? &root (t):
            x < root (t) ? contains (left (t), x) : contains (right (t), x);
    }
    
    template <typename T, typename element = element_of<T>>
    concept buildable_tree = tree<T, element> && 
        interface::has_tree_constructor<T, element> && 
        std::default_initializable<T>;

    template <typename T, typename X = element_of<T>>
    requires search_tree<T, X> && buildable_tree<T, X>
    T inline insert (T t, X x) {
        return empty (t) ? T {x, T {}, T {}}:
            x == root (t) ? t:
                x < root (t) ? T {root (t), insert (left (t), x), right (t)}:
                    T {root (t), left (t), insert (right (t), x)};
    }
    
    template <typename tree, typename P> 
    void for_each_infix (const tree &t, P f) {
        if (data::empty (t)) return;
        for_each_infix (data::left (t), f);
        f (data::root (t));
        for_each_infix (data::right (t), f);
    }
    
    template <typename tree, typename P> 
    void for_each_prefix (const tree &t, P f) {
        if (data::empty (t)) return;
        f (data::root (t));
        for_each_prefix (data::left (t), f);
        for_each_prefix (data::right (t), f);
    }
    
    template <functional::stack out, functional::tree in> 
    out values_infix (const in &t) {
        out o {};
        functional::for_each_infix (t, [&o] (const element_of<in> &x) -> void {
            o = prepend (o, x);
        });
        return reverse (o);
    }
    
    template <functional::stack out, functional::tree in> 
    out values_prefix (const in &t) {
        out o {};
        functional::for_each_prefix (t, [&o] (const element_of<in> &x) -> void {
            o = prepend (o, x);
        });
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
    template <ordered X, tree<X> T>
    struct binary_search_iterator {
        using sentinel = data::sentinel<T>;

        using value_type = unref<X>;
        using difference_type = int;

        const T *Tree;
        T Next;
        linked_stack<const T> Last;

        // we need this constructor in order to satisfy some
        // std concepts but it's not really good for anything.
        binary_search_iterator ();

        // for the beginning, the second argument should be the tree itself.
        // for the end, the second argument should be the empty tree.
        binary_search_iterator (const T *m, const T &n);

        binary_search_iterator operator ++ (int);
        binary_search_iterator &operator ++ ();

        const unref<X> &operator * () const;
        const unref<X> *operator -> () const;

        bool operator == (const binary_search_iterator &i) const;

        bool operator == (const sentinel i) const;

    private:
        void go_left ();
    };
}

namespace data {

    template <functional::tree X>
    struct element<X> {
        using type = unref<decltype (std::declval<X> ().root ())>;
    };

}

template <data::functional::tree X, data::functional::tree Y> requires std::equality_comparable_with<data::element_of<X>, data::element_of<Y>>
bool inline operator == (const X &a, const X &b) {
    return &a == &b ? true :
        data::empty (a) && data::empty (b) ? true :
            data::empty (a) || data::empty (b) ? false :
                data::root (a) != data::root (b) ? false :
                    data::left (a) == data::left (b) && data::right (a) == data::right (b);
}

namespace data::functional {
    template <ordered X, tree<X> T>
    binary_search_iterator<X, T>
    binary_search_iterator<X, T>::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <ordered X, tree<X> T>
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

    template <ordered X, tree<X> T>
    const unref<X> inline &binary_search_iterator<X, T>::operator * () const {
        return Next.root ();
    }

    template <ordered X, tree<X> T>
    const unref<X> inline *binary_search_iterator<X, T>::operator -> () const {
        return &Next.root ();
    }

    template <ordered X, tree<X> T>
    bool inline binary_search_iterator<X, T>::operator == (const binary_search_iterator &i) const {
        return Tree == i.Tree && Next == i.Next;
    }

    template <ordered X, tree<X> T>
    void binary_search_iterator<X, T>::go_left () {
        if (Next.empty ()) return;
        while (!Next.left ().empty ()) {
            Last = Last << Next;
            Next = Next.left ();
        }
    }

    template <ordered X, tree<X> T>
    inline binary_search_iterator<X, T>::binary_search_iterator () : Tree {nullptr}, Next {}, Last {} {}

    template <ordered X, tree<X> T>
    inline binary_search_iterator<X, T>::binary_search_iterator (const T *m, const T &n) : Tree {m}, Next {n}, Last {} {
        go_left ();
    }
}

#endif
