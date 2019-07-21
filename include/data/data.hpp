// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

#include <data/types.hpp>
#include <data/math/number/bounded.hpp>

namespace data {
    
    template <typename X, typename Y>
    inline X plus(X a, Y b) {
        return a + b;
    }
    
    template <typename Any>
    bool valid(Any);
    
    template <typename List>
    List reverse(List);
    
    using uint160 = math::number::bounded::number<5, false>;
    using uint256 = math::number::bounded::number<8, false>;
    using uint512 = math::number::bounded::number<16, false>;
    
    using int160 = math::number::bounded::number<5, true>;
    using int256 = math::number::bounded::number<8, true>;
    using int512 = math::number::bounded::number<16, true>;

}

#endif
