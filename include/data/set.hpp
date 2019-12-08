// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SET
#define DATA_SET

#include "interface.hpp"

namespace data {
    
    namespace interface {
        
        template <typename X>
        struct set {
            using element = typename container<X>::element;
            using values = typename container<X>::values;
        private:
            using require_default_constructable = typename std::enable_if<std::is_default_constructible<X>::value, bool>::type;
            using require_insert_and_remove = typename std::enable_if<
                meta::has_insert_method<X, element>::value && 
                meta::has_remove_method<X, element>::value, bool>::type;
        }; 
        
    }
    
}

#endif
