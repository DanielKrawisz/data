// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE
#define DATA_TREE

#include <data/list.hpp>
#include <data/set.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data::meta {
        
    template <typename list, typename element>
    class has_root_method {
        template <typename X> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const X>().root()), element>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
    };

    template <typename list, typename element>
    class has_tree_constructor {
        template <typename X> static auto test(int) -> decltype((void)(X{std::declval<const element>(), std::declval<const X>(), std::declval<const X>()}), yes());
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
    };
    
}
    
namespace data::interface {
    
    // need constructor. 
    template <typename T, typename element = decltype(std::declval<T>().root())> requires container<T, element> && std::totally_ordered<element>
    struct tree {
    private:
        using require_left_method = typename std::enable_if<meta::has_left_method<T>::value, bool>::type;
        using require_right_method = typename std::enable_if<meta::has_right_method<T>::value, bool>::type;
        using require_constructor = typename std::enable_if<meta::has_tree_constructor<T, element>::value, bool>::type;
    }; 
}

namespace data::functional::tree {
    
    template <typename T, typename X>
    T insert(T t, X x) {
        if (empty(t)) return T{x, T{}, T{}};
        X& r = root(t);
        if (x == r) return t;
        if (x < r) return T{r, insert(left(t), x), right(t)};
        if (x > r) return T{r, left(t), insert(right(t))};
    }
    
    template <typename value, typename tree>
    struct node final {
        value Value;
        tree Left;
        tree Right;
        
        node(const value& v, tree l, tree r) : Value{v}, Left{l}, Right{r} {}
    };
    
}

namespace data {

    template <typename X>
    inline const decltype(std::declval<const X>().root()) root(const X& x) {
        return x.root();
    }

}

#endif
