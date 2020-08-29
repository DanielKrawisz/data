// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST_FOR_EACH
#define DATA_LIST_FOR_EACH

#include <type_traits>
#include <data/stack.hpp>

namespace data::meta::functional::list {
    
    template <typename function, typename from, typename to> requires data::sequence<to>
    struct for_each {
        using input_element = decltype(std::declval<from>().first());
        using output_element = std::invoke_result<function, input_element>;
        
        to operator()(function f, from l) {
            if (empty(l)) return {};
            return for_each(f, rest(l)) + f(*first(l));
        }
    };
    
}

#endif
