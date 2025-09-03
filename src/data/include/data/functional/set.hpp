// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_SET
#define DATA_FUNCTIONAL_SET

#include <data/container.hpp>

namespace data { 
    
    template <typename X, typename element>
    concept Set = Container<X, element> && interface::has_insert_method<X, element> && std::default_initializable<X>;
    
    template <typename X, typename element = decltype (std::declval<X> ().values ().first ())>
    concept OrderedSet = Set<X, element> && interface::has_values_method<X> && std::totally_ordered<element>;
    
}

#endif
