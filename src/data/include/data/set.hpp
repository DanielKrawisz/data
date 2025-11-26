// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SET
#define DATA_SET

#include <data/tools/rb.hpp>
#include <data/tree.hpp>

namespace data {

    // set implemented as a map.
    template <typename X> using set = RB::tree<X, tree<RB::colored<X>>>;

    template <typename X> set<X> merge (const set<X> &, const set<X> &);
    template <typename X> set<X> remove (const set<X> &, const X &);
    template <typename X> set<X> erase (const set<X> &, const X &);

    template <typename X> set<X> inline merge (const set<X> &x, const set<X> &e) {
        return RB::merge (x, e);
    }

    template <typename X> set<X> inline remove (const set<X> &x, const X &e) {
        return RB::remove (x, e);
    }

    template <typename X> set<X> inline erase (const set<X> &x, const X &e) {
        return RB::erase (x, e);
    }

}

#endif
