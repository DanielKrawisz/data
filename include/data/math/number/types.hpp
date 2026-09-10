// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_TYPES
#define DATA_MATH_NUMBER_TYPES

#include <data/arithmetic.hpp>
#include <data/encoding/endian.hpp>
#include <data/arithmetic/negativity.hpp>
#include <data/math/power.hpp>

// Basic number types.
namespace data::math::number {

    // bounded numbers that resemble built-in types, only bigger.
    // satisfies range<word> and integral.
    template <bool u, endian::order, size_t size, std::unsigned_integral word = byte> struct bounded;

    // an implementation of the natural numbers that is
    // encoded as a big or little endian sequence of bytes.
    template <endian::order, std::unsigned_integral = byte> struct N_bytes;

    // similar implementation of the integers. We have two's complement
    // and the sign-and-magnetude system used in Bitcoin. N_bytes works
    // as the absolute value of the two's complement types. Bitcoin numbers
    // work as their own number system without a type for the naturals.
    template <endian::order, neg, std::unsigned_integral = byte> struct Z_bytes;

    struct N;
    struct Z;

}

// alternate names.
namespace data {
    using Z = math::number::Z;
    using N = math::number::N;
}

namespace data::math {

    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = number::Z_bytes<r, neg::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = number::Z_bytes<r, neg::BC, word>;

    // satisfies unsigned_integral
    template <endian::order r, size_t x, std::unsigned_integral word = byte>
    using uint = number::bounded<false, r, x, word>;

    // satisfies signed_integral
    template <endian::order r, size_t x, std::unsigned_integral word = byte>
    using sint = number::bounded<true, r, x, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using uint_little = typename number::bounded<false, endian::little, size, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using uint_big = typename number::bounded<false, endian::big, size, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using int_little = typename number::bounded<true, endian::little, size, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using int_big = typename number::bounded<true, endian::big, size, word>;

}

namespace data::encoding {
    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = math::number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = math::number::Z_bytes<r, neg::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = math::number::Z_bytes<r, neg::BC, word>;
}

// string encodings.
namespace data::encoding::decimal {

    constexpr bool valid (string_view s);

    template <endian::order r, std::unsigned_integral word> maybe<N_bytes<r, word>> read (string_view s);

    struct string;

    string write (const N &);

    std::ostream &write (std::ostream &, const N &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &write (std::ostream &o, const math::uint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    string write (const math::uint<r, x, word> &);

    template <endian::order r, std::unsigned_integral word> string write (const N_bytes<r, word> &z);

    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const N_bytes<r, word> &n);

    constexpr bool valid (string_view s);

}

namespace data::encoding::signed_decimal {
    constexpr bool valid (string_view s);

    template <endian::order r, neg n, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, n, word>> read (string_view);

    struct string;

    string write (const Z &);

    std::ostream &write (std::ostream &, const Z &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &write (std::ostream &o, const math::sint<r, x, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    string write (const math::sint<r, x, word> &);

    template <endian::order r, neg n, std::unsigned_integral word>
    string write (const math::number::Z_bytes<r, n, word> &);

    template <endian::order r, neg n, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, n, word> &);

}

namespace data::encoding::hexidecimal {
    constexpr bool valid (string_view s);

    template <hex_case zz> struct string;

    template <neg, hex_case> struct integer;

    template <hex_case zz> integer<neg::nones, zz> write (const N &);
    template <neg n, hex_case zz> integer<n, zz> write (const Z &);

    std::ostream &write (std::ostream &, const N &, hex_case = hex_case::lower);
    std::ostream &write (std::ostream &, const Z &, hex_case = hex_case::lower, neg = neg::twos);

    template <hex_case zz> struct string;

    template <hex_case zz, endian::order r, std::unsigned_integral word>
    integer<neg::nones, zz> write (const math::number::N_bytes<r, word> &);

    template <hex_case zz, endian::order r, neg n, std::unsigned_integral word>
    integer<n, zz> write (const math::number::Z_bytes<r, n, word> &);

}

namespace data::encoding::natural {

    template <endian::order r, std::unsigned_integral word>
    maybe<math::N_bytes<r, word>> read (string_view s);

}

namespace data::encoding::integer {

    template <endian::order r, neg c, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, c, word>> read (string_view s);

    template <endian::order r, neg c, std::unsigned_integral word>
    std::ostream &write (std::ostream &, const math::number::Z_bytes<r, c, word> &);

}

namespace data::math::number {

    // read from and write to streams.
    std::ostream &operator << (std::ostream &o, const Z &n);
    std::ostream &operator << (std::ostream &o, const N &n);

    std::istream &operator >> (std::istream &i, Z &z);
    std::istream &operator >> (std::istream &i, N &n);

    template <endian::order r, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &o, const N_bytes<r, word> &n);

    template <endian::order r, neg c, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &o, const Z_bytes<r, c, word> &n);

    template <endian::order r, std::unsigned_integral word>
    std::istream &operator >> (std::istream &o, N_bytes<r, word> &n);

    template <endian::order r, neg c, std::unsigned_integral word>
    std::istream &operator >> (std::istream &o, Z_bytes<r, c, word> &n);

    template <endian::order r, std::unsigned_integral word>
    writer<word> &operator << (writer<word> &o, const N_bytes<r, word> &n);

    template <endian::order r, neg c, std::unsigned_integral word>
    writer<word> &operator << (writer<word> &o, const Z_bytes<r, c, word> &n);

    template <endian::order r, std::unsigned_integral word>
    reader<word> &operator >> (reader<word> &o, N_bytes<r, word> &n);

    template <endian::order r, neg c, std::unsigned_integral word>
    reader<word> &operator >> (reader<word> &o, Z_bytes<r, c, word> &n);

    template <data::endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &s, const uint<r, x, word> &n);

    template <data::endian::order r, size_t x, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &s, const sint<r, x, word> &n);

    template <bool u, data::endian::order r, size_t x, std::unsigned_integral word>
    std::istream &operator >> (std::istream &i, const bounded<u, r, x, word> &n);

    template <bool u, data::endian::order r, size_t x, std::unsigned_integral word>
    writer<word> &operator << (writer<word> &o, const bounded<u, r, x, word> &n);

    template <bool u, data::endian::order r, size_t x, std::unsigned_integral word>
    reader<word> &operator >> (reader<word> &o, const bounded<u, r, x, word> &n);

    // increment and decrement.
    Z &operator ++ (Z &);
    Z &operator -- (Z &);

    Z operator ++ (Z &, int);
    Z operator -- (Z &, int);

    N &operator ++ (N &);
    N &operator -- (N &);

    N operator ++ (N &, int);
    N operator -- (N &, int);

    template <> struct increment<N> {
        nonzero<N> operator () (const N &);
    };

    template <> struct increment<Z> {
        Z operator () (const Z &);
    };

    template <> struct decrement<Z> {
        Z operator () (const Z &);
    };

    template <> struct decrement<N> {
        N operator () (const nonzero<N> &);
        N operator () (const N &);
    };

    //pre increment and decrement
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator ++ (N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator ++ (Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator -- (N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -- (Z_bytes<r, c, word> &);

    // post-increment and decrement
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator ++ (N_bytes<r, word> &, int);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator ++ (Z_bytes<r, c, word> &, int);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator -- (N_bytes<r, word> &, int);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator -- (Z_bytes<r, c, word> &, int);

    template <endian::order r, neg c, std::unsigned_integral word>
    struct increment<Z_bytes<r, c, word>> {
        Z_bytes<r, c, word> operator () (const Z_bytes<r, c, word> &);
    };

    template <endian::order r, neg c, std::unsigned_integral word>
    struct decrement<Z_bytes<r, c, word>> {
        Z_bytes<r, c, word> operator () (const Z_bytes<r, c, word> &);
    };

    template <endian::order r, std::unsigned_integral word> struct increment<N_bytes<r, word>> {
        nonzero<N_bytes<r, word>> operator () (const N_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word> struct decrement<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const nonzero<N_bytes<r, word>> &);
        N_bytes<r, word> operator () (const N_bytes<r, word> &);
    };

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> operator ++ (bounded<u, r, size, word> &, int);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator ++ (bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> operator -- (bounded<u, r, size, word> &, int);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator -- (bounded<u, r, size, word> &);

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct increment<uint<r, x, word>> {
        constexpr nonzero<uint<r, x, word>> operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct decrement<uint<r, x, word>> {
        constexpr uint<r, x, word> operator () (const nonzero<uint<r, x, word>> &);
        constexpr uint<r, x, word> operator () (const uint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct increment<sint<r, x, word>> {
        constexpr sint<r, x, word> operator () (const sint<r, x, word> &);
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct decrement<sint<r, x, word>> {
        constexpr sint<r, x, word> operator () (const sint<r, x, word> &);
    };
}

// signed versus unsigned.
namespace data {

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct make_unsigned<math::uint<r, x, word>> {
        using type = math::uint<r, x, word>;
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct make_signed<math::uint<r, x, word>> {
        using type = math::sint<r, x, word>;
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct make_unsigned<math::sint<r, x, word>> {
        using type = math::uint<r, x, word>;
    };

    template <endian::order r, size_t x, std::unsigned_integral word>
    struct make_signed<math::sint<r, x, word>> {
        using type = math::sint<r, x, word>;
    };

    template <endian::order a, std::unsigned_integral word>
    struct make_signed<math::Z_bytes_BC<a, word>> {
        using type = math::Z_bytes_BC<a, word>;
    };

    template <endian::order a, endian::order b, std::unsigned_integral word>
    bool identical (const math::number::N_bytes<a, word> &, const math::number::N_bytes<b, word> &);

    template <endian::order a, neg b, endian::order c, neg d, std::unsigned_integral word>
    bool identical (const math::number::Z_bytes<a, b, word> &, const math::number::Z_bytes<c, d, word> &);

}

namespace data::math::number {

    /*************************************************************/
    /*                 equality and comparison.                  */
    /*************************************************************/

    // For N, Z
    bool operator == (const Z &, const Z &);
    std::strong_ordering operator <=> (const Z &, const Z &);

    bool operator == (const N &, const N &);
    std::strong_ordering operator <=> (const N &, const N &);

    // N_bytes, Z_bytes
    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, neg cl, neg cr, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, cl, word> &, const Z_bytes<r, cr, word> &);

    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);

    template <endian::order r, neg cl, neg cr, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, cl, word> &, const Z_bytes<r, cl, word> &);

    // bounded
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    constexpr bool operator == (const bounded<x, r, n, word> &, const bounded<y, o, z, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr std::strong_ordering operator <=> (const sint<r, size, word> &, const sint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr std::strong_ordering operator <=> (const uint<r, size, word> &, const uint<r, size, word> &);

    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    constexpr std::strong_ordering operator <=> (const bounded<x, r, n, word> &, const bounded<y, o, z, word> &);

    // comparisons of N/Z with N_bytes/Z_bytes and bounded types.
    template <endian::order r, size_t size, std::unsigned_integral word>
    bool operator == (const Z &, const sint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const Z &, const sint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    bool operator == (const N &, const uint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const N &, const uint<r, size, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    bool operator == (const Z &, const Z_bytes<r, c, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    std::strong_ordering operator <=> (const Z &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    std::strong_ordering operator <=> (const N &, const N_bytes<r, word> &);

    // Equality of _bytes types with bounded
    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    bool operator == (const sint<r, size, word> &, const Z_bytes<o, neg::twos, word> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    std::weak_ordering operator <=> (const sint<r, size, word> &, const Z_bytes<o, neg::twos, word> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    bool operator == (const uint<r, size, word> &, const N_bytes<o, word> &);

    template <endian::order r, size_t size, endian::order o, std::unsigned_integral word>
    std::weak_ordering operator <=> (const uint<r, size, word> &, const N_bytes<o, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word,
    endian::order o, neg neg, std::unsigned_integral w>
    std::weak_ordering operator <=> (const uint<r, size, word> &, const Z_bytes<o, neg, w> &);

    // comparisons with endian integral types.
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    constexpr bool operator == (const bounded<x, r, n, word> &, const endian::integral<y, o, z> &);

    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z, std::unsigned_integral word>
    constexpr std::strong_ordering operator <=> (const bounded<x, r, n, word> &, const endian::integral<y, o, z> &);

    // comparisons with built-in types.
    template <std::integral I> bool operator == (const N &, I);

    template <std::integral I> bool operator == (const Z &, I);

    template <std::integral I> std::strong_ordering operator <=> (const N &, I);

    template <std::integral I> std::strong_ordering operator <=> (const Z &, I);

    // TODO: these ought to use a std::integral type parameter.
    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, c, word> &, int64);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word, std::integral I>
    constexpr bool operator == (const bounded<u, r, size, word> &, I);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word, std::integral I>
    constexpr std::strong_ordering operator <=> (const bounded<u, r, size, word> &, I x);

    /*************************************************************/
    /*                      bit operations                       */
    /*************************************************************/

    // bit negate
    Z operator ~ (const N &);
    Z operator ~ (const Z &);

    Z operator | (const Z &, const Z &);
    Z operator & (const Z &, const Z &);
    Z operator ^ (const Z &, const Z &);

    N operator | (const N &, const N &);
    N operator & (const N &, const N &);
    N operator ^ (const N &, const N &);

    Z operator << (const Z &, int);
    Z operator >> (const Z &, int);

    N operator << (const N &, int);
    N operator >> (const N &, int);

    template <std::unsigned_integral I> N operator ^ (I, const N &);
    template <std::unsigned_integral I> N operator ^ (const N &, I);

    template <std::unsigned_integral I> N operator & (I, const N &);
    template <std::unsigned_integral I> N operator & (const N &, I);

    template <std::unsigned_integral I> N operator | (I, const N &);
    template <std::unsigned_integral I> N operator | (const N &, I);

    Z &operator &= (Z &, const Z &);
    Z &operator |= (Z &, const Z &);
    Z &operator ^= (Z &, const Z &);

    N &operator &= (N &, const N &);
    N &operator |= (N &, const N &);
    N &operator ^= (N &, const N &);

    template <std::unsigned_integral I> N &operator &= (N &, I);
    template <std::unsigned_integral I> N &operator |= (N &, I);
    template <std::unsigned_integral I> N &operator ^= (N &, I);

    Z &operator <<= (Z &, int);
    Z &operator >>= (Z &, int);

    N &operator <<= (N &, int);
    N &operator >>= (N &, int);

    // bit operations for bounded types.
    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    constexpr bounded<u, r, x, word> operator ~ (const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    constexpr bounded<u, r, x, word> operator ^ (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    constexpr bounded<u, r, x, word> operator & (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    constexpr bounded<u, r, x, word> operator | (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> operator << (const bounded<u, r, size, word> &, int);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> operator >> (const bounded<u, r, size, word> &, int);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator &= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator |= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator ^= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr uint<r, size, word> &operator &= (uint<r, size, word> &, const sint<r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr uint<r, size, word> &operator |= (uint<r, size, word> &, const sint<r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr uint<r, size, word> &operator ^= (uint<r, size, word> &, const sint<r, size, word> &);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator <<= (bounded<u, r, size, word> &, int);

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<u, r, size, word> &operator >>= (bounded<u, r, size, word> &, int);

    // bit negate
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ~ (const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ~ (const Z_bytes<r, neg::twos, word> &);

    // bit xor
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator ^ (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator ^ (const N_bytes<r, word> &, I x);

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator ^ (I x, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &, I x);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ^
    (const Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator ^=
    (Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ^ (const Z_bytes<r, neg::twos, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ^ (const N_bytes<r, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator ^=
    (Z_bytes<r, neg::twos, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> operator ^ (const Z_bytes<r, neg::twos, word> &, I x);

    template <std::integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ^ (I x, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> &operator ^= (Z_bytes<r, neg::twos, word> &, I x);

    // bit and
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator & (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator & (const N_bytes<r, word> &, I x);

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator & (I x, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, neg::twos, word> operator & (const N_bytes<r, word> &, I x);

    template <std::signed_integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator & (I x, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &, I x);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator &
    (const Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator &=
    (Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator & (const Z_bytes<r, neg::twos, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator & (const N_bytes<r, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator &=
    (Z_bytes<r, neg::twos, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> operator & (const Z_bytes<r, neg::twos, word> &, I x);

    template <std::integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator & (I x, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> &operator &= (Z_bytes<r, neg::twos, word> &, I x);

    // bit or
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator | (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator | (const N_bytes<r, word> &, I x);

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator | (I x, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &, I x);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator |
    (const Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator |=
    (Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator | (const Z_bytes<r, neg::twos, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator | (const N_bytes<r, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator |=
    (Z_bytes<r, neg::twos, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> operator | (const Z_bytes<r, neg::twos, word> &, I x);

    template <std::integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator | (I x, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> &operator |= (Z_bytes<r, neg::twos, word> &, I x);

    // bit shift
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator << (const N_bytes<r, word> &, int);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator >> (const N_bytes<r, word> &, int);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator << (const Z_bytes<r, c, word> &, int);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator >> (const Z_bytes<r, c, word> &, int);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator <<= (N_bytes<r, word> &, int64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator <<= (Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator >>= (N_bytes<r, word> &, int64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator >>= (Z_bytes<r, c, word> &, int64);

    // logical (only for Bitcoin style numbers)
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator ! (const Z_bytes<r, neg::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator && (const Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator || (const Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);


    /*************************************************************/
    /*                        Arithmetic                         */
    /*************************************************************/

    // negation
    Z operator - (const N &);
    Z operator - (const Z &);

    Z operator + (const Z &, const Z &);
    Z operator - (const Z &, const Z &);
    Z operator * (const Z &, const Z &);

    N operator + (const N &, const N &);
    N operator - (const N &, const N &);
    N operator * (const N &, const N &);

    template <std::integral I> Z operator + (const Z &, I);

    template <std::integral I> Z operator - (const Z &, I);

    template <std::integral I> Z operator * (const Z &, I);

    template <std::signed_integral I> Z operator + (const N &, I);

    template <std::signed_integral I> Z operator - (const N &, I);

    template <std::signed_integral I> Z operator * (const N &, I);

    template <std::unsigned_integral I> N operator + (const N &, I);

    template <std::unsigned_integral I> N operator - (const N &, I);

    template <std::unsigned_integral I> N operator * (const N &, I);

    Z operator / (const Z &, const Z &);
    N operator / (const N &, const N &);

    Z operator / (const Z &, int64);
    N operator / (const N &, uint64);

    N operator % (const Z &, const Z &);
    N operator % (const Z &, const N &);
    N operator % (const N &, const N &);

    uint64 operator % (const Z &, uint64);
    uint64 operator % (const N &, uint64);

    Z &operator += (Z &, const Z &);
    Z &operator -= (Z &, const Z &);
    Z &operator *= (Z &, const Z &);
    Z &operator /= (Z &, const Z &);

    N &operator += (N &, const N &);
    N &operator -= (N &, const N &);
    N &operator *= (N &, const N &);
    N &operator /= (N &, const N &);
    N &operator %= (N &, const N &);

    template <std::unsigned_integral I> N &operator += (N &, I);
    template <std::unsigned_integral I> N &operator -= (N &, I);
    template <std::unsigned_integral I> N &operator *= (N &, I);
    template <std::unsigned_integral I> N &operator /= (N &, I);
    template <std::unsigned_integral I> N &operator %= (N &, I);

}

namespace data::math::def {

    template <> struct abs<Z> {
        N operator () (const Z &);
    };

    template <> struct abs<N> {
        N operator () (const N &n);
    };

    template <> struct times<Z> {
        Z operator () (const Z &a, const Z &b);
        nonzero<Z> operator () (const nonzero<Z> &a, const nonzero<Z> &b);
    };

    template <> struct divmod<N, N> {
        division<N, N> operator () (const N &a, const nonzero<N> &b);
    };

    template <> struct divmod<Z, N> {
        division<Z, N> operator () (const Z &a, const nonzero<N> &b);
    };

    template <> struct divmod<Z, Z> {
        division<Z, N> operator () (const Z &a, const nonzero<Z> &b);
    };

    template <> struct identity<plus<Z>, Z> {
        Z operator () ();
    };

    template <> struct inverse<plus<Z>, Z> {
        Z operator () (const Z &a, const Z &b);
    };

    template <> struct inverse<times<Z>, Z> {
        nonzero<Z> operator () (const nonzero<Z> &a, const nonzero<Z> &b);
    };

    template <> struct identity<times<Z>, Z> {
        Z operator () ();
    };

    template <> struct bit_xor<N> {
        N operator () (const N &a, const N &b);
    };

    template <> struct bit_xor<Z> {
        Z operator () (const Z &, const Z &);
    };

    template <> struct div_2<N> {
        N operator () (const N &a);
    };

    template <> struct div_2<Z> {
        Z operator () (const Z &a);
    };

    template <> struct mod_2<N> {
        N operator () (const N &a);
    };

    template <> struct mod_2<Z> {
        Z operator () (const Z &a);
    };

    template <group_number Exp>
    struct pow<N, Exp> {
        N operator () (const N &x, const Exp &y);
    };

    template <group_number Exp>
    struct pow<Z, Exp> {
        Z operator () (const Z &x, const Exp &y);
    };

    template <group_number Exp> struct pow_mod<N, Exp, N> {
        N operator () (const N &x, const Exp &y, const nonzero<N> &z);
    };

    template <group_number Exp> struct pow_mod<Z, Exp, N> {
        N operator () (const Z &x, const Exp &y, const nonzero<N> &z);
    };

    template <endian::order r, neg c, std::unsigned_integral word>
    struct convert<number::Z_bytes<r, c, word>, Z> {
        number::Z_bytes<r, c, word> operator () (const Z &) const;
    };

}

#endif
