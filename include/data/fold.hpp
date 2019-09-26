// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOLD
#define DATA_FOLD

#include "types.hpp"
#include <type_traits>

namespace data {

    template <typename f, typename x, typename l>
    x fold(f fun, x init, l ls) {
        if (empty(ls)) return init;
        return fold(fun, fun(init, first(ls)), rest(ls));
    }
    
    template <typename f, typename l>
    typename std::invoke_result<f, 
        decltype(std::declval<l>().first()), 
        decltype(std::declval<l>().first())>::type reduce(f fun, l ls) {
        if (empty(ls)) return typename std::invoke_result<f, 
            decltype(std::declval<l>().first()), 
            decltype(std::declval<l>().first())>::type{};
        return fun(first(ls), reduce(fun, rest(ls)));
    }

}

#endif
