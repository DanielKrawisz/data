// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_WORDS
#define DATA_WORDS

#include <data/types.hpp>
#include <data/encoding/halves.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/sign.hpp>
#include <data/meta/equal.hpp>

namespace data::encoding {
    
    template <size_t bytes, size_t size, byte extra, typename bit32, endian::order o> struct words;
    
    template <size_t bytes, size_t size, typename bit32, endian::order o>
    struct words<bytes, size, 0, bit32, o> : meta::equal<bytes, size * 4> {
        slice<byte, bytes> Data;
        
        words(slice<byte, bytes> d) : Data{d} {}
        
        static const words make(const slice<byte, bytes> d) {
            return words(d);
        }
        
        using index = uint32;
        
        constexpr static const index last = size - 1;
        
        const endian::ordered<bit32, o> operator[](index i) const {
            // inefficient as o is known at compile time. 
            return endian::ordered<bit32, o>::as(*(bit32*)(&Data[o == endian::little ? 4 * i : 4 * (last - i)]));
        }
        
        const endian::ordered<bit32, o> set(index i, endian::ordered<bit32, o> x) const {
            return *(bit32*)(&Data[o == endian::little ? 4 * i : 4 * (last - i)]) = x.Value;
        }
    };
    
    template <uint32 size, typename word, typename words>
    struct methods {
        using twice = typename data::twice<word>::type;
        
        static twice extend(word w) {
            return data::twice<word>::extend(w);
        }
        
        constexpr static const uint32 last = size - 1;
        
        static bool overflow(word x) {
            return greater(x) != 0;
        }
        
        static void bit_negate(words);
        static void bit_or(const words, const words, words);
        static void bit_and(const words, const words, words);
        
        static void bit_shift_left(const words, uint32, words);
        static void bit_shift_right(const words, uint32, words);
        
        static void bit_shift_left(const words in, int32 bits, words w) {
            if (bits < 0) return bit_shift_right(in, -bits, w);
            return bit_shift_left(in, bits, w);
        }
        
        static void bit_shift_right(const words in, int32 bits, words w) {
            if (bits < 0) return bit_shift_left(in, -bits, w);
            return bit_shift_right(in, bits, w);
        }
        
        static void minus(const words, const words, words);
        static void plus(const words, const words, words);
        static void times(const words, const words, words);
        
        static void minus(const words, const word, words);
        static void plus(const words, const word, words);
        static void times(const words, const word, words);
        
        bool operator==(const words& xx);
    
    };
    
    template <uint32 size, typename word, typename words> 
    inline void methods<size, word, words>::bit_negate(words xx) {
        for (word u : xx) u = ~u;
    }
    
    template <uint32 size, typename word, typename words>  
    inline void methods<size, word, words>::bit_and(
        const words x,
        const words y, 
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = x[i]^y[i];
    }
    
    template <uint32 size, typename word, typename words>  
    inline void methods<size, word, words>::bit_or(
        const words x,
        const words y,
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = x[i]|y[i];
    }
    
    template <uint32 size, typename word, typename words> 
    void methods<size, word, words>::minus(const words a, const words b, words result) {
        word remainder{0};
        for (int32 i = words::last; i >= 0; i--) {
            twice w = extend(a[i]) - extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32 size, typename word, typename words>  
    void methods<size, word, words>::plus(const words a, const words b, words result) {
        word remainder{0};
        for (int32 i = 0; i < size; i++) {
            twice w = extend(a[i]) + extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32 size, typename word, typename words>  
    void methods<size, word, words>::times(const words a, const words b, words result) {
        auto from_end = [](uint32 i)->uint32{return size - 1 - i;};
        word remainder{0};
        for (int i = 0; i < size; i ++) {
            twice sum = extend(remainder);
            for (int j = 0; j <= i; j++) sum += a[from_end(j)]*b[from_end(i - j)];
            remainder = greater(sum);
            result[from_end(i)] = lesser(sum);
        }
    }
    
    template <uint32 size, typename word, typename words> 
    void methods<size, word, words>::minus(const words a, const word b, words result) {
        int i = 0;
        word remainder = b;
        
        while (i <= last && remainder != word{0}) {
            word wo = a[i];
            twice w = extend(wo) - extend(remainder);
            result.set(i, lesser(w));
            remainder = -greater(w);
            i++;
        }
    }
    
    template <uint32 size, typename word, typename words>  
    void methods<size, word, words>::plus(const words a, const word b, words result) {
        word remainder{0};
        
        twice w = extend(a[last]) + extend(b);
        result.set(last, remainder + lesser(w));
        remainder = greater(w);
        
        w = extend(a[last-1]);
        result.set(last - 1, remainder + lesser(w));
        remainder = greater(w);
    }
    
}

#endif
