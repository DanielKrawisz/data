#ifndef DATA_TREE_FOR_EACH_HPP
#define DATA_TREE_FOR_EACH_HPP

#include <data/tree.hpp>
#include <type_traits>

namespace data {
    
    namespace tree {
                
        template <typename function, typename from, typename to> 
        struct for_each : public definition::buildable<to> {
            using input_element = typename definition::buildable<to>::element;
            using output_element = std::__invoke_result<function, input_element>;
            
            to operator()(function f, from l) {
                if (empty(l)) return {};
                return {f(root(l)), for_each{}(left(l)), for_each{}(right(l))};
            }
        };
        
    }

}

#endif
