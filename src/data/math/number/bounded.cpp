// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/bounded.hpp>

namespace data::math::number::bounded {
    
    template <uint32_t size>
    bool number<size, false>::operator<(const number<size, false>& n) const {
        data::words a{*this};
        data::words b{n};
        for (uint32_t i = 0; i < size; i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        
        return false;
    }
    
    template <uint32_t size>
    bool number<size, false>::operator<=(const number<size, false>& n) const {
        data::words a{*this};
        data::words b{n};
        for (uint32_t i = 0; i < size; i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        
        return true;
    }
    
    template <uint32_t size>
    bool number<size, true>::operator<(const number<size, true>& n) const {
        throw 0;
    }
    
    template <uint32_t size>
    bool number<size, true>::operator<=(const number<size, true>& n) const {
        throw 0;
    }

}

