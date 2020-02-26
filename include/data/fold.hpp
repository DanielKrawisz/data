// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOLD
#define DATA_FOLD

#include <data/interface.hpp>

namespace data::functional {

    template <typename x, typename f, typename l>
    inline x fold(f fun, l ls, x init) {
        if (data::empty(ls)) return init;
        return fold(fun, data::rest(ls), fun(init, data::first(ls)));
    }
    
    template <typename x, typename f>
    inline x nest(f fun, uint32 rounds, x init) {
        if (rounds == 0) return init;
        return nest(fun, fun(init), rounds - 1);
    }
    
    template <typename x, typename f, typename l>
    inline x reduce(f fun, l ls) {
        if (data::empty(ls)) return x{};
        return fun(data::first(ls), reduce(fun, data::rest(ls)));
    }

}

#endif
