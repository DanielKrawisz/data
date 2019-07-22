// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_WORDS
#define DATA_WORDS

#include <data/types.hpp>
#include <data/math/number/endian.hpp>
#include <data/tools/index_iterator.hpp>

namespace data {
    
    template <uint32_t size, typename bit32, typename bit64>
    struct words {
        using word = data::number::endian::big<bit64>;
        using index = uint32;
        
        std::array<byte, size * 4>& Words;
        
        static const index last = size - 1;
        
        static bit32 lesser(word w);
        static bit32 greater(word w);
        
        bit32& operator[](index i) {
            if (i >= size) throw 0;
            return *(uint32*)(Words.data() + 4 * i);
        }
        
        static word extend(uint32);
        
        static bool overflow(word w) {
            return greater(w) != 0;
        }
        
        words(std::array<byte, 4 * size>& b) : Words{b} {}
        
        static void bit_negate(words);
        static void bit_or(const words, const words, words);
        static void bit_and(const words, const words, words);
        
        static void bit_shift_left(const words, uint32, words);
        static void bit_shift_right(const words, uint32, words);
        
        static void bit_shift_left(const words in, int32 bits, words w) {
            if (bits < 0) return bit_shift_right(in, -bits, w);
            return bit_shift_left(uint32(in), bits, w);
        }
        
        static void bit_shift_right(const words in, int32 bits, words w) {
            if (bits < 0) return bit_shift_left(in, -bits, w);
            return bit_shift_right(uint32(in), bits, w);
        }
        
        static void minus(const words, const words, words);
        static void plus(const words, const words, words);
        static void times(const words, const words, words);
        
        using iterator = index_iterator<words&, bit32&>;
        
        iterator begin();
        iterator end();
        const iterator begin() const;
        const iterator end() const;
    
    };
    
    template <uint32 size, typename bit32, typename bit64> 
    inline void words<size, bit32, bit64>::bit_negate(words w) {
        for (bit32& u : w) u = ~u;
    }
    
    template <uint32 size, typename bit32, typename bit64> 
    inline void words<size, bit32, bit64>::bit_and(
        const words a,
        const words b, 
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = a[i]^b[i];
    }
    
    template <uint32 size, typename bit32, typename bit64> 
    inline void words<size, bit32, bit64>::bit_or(
        const words a,
        const words b,
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = a[i]|b[i];
    }
    
}

#endif
