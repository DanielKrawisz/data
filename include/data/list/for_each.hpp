// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST_FOR_EACH
#define DATA_LIST_FOR_EACH

#include <data/stack.hpp>
#include <type_traits>

namespace data::functional::meta::list {
    
    template <typename function, typename from, typename to> 
    struct for_each : public interface::sequence<to> {
        using input_element = typename interface::sequence<from>::element;
        using output_element = std::invoke_result<function, input_element>;
        
        to operator()(function f, from l) {
            if (empty(l)) return {};
            return for_each(f, rest(l)) + f(*first(l));
        }
    };
    
}

#endif
