// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SET
#define DATA_SET

#include "interface.hpp"

namespace data::interface {
        
    template <typename X, typename element = decltype(std::declval<X>().values().first())> requires container<X, element>
    struct set {
        //using values = typename container<X>::values;
        // TODO ensure values is a list of elements. 
    private:
        using require_default_constructable = typename std::enable_if<std::is_default_constructible<X>::value, void>::type;
        using require_insert_method = typename std::enable_if<
            meta::has_insert_method<X, element>::value, void>::type;
        /*using require_remove_method = typename std::enable_if<
            meta::has_remove_method<X, element>::value, void>::type;*/
    }; 
    
}

#endif
