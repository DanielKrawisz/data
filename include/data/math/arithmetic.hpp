// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ARITHMETIC
#define DATA_MATH_ARITHMETIC

#include <data/float.hpp> 
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
    
    template <std::floating_point X> struct inverse<plus<X>, X> {
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
    
    // Note: some of the type and var names in the following functions
    // are very confusing. It would be good to fix them. 
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool equal(sen z, it i, it j) {
        while (i != z) {
            if(*i != *j) return false;
            i++;
            j++;
        }
        return true;
    }
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool greater(sen z, it i, it j) {
        while (i != z) {
            if(*i > *j) return true;
            i++;
            j++;
        }
        return false;
    }
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool less(sen z, it i, it j) {
        while (i != z) {
            if(*i < *j) return true;
            i++;
            j++;
        }
        return false;
    }
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    std::weak_ordering compare(sen z, it i, it j) {
        while (i != z) {
            if(*i < *j) return std::weak_ordering::less;
            if(*i > *j) return std::weak_ordering::greater;
            i++;
            j++;
        }
        return std::weak_ordering::equivalent;
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    void bit_negate(sen z, ita i, itb j) {
        while (i != z) {
            *i = ~ *j;
            i++;
            j++;
        }
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    void bit_and(sen z, ita i, itb a, itb b) {
        while (i != z) {
            *i = *a & *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    void bit_or(sen z, ita i, itb a, itb b) {
        while (i != z) {
            *i = *a | *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    void bit_xor(sen z, ita i, itb a, itb b) {
        while (i != z) {
            *i = *a ^ *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    digit plus(sen z, ita a, digit d, itb b) {
        using two_digits = typename encoding::twice<digit>::type;
        
        digit remainder = d;
        while (a != z) {
            two_digits result = encoding::add<digit>(*b, 0, remainder);
            *a = encoding::lesser_half(result);
            remainder = encoding::greater_half(result);
            if (remainder == 0) break;
            a++;
            b++;
        }
        
        return remainder;
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    digit minus(sen z, ita a, digit d, itb b) {
        digit remainder = d;
        while (a != z) {
            if (*b >= remainder) {
                *a = *b - remainder; 
                return 0;
            } 
            
            *a = encoding::lesser_half<typename encoding::twice<digit>::type>(encoding::combine<digit>(1, *b) - remainder);
            remainder = 1;
            a++;
            b++;
        }
        
        return remainder;
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    digit plus(sen z, ita i, itb a, itb b) {
        using two_digits = typename encoding::twice<digit>::type;
        
        digit remainder = 0;
        
        while (i != z) {
            two_digits result = encoding::add<digit>(*a, *b, remainder);
            remainder = encoding::greater_half(result);
            *i = encoding::lesser_half(result);
            i++;
            a++;
            b++;
        }
        
        return remainder;
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    digit minus(sen z, ita a, itb b, itb i) {
        throw method::unimplemented{"arihmetic::minus"};
    }
    
    template <typename digit, typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, digit> && std::sentinel_for<sen, ita>
    digit times(sen z, ita a, itb b, itb i) {
        throw method::unimplemented{"arihmetic::minus"};
    }
    
    // bit shift operations are defined in terms of big-endian numbers. 
    // but really they are operations to powers of 2 ignoring sign, 
    // regardless of the way that a number is actually represented. 
    // we can only do this with bytes and not bigger numbers because of 
    // little-endian encoding. 
    template <typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, byte> && std::sentinel_for<sen, itb>
    void shift_left(ita &i, sen z, itb b, byte amount, byte fill) {
        using two_digits = typename encoding::twice<byte>::type;
        
        while (b != z) {
            auto bp = b;
            bp++;
            two_digits result = encoding::combine<byte>(*b, bp != z ? *bp : fill) << amount;
            *i = encoding::greater_half(result);
            i++;
            b = bp;
        }
    }
    
    // you have to use reverse iterators for this function. 
    template <typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, byte> && std::sentinel_for<sen, ita>
    void shift_right(ita &i, sen z, itb b, byte amount, byte fill) {
        using two_digits = typename encoding::twice<byte>::type;
        
        while (b != z) {
            auto bp = b;
            bp++;
            two_digits x = encoding::combine<byte>(bp != z ? *bp : fill, *b);
            two_digits result = encoding::combine<byte>(bp != z ? *bp : fill, *b) >> amount;
            *i = encoding::lesser_half(result);
            i++;
            b = bp;
        }
    }
    
    template <typename it, typename sen>
    void bit_shift_left(it i, sen z, uint32 x, bool fill) {
        auto bytes = x / 8;
        auto bits = x % 8;
        byte filler = fill ? ~0 : 0;
        
        size_t size = z - i;
        if (bytes <= size) math::arithmetic::shift_left(i, z, i + bytes, bits, filler);
        
        while (i != z) {
            *i = filler;
            i++;
        }
    }
    
    template <typename it, typename sen>
    void bit_shift_right(it i, sen z, uint32 x, bool fill) {
        auto bytes = x / 8;
        auto bits = x % 8;
        byte filler = fill ? ~0 : 0;
        
        size_t size = z - i;
        if (bytes <= size) math::arithmetic::shift_right(i, z, i + bytes, bits, filler);
        
        while (i != z) {
            *i = filler;
            i++;
        }
    }
    
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
        
        return compare(a.rend(), ai, b.rbegin());
    } 
    
    template <endian::order r, typename digit> 
    void bit_shift(encoding::words<r, digit> a, int i) {
        throw method::unimplemented{"bit shift"};
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

