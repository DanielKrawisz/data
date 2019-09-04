// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/bounded.hpp>

namespace data::math::number::bounded {
    
    template <typename indexed, uint32_t size>
    bool number<indexed, size, false>::operator<(const number& n) const {
        data::words a{*this};
        data::words b{n};
        for (uint32_t i = 0; i < size; i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        
        return false;
    }
    
    template <typename indexed, uint32_t size>
    bool number<indexed, size, false>::operator<=(const number& n) const {
        data::words a{*this};
        data::words b{n};
        for (uint32_t i = 0; i < size; i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        
        return true;
    }

}

