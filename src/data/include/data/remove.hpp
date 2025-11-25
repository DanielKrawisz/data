// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_REMOVE
#define DATA_REMOVE

#include <data/reverse.hpp>
#include <data/container.hpp>
#include <data/for_each.hpp>
#include <data/io/unimplemented.hpp>

namespace data {

    // we already have a remove for Pendable types.
    template <typename X, typename E> X remove (const X &x, const E &e);
}

namespace data::functional {
    template <Stack list>
    list remove_stack (const list &ll, size_t e) {
        list result;
        list x = ll;
        size_t i = 0;
        while (!empty (x)) {
            if (i != e) result >>= first (x);
            x = rest (x);
            i++;
        }
        return reverse (result);
    }
}

namespace data {

    template <typename X, typename E> requires Indexed<X, E>
    X remove (const X &x, const E &e) {
        if constexpr (requires () {
            { x.remove (e) } -> ImplicitlyConvertible<const X>;
        }) {
            return x.remove (e);
        } else if constexpr (Queue<X>) {
            return functional::remove_queue (x, e);
        } else if constexpr (Stack<X>) {
            return functional::remove_stack (x, e);
        } else if constexpr (Iterable<X>) {
            throw method::unimplemented {"Remove from iterable"};
        } else {
            throw data::exception {} << "cannot construct remove method";
        }
    }

}

#endif


