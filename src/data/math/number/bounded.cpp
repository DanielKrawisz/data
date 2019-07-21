// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/bounded.hpp>

namespace data::math::number::bounded {
            
    template <uint32_t size>
    bool number<size, false>::operator<(const number<size, false>& n) const {
        for (uint32_t i = 0; i < size; i++) {
            if (array::operator[](i) < n[i]) return true;
            if (array::operator[](i) > n[i]) return false;
        }
        
        return false;
    }
    
    template <uint32_t size>
    bool number<size, false>::operator<=(const number<size, false>& n) const {
        for (uint32_t i = 0; i < size; i++) {
            if (array::operator[](i) > n.get(i)) return false;
            if (array::operator[](i) < n.get(i)) return true;
        }
        
        return true;
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    void array<size, bounded, bit32, bit64>::minus(const bounded& a, const bounded& b, bounded& result) {
        bit32 remainder{0};
        for (int32 i = last; i >= 0; i--) {
            word w = extend(a[i]) - extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64> 
    void array<size, bounded, bit32, bit64>::plus(const bounded& a, const bounded& b, bounded& result) {
        bit32 remainder{0};
        for (int32 i = last; i >= 0; i--) {
            word w = extend(a[i]) + extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }

}

