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
        if (empty (t)) return T {x, T {}, T {}};
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


#endif
