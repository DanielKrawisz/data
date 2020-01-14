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
    
    const endian::order big = endian::big;
    const endian::order little = endian::little;
    
    template <size_t bytes, size_t size, size_t remainder, typename bit32, endian::order o>
    struct fixed_words;
    
    template <size_t bytes, size_t size, typename bit32>
    struct fixed_words<bytes, size, 0, bit32, big> {
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
        
        using element = boost::endian::endian_arithmetic<big, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[Last] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > Last) return default_value();
            element x{0};
            std::copy_n(&Data[4 * (Last - i)], 4, x.data());
            return x;
        }
        
        const element set(index i, element x) {
            if (i > Last) return default_value();
            std::copy_n(x.data(), 4, &Data[4 * (Last - i)]);
            return x;
        }
    };
    
    template <size_t bytes, size_t size, typename bit32>
    struct fixed_words<bytes, size, 0, bit32, little> {
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
        
        using element = boost::endian::endian_arithmetic<little, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[Last] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > Last) return default_value();
            element x{0};
            std::copy_n(&Data[4 * i], 4, x.data());
            return x;
        }
        
        const element set(index i, element x) {
            if (i > Last) return default_value();
            std::copy_n(x.data(), 4, &Data[4 * i]);
            return x;
        }
    };
    
    template <size_t remainder, endian::order o> struct remainders;
    
    template <endian::order o> struct remainders<1, o> {
        boost::endian::endian_arithmetic<o, uint32, 32> operator()() {return 255;}
    };
    
    template <endian::order o> struct remainders<2, o> {
        boost::endian::endian_arithmetic<o, uint32, 32> operator()() {return 65535;}
    };
    
    template <endian::order o> struct remainders<3, o> {
        boost::endian::endian_arithmetic<o, uint32, 32> operator()() {return 16777215;}
    };
    
    template <endian::order o>
    inline boost::endian::endian_arithmetic<o, uint32, 32> get_remainder(int n) {
        switch (n) {
            default: throw std::logic_error{"invalid remainder value"};
            case 1 : return remainders<1, o>{}();
            case 2 : return remainders<2, o>{}();
            case 3 : return remainders<3, o>{}();
        }
    }
    
    template <size_t bytes, size_t size, size_t remainder, typename bit32>
    struct fixed_words<bytes, size, remainder, bit32, big> {
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
        
        using element = boost::endian::endian_arithmetic<big, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[0] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > Last) return default_value();
            element x{0};
            if (i < Last) std::copy_n(&Data[4 * (Last - i)], 4, x.data());
            else std::copy_n(&Data[0], remainder, x.data() + 4 - remainder);
            return x;
        }
        
        const element set(index i, element x) {
            if (i > Last) return default_value();
            if (i < Last) std::copy_n(x.data(), 4, &Data[4 * (Last - i)]);
            else std::copy_n(x.data() + 4 - remainder, remainder, &Data[0]);
            return x ^ remainders<remainder, big>{}();
        }
    };
    
    template <size_t bytes, size_t size, size_t remainder, typename bit32>
    struct fixed_words<bytes, size, remainder, bit32, little> {
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
        
        using element = boost::endian::endian_arithmetic<little, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[Last] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > Last) return default_value();
            element x{0};
            if (i < Last) std::copy_n(&Data[4 * i], 4, x.data());
            else std::copy_n(&Data[4 * i], remainder, x.data());
            return x;
        }
        
        const element set(index i, element x) {
            if (i > Last) return default_value();
            if (i < Last) std::copy_n(x.data(), 4, &Data[4 * i]);
            else std::copy_n(x.data(), remainder, &Data[4 * i]);
            return x ^ remainders<remainder, little>{}();
        }
    };
    
    template <typename bit32, endian::order o> struct unfixed_words;
    
    template <typename bit32> struct unfixed_words<bit32, big> {
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
        
        using element = boost::endian::endian_arithmetic<big, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[last()] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > last()) return default_value();
            element x{0};
            if (i < last()) std::copy_n(&Data[4 * (last() - i)], 4, x.data());
            else std::copy_n(&Data[0], Remainder, x.data() + 4 - Remainder);
            return x;
        }
        
        const element set(index i, element x) {
            if (i > last()) return default_value();
            if (i < last()) std::copy_n(x.data(), 4, &Data[4 * (last() - i)]);
            else std::copy_n(x.data() + 4 - Remainder, Remainder, &Data[0]);
            return x & get_remainder<big>(Remainder);
        }
    };
    
    template <typename bit32> struct unfixed_words<bit32, little> {
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
        
        using element = boost::endian::endian_arithmetic<little, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[last()] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > last()) return default_value();
            element x{0};
            if (i < last()) std::copy_n(&Data[4 * i], 4, x.data());
            else std::copy_n(&Data[4 * i], Remainder, x.data());
            return x;
        }
        
        const element set(index i, element x) {
            if (i > last()) return default_value();
            if (i < last()) std::copy_n(x.data(), 4, &Data[4 * i]);
            else std::copy_n(x.data(), Remainder, &Data[4 * i]);
            return x ^ get_remainder<little>(Remainder);
        }
    };
    
    template <typename words, typename word>
    struct unoriented {
        using twice = typename data::twice<word>::type;
        
        static twice extend(word w) {
            return data::twice<word>::extend(w);
        }
        
        static bool less(uint32 last, const words, const words);
        
        static bool less_equal(uint32 last, const words, const words);
        
        static void bit_not(uint32 last, const words, words);
        
        static void bit_and(uint32 last, const words, const words, words);
        
        static void bit_or(uint32 last, const words, const words, words);
        
        static void plus(uint32 last, const words, const words, words);
        
        static void plus(uint32 last, const words, const word, words);
        
        static void plus(uint32 last, const words, const uint64, words) {
            throw method::unimplemented{"unoriented::plus"};
        }
        
        static void minus(uint32 last, const words, const words, words);
        
        static void minus(uint32 last, const words, const word, words);
        
        static void times(uint32 last, const words, const words, words);
        
        static void times(uint32 last, const words, const word, words);
        
        using index = uint32;
        
        static void down(uint32 last, const words a, const uint32 i, words out) {
            throw method::unimplemented{"methods::down"};
        }
        
        static void up(uint32 last, const words a, const uint32 i, words out) {
            throw method::unimplemented{"methods::up"};
        }
    };
    
    template <typename words, typename word>
    bool unoriented<words, word>::less(uint32 last, const words a, const words b) {
        if (a[last] < b[last]) return true;
        if (a[last] > b[last]) return false;
        
        for (int i = last - 1; i >= 0; i--) {
            if (uint32(a[i]) < uint32(b[i])) return true;
            if (uint32(a[i]) > uint32(b[i])) return false;
        }
        
        return false;
    }
    
    template <typename words, typename word>
    bool unoriented<words, word>::less_equal(uint32 last, const words a, const words b) {
        if (a[last] < b[last]) return true;
        if (a[last] > b[last]) return false;
        
        for (int i = last - 1; i >= 0; i--) {
            if (uint32(a[i]) < uint32(b[i])) return true;
            if (uint32(a[i]) > uint32(b[i])) return false;
        }
        
        return true;
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_not(uint32 last, const words xx, words out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, ~xx[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_and(uint32 last, const words x, const words y, words out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, x[i]^y[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_or(uint32 last, const words x, const words y, words out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, x[i]|y[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::plus(uint32 last, const words a, const words b, words result) {
        word remainder{0};
        for (int32 i = 0; i <= last; i++) {
            twice w = extend(a[i]) + extend(b[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::plus(uint32 last, const words a, const word b, words result) {
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
    void unoriented<words, word>::minus(uint32 last, const words a, const words b, words result) {
        word remainder{0};
        for (int32 i = last; i >= 0; i--) {
            twice w = extend(a[i]) - extend(b[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::minus(uint32 last, const words a, const word b, words result) {
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
    void unoriented<words, word>::times(uint32 last, const words a, const words b, words result) {
        throw method::unimplemented{"unoriented::times"};
        /*auto from_end = [last](uint32 i)->uint32{return last - i;};
        word remainder{0};
        for (int i = 0; i <= last; i ++) {
            twice sum = extend(remainder);
            for (int j = 0; j <= i; j++) sum += a[from_end(j)]*b[from_end(i - j)];
            remainder = greater_half(sum);
            result[from_end(i)] = lesser_half(sum);
        }*/
    }
}

#endif
