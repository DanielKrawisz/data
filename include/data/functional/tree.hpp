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
        { t.left () } -> convertible_to<tree>;
    };
    
    template <typename tree>
    concept has_right_method  = requires (tree t) {
        { t.right () } -> convertible_to<tree>;
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
    concept tree = container<const T, element> && 
        interface::has_left_method<const T> && 
        interface::has_right_method<const T> && 
        interface::has_root_method<const T>;
    
    template <typename T, typename element = decltype (std::declval<T> ().root ())>
    concept search_tree = tree<T, element> && ordered<element>;
    
    template <typename T, typename element = decltype (std::declval<T> ().root ())>
    concept buildable_tree = tree<T, element> && 
        interface::has_tree_constructor<T, element> && 
        std::default_initializable<T>;
    
    template <typename T, typename X> requires search_tree<T, X> && buildable_tree<T, X>
    T insert (T t, X x) {
        if (empty (t)) return T {x, T {}, T{}};
        X &r = root (t);
        if (x == r) return t;
        if (x < r) return T {r, insert (left (t), x), right (t)};
        if (x > r) return T {r, left (t), insert (right (t))};
    }
    
    template <typename value, typename tree>
    struct tree_node final {
        value Value;
        tree Left;
        tree Right;
        tree_node (const value &v, tree l, tree r) : Value {v}, Left {l}, Right {r} {}
    };
    
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
    
}

namespace data {
    
    template <functional::tree X>
    struct element<X> {
        using type = std::remove_reference_t<decltype (std::declval<X> ().root ())>;
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
    
    template <typename T> 
    struct tree_iterator {
        const T *Tree;
        T Current;
        linked_stack<const T> Prev;
        int Index;
        
        tree_iterator (const T &s, T n, int i) : Tree {&s}, Current {n}, Prev {}, Index {i} {}
        
        tree_iterator () : Tree {}, Current {}, Prev {}, Index {0} {}
        tree_iterator (const T &t);
        
        tree_iterator &operator = (const tree_iterator &n);
        
        tree_iterator operator ++ (int);
        tree_iterator &operator ++ ();
        
        const element_of<T> &operator * () const;
        const element_of<T> *operator -> () const;
        
        bool operator == (const sentinel<T> i) const;
        bool operator != (const sentinel<T> i) const;
        
        bool operator == (const tree_iterator i) const;
        
        int operator - (const tree_iterator& i) const;
        
    private:
        void go_left ();
    };
    
}

namespace std {
    template <typename tree> 
    struct iterator_traits<data::functional::tree_iterator<tree>> {
        using value_type = remove_const_t<data::element_of<tree>>;
        using difference_type = int;
        using pointer = const remove_reference_t<data::element_of<tree>> *;
        using reference = const data::element_of<tree> &;
        using iterator_concept = input_iterator_tag;
    };
}
    
namespace data::functional {
    template <typename T> 
    inline tree_iterator<T>::tree_iterator (const T &t) : Tree {&t}, Current {t}, Prev {}, Index {0} {
        go_left ();
    } 
    
    template <typename T> 
    tree_iterator<T> &tree_iterator<T>::operator = (const tree_iterator &n) {
        Tree = n.Tree;
        Current = n.Current;
        Prev = n.Prev;
        Index = n.Index;
    }
    
    template <typename T> 
    tree_iterator<T> tree_iterator<T>::operator ++ (int) {
        auto z = *this;
        ++(*this);
        return z;
    }
    
    template <typename T> 
    tree_iterator<T> &tree_iterator<T>::operator ++ () {
        if (data::empty (Current)) return *this;
        
        Index++;
        if (!data::empty(right (Current))) {
            Current = right (Current);
            go_left ();
        } else if (!data::empty (Prev)) {
            Current = first (Prev);
            Prev = rest (Prev);
        } else Current = T {};
        return *this;
    }
    
    template <typename T> 
    const element_of<T> inline &tree_iterator<T>::operator * () const {
        return Current.root ();
    }

    template <typename T>
    const element_of<T> inline *tree_iterator<T>::operator -> () const {
        return *Current.root ();
    }
    
    template <typename T> 
    bool inline tree_iterator<T>::operator == (const sentinel<T> i) const {
        return Tree == i.Structure && Index == data::size (Current);
    }
    
    template <typename T> 
    bool inline tree_iterator<T>::operator != (const sentinel<T> i) const {
        return !(*this == i);
    }
    
    template <typename T> 
    bool inline tree_iterator<T>::operator == (const tree_iterator i) const {
        return Tree == i.Tree && Index == i.Index;
    }
    
    template <typename T> 
    int inline tree_iterator<T>::operator - (const tree_iterator &i) const {
        return Index - i.Index;
    }
    
    template <typename T> 
    void tree_iterator<T>::go_left () {
        if (data::empty (Current)) return;
        
        while (!data::empty (left (Current))) {
            Prev = prepend (Prev, Current);
            Current = left (Current);
        }
    }
    
}

#endif
