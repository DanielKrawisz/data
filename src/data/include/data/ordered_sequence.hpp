// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ORDERED_SEQUENCE
#define DATA_ORDERED_SEQUENCE

#include <data/stack.hpp>

// implementation of List
#include <data/tools/ordered_list.hpp>

namespace data {

    // functional queue built using the list. 
    template <Sortable X> using ordered_sequence = tool::ordered_stack<stack<X>, X>;

    template <typename elem, typename E>
    ordered_sequence<elem> erase (const ordered_sequence<elem> &x, const E &);

    template <typename elem, typename P>
    ordered_sequence<elem> select (const ordered_sequence<elem> &x, P &&satisfies);

    template <typename elem, typename P>
    ordered_sequence<elem> select (const ordered_sequence<elem> &x, P &&satisfies) {
        stack<const elem &> n;
        for (const elem &e : x) if (satisfies (e)) n >>= e;
        ordered_sequence<elem> r;
        for (const elem &e : n) r = r.insert (e);
        return r;
    }

    template <typename elem, typename E>
    ordered_sequence<elem> inline erase (const ordered_sequence<elem> &x, const E &v) {
        return select (x, [&v] (const elem &e) -> bool {
            return v != e;
        });
    }
    
}

#endif
