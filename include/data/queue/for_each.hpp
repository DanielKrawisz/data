// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_QUEUE_FOR_EACH
#define DATA_QUEUE_FOR_EACH

#include <data/list.hpp>
#include <data/fold.hpp>
#include <type_traits>

namespace data::functional::meta::queue {
    
    template <typename function, typename Q, typename X>
    struct fold_queue {
        function Function;
        
        Q operator()(Q q, X x) {
            return append(q, Function(x));
        }
    };
    
    template <typename function, typename from, typename to> 
    struct for_each : public interface::list<to> {
        using input_element = typename interface::list<from>::element;
        using output_element = std::invoke_result<function, input_element>;
        
        to operator()(function f, from l) {
            return functional::fold(fold_queue<function, to, input_element>{f}, l, to{});
        }
    };
    
}

#endif
