// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SET
#define DATA_SET

#include <data/map.hpp>

namespace data {


    // set implemented as a map.
    template <typename X> using set = RB::tree<X, tree<RB::colored<X>>>;

    template <typename X> set<X> merge (set<X>, set<X>);
    template <typename X> set<X> remove (set<X>, const X &);

    template <typename X, Proposition<X> F>
    set<X> select (const set<X> &, F fun);

}

#endif
