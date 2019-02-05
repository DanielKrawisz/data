#ifndef DATA_LIST_FOR_EACH_HPP
#define DATA_LIST_FOR_EACH_HPP

#include <data/list.hpp>
#include <type_traits>

namespace data {
    
    namespace list {
                
        template <typename function, typename from, typename to> 
        struct for_each : public is_buildable<to> {
            using input_element = typename is_list<from>::element;
            using output_element = std::invoke_result<function, input_element>;
            
            to operator()(function f, from l) {
                if (empty(l)) return {};
                return for_each(f, rest(l)) + f(*first(l));
            }
        };
        
    }

}

#endif
