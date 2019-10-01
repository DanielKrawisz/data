// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_WORDS
#define DATA_WORDS

#include <data/types.hpp>
#include <data/encoding/endian.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/sign.hpp>
#include <data/meta/equal.hpp>

namespace data::encoding {
    
    template <endian::order> struct displacement;
    
    template <> struct displacement<endian::order::big> {
        constexpr static const uint32 Value = 0;
    };
    
    template <> struct displacement<endian::order::little> {
        constexpr static const uint32 Value = 3;
    };
    
    template <size_t bytes, size_t size, byte extra, typename bit32, endian::order o> struct words;
    
    template <size_t bytes, size_t size, typename bit32, endian::order o>
    struct words<bytes, size, 0, bit32, o> : meta::equal<bytes, size * 4> {
        endian::ordered<slice<byte, bytes>, o> Data;
        
        using index = uint32;
        
        constexpr static const index last = size - 1;
        
        template <typename A>
        words(A a) : Data{endian::ordered<slice<byte, bytes>, o>{slice<byte, bytes>{a}}} {}
        
        endian::ordered<bit32&, o> operator[](index i) {
            return endian::ordered<bit32&, o>::as(*(bit32*)(&Data[4 * i + displacement<o>::Value]));
        }
        
        const endian::ordered<bit32&, o> operator[](index i) const {
            return endian::ordered<bit32&, o>::as(*(bit32*)(&Data[4 * i + displacement<o>::Value]));
        }
        
        void set(index i, bit32 x) {
            operator[](i) = x;
        }
    };
    
    template <uint32 size, typename bit32, typename bit64, endian::order o>
    struct number : public endian::halves<bit32, bit64, o> {
        using words = encoding::words<size, size / 4 + (0 != (size % 4)), (4 - (size % 4)) % 4, bit32, o>;
        using word = endian::ordered<bit64, endian::order::big>;
        number(endian::ordered<slice<byte, size>, o> a) : words{a} {}
        
        constexpr static const uint32 last = size - 1;
        
        static word extend(uint32);
        
        static bool overflow(word x) {
            return greater(x) != 0;
        }
        
        static number make(byte* a) {
            return {a};
        }
        
        static const number make(const byte* const a) {
            return {a};
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
        
        static void minus(const words, const bit32, words);
        static void plus(const words, const bit32, words);
        static void times(const words, const bit32, words);
        
        bool operator==(const words& xx);
        
        using iterator = index_iterator<words&, bit32&>;
        
        iterator begin();
        iterator end();
        const iterator begin() const;
        const iterator end() const;
    
    };
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    inline void number<size, bit32, bit64, o>::bit_negate(words xx) {
        for (endian::ordered<bit32&, o> u : xx) u = ~u;
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    inline void number<size, bit32, bit64, o>::bit_and(
        const words x,
        const words y, 
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = x[i]^y[i];
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    inline void number<size, bit32, bit64, o>::bit_or(
        const words x,
        const words y,
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = x[i]|y[i];
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o>
    void number<size, bit32, bit64, o>::minus(const words a, const words b, words result) {
        bit32 remainder{0};
        for (int32 i = 0; i < size; i++) {
            word w = extend(a[i]) - extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    void number<size, bit32, bit64, o>::plus(const words a, const words b, words result) {
        bit32 remainder{0};
        for (int32 i = 0; i < size; i++) {
            word w = extend(a[i]) + extend(b[i]);
            result[i] = remainder + lesser(w);
            remainder = greater(w);
        };
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    void number<size, bit32, bit64, o>::times(const words a, const words b, words result) {
        auto from_end = [](uint32 i)->uint32{return size - 1 - i;};
        bit32 remainder{0};
        for (int i = 0; i < size; i ++) {
            bit64 sum = remainder;
            for (int j = 0; j <= i; j++) sum += a[from_end(j)]*b[from_end(i - j)];
            remainder = greater(sum);
            result[from_end(i)] = lesser(sum);
        }
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o>
    void number<size, bit32, bit64, o>::minus(const words a, const bit32 b, words result) {
        bit32 remainder{0};
        
        word w = extend(a[last]) - extend(b);
        result[last] = remainder + lesser(w);
        remainder = greater(w);
        
        w = extend(a[last-1]);
        result[last - 1] = remainder + lesser(w);
        remainder = greater(w);
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    void number<size, bit32, bit64, o>::plus(const words a, const bit32 b, words result) {
        bit32 remainder{0};
        
        word w = extend(a[last]) + extend(b);
        result[last] = remainder + lesser(w);
        remainder = greater(w);
        
        w = extend(a[last-1]);
        result[last - 1] = remainder + lesser(w);
        remainder = greater(w);
    }
    
}

#endif
