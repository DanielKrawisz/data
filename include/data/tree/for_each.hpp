// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE_FOR_EACH
#define DATA_TREE_FOR_EACH

#include <data/tree.hpp>
#include <type_traits>

namespace data::meta::functional::tree {
    
    template <typename function, typename from, typename to> 
    struct for_each : public interface::tree<from> {
        using input_element = decltype(std::declval<from>().root());
        using output_element = std::invoke_result<function, input_element>;
        
        to operator()(function f, from l) {
            if (empty(l)) return {};
            return {f(root(l)), for_each{}(left(l)), for_each{}(right(l))};
        }
    };
    
}

#endif
