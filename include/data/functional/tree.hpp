// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE
#define DATA_TREE

#include <data/functional/list.hpp>
#include <data/functional/set.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data::interface {
        
    template <typename tree>
    concept has_left_method = requires (tree t) {
        { t.left() } -> std::convertible_to<tree>;
    };
    
    template <typename tree>
    concept has_right_method  = requires (tree t) {
        { t.right() } -> std::convertible_to<tree>;
    };
        
    template <typename tree, typename element>
    concept has_root_method = requires (tree t) {
        { t.root() } -> std::convertible_to<element>;
    };

    template <typename tree, typename element>
    concept has_tree_constructor = requires (element e, tree r, tree l) {
        { tree{e, r, l} };
    } && requires (element e) {
        { tree{e} };
    };
    
}
    
namespace data::functional {
    
    template <typename T, typename element = decltype(std::declval<T>().root())>
    concept tree = container<const T, element> && 
        interface::has_left_method<const T> && 
        interface::has_right_method<const T> && 
        interface::has_root_method<const T, element> && 
        interface::has_tree_constructor<T, element> && 
        std::default_initializable<T>;
    
    template <typename T, typename X> requires tree<T, X>
    T insert(T t, X x) {
        if (empty(t)) return T{x, T{}, T{}};
        X& r = root(t);
        if (x == r) return t;
        if (x < r) return T{r, insert(left(t), x), right(t)};
        if (x > r) return T{r, left(t), insert(right(t))};
    }
    
    template <typename value, typename tree>
    struct tree_node final {
        value Value;
        tree Left;
        tree Right;
        tree_node(const value& v, tree l, tree r) : Value{v}, Left{l}, Right{r} {}
    };
    
}

namespace data {

    template <typename X, typename elem> requires interface::has_root_method<X, elem>
    inline const decltype(std::declval<const X>().root()) root(const X& x) {
        return x.root();
    }
    
    template <typename X>
    inline X left(const X& x) {
        return x;
    }
    
    template <interface::has_left_method X>
    inline X left(const X& x) {
        return x.left();
    }
    
    template <typename X>
    inline X right(const X& x) {
        return x;
    }
    
    template <interface::has_right_method X>
    inline X right(const X& x) {
        return x.right();
    }

}

#endif
