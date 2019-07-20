// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/bounded.hpp>

namespace data::math::number {
            
    template <uint32 size> 
    bool bounded<size>::operator<(const bounded& n) const {
        for (uint32_t i = 0; i < size; i++) {
            if (operator[](i) < n[i]) return true;
            if (operator[](i) > n[i]) return false;
        }
        
        return false;
    }
    
    template <uint32 size> 
    bool bounded<size>::operator<=(const bounded& n) const {
        for (uint32_t i = 0; i < size; i++) {
            if (operator[](i) > n.get(i)) return false;
            if (operator[](i) < n.get(i)) return true;
        }
        
        return true;
    }
    
    template <uint32 size> 
    void bounded<size>::minus(const bounded& a, const bounded& b, bounded& result) {
        uint32 remainder{0};
        for (int32 i = last; i >= 0; i--) {
            word w = extend(a[i]) - extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32 size> 
    void bounded<size>::plus(const bounded& a, const bounded& b, bounded& result) {
        uint32 remainder{0};
        for (int32 i = last; i >= 0; i--) {
            word w = extend(a[i]) + extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }

}

