// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/bounded.hpp>

namespace data::math::number::bounded {
    
    template <uint32_t size, typename bit32, typename bit64>
    void words<size, bit32, bit64>::minus(const words a, const words b, words result) {
        bit32 remainder{0};
        for (int32 i = last; i >= 0; i--) {
            word w = extend(a[i]) - extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32_t size, typename bit32, typename bit64> 
    void words<size, bit32, bit64>::plus(const words a, const words b, words result) {
        bit32 remainder{0};
        for (int32 i = last; i >= 0; i--) {
            word w = extend(a[i]) + extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32_t size, typename bit32, typename bit64> 
    void words<size, bit32, bit64>::bit_shift_left(const words in, uint32 bits, words result) {
        throw 0;
    }
    
    template <uint32_t size, typename bit32, typename bit64> 
    void words<size, bit32, bit64>::bit_shift_right(const words in, uint32 bits, words result) {
        throw 0;
    }
    
    template <uint32_t size, typename bit32, typename bit64> 
    void words<size, bit32, bit64>::times(const words a, const words b, words result) {
        auto from_end = [](uint32 i)->uint32{return size - 1 - i;};
        bit32 remainder{0};
        for (int i = 0; i < size; i ++) {
            bit64 sum = remainder;
            for (int j = 0; j <= i; j++) sum += a[from_end(j)]*b[from_end(i - j)];
            remainder = greater(sum);
            result[from_end(i)] = lesser(sum);
        }
    }
    
    template <uint32_t size>
    bool number<size, false>::operator<(const number<size, false>& n) const {
        words a{*this};
        words b{n};
        for (uint32_t i = 0; i < size; i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        
        return false;
    }
    
    template <uint32_t size>
    bool number<size, false>::operator<=(const number<size, false>& n) const {
        words a{*this};
        words b{n};
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

