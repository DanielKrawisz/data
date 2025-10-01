// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED_BOUNDED

#include <data/concepts.hpp>
#include <data/math/group.hpp>
#include <data/abs.hpp>
#include <data/sign.hpp>
#include <data/divide.hpp>
#include <data/string.hpp>
#include <data/math/number/bytes/bytes.hpp>

namespace data::math::number {
    
    // satisfies range<word> and integral.
    template <bool u, endian::order, size_t size, std::unsigned_integral word> struct bounded;
}

namespace data::math {

    // satisfies unsigned_integral
    template <endian::order r, size_t x, std::unsigned_integral word>
    using uint = number::bounded<false, r, x, word>;

    // satisfies signed_integral
    template <endian::order r, size_t x, std::unsigned_integral word>
    using sint = number::bounded<true, r, x, word>;

    template <size_t size, std::unsigned_integral word>
    using uint_little = typename number::bounded<false, endian::little, size, word>;

    template <size_t size, std::unsigned_integral word>
    using uint_big = typename number::bounded<false, endian::big, size, word>;

    template <size_t size, std::unsigned_integral word>
    using int_little = typename number::bounded<true, endian::little, size, word>;

    template <size_t size, std::unsigned_integral word>
    using int_big = typename number::bounded<true, endian::big, size, word>;

}

namespace data::math::number {

    // comparison
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    bool operator == (const bounded<x, r, n, word> &, const bounded<y, o, z, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const sint<r, size, word> &, const sint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const uint<r, size, word> &, const uint<r, size, word> &);

    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    std::strong_ordering operator <=> (const bounded<x, r, n, word> &, const bounded<y, o, z, word> &);

    // bit operations
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator ~ (const bounded<u, r, x, word> &);
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator ^ (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator & (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator | (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator << (const bounded<u, r, size, word> &, int);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator >> (const bounded<u, r, size, word> &, int);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator &= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator |= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator ^= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator <<= (bounded<u, r, size, word> &, int);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator >>= (bounded<u, r, size, word> &, int);
    
    // increment and decrement
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator ++ (bounded<u, r, size, word> &, int);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator ++ (bounded<u, r, size, word> &);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator -- (bounded<u, r, size, word> &, int);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator -- (bounded<u, r, size, word> &);
    
    // arithmetic
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator - (const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator + (const bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator - (const bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> operator * (const bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator -= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> &operator -= (sint<r, size, word> &, const uint<r, size, word> &);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator += (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> &operator += (sint<r, size, word> &, const uint<r, size, word> &);
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> &operator *= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> &operator *= (sint<r, size, word> &, const uint<r, size, word> &);

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator / (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator / (const sint<r, x, word> &, const uint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator / (const uint<r, x, word> &, const sint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator % (const uint<r, x, word> &, const uint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator % (const sint<r, x, word> &, const uint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator % (const sint<r, x, word> &, const sint<r, x, word> &);

    // write
    template <data::endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &s, const uint<r, x, word> &n);

    template <data::endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &s, const sint<r, x, word> &n);

    template <bool u, data::endian::order r, size_t x, std::unsigned_integral word>
    std::istream &operator >> (std::istream &i, const bounded<u, r, x, word> &n);

    // declarations of standard functions.
    template <endian::order r, size_t x, std::unsigned_integral word>
    struct increment<uint<r, x, word>> {
        nonzero<uint<r, x, word>> operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct decrement<uint<r, x, word>> {
        uint<r, x, word> operator () (const nonzero<uint<r, x, word>> &);
        uint<r, x, word> operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct increment<sint<r, x, word>> {
        sint<r, x, word> operator () (const sint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct decrement<sint<r, x, word>> {
        sint<r, x, word> operator () (const sint<r, x, word> &);
    };
}

namespace data::math { 
    template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
    struct times<number::bounded<is_signed, r, size, word>> {
        number::bounded<is_signed, r, size, word> operator ()
        (const number::bounded<is_signed, r, size, word> &a, const number::bounded<is_signed, r, size, word> &b) {
        return a * b;
    }

        nonzero<number::bounded<is_signed, r, size, word>> operator ()
        (const nonzero<number::bounded<is_signed, r, size, word>> &a, const nonzero<number::bounded<is_signed, r, size, word>> &b);
    };
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    struct inverse<plus<number::bounded<u, r, x, word>>, number::bounded<u, r, x, word>> {
        number::bounded<u, r, x, word> operator () (const number::bounded<u, r, x, word> &a, const number::bounded<u, r, x, word> &b);
    };
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    struct abs<uint<r, x, word>> {
        uint<r, x, word> operator () (const uint<r, x, word> &);
    };
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    struct abs<sint<r, x, word>> {
        sint<r, x, word> operator () (const sint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct negate<uint<r, x, word>> {
        sint<r, x, word> operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct negate<sint<r, x, word>> {
        sint<r, x, word> operator () (const sint<r, x, word> &);
    };
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    struct quadrance<number::bounded<u, r, x, word>> {
        number::bounded<u, r, x, word> operator () (const number::bounded<u, r, x, word> &z) {
            return z * z;
        }
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct sign<uint<r, x, word>> {
        signature operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct sign<sint<r, x, word>> {
        signature operator () (const sint<r, x, word> &);
    };

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    struct re<number::bounded<u, r, x, word>> {
        number::bounded<u, r, x, word> operator () (const number::bounded<u, r, x, word> &);
    };

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    struct im<number::bounded<u, r, x, word>> {
        number::bounded<u, r, x, word> operator () (const number::bounded<u, r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct conjugate<sint<r, x, word>> {
        sint<r, x, word> operator () (const sint<r, x, word> &);
    };

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    struct inner<number::bounded<u, r, x, word>> {
        number::bounded<u, r, x, word> & operator () (const number::bounded<u, r, x, word> &, const number::bounded<u, r, x, word> &);
    };
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    struct is_zero<number::bounded<u, r, x, word>> {
        bool operator () (const number::bounded<u, r, x, word> &);
    };
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    struct is_negative<uint<r, x, word>> {
        bool operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct is_negative<sint<r, x, word>> {
        bool operator () (const sint<r, x, word> &);
    };
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    struct is_positive<uint<r, x, word>> {
        bool operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct is_positive<sint<r, x, word>> {
        bool operator () (const sint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct divide<uint<r, x, word>, uint<r, x, word>> {
        division<uint<r, x, word>, uint<r, x, word>> operator () (const uint<r, x, word> &, const nonzero<uint<r, x, word>> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct divide<sint<r, x, word>, sint<r, x, word>> {
        division<sint<r, x, word>, uint<r, x, word>> operator () (const sint<r, x, word> &, const nonzero<sint<r, x, word>> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct divide<sint<r, x, word>, uint<r, x, word>> {
        division<sint<r, x, word>, uint<r, x, word>> operator () (const sint<r, x, word> &, const nonzero<uint<r, x, word>> &);
    };

    template <bool a, bool b, endian::order r, size_t x, std::unsigned_integral word>
    struct plus_mod<number::bounded<a, r, x, word>, number::bounded<b, r, x, word>, uint<r, x, word>> {
        uint<r, x, word> operator () (
            const number::bounded<a, r, x, word> &,
            const number::bounded<b, r, x, word> &,
            const nonzero<uint<r, x, word>> &);
    };

    template <bool a, endian::order r, size_t x, std::unsigned_integral word>
    struct square_mod<number::bounded<a, r, x, word>, uint<r, x, word>> {
        uint<r, x, word> operator () (
            const number::bounded<a, r, x, word> &,
            const nonzero<uint<r, x, word>> &);
    };

    template <bool a, bool b, endian::order r, size_t x, std::unsigned_integral word>
    struct times_mod<number::bounded<a, r, x, word>, number::bounded<b, r, x, word>, uint<r, x, word>> {
        uint<r, x, word> operator () (
            const number::bounded<a, r, x, word> &,
            const number::bounded<b, r, x, word> &,
            const nonzero<uint<r, x, word>> &);
    };

    template <bool a, bool b, endian::order r, size_t x, std::unsigned_integral word>
    struct pow_mod<number::bounded<a, r, x, word>, number::bounded<b, r, x, word>, uint<r, x, word>> {
        uint<r, x, word> operator () (
            const number::bounded<a, r, x, word> &,
            const number::bounded<b, r, x, word> &,
            const nonzero<uint<r, x, word>> &);
    };

    template <bool a, endian::order r, size_t x, std::unsigned_integral word>
    struct invert_mod<number::bounded<a, r, x, word>, uint<r, x, word>> {
        maybe<uint<r, x, word>> operator () (
            const number::bounded<a, r, x, word> &,
            const nonzero<uint<r, x, word>> &);
    };
}

namespace data::encoding::decimal {
    constexpr bool valid (string_view s);

    struct string;

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &write (std::ostream &o, const math::uint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    string write (const math::uint<r, x, word> &);

}

namespace data::encoding::signed_decimal {
    constexpr bool valid (string_view s);

    struct string;

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &write (std::ostream &o, const math::sint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    string write (const math::sint<r, x, word> &);

}

namespace data::encoding::hexidecimal {
    constexpr bool valid (string_view s);

    template <endian::order r, std::unsigned_integral word, size_t x>
    std::ostream &write (std::ostream &, const oriented<r, word, x> &, hex_case q = hex_case::lower);

    template <hex_case zz> struct string;

    template <hex_case zz, endian::order r, std::unsigned_integral word, size_t x>
    string<zz> write (const oriented<r, word, x> &);

}

// here we have some arithmetic with other number types.
namespace data::math::number {
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    bool operator == (const bounded<x, r, n, word> &, const endian::integral<y, o, z> &);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    std::strong_ordering operator <=> (const bounded<x, r, n, word> &, const endian::integral<y, o, z> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    bool operator == (const sint<r, size, word> &, const Z_bytes<o, negativity::twos, word> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    std::weak_ordering operator <=> (const sint<r, size, word> &, const Z_bytes<o, negativity::twos, word> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    bool operator == (const uint<r, size, word> &, const N_bytes<o, word> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    std::weak_ordering operator <=> (const uint<r, size, word> &, const N_bytes<o, word> &);

    template <std::integral I, bool u, endian::order r, size_t size, std::unsigned_integral word>
    bool operator == (I, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word, std::integral I>
    bool operator == (const bounded<u, r, size, word> &, I);

    template <std::integral I, bool u, endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (I x, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word, std::integral I>
    std::strong_ordering operator <=> (const bounded<u, r, size, word> &, I x);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering operator <=> (const uint<r, x, word> &a, int64 b);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering operator <=> (const sint<r, x, word> &a, int64 b);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering operator <=> (const uint<r, x, word> &a, uint64 b);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering operator <=> (const sint<r, x, word> &a, uint64 b);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering operator <=> (const uint<r, x, word> &a, int32 b);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering operator <=> (const sint<r, x, word> &a, int32 b);

    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator | (const sint<r, size, word> &, const uint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> operator | (const uint<r, size, word> &, uint64);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator & (const sint<r, size, word> &, const uint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> operator & (const uint<r, size, word> &, uint64);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator ^ (const sint<r, size, word> &, const uint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator + (const sint<r, size, word> &, const uint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator - (const sint<r, size, word> &, const uint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator * (const sint<r, size, word> &, const uint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator | (const uint<r, size, word> &, const sint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator & (const uint<r, size, word> &, const sint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator ^ (const uint<r, size, word> &, const sint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator + (const uint<r, size, word> &, const sint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator - (const uint<r, size, word> &, const sint<r, size, word> &);
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> operator * (const uint<r, size, word> &, const sint<r, size, word> &);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator + (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator + (const sint<r, x, word> &, int64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator - (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator - (const sint<r, x, word> &, int64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator * (const uint<r, x, word> &, uint64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator * (const sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator & (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline operator & (const sint<r, x, word> &z, int64 u) {
        return z & sint<r, x, word> {u};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator | (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator | (const sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator ^ (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator ^ (const sint<r, x, word> &, int64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> operator / (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> operator / (const sint<r, x, word> &, int64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint64 operator % (const uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint64 operator % (const sint<r, x, word> &, uint64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator += (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator += (sint<r, x, word> &, int64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator -= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator -= (sint<r, x, word> &, int64);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator *= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator *= (sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator &= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator &= (sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator |= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator |= (sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator ^= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator ^= (sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator /= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator /= (sint<r, x, word> &, int64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator %= (uint<r, x, word> &, uint64);

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> &operator /= (bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t z, std::unsigned_integral word>
    uint<r, z, word> &operator %= (bounded<u, r, z, word> &, const uint<r, z, word> &);
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> &operator /= (uint<r, x, word> &, uint64);

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> &operator /= (sint<r, x, word> &, int64);

    template <endian::order r, size_t size, std::unsigned_integral word>
    struct bounded<false, r, size, word> : oriented<r, word, size> {
        
        constexpr bounded () : oriented<r, word, size> () {
            for (word &w : *this) w = 0;
        }

        // We need these to ensure that we can accept
        // any number literal.
        template <std::signed_integral I> constexpr bounded (I);
        template <std::unsigned_integral I> constexpr bounded (I);
        
        constexpr explicit bounded (slice<const word, size> x);
        
        // The string can be a hex string or a representation of a number. 
        //explicit bounded (const data::string &s): bounded {read (s)} {}
        explicit bounded (const char *x): bounded {read (string_view {x, std::strlen (x)})} {}
        explicit bounded (const string_view);
        static bounded read (string_view x) {
            return bounded {x};
        }
        
        division<bounded> divide (const bounded &) const;
        
        static bounded max ();
        static bounded min ();
        static N_bytes<r, word> modulus ();
        
        operator N_bytes<r, word> () const;
        bounded (const N_bytes<r, word> &);
        
        explicit operator double () const;

        // we can implicitly convert to any bigger bounded number, signed or unsigned.
        template <bool x, size_t bigger> requires (bigger > size)
        operator bounded<x, r, bigger, word> () const {
            bounded<x, r, bigger, word> n {0};
            std::copy (this->words ().begin (), this->words ().end (), n.words ().begin ());
            return n;
        }

        // we can explicitly convert to a signed bounded number of the same size.
        explicit operator bounded<true, r, size, word> () const {
            bounded<true, r, size, word> n;
            std::copy (this->begin (), this->end (), n.begin ());
            return n;
        }

        // we can explicitly convert to a smaller number.
        template <bool x, size_t smaller> requires (smaller < size)
        explicit operator bounded<x, r, smaller, word> () const {
            bounded<x, r, smaller, word> n;
            std::copy (this->words ().begin (), this->words ().begin () + smaller, n.words ().begin ());
            return n;
        }

        explicit operator uint64 () const {
            if constexpr (!Same<word, byte>) throw data::exception {"unimplemented function"};
            uint64_little u {0};
            if constexpr (size <= 8) {
                std::copy (this->words ().begin (), this->words ().end (), u.begin ());
            } else if (*this > std::numeric_limits<uint64>::max ())
                throw data::exception {"Number is too big to cast to uint64"};
            else std::copy (this->words ().begin (), this->words ().begin () + 8, u.begin ());
            return uint64 (u);
        }
        
    };
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    struct bounded<true, r, size, word> : oriented<r, word, size> {
        
        constexpr bounded () : oriented<r, word, size> {} {
            for (word &w : *this) w = 0;
        }
        
        constexpr bounded (const int64 &);
        constexpr bounded (const int32 &);
        constexpr bounded (uint64 x);
        constexpr bounded (uint32);
        
        constexpr explicit bounded (slice<const word, size> &x) : oriented<r, word, size> {x} {}
        
        explicit bounded (const char *x): bounded {read (string_view {x, std::strlen (x)})} {}
        explicit bounded (const data::string &s): bounded {read (s)} {}
        explicit bounded (string_view s);
        static bounded read (string_view x) {
            return bounded {x};
        }

        division<bounded> divide (const bounded &) const;
        
        operator Z_bytes<r, negativity::twos, word> () const;
        
        explicit bounded (slice<word, size>);
        
        explicit operator double () const;
        
        static bounded max ();
        static bounded min ();

        explicit operator int64 () const;
        
        constexpr explicit bounded (const Z_bytes<r, negativity::twos, word> &z) {
            if (z.size () <= size) {
                std::copy (z.words ().begin (), z.words ().end (), this->words ().begin ());
                char leading = data::is_negative (z) ? 0xff : 0x00;
                for (int i = z.size (); i < size; i++) this->words ()[i] = leading;
            } else if (z <= Z_bytes<r, negativity::twos, word> {max ()} && z >= Z_bytes<r, negativity::twos, word> {min ()})
                std::copy (z.words ().begin (), z.words ().begin () + size, this->begin ());
            else throw exception {} << "Z_bytes too big";
        }

        // implicitly convert to a bigger signed size.
        template <size_t bigger> requires (bigger > size)
        operator bounded<true, r, bigger, word> () const {
            bounded<true, r, bigger, word> n {*this < 0 ? -1 : 0};
            std::copy (this->words().begin (), this->words ().end (), n.words ().begin ());
            return n;
        }

        // explicitly convert to an unsigned number of the same size.
        template <size_t any>
        explicit operator bounded<false, r, any, word> () const {
            bounded<false, r, any, word> n {0};
            std::copy (this->begin (), this->end (), n.begin ());
            return n;
        }

        // explicitly convert to smaller sizes.
        template <bool x, size_t smaller> requires (smaller < size)
        explicit operator bounded<true, r, smaller, word> () const {
            bounded<true, r, smaller, word> n {*this < 0 ? -1 : 0};
            std::copy (this->begin (), this->begin () + smaller, n.begin ());
            return n;
        }
        
    };
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline operator ~ (const bounded<u, r, x, word> &n) {
        auto z = n;
        arithmetic::bit_negate<word> (z.end (), z.begin (), z.begin ());
        return z;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    bool inline operator == (const bounded<x, r, n, word> &a, const bounded<y, o, z, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    bool inline operator == (const bounded<x, r, n, word> &a, const endian::integral<y, o, z> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    bool inline operator == (const sint<r, size, word> &a, const Z_bytes<o, negativity::twos, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    bool inline operator == (const uint<r, size, word> &a, const N_bytes<o, word> &b) {
        return (a <=> b) == 0;
    }

    template <std::integral I, bool u, endian::order r, size_t size, std::unsigned_integral word>
    bool inline operator == (I x, const bounded<u, r, size, word> &y) {
        return (x <=> y) == 0;
    }

    template <bool u, endian::order r, size_t size, std::unsigned_integral word, std::integral I>
    bool inline operator == (const bounded<u, r, size, word> &x, I y) {
        return (x <=> y) == 0;
    }

    template <std::integral I, bool u, endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (I x, const bounded<u, r, size, word> &n) {
        return bounded<u, r, size, word> {x} <=> n;
    }

    template <bool u, endian::order r, size_t size, std::unsigned_integral word, std::integral I>
    std::strong_ordering inline operator <=> (const bounded<u, r, size, word> &n, I x) {
        return n <=> bounded<u, r, size, word> {x};
    }
    
    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const sint<r, size, word> &a, const Z_bytes<o, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) <=> b;
    }
    
    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const uint<r, size, word> &a, const N_bytes<o, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) <=> b;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const bounded<x, r, n, word> &a, const endian::integral<y, o, z> &b) {
        return a <=> bounded<y, o, z, word> (b);
    }

    template <data::endian::order r, size_t size, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const uint<r, size, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::decimal::write (o, n);
        throw exception {} << "Try to write a uint with invalid flags";
    }

    template <data::endian::order r, size_t size, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const sint<r, size, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::signed_decimal::write (o, n);
        throw exception {} << "Try to write a uint with invalid flags";
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline operator * (const uint<r, x, word> &a, uint64 b) {
        return a * uint<r, x, word> (b);
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline operator ++ (bounded<u, r, size, word> &x, int) {
        bounded n = x;
        ++x;
        return n;
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline operator -- (bounded<u, r, size, word> &x, int) {
        bounded n = x;
        ++x;
        return n;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator | (const sint<r, size, word> &a, const uint<r, size, word> &b) {
        sint<r, size, word> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a | x;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator & (const sint<r, size, word> &a, const uint<r, size, word> &b) {
        sint<r, size, word> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a & x;        
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator ^ (const sint<r, size, word> &a, const uint<r, size, word> &b) {
        sint<r, size, word> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a ^ x;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator + (const sint<r, size, word> &a, const uint<r, size, word> &b) {
        sint<r, size, word> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a + x;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator - (const sint<r, size, word> &a, const uint<r, size, word> &b) {
        sint<r, size, word> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a - x;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator * (const sint<r, size, word> &a, const uint<r, size, word> &b) {
        sint<r, size, word> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a * x;
    }

    template <endian::order r, size_t z, std::unsigned_integral word>
    uint<r, z, word> inline &operator /= (uint<r, z, word> &a, uint64 b) {
        return a = a / b;
    }

    template <endian::order r, size_t z, std::unsigned_integral word>
    sint<r, z, word> inline &operator /= (sint<r, z, word> &a, int64 b) {
        return a = a / b;
    }

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline &operator /= (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        return a = a / b;
    }

    template <bool u, endian::order r, size_t z, std::unsigned_integral word>
    uint<r, z, word> inline &operator %= (bounded<u, r, z, word> &a, const uint<r, z, word> &b) {
        return a = a % b;
    }

}

namespace data {
    
    namespace encoding::hexidecimal {

        template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
        std::string inline write (const math::number::bounded<is_signed, r, size, word> &n) {
            std::stringstream ss;
            ss << std::hex << n;
            return ss.str ();
        }

    }

    namespace encoding::decimal {

        template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
        std::string inline write (const math::number::bounded<is_signed, r, size, word> &n) {
            std::stringstream ss;
            ss << std::dec << n;
            return ss.str ();
        }

    }

    template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
    reader<word> inline &operator >> (reader<word> &rr, math::number::bounded<is_signed, r, size, word> &x) {
        rr.read (x.data (), size);
        return rr;
    }

}

namespace data::math::number {

    template <endian::order r, size_t x, std::unsigned_integral word>
    nonzero<uint<r, x, word>> inline increment<uint<r, x, word>>::operator () (const uint<r, x, word> &n) {
        if (n == uint<r, x, word>::max ()) throw exception {} << "cannot increment";
        return nonzero<uint<r, x, word>> {n + 1};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline decrement<uint<r, x, word>>::operator () (const nonzero<uint<r, x, word>> &n) {
        auto z = n.Value;
        return --z;
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline decrement<uint<r, x, word>>::operator () (const uint<r, x, word> &n) {
        if (data::is_zero (n)) return n;
        auto z = n;
        return --z;
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline increment<sint<r, x, word>>::operator () (const sint<r, x, word> &n) {
        if (n == sint<r, x, word>::max ()) throw exception {} << "cannot increment";
        auto z = n;
        return ++z;
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline decrement<sint<r, x, word>>::operator () (const sint<r, x, word> &n) {
        if (n == sint<r, x, word>::min ()) throw exception {} << "cannot decrement";
        auto z = n;
        return --z;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const sint<r, size, word> &a, const sint<r, size, word> &b) {
        bool na = data::is_negative (a);
        bool nb = data::is_negative (b);
        if (na == nb) return arithmetic::compare<negativity::nones> (a.words (), b.words ());
        return na ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (const uint<r, size, word> &a, const uint<r, size, word> &b) {
        return arithmetic::compare<negativity::nones> (a.words (), b.words ());
    }

    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (const bounded<x, r, n, word> &a, const bounded<y, o, z, word> &b) {
        if constexpr (x && !y) {
            if (a < 0) return std::strong_ordering::less;
            return arithmetic::compare<negativity::nones> (a.words (), b.words ());
        } else if constexpr (y && !x) {
            if (b < 0) return std::strong_ordering::greater;
            return arithmetic::compare<negativity::nones> (a.words (), b.words ());
        } else throw exception {} << "undefined method bounded <=> bounded";
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> inline operator | (const uint<r, size, word> &a, uint64 b) {
        return a | uint<r, size, word> (b);
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> inline operator & (const uint<r, size, word> &a, uint64 b) {
        return a & uint<r, size, word> (b);
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator | (const uint<r, size, word> &a, const sint<r, size, word> &b) {
        return sint<r, size, word> (a) | b;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator & (const uint<r, size, word> &a, const sint<r, size, word> &b) {
        return sint<r, size, word> (a) & b;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator ^ (const uint<r, size, word> &a, const sint<r, size, word> &b) {
        return sint<r, size, word> (a) ^ b;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator + (const uint<r, size, word> &a, const sint<r, size, word> &b) {
        return sint<r, size, word> (a) + b;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator - (const uint<r, size, word> &a, const sint<r, size, word> &b) {
        return sint<r, size, word> (a) - b;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline operator * (const uint<r, size, word> &a, const sint<r, size, word> &b) {
        return sint<r, size, word> (a) * b;
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline operator + (const sint<r, x, word> &a, int64 b) {
        return a + sint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline operator - (const sint<r, x, word> &a, int64 b) {
        return a - sint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline operator * (const sint<r, x, word> &a, int64 b) {
        return a * sint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline &operator += (uint<r, x, word> &a, uint64 b) {
        return a += uint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline &operator += (sint<r, x, word> &a, int64 b) {
        return a += sint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline &operator -= (uint<r, x, word> &a, uint64 b) {
        return a -= uint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline &operator -= (sint<r, x, word> &a, int64 b) {
        return a -= sint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline &operator *= (uint<r, x, word> &a, uint64 b) {
        return a *= uint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline &operator *= (sint<r, x, word> &a, int64 b) {
        return a *= sint<r, x, word> (b);
    }
    
    template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
    bounded<is_signed, r, size, word> inline operator << (const bounded<is_signed, r, size, word> &z, int i) {
        auto n = z;
        return n <<= i;
    }
    
    template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
    bounded<is_signed, r, size, word> inline operator >> (const bounded<is_signed, r, size, word> &z, int i) {
        auto n = z;
        return n >>= i;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    inline bounded<true, r, size, word> operator - (const bounded<true, r, size, word> &a) {
        auto x = ~a;
        return ++x;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    bool inline operator == (const uint<r, size, word> &a, uint64 b) {
        return a == uint<r, size, word> {b};
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    bool inline operator == (const sint<r, size, word> &a, int64 b) {
        return a == sint<r, size, word> {b};
    }
    
}

namespace data::math {

    template <endian::order r, size_t x, std::unsigned_integral word>
    math::signature inline sign<math::uint<r, x, word>>::operator () (const math::uint<r, x, word> &z) {
        return arithmetic::nones::sign (z.words ());
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    math::signature inline sign<math::sint<r, x, word>>::operator () (const math::sint<r, x, word> &z) {
        return arithmetic::twos::sign (z.words ());
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    bool inline is_positive<uint<r, x, word>>::operator () (const uint<r, x, word> &n) {
        return !data::is_zero (n);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    bool inline is_positive<sint<r, x, word>>::operator () (const sint<r, x, word> &n) {
        return !data::is_negative (n) && !data::is_zero (n);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    bool inline is_negative<uint<r, x, word>>::operator () (const uint<r, x, word> &n) {
        return false;
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    bool inline is_negative<sint<r, x, word>>::operator () (const sint<r, x, word> &z) {
        return arithmetic::sign_bit (z.words ());
    }

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bool inline is_zero<number::bounded<u, r, x, word>>::operator () (const number::bounded<u, r, x, word> &z) {
        return arithmetic::is_zero (z.words ());
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline abs<uint<r, x, word>>::operator () (const uint<r, x, word> &z) {
        return z;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline abs<sint<r, x, word>>::operator () (const sint<r, x, word> &z) {
        return z < 0 ? -z : z;
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    number::bounded<u, r, x, word> inline inverse<plus<number::bounded<u, r, x, word>>, number::bounded<u, r, x, word>>::operator () (
        const number::bounded<u, r, x, word> &a,
        const number::bounded<u, r, x, word> &b) {
        return b - a;
    }
}

namespace data::math::number {

    template <endian::order r, size_t size, std::unsigned_integral word>
    bounded<false, r, size, word>::bounded (string_view x) {
        if (encoding::decimal::valid (x) || encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
            *this = bounded {N_bytes<r, word>::read (x)};
        else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
            boost::algorithm::unhex (x.begin (), x.end (), this->begin ());
        else throw data::exception {} << "invalid natural string \"" << x << "\"";
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    bounded<true, r, size, word>::bounded (string_view x) {
        if (encoding::signed_decimal::valid (x) ||
            encoding::hexidecimal::valid (x) && x.size () == 2 * size * sizeof (word) + 2)
                *this = bounded {Z_bytes<r, negativity::twos, word>::read (x)};
        else if (encoding::hex::valid (x) && x.size () == 2 * size * sizeof (word))
            boost::algorithm::unhex (x.begin (), x.end (), this->begin ());
        else throw exception {} << "invalid integer string \"" << x << "\"";
    }
    
    template <endian::order o, size_t size, std::unsigned_integral word>
    uint<o, size, word> bounded<false, o, size, word>::min () {
        bounded b {};
        for (int i = 0; i <= size; i++) b[i] = 0x00;
        return b;
    }
 
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline operator ^ (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        bounded<u, r, x, word> z {};
        arithmetic::bit_xor<word> (z.end (), z.begin (), a.begin (), b.begin ());
        return z;
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline operator & (const bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
        bounded<u, r, size, word> x;
        arithmetic::bit_and<word>
            (x.words ().end (), x.words ().begin (), a.words ().begin (), b.words ().begin ());
        return x;
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline operator | (const bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
        bounded<u, r, size, word> x;
        arithmetic::bit_or<word>
            (x.words ().end (), x.words ().begin (), a.words ().begin (), b.words ().begin ());
        return x;
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline &operator &= (bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
        arithmetic::bit_and<word> (a.words ().end (), a.words ().begin (),
            const_cast<const bounded<u, r, size, word> &> (a).words ().begin (), b.words ().begin ());
        return a;
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline &operator |= (bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
        arithmetic::bit_or<word> (a.words ().end (), a.words ().begin (),
            const_cast<const bounded<u, r, size, word> &> (a).words ().begin (), b.words ().begin ());
        return a;
    }
    
    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    bounded<u, r, size, word> inline &operator ^= (bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
        arithmetic::bit_xor<word> (a.words ().end (), a.words ().begin (),
            const_cast<const bounded<u, r, size, word> &> (a).words ().begin (), b.words ().begin ());
        return a;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const uint<r, x, word> &a, int64 b) {
        if (b < 0) return std::weak_ordering::greater;
        return a <=> uint<r, x, word> {static_cast<uint64> (b)};
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const sint<r, x, word> &a, int64 b) {
        return a <=> sint<r, x, word> {b};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const uint<r, x, word> &a, uint64 b) {
        return a <=> uint<r, x, word> {b};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const sint<r, x, word> &a, uint64 b) {
        return a <=> uint<r, x, word> {b};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const uint<r, x, word> &a, int32 b) {
        if (b < 0) return std::weak_ordering::greater;
        return a <=> uint<r, x, word> {static_cast<uint32> (b)};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const sint<r, x, word> &a, int32 b) {
        return a <=> sint<r, x, word> {b};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const uint<r, x, word> &a, uint32 b) {
        return a <=> uint<r, x, word> {b};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const sint<r, x, word> &a, uint32 b) {
        return a <=> uint<r, x, word> {b};
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    template <std::signed_integral I>
    constexpr bounded<false, r, size, word>::bounded (I x): bounded {} {
        if (x < 0) throw exception {} << "assign negative number to unsigned bounded";
        if constexpr (sizeof (I) <= sizeof (word)) {
            *this->words ().begin () = x;
        } else {
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (I) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    template <std::unsigned_integral I>
    constexpr bounded<false, r, size, word>::bounded (I x): bounded {} {
        if constexpr (sizeof (I) <= sizeof (word)) *this->words ().begin () = x;
        else {
            auto w = this->words ();
            auto i = w.begin ();
            for (int n = 0; n < sizeof (I) / sizeof (word); n++) {
                *i = static_cast<word> (x & std::numeric_limits<word>::max ());
                x >>= sizeof (word) * 8;
                i++;
            }
        }
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<true, r, size, word>::bounded (uint64 x) : bounded {} {
        if constexpr (sizeof (uint64) <= sizeof (word)) *this->words ().begin () = x;

        else {


            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint64) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<true, r, size, word>::bounded (const int32 &x) : oriented<r, word, size>
        {x < 0 ?
            bytes_array<word, size>::filled (std::numeric_limits<word>::max ()) :
            bytes_array<word, size>::filled (0x00)} {

        if constexpr (sizeof (int32) <= sizeof (word)) {
            *this->words ().begin () = x;
        } else {
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (int32) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<true, r, size, word>::bounded (uint32 x) : bounded {} {
        if constexpr (sizeof (uint32) <= sizeof (word)) {
            *this->words ().begin () = x;
        } else {
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint32) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<true, r, size, word>::bounded (const int64 &x) : oriented<r, word, size>
        {x < 0 ?
            bytes_array<word, size>::filled (std::numeric_limits<word>::max ()) :
            bytes_array<word, size>::filled (0x00)} {

        if constexpr (sizeof (int64) <= sizeof (word)) {
            *this->words ().begin () = x;
        } else {
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (int64) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<false, r, size, word>::bounded (slice<const word, size> x) {
        std::copy (x.begin (), x.end (), this->begin ());
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    bounded<false, r, size, word>::operator N_bytes<r, word> () const {
        N_bytes<r, word> n {};
        n.resize (size);
        std::copy (this->begin (), this->end (), n.begin ());
        return n;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    bounded<true, r, size, word>::operator Z_bytes<r, negativity::twos, word> () const {
        Z_bytes<r, negativity::twos, word> z {};
        z.resize (size);
        std::copy (this->begin (), this->end (), z.begin ());
        return z;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> inline operator + (const uint<r, size, word> &a, uint64 b) {
        return a + bounded<false, r, size, word> {b};
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline operator - (const bounded<u, r, x, word> &n) {
        bounded<u, r, x, word> z;
        arithmetic::bit_negate<word> (z.words ().end (), z.words ().begin (), n.words ().begin ());
        return ++z;
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator + (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        bounded<u, r, x, word> z {};
        auto i = z.words ().begin ();
        auto j = a.words ().begin ();
        auto k = b.words ().begin ();
        arithmetic::add_with_carry<word> (z.words ().end (), i, j, k);
        return z;
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> operator - (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        bounded<u, r, x, word> z {};
        auto i = z.words ().begin ();
        auto j = a.words ().begin ();
        auto k = b.words ().begin ();
        arithmetic::subtract_with_carry<word> (z.words ().end (), i, j, k);
        return z;
    }

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> &operator += (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        auto awb = a.words ().begin ();
        auto cawb = const_cast<const bounded<u, r, x, word> &> (a).words ().begin ();
        auto bwb = b.words ().begin ();
        arithmetic::add_with_carry<word> (a.words ().end (), awb, cawb, bwb);
        return a;
    }

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> &operator -= (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        auto awb = a.words ().begin ();
        auto cawb = const_cast<const bounded<u, r, x, word> &> (a).words ().begin ();
        auto bwb = b.words ().begin ();
        arithmetic::subtract_with_carry<word> (a.words ().end (), awb, cawb, bwb);
        return a;
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline operator * (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        bounded<u, r, x, word> z {};
        auto w = z.words ();
        arithmetic::times (w, a.words (), b.words ());
        return z;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> inline operator - (const uint<r, size, word> &a, uint64 b) {
        return a - bounded<false, r, size, word> {b};
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline &operator *= (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        return a = a * b;
    }
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline &operator ++ (bounded<u, r, x, word> &n) {
        auto o = n.words ().begin ();
        auto i = n.words ().begin ();
        arithmetic::add_with_carry<word> (n.words ().end (), o, i, 1);
        return n;
    }   
    
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline &operator -- (bounded<u, r, x, word> &n) {
        auto xx = n.words ().begin ();
        auto xy = n.words ().begin ();
        arithmetic::subtract_with_carry<word> (n.words ().end (), xx, xy, 1);
        return n;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    uint<r, size, word> inline bounded<false, r, size, word>::max () {
        uint<r, size, word> n;
        arithmetic::set_max_unsigned (n.words ());
        return n;
    }
        
    template <endian::order r, size_t size, std::unsigned_integral word>
    N_bytes<r, word> inline bounded<false, r, size, word>::modulus () {
        return N_bytes<r, word> (max ()) + 1;
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline bounded<true, r, size, word>::max () {
        uint<r, size, word> n;
        arithmetic::set_max_signed_ones (n.words ());
        return sint<r, size, word> (n);
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    sint<r, size, word> inline bounded<true, r, size, word>::min () {
        uint<r, size, word> n;
        arithmetic::set_min_signed_ones (n.words ());
        return sint<r, size, word> (n);
    }
    
    namespace {
        template <endian::order r, size_t size, std::unsigned_integral word>
        void shift_right (bytes_array<word, size> &n, uint32 i, byte fill) {
            if (r == endian::big) arithmetic::bit_shift_right<word> (n.rbegin (), n.rend (), i, fill);
            else arithmetic::bit_shift_right<word> (n.begin (), n.end (), i, fill);
        }
        
        template <endian::order r, size_t size, std::unsigned_integral word>
        void shift_left (bytes_array<word, size> &n, uint32 i, byte fill) {
            if (r == endian::big) arithmetic::bit_shift_left<word> (n.begin (), n.end (), i, fill);
            else arithmetic::bit_shift_left<word> (n.rbegin (), n.rend (), i, fill);
        }
    }
    
    template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
    bounded<is_signed, r, size, word> inline &operator <<= (bounded<is_signed, r, size, word> &n, int i) {
        (i < 0 ? shift_right<r, size, word> : shift_left<r, size, word>) (n, i, data::is_negative (n));
        return n; 
    }
    
    template <bool is_signed, endian::order r, size_t size, std::unsigned_integral word>
    bounded<is_signed, r, size, word> inline &operator >>= (bounded<is_signed, r, size, word> &n, int i) {
        (i < 0 ? shift_left<r, size, word> : shift_right<r, size, word>) (n, i, data::is_negative (n));
        return n; 
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    bounded<false, r, size, word>::bounded (const N_bytes<r, word> &n) {
        auto nt = trim (n);
        if (nt.size () > size) throw exception {} << "too big";
        auto nx = extend (n, size);
        std::copy (nx.begin (), nx.end (), this->begin ());
    }
    
}

#endif
