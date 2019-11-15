// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOLD
#define DATA_FOLD

#include "empty.hpp"
#include <type_traits>

namespace data {

    template <typename f, typename x, typename l>
    x fold(f fun, x init, l ls) {
        if (data::empty(ls)) return init;
        return fold(fun, fun(init, data::first(ls)), data::rest(ls));
    }
    
    template <typename f, typename l>
    typename std::invoke_result<f, 
        decltype(std::declval<l>().first()), 
        decltype(std::declval<l>().first())>::type reduce(f fun, l ls) {
        if (data::empty(ls)) return typename std::invoke_result<f, 
            decltype(std::declval<l>().first()), 
            decltype(std::declval<l>().first())>::type{};
        return fun(data::first(ls), reduce(fun, data::rest(ls)));
    }
    
    template <typename f, typename x>
    x nest(f fun, x init, uint32 rounds) {
        if (rounds == 0) return init;
        return nest(fun, fun(init), rounds - 1);
    }

}

#endif
