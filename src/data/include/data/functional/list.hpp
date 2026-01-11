// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_LIST
#define DATA_FUNCTIONAL_LIST

#include <data/functional/stack.hpp>
#include <data/functional/queue.hpp>

namespace data {
    
    template <typename L, typename elem = first_return_type<L>>
    concept List = Stack<L, elem> && Queue<L, elem>;
    
    template <typename L, typename elem = first_return_type<L>>
    concept Pendable = Stack<L, elem> || Queue<L, elem>;
    
    template <Pendable list>
    list inline join (const list &a, const list &b) {
        if constexpr (Queue<list>) return functional::join_queue (a, b);
        else return functional::join_stack (a, b);
    }

    template <Pendable list, typename elem>
    list inline erase (const list &x, const elem &e) {
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

