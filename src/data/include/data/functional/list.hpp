// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_LIST
#define DATA_FUNCTIONAL_LIST

#include <data/functional/stack.hpp>
#include <data/functional/queue.hpp>
#include <data/cross.hpp>

namespace data {
    
    template <typename Q>
    concept List = Stack<Q> && Queue<Q>;
    
    template <typename Q>
    concept Pendable = Stack<Q> || Queue<Q>;
    
    template <Pendable list>
    list take (const list &l, size_t x);

    template <Pendable list>
    list inline take (const list &l, size_t from, size_t to) {
        return take (drop (l, from), to - from);
    }
    
    template <Pendable list>
    list inline join (const list &a, const list &b) {
        if constexpr (Queue<list>) return functional::join_queue (a, b);
        else return functional::join_stack (a, b);
    }

    template <Pendable list>
    list inline remove (const list &l, size_t x) {
        return join (take (l, x), drop (l, x + 1));
    }

    template <Pendable list, typename elem>
    list inline erase (const list &x, elem e) {
        if constexpr (Queue<list>) return functional::erase_queue (x, e);
        else return functional::erase_stack (x, e);
    }
}

namespace data::functional {
    
    template <Pendable list> requires Ordered<decltype (std::declval<list> ().first ())>
    list merge (const list &a, const list &b) {
        if constexpr (Queue<list>) return functional::merge_queue (a, b);
        else return functional::merge_stack (a, b);
    }

}

namespace data {

    template <Pendable list> requires Ordered<decltype (std::declval<list> ().first ())>
    list merge_sort (const list &x) {
        size_t z = size (x);
        if (z < 2) return x;

        size_t half = z / 2;
        return functional::merge (merge_sort (take (x, half)), merge_sort (drop (x, half)));
    }

    template <List L, typename elem>
    L riffle (L l, const elem &e) {
        L result {};
        while (l.size () > 1) {
            result <<= l.first ();
            result <<= e;
            l = l.rest ();
        }
        if (l.size () == 1) result <<= l.first ();
        return result;
    }
}

#endif

