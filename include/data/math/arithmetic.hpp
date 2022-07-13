// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ARITHMETIC
#define DATA_MATH_ARITHMETIC

#include <data/math/sign.hpp> 

namespace data::math {
    
    template <typename X> struct first;
    
    template <std::integral X> struct first<X> {
        X operator()() {
            return X{0};
        }
    };
    
    template <std::integral X>
    X inline increment(const X &x) {
        return x + 1;
    };
    
    template <std::unsigned_integral X>
    X inline next(const X &x) {
        return increment(x);
    };
    
    template <std::signed_integral X>
    X inline next(const X &x) {
        return x < 0 ? (-x + 1) : -x;
    };
    
    template <std::signed_integral N>
    N inline decrement(const N &n) {
        return n - 1;
    };
    
    template <std::unsigned_integral N>
    N inline decrement(const N &n) {
        return n == 0 ? 0 : n - 1;
    };
    
    template <typename F, typename x> struct identity;
    template <typename F, typename x> struct inverse;
    
    template <typename X> struct plus {
        X operator()(const X &a, const X &b) {
            return a + b;
        }
    };
    
    template <typename X> struct identity<plus<X>, X> {
        X operator()() {
            return X{0};
        }
    };
    
    template <typename X> struct times {
        X operator()(const X &a, const X &b) {
            return a * b;
        }
    };
    
    template <typename X> struct identity<times<X>, X> {
        X operator()() {
            return X{1};
        }
    };
    
    template <> struct inverse<plus<int8_t>, int8_t> {
        int8_t operator()(const int8_t &a, const int8_t &b) {
            return b - a;
        }
    };
    
    template <std::integral X> struct inverse<plus<X>, X> {
        X operator()(X a, X b) {
            return b - a;
        }
    };

}

#include <data/encoding/halves.hpp>
#include <data/encoding/words.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::arithmetic {
    template<size_t size, size_t mod8 = size % 8, size_t mod4 = size % 4, size_t mod2 = size % 2> 
    struct max_word_size;
    
    template<size_t size> struct max_word_size<size, 0, 0, 0> {
        using type = uint64;
    };
    
    template<size_t size> struct max_word_size<size, 4, 0, 0> {
        using type = uint32;
    };
    
    template<size_t size, size_t mod8> struct max_word_size<size, mod8, 2, 0> {
        using type = uint16;
    };
    
    template<size_t size, size_t mod8, size_t mod4> struct max_word_size<size, mod8, mod4, 0> {
        using type = byte;
    };
    
    template <typename digit> struct get_sign_bit;
    
    template<> struct get_sign_bit<byte> {
        static const byte value = 0x80;
    };
    
    template<> struct get_sign_bit<uint16> {
        static const uint16 value = 0x8000;
    };
    
    template<> struct get_sign_bit<uint32> {
        static const uint32 value = 0x80000000;
    };
    
    template<> struct get_sign_bit<uint64> {
        static const uint64 value = 0x8000000000000000;
    };
    
    template <typename digit> const digit sign_bit = get_sign_bit<digit>::value;
    
    template <endian::order r, typename digit> bool inline sign_bit_set(const encoding::words<r, digit> x) {
        return (x.size() != 0) && (x[-1] & sign_bit<digit>) != 0;
    }
    
    template <endian::order r, typename digit> void inline flip_sign_bit(encoding::words<r, digit> x) {
        if (x[-1] & sign_bit<digit>) x[-1] &= ~sign_bit<digit>;
        else x[-1] |= sign_bit<digit>;
    }
    
    template <endian::order r, typename digit> bool inline ones_is_zero(const encoding::words<r, digit> x) {
        for (const digit d : x) if (d != 0) return false;
        return true;
    }
    
    template <endian::order r, typename digit> bool twos_is_zero(const encoding::words<r, digit> x) {
        auto i = x.rbegin(); 
        auto e = x.rend();
        if (i == e) return true;
        if (*i != 0 && *i != sign_bit<digit>) return false;
        while (true) {
            i++;
            if (i == e) return true;
            if (*i != 0) return false;
        }
    }
    
    template <endian::order r, typename digit> math::sign N_sign(const encoding::words<r, digit> x) {
        for (auto i = x.rbegin(); i != x.rend(); i++) if (*i != 0) return math::positive;
        return math::zero;
    } 
    
    template <endian::order r, typename digit> math::sign Z_sign_ones(const encoding::words<r, digit> x) {
        auto i = x.rbegin(); 
        if (i == x.rend()) return math::zero;
        if (*i & sign_bit<digit>) return math::negative;
        while (i != x.rend()) {
            if (*i != 0) return math::positive;
            i++;
        }
        return math::zero;
    } 
    
    template <endian::order r, typename digit> math::sign Z_sign_twos(const encoding::words<r, digit> x) {
        auto i = x.rbegin(); 
        if (i == x.rend()) return math::zero;
        math::sign nonzero = (*i & sign_bit<digit>) ? math::negative : math::positive;
        if (*i & ~sign_bit<digit>) return nonzero;
        while (true) {
            i++; 
            if (i != x.rend()) return math::zero;
            if (*i != 0) return nonzero;
        }
    } 
    
    template <endian::order r, typename digit> bool inline N_is_minimal(const encoding::words<r, digit> x) {
        return x.size() == 0 || x[1] != 0;
    }
    
    template <endian::order r, typename digit> bool ones_is_minimal(const encoding::words<r, digit> x) {
        // minimal zero. 
        if (x.size() == 0) return true;
        // numbers of one byte. 
        if (x.size() == 1) return x[1] != 0;
        // numbers without an initial 00 or ff. 
        digit biggest = x[-1];
        digit max_value = std::numeric_limits<digit>::max();
        if (biggest != max_value && biggest != 0) return true;
        // numbers that would be interpreted with the wrong sign if they were shortened. 
        digit next_biggest = x[-2];
        return (biggest == 0 && (next_biggest & sign_bit<digit>)) || (biggest == max_value && !(next_biggest & sign_bit<digit>));
    }
    
    template <endian::order r, typename digit> bool inline twos_is_minimal(const encoding::words<r, digit> x) {
        // minimal zero. 
        return (x.size() == 0) || 
            // numbers without an initial 00 or 80. 
            (x[-1] != 0 && x[-1] != sign_bit<digit>) || 
            // numbers that would be interpreted as having the wrong sign if they were shortened. 
            (x.size() > 1 && (x[-2] & sign_bit<digit>));
    }
    
    template <endian::order r, typename digit> size_t inline N_minimal_size(const encoding::words<r, digit> x) {
        int size = x.size();
        for (auto i = x.rbegin(); i != x.rend(); i++) {
            if (*i != 0x00) break;
            else size--;
        }
        return size;
    }
    
    template <endian::order r, typename digit> size_t ones_minimal_size(const encoding::words<r, digit> x) {
        if (x.size() == 0) return 0;
        digit d = *x.rbegin();
        if (d != 0xff && d != 0x00) return x.size();
        int repeated_bytes = 0;
        auto i = x.rbegin() + 1;
        while (true) {
            // if we reach the end, then the number is either 0 or -1.
            if (i == x.rend()) return d == 0x00 ? 0 : 1; 
            if (*i == d) repeated_bytes ++;
            else {
                return x.size() - repeated_bytes - (*i < 0x80 && d == 0x00 || *i >= 0x80 && d == 0xff ? 1 : 0);
            }
            i++;
        }
    }
    
    template <endian::order r, typename digit> size_t inline twos_minimal_size(const encoding::words<r, digit> x) {
        if (x.size() == 0) return 0;
        // numbers that don't begin with 00 or 80 are minimal. 
        auto i = x.rbegin(); 
        digit d = *i;
        if (d != 0x80 && d != 0x00) return x.size();
        
        // count the number of zero bytes after the first. 
        int zero_bytes = 0; 
        
        while(true) {
            i++;
            // if we reach the end then this number is zero. 
            if (i == x.rend()) return 0;
            if (*i == 0x00) zero_bytes ++;
            // if the first non-zero digit does not have the
            // sign bit set then we can remove an extra digit. 
            else return x.size() - zero_bytes - (*i < 8 ? 1 : 0);
        }
    }
    
    template <endian::order r, typename digit> void inline negate_ones(encoding::words<r, digit> x) {
        bit_negate<digit>(x.end(), x.begin(), x.begin());
        minus(x.end(), x.begin(), 1, x.begin());
    }
    
    template <endian::order r, typename digit> void inline negate_twos(encoding::words<r, digit> x) {
        if (x.size() == 0) return;
        flip_sign_bit(x);
    }
    
    template <endian::order r, typename digit> std::weak_ordering N_compare(
        const encoding::words<r, digit> a, 
        const encoding::words<r, digit> b) {
        if (a.size() < b.size()) return 0 <=> N_compare(b, a);
        size_t size_difference = a.size() - b.size();
        
        auto ai = a.rbegin();
        for (int i = 0; i < size_difference; i++) {
            if (0 != *ai) return std::weak_ordering::greater;
            ai++;
        }
        
        return data::arithmetic::compare(a.rend(), ai, b.rbegin());
    } 
    
    template <typename digit> struct get_limit;
    
    template<> struct get_limit<byte> {
        static const byte max_unsigned = 0xff;
        static const byte max_signed = 0x7f;
    };
    
    template<> struct get_limit<uint16> {
        static const uint16 max_unsigned = 0xffff;
        static const uint16 max_signed = 0x7fff;
    };
    
    template<> struct get_limit<uint32> {
        static const uint32 max_unsigned = 0xffffffff;
        static const uint32 max_signed = 0x7ffffff;
    };
    
    template<> struct get_limit<uint64> {
        static const uint64 max_unsigned = 0xffffffffffffffff;
        static const uint64 max_signed = 0x7fffffffffffffff;
    };
    
    template <typename digit> const digit max_unsigned = get_limit<digit>::max_unsigned;
    template <typename digit> const digit max_signed_ones = get_limit<digit>::max_signed;
    template <typename digit> const digit min_unsigned_ones = sign_bit<digit>;
    
    template <endian::order r, typename digit> 
    void set_max_unsigned(encoding::words<r, digit> a) {
        for (digit &x : a) x = max_unsigned<digit>;
    } 
    
    template <endian::order r, typename digit> 
    void set_max_signed_ones(encoding::words<r, digit> a) {
        auto i = a.rbegin();
        if (i == a.rend()) return;
        *i = max_signed_ones<digit>;
        while(true) {
            i++;
            if (i == a.rend()) return;
            *i = max_unsigned<digit>;
        }
    } 
    
    template <endian::order r, typename digit> 
    void set_min_signed_ones(encoding::words<r, digit> a) {
        auto i = a.rbegin();
        if (i == a.rend()) return;
        *i = min_unsigned_ones<digit>;
        while(true) {
            i++;
            if (i == a.rend()) return;
            *i = 0;
        }
    } 
}

#endif

