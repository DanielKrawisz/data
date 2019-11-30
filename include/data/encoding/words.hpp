// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_WORDS
#define DATA_WORDS

#include <type_traits>
#include <data/types.hpp>
#include <data/encoding/halves.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/sign.hpp>
#include <data/meta/equal.hpp>
#include <data/slice.hpp>
#include <data/io/unimplemented.hpp>

namespace data::arithmetic {
    
    template <size_t bytes, size_t size, size_t remainder, typename bit32, endian::order o>
    struct fixed_words {
        slice<byte, bytes> Data;
        
        fixed_words(slice<byte, bytes> d) : Data{d} {}
        
        static const fixed_words make(const slice<byte, bytes> d) {
            return fixed_words(d);
        }
        
        using index = uint32;
        
        constexpr static const index Last = size - 1;
        
        index last() const {
            return Last;
        }
        
        const endian::ordered<bit32, o> default_value() const {
            return endian::ordered<bit32, o>::as(std::is_signed<bit32>::value && Data[Last] < 0 ? -1 : 0);
        }
        
        const endian::ordered<bit32, o> operator[](index i) const {
            if (i > Last) return default_value(); 
            if (i < Last) return endian::ordered<bit32, o>::as(*(bit32*)(&Data[o == endian::little ? 4 * i : 4 * (Last - i)]));
            throw method::unimplemented{"fixed_words[]"};
        }
        
        const endian::ordered<bit32, o> set(index i, endian::ordered<bit32, o> x) {
            if (i > Last) return default_value();
            if (i < Last) return endian::ordered<bit32, o>::as(*(bit32*)(&Data[o == endian::little ? 4 * i : 4 * (Last - i)]) = x.Value);
            throw method::unimplemented{"fixed_words set"};
        }
    };
    
    template <typename bit32, endian::order o>
    struct unfixed_words {
        slice<byte> Data;
        size_t Bytes;
        size_t Remainder;
        size_t Size;
        
        unfixed_words(slice<byte> d) : Data{d}, Bytes{d.size()}, Remainder{Bytes % 4}, Size{Bytes / 4 + Remainder} {}
        
        static const unfixed_words make(const slice<byte> d) {
            return unfixed_words(d);
        }
        
        using index = uint32;
        
        index last() const {
            return Size - 1;
        }
        
        const endian::ordered<bit32, o> default_value() const {
            return endian::ordered<bit32, o>::as(std::is_signed<bit32>::value && Data[last()] < 0 ? -1 : 0);
        }
        
        const endian::ordered<bit32, o> operator[](index i) const {
            throw method::unimplemented{"unfixed_words[]"};
        }
        
        const endian::ordered<bit32, o> set(index i, endian::ordered<bit32, o> x) {
            throw method::unimplemented{"unfixed_words set"};
        }
    };
    
    template <typename words, typename word>
    struct unoriented {
        using twice = typename data::twice<word>::type;
        
        static twice extend(word w) {
            return data::twice<word>::extend(w);
        }
        static bool less(uint32 last, const words&, const words&);
        
        static bool less_equal(uint32 last, const words&, const words&);
        
        static void bit_not(uint32 last, const words&, words&);
        
        static void bit_and(uint32 last, const words&, const words&, words&);
        
        static void bit_or(uint32 last, const words&, const words&, words&);
        
        static void plus(uint32 last, const words&, const words&, words&);
        
        static void plus(uint32 last, const words&, const word, words&);
        
        static void minus(uint32 last, const words&, const words&, words&);
        
        static void minus(uint32 last, const words&, const word, words&);
        
        static void times(uint32 last, const words&, const words&, words&);
    };
    
    template <typename words, typename word, endian::order o>
    struct bit_shift {
        using index = uint32;
        
        void left(const words& a, const uint32 i, words& out);
        
        void right(const words& a, const uint32 i, words& out);
        
        void operator()(const words& a, const int32 i, words& out) {
            if (i < 0) return right(a, -i, out);
            return left(a, i, out);
        }
    };
    
    template <typename words, typename word>
    bool unoriented<words, word>::less(uint32 last, const words& a, const words& b) {
        if (a[last] < b[last]) return true;
        if (a[last] > b[last]) return false;
        
        for (int i = last - 1; i >= 0; i--) {
            if (uint32(a[i]) < uint32(b[i])) return true;
            if (uint32(a[i]) > uint32(b[i])) return false;
        }
        
        return false;
    }
    
    template <typename words, typename word>
    bool unoriented<words, word>::less_equal(uint32 last, const words& a, const words& b) {
        if (a[last] < b[last]) return true;
        if (a[last] > b[last]) return false;
        
        for (int i = last - 1; i >= 0; i--) {
            if (uint32(a[i]) < uint32(b[i])) return true;
            if (uint32(a[i]) > uint32(b[i])) return false;
        }
        
        return true;
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_not(uint32 last, const words& xx, words& out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, ~xx[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_and(uint32 last, const words& x, const words& y, words& out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, x[i]^y[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_or(uint32 last, const words& x, const words& y, words& out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, x[i]|y[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::plus(uint32 last, const words& a, const words& b, words& result) {
        word remainder{0};
        for (int32 i = 0; i <= last; i++) {
            twice w = extend(a[i]) + extend(b[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::plus(uint32 last, const words& a, const word b, words& result) {
        word remainder{0};
        
        twice w = extend(a[0]) + extend(b);
        result.set(0, remainder + lesser_half(w));
        remainder = greater_half(w);
        
        for (int32 i = 1; i <= last; i++) {
            w = extend(a[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::minus(uint32 last, const words& a, const words& b, words& result) {
        word remainder{0};
        for (int32 i = last; i >= 0; i--) {
            twice w = extend(a[i]) - extend(b[i]);
            result[i] = remainder + lesser_half(w);
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::minus(uint32 last, const words& a, const word b, words& result) {
        int i = 0;
        word remainder = b;
        
        while (i <= last && remainder != word{0}) {
            word wo = a[i];
            twice w = extend(wo) - extend(remainder);
            result.set(i, lesser_half(w));
            remainder = -greater_half(w);
            i++;
        }
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::times(uint32 last, const words& a, const words& b, words& result) {
        auto from_end = [last](uint32 i)->uint32{return last - i;};
        word remainder{0};
        for (int i = 0; i <= last; i ++) {
            twice sum = extend(remainder);
            for (int j = 0; j <= i; j++) sum += a[from_end(j)]*b[from_end(i - j)];
            remainder = greater_half(sum);
            result[from_end(i)] = lesser_half(sum);
        }
    }
}

#endif
