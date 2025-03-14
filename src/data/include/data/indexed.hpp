// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_INDEXED
#define DATA_INDEXED

#include <data/types.hpp>
#include <data/concepts.hpp>

namespace data {
    
    template <typename M, typename key, typename value = decltype (std::declval<const M> () [std::declval<const key> ()])>
    concept indexed = requires (M m, key k) {
        { m[k] } -> implicitly_convertible_to<value>;
    };
    
}

#endif

