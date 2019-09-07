// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_WORDS
#define DATA_WORDS

#include <data/types.hpp>
#include <data/encoding/endian.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/sign.hpp>

namespace data {
    
    template <uint32 left, uint32 right> struct equal {
        equal() = delete;
    };
        
    template <uint32 val> struct equal<val, val> {};
    
    template <uint32 left, uint32 right> struct unequal {};
        
    template <uint32 val> struct unequal<val, val> {
        unequal() = delete;
    };
    
    namespace low {
    
        template <uint32 bytes, uint32 words, byte extra, typename bit32, typename bit64, endian::order o> class wrapper;
        
        template <uint32 bytes, uint32 words, typename bit32, typename bit64, endian::order o>
        class wrapper<bytes, words, 0, bit32, bit64, o> : equal<bytes, words * 4>, endian::halves<bit32, bit64, o> {
            byte* Words;
        public:
            
            using index = uint32;
            
            static const index last = words - 1;
            
            wrapper(byte* a) : Words{a} {}
            
            bit32& operator[](index i) {
                if (i > words) throw std::out_of_range{""};
                return *(bit32*)(Words + 4 * i);
            }
            
            const bit32& operator[](index i) const {
                if (i > words) throw std::out_of_range{""};
                return *(bit32*)(Words + 4 * i);
            }
            
            void set(index i, bit32 x) {
                operator[](i) = x;
            }
        };
        
        template <uint32 bytes, uint32 words, byte extra, typename bit32, typename bit64, endian::order o>
        class wrapper : equal<bytes + extra, words * 4>, endian::halves<bit32, bit64, o>, unequal<extra, 0> {
            byte* Words;
            static const byte remainder = 4 - extra;
            static const uint32 shift_right = 8 * extra;
            static const uint32 shift_left = 8 * remainder;
        public:
            
            using index = uint32;
            
            static const index last = words - 1;
            
            wrapper(byte* a) : Words{a} {}
            
            bit32 operator[](index i) const {
                if (i > words) throw std::out_of_range{""};
                if (i == 0) return ((*(bit32*)(Words)) >> shift_right) + 
                    (endian::halves<bit32, bit64, o>::is_signed && (((Words[0] & 0x8000) == 0x8000) ? 0xffffffff << shift_left : 0));
                return *(bit32*)(Words - remainder + 4 * i);
            }
            
            void set(index i, bit32 x) {
                if (i > words) throw std::out_of_range{""};
                //if (i == 0) ; // TODO!!
                *(bit32*)(Words - remainder + 4 * i) = x;
            }
        };
    
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o>
    struct words : public low::wrapper<size, size / 4 + (0 != (size % 4)), (4 - (size % 4)) % 4, bit32, bit64, o> {
        using wrapper = low::wrapper<size, size / 4 + (0 != (size % 4)), (4 - (size % 4)) % 4, bit32, bit64, o>;
        using word = endian::ordered<bit64, endian::order::big>;
        words(byte* a) : wrapper{a} {}
        
        static word extend(uint32);
        
        static bool overflow(word x) {
            return greater(x) != 0;
        }
        
        static words make(byte* a) {
            return {a};
        }
        
        static const words make(const byte* const a) {
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
        
        using wrapper::last;
        using iterator = index_iterator<words&, bit32&>;
        
        iterator begin();
        iterator end();
        const iterator begin() const;
        const iterator end() const;
    
    };
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    inline void words<size, bit32, bit64, o>::bit_negate(words xx) {
        for (bit32& u : xx) u = ~u;
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    inline void words<size, bit32, bit64, o>::bit_and(
        const words x,
        const words y, 
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = x[i]^y[i];
    }
    
    template <uint32 size, typename bit32, typename bit64, endian::order o> 
    inline void words<size, bit32, bit64, o>::bit_or(
        const words x,
        const words y,
        words result) {
        for (uint32 i = 0; i < size; i++) result[i] = x[i]|y[i];
    }
    
}

#endif
