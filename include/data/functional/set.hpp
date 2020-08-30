// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SET
#define DATA_SET

#include <data/container.hpp>

namespace data::functional { 
    
    template <typename X, typename element>
    concept set = container<X, element> && interface::has_insert_method<const X, element> && 
        interface::has_remove_method<const X, element> && std::default_initializable<X>;
    
    template <typename X, typename element = decltype(std::declval<X>().values().first())>
    concept ordered_set = set<const X, element> && interface::has_values_method<const X, element> && std::totally_ordered<element>;
    
}

template <typename X, typename element> requires data::functional::set<X, element>
inline std::ostream& operator<<(std::ostream& o, const X& x) {
    return o << x.values();
}

#endif
