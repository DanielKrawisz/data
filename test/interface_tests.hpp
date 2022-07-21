// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iterator>
#include "data/tools.hpp"

namespace data {

    template <sequence X>
    void is_sequence() {}

    template <typename X, typename element> requires container<X, element>
    void is_container() {}

    template <typename X, typename elem>
    requires iterable<X, elem>
    void is_iterable() {}

    template <const_iterable X> 
    void is_const_iterable() {}
    
    template <functional::stack X>
    void is_stack() {}

    template <functional::list X>
    void is_list() {}

    template <functional::tree X>
    void is_tree() {}

    template <functional::map X>
    void is_map() {}

    template <functional::ordered_set X>
    void is_ordered_set() {}
    
    template <std::input_iterator X>
    void is_input_iterator() {}
    
    template <typename X, typename elem> requires std::output_iterator<X, elem>
    void is_output_iterator() {}
    
    template <std::bidirectional_iterator X>
    void is_bidirectional_iterator() {}
    
}
