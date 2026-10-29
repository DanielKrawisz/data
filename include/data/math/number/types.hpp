// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/arithmetic/negativity.hpp>

#include <data/math/group.hpp>

#include <data/complex.hpp>

#include <data/divmod.hpp>

#include <data/arithmetic.hpp>

#include <data/encoding/hex.hpp>

#include <data/encoding/endian.hpp>

#include <data/bytes.hpp>

// Basic number types.
namespace data::math::number {

    // bounded numbers that resemble built-in types, only bigger.
    // satisfies range<word> and integral.
    template <bool u, endian, size_t size, Word word = byte> struct bounded;

    // an implementation of the natural numbers that is
    // encoded as a big or little endian sequence of bytes.
    template <endian, Word = byte> struct N_bytes;

    // similar implementation of the integers. We have two's complement
    // and the sign-and-magnetude system used in Bitcoin. N_bytes works
    // as the absolute value of the two's complement types. Bitcoin numbers
    // work as their own number system without a type for the naturals.
    template <endian, negativity, Word = byte> struct Z_bytes;

    struct N;
    struct Z;

}

// alternate names.
namespace data {
    using Z = math::number::Z;
    using N = math::number::N;

    using N_bytes_little = math::number::N_bytes<endian::little, byte>;

    using N_bytes_big = math::number::N_bytes<endian::big, byte>;

    using Z_bytes_little = math::number::Z_bytes<endian::little, negativity::twos, byte>;

    using Z_bytes_big = math::number::Z_bytes<endian::big, negativity::twos, byte>;

    using Z_bytes_BC_little = math::number::Z_bytes<endian::little, negativity::BC, byte>;

    using Z_bytes_BC_big = math::number::Z_bytes<endian::big, negativity::BC, byte>;
}

namespace data::math {

    template <endian r, Word word = byte>
    using N_bytes = number::N_bytes<r, word>;

    template <endian r, Word word = byte>
    using Z_bytes = number::Z_bytes<r, negativity::twos, word>;

    template <endian r, Word word = byte>
    using Z_bytes_BC = number::Z_bytes<r, negativity::BC, word>;

    // satisfies UnsignedIntegral
    template <endian r, size_t x, Word word = byte>
    using uint = number::bounded<false, r, x, word>;

    // satisfies SignedIntegral
    template <endian r, size_t x, Word word = byte>
    using sint = number::bounded<true, r, x, word>;

    template <size_t size, Word word = byte>
    using uint_little = typename number::bounded<false, endian::little, size, word>;

    template <size_t size, Word word = byte>
    using uint_big = typename number::bounded<false, endian::big, size, word>;

    template <size_t size, Word word = byte>
    using int_little = typename number::bounded<true, endian::little, size, word>;

    template <size_t size, Word word = byte>
    using int_big = typename number::bounded<true, endian::big, size, word>;

}

namespace data::encoding {
    template <endian::order r, Word word = byte>
    using N_bytes = math::number::N_bytes<r, word>;

    template <endian::order r, Word word = byte>
    using Z_bytes = math::number::Z_bytes<r, negativity::twos, word>;

    template <endian::order r, Word word = byte>
    using Z_bytes_BC = math::number::Z_bytes<r, negativity::BC, word>;
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number.

// string encodings.
namespace data::encoding::decimal {

    constexpr bool valid (string_view s);

    template <endian::order r, Word word> maybe<N_bytes<r, word>> read (string_view s);

    struct string;

    string write (const N &);

    std::ostream &write (std::ostream &, const N &);

    template <endian::order r, size_t x, Word word>
    std::ostream &write (std::ostream &o, const math::uint<r, x, word> &);

    template <endian::order r, size_t x, Word word>
    string write (const math::uint<r, x, word> &);

    template <endian::order r, Word word> string write (const N_bytes<r, word> &z);

    template <endian::order r, Word word>
    std::ostream inline &write (std::ostream &o, const N_bytes<r, word> &n) {
        return o << static_cast<std::string> (write (n));
    }

    constexpr bool valid (string_view s);

}

namespace data::encoding::signed_decimal {
    constexpr bool valid (string_view s);

    template <endian::order r, negativity neg, Word word = byte>
    maybe<math::number::Z_bytes<r, neg, word>> read (string_view);

    struct string;

    string write (const Z &);

    std::ostream &write (std::ostream &, const Z &);

    template <endian::order r, size_t x, Word word>
    std::ostream &write (std::ostream &o, const math::sint<r, x, word> &);

    template <endian::order r, size_t x, Word word>
    string write (const math::sint<r, x, word> &);

    template <endian::order r, negativity neg, Word word>
    string write (const math::number::Z_bytes<r, neg, word> &);

    template <endian::order r, negativity neg, Word word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, neg, word> &);

}

namespace data::encoding::hexidecimal {
    constexpr bool valid (string_view s);

    std::ostream &write (std::ostream &, const N &, hex_case = hex_case::lower);
    std::ostream &write (std::ostream &, const Z &, hex_case = hex_case::lower, negativity = negativity::twos);

    template <hex_case zz> struct string;

    template <negativity, hex_case> struct integer;

    template <hex_case zz, endian::order r, Word word>
    integer<negativity::nones, zz> write (const math::number::N_bytes<r, word> &);

    template <hex_case zz, endian::order r, negativity neg, Word word>
    integer<neg, zz> write (const math::number::Z_bytes<r, neg, word> &);

    template <hex_case zz> integer<negativity::nones, zz> write (const N &);
    template <hex_case zz, negativity n = negativity::twos> integer<n, zz> write (const Z &);

}

namespace data::encoding::hexidecimal {

    template <endian::order r, Word word>
    maybe<oriented<r, word>> read (string_view);

    template <endian::order r, Word word>
    std::ostream &write (std::ostream &, const oriented<r, word> &, hex_case q = hex_case::lower);

    template <hex_case zz> struct string;

    template <hex_case zz, endian::order r, Word word>
    string<zz> write (const oriented<r, word> &);

}

namespace data::encoding::natural {

    template <endian::order r, Word word>
    maybe<math::N_bytes<r, word>> read (string_view s);

}

namespace data::encoding::integer {

    template <endian::order r, negativity neg, Word word>
    maybe<math::number::Z_bytes<r, neg, word>> read (string_view s);

    template <endian::order r, negativity neg, Word word>
    std::ostream &write (std::ostream &, const math::number::Z_bytes<r, neg, word> &);

}

namespace data::math::number {

    // read from and write to streams.
    std::ostream &operator << (std::ostream &o, const Z &n);
    std::ostream &operator << (std::ostream &o, const N &n);

    std::istream &operator >> (std::istream &i, Z &z);
    std::istream &operator >> (std::istream &i, N &n);

    template <endian r, Word word>
    std::ostream &operator << (std::ostream &o, const N_bytes<r, word> &n);

    template <endian r, negativity neg, Word word>
    std::ostream &operator << (std::ostream &o, const Z_bytes<r, neg, word> &n);

    template <endian r, Word word>
    std::istream &operator >> (std::istream &o, N_bytes<r, word> &n);

    template <endian r, negativity neg, Word word>
    std::istream &operator >> (std::istream &o, Z_bytes<r, neg, word> &n);

    template <endian r, Word word>
    writer<word> &operator << (writer<word> &o, const N_bytes<r, word> &n);

    template <endian r, negativity neg, Word word>
    writer<word> &operator << (writer<word> &o, const Z_bytes<r, neg, word> &n);

    template <endian r, Word word>
    reader<word> &operator >> (reader<word> &o, N_bytes<r, word> &n);

    template <endian r, negativity neg, Word word>
    reader<word> &operator >> (reader<word> &o, Z_bytes<r, neg, word> &n);

    template <data::endian r, size_t x, Word word>
    std::ostream &operator << (std::ostream &s, const uint<r, x, word> &n);

    template <data::endian r, size_t x, Word word>
    std::ostream &operator << (std::ostream &s, const sint<r, x, word> &n);

    template <bool u, data::endian r, size_t x, Word word>
    std::istream &operator >> (std::istream &i, const bounded<u, r, x, word> &n);

    template <bool u, data::endian r, size_t x, Word word>
    writer<word> &operator << (writer<word> &o, const bounded<u, r, x, word> &n);

    template <bool u, data::endian r, size_t x, Word word>
    reader<word> &operator >> (reader<word> &o, bounded<u, r, x, word> &n);

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
    template <endian r, Word word>
    N_bytes<r, word> &operator ++ (N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator ++ (Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator -- (N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator -- (Z_bytes<r, neg, word> &);

    // post-increment and decrement
    template <endian r, Word word>
    N_bytes<r, word> operator ++ (N_bytes<r, word> &, int);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator ++ (Z_bytes<r, neg, word> &, int);

    template <endian r, Word word>
    N_bytes<r, word> operator -- (N_bytes<r, word> &, int);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator -- (Z_bytes<r, neg, word> &, int);

    template <endian r, negativity neg, Word word>
    struct increment<Z_bytes<r, neg, word>> {
        Z_bytes<r, neg, word> operator () (const Z_bytes<r, neg, word> &);
    };

    template <endian r, negativity neg, Word word>
    struct decrement<Z_bytes<r, neg, word>> {
        Z_bytes<r, neg, word> operator () (const Z_bytes<r, neg, word> &);
    };

    template <endian r, Word word> struct increment<N_bytes<r, word>> {
        nonzero<N_bytes<r, word>> operator () (const N_bytes<r, word> &);
    };

    template <endian r, Word word> struct decrement<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const nonzero<N_bytes<r, word>> &);
        N_bytes<r, word> operator () (const N_bytes<r, word> &);
    };

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator ++ (bounded<u, r, size, word> &, int);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator ++ (bounded<u, r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator -- (bounded<u, r, size, word> &, int);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator -- (bounded<u, r, size, word> &);

    template <endian r, size_t x, Word word>
    struct increment<uint<r, x, word>> {
        constexpr nonzero<uint<r, x, word>> operator () (const uint<r, x, word> &);
    };

    template <endian r, size_t x, Word word>
    struct decrement<uint<r, x, word>> {
        constexpr uint<r, x, word> operator () (const nonzero<uint<r, x, word>> &);
        constexpr uint<r, x, word> operator () (const uint<r, x, word> &);
    };

    template <endian r, size_t x, Word word>
    struct increment<sint<r, x, word>> {
        constexpr sint<r, x, word> operator () (const sint<r, x, word> &);
    };

    template <endian r, size_t x, Word word>
    struct decrement<sint<r, x, word>> {
        constexpr sint<r, x, word> operator () (const sint<r, x, word> &);
    };

    /*************************************************************/
    /*                           Size                            */
    /*************************************************************/

    template <endian r, Word word>
    bool is_minimal (const N_bytes<r, word> &);

    template <endian r, Word word>
    bool is_minimal (const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word>
    bool is_minimal (const Z_bytes<r, negativity::BC, word> &);

    template <endian r, Word word>
    size_t minimal_size (const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    size_t minimal_size (const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> extend (const N_bytes<r, word> &, size_t);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> extend (const Z_bytes<r, neg, word> &, size_t);

    template <endian r, Word word>
    N_bytes<r, word> trim (const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> trim (const Z_bytes<r, neg, word> &);
}

namespace data::hex {
    template <hex_case zz> using uint = encoding::hexidecimal::integer<negativity::nones, zz>;
    template <hex_case zz> using int2 = encoding::hexidecimal::integer<negativity::twos, zz>;
    template <hex_case zz> using intBC = encoding::hexidecimal::integer<negativity::BC, zz>;
    template <negativity neg, hex_case zz> using integer = encoding::hexidecimal::integer<neg, zz>;
}

namespace data {
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;

    template <hex_case zz>
    struct make_signed<hex::intBC<zz>> {
        using type = hex::intBC<zz>;
    };

    // signed versus unsigned.
    template <endian r, size_t x, Word word>
    struct make_unsigned<math::uint<r, x, word>> {
        using type = math::uint<r, x, word>;
    };

    template <endian r, size_t x, Word word>
    struct make_signed<math::uint<r, x, word>> {
        using type = math::sint<r, x, word>;
    };

    template <endian r, size_t x, Word word>
    struct make_unsigned<math::sint<r, x, word>> {
        using type = math::uint<r, x, word>;
    };

    template <endian r, size_t x, Word word>
    struct make_signed<math::sint<r, x, word>> {
        using type = math::sint<r, x, word>;
    };

    template <endian a, Word word>
    struct make_signed<math::Z_bytes_BC<a, word>> {
        using type = math::Z_bytes_BC<a, word>;
    };

    template <endian a, endian b, Word word>
    bool identical (const math::number::N_bytes<a, word> &, const math::number::N_bytes<b, word> &);

    template <endian a, negativity nb, endian c, negativity nd, Word word>
    bool identical (const math::number::Z_bytes<a, nb, word> &, const math::number::Z_bytes<c, nd, word> &);

    template<negativity negl, hex_case b, negativity negr, hex_case d>
    bool identical (const hex::integer<negl, b> &, const hex::integer<negr, d> &);

    template <bool u, endian e, size_t size, Word word>
    struct twice<math::number::bounded<u, e, size, word>> {
        using type = math::number::bounded<u, e, size * 2, word>;
    };

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
    template <endian r, Word word>
    bool operator == (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    bool operator == (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity negl, negativity negr, Word word>
    bool operator == (const Z_bytes<r, negl, word> &, const Z_bytes<r, negr, word> &);

    template <endian r, Word word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    std::weak_ordering operator <=> (const Z_bytes<r, negativity::twos, word> &, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word>
    std::weak_ordering operator <=> (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian r, negativity cl, negativity cr, Word word>
    std::weak_ordering operator <=> (const Z_bytes<r, cl, word> &, const Z_bytes<r, cl, word> &);

    // bounded
    template <bool x, endian r, size_t n, bool y, endian o, size_t z, Word word>
    constexpr bool operator == (const bounded<x, r, n, word> &, const bounded<y, o, z, word> &);

    template <endian r, size_t size, Word word>
    constexpr std::strong_ordering operator <=> (const sint<r, size, word> &, const sint<r, size, word> &);

    template <endian r, size_t size, Word word>
    constexpr std::strong_ordering operator <=> (const uint<r, size, word> &, const uint<r, size, word> &);

    template <bool x, endian r, size_t n, bool y, endian o, size_t z, Word word>
    constexpr std::strong_ordering operator <=> (const bounded<x, r, n, word> &, const bounded<y, o, z, word> &);

    // comparisons of N/Z with N_bytes/Z_bytes and bounded types.
    template <endian r, size_t size, Word word>
    bool operator == (const Z &, const sint<r, size, word> &);

    template <endian r, size_t size, Word word>
    std::strong_ordering operator <=> (const Z &, const sint<r, size, word> &);

    template <endian r, size_t size, Word word>
    bool operator == (const N &, const uint<r, size, word> &);

    template <endian r, size_t size, Word word>
    std::strong_ordering operator <=> (const N &, const uint<r, size, word> &);

    template <endian r, negativity neg, Word word>
    bool operator == (const Z &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    std::strong_ordering operator <=> (const Z &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    bool operator == (const N &, const N_bytes<r, word> &);

    template <endian r, Word word>
    std::strong_ordering operator <=> (const N &, const N_bytes<r, word> &);

    // Equality of _bytes types with bounded
    template <endian r, size_t size, endian o, Word word>
    bool operator == (const sint<r, size, word> &, const Z_bytes<o, negativity::twos, word> &);

    template <endian r, size_t size, endian o, Word word>
    std::weak_ordering operator <=> (const sint<r, size, word> &, const Z_bytes<o, negativity::twos, word> &);

    template <endian r, size_t size, endian o, Word word>
    bool operator == (const uint<r, size, word> &, const N_bytes<o, word> &);

    template <endian r, size_t size, endian o, Word word>
    std::weak_ordering operator <=> (const uint<r, size, word> &, const N_bytes<o, word> &);

    template <endian r, size_t size, Word word,
    endian o, negativity neg, Word w>
    std::weak_ordering operator <=> (const uint<r, size, word> &, const Z_bytes<o, neg, w> &);

    // comparisons with endian integral types.
    template <bool x, endian r, size_t n, bool y, endian o, size_t z, Word word>
    constexpr bool operator == (const bounded<x, r, n, word> &, const endian_integral<y, o, z> &);

    template <bool x, endian r, size_t n, bool y, endian o, size_t z, Word word>
    constexpr std::strong_ordering operator <=> (const bounded<x, r, n, word> &, const endian_integral<y, o, z> &);

    // comparisons with built-in types.
    template <std::integral I> bool operator == (const N &, I);

    template <std::integral I> bool operator == (const Z &, I);

    template <std::integral I> std::strong_ordering operator <=> (const N &, I);

    template <std::integral I> std::strong_ordering operator <=> (const Z &, I);

    // TODO: these ought to use a std::integral type parameter.
    template <endian r, Word word>
    bool operator == (const N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    bool operator == (const Z_bytes<r, neg, word> &, int64);

    template <endian r, Word word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    std::weak_ordering operator <=> (const Z_bytes<r, neg, word> &, int64);

    template <bool u, endian r, size_t size, Word word, std::integral I>
    constexpr bool operator == (const bounded<u, r, size, word> &, I);

    template <bool u, endian r, size_t size, Word word, std::integral I>
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

    template <std::unsigned_integral I> N operator ^ (const N &, I);

    template <std::unsigned_integral I> N operator & (const N &, I);

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

    template <std::integral I> Z &operator &= (Z &, I);
    template <std::integral I> Z &operator |= (Z &, I);
    template <std::integral I> Z &operator ^= (Z &, I);

    Z &operator <<= (Z &, int);
    Z &operator >>= (Z &, int);

    N &operator <<= (N &, int);
    N &operator >>= (N &, int);

    // bit operations for bounded types.
    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator ~ (const bounded<u, r, x, word> &);

    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator ^ (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator & (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator | (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator << (const bounded<u, r, size, word> &, int);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator >> (const bounded<u, r, size, word> &, int);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator &= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator |= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator ^= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <endian r, size_t size, Word word>
    constexpr uint<r, size, word> &operator &= (uint<r, size, word> &, const sint<r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr uint<r, size, word> &operator |= (uint<r, size, word> &, const sint<r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr uint<r, size, word> &operator ^= (uint<r, size, word> &, const sint<r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator <<= (bounded<u, r, size, word> &, int);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator >>= (bounded<u, r, size, word> &, int);

    // bit negate
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator ~ (const N_bytes<r, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator ~ (const Z_bytes<r, negativity::twos, word> &);

    // basic bit operations.
    template <endian r, Word word>
    N_bytes<r, word> operator & (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator & (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator &= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> operator | (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator | (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator |= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> operator ^ (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator ^ (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator ^= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    // bit xor
    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> operator ^ (const N_bytes<r, word> &, I x);

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> operator ^ (I x, const N_bytes<r, word> &);

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &, I x);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator ^= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator ^ (const Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator ^ (const N_bytes<r, word> &, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> &operator ^=
    (Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> operator ^ (const Z_bytes<r, negativity::twos, word> &, I x);

    template <std::integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator ^ (I x, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> &operator ^= (Z_bytes<r, negativity::twos, word> &, I x);

    // bit and
    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> operator & (const N_bytes<r, word> &, I x);

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> operator & (I x, const N_bytes<r, word> &);

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator & (const N_bytes<r, word> &, I x);

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator & (I x, const N_bytes<r, word> &);

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &, I x);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator & (const Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator & (const N_bytes<r, word> &, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> &operator &=
    (Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> operator & (const Z_bytes<r, negativity::twos, word> &, I x);

    template <std::integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator & (I x, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> &operator &= (Z_bytes<r, negativity::twos, word> &, I x);

    // bit or
    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> operator | (const N_bytes<r, word> &, I x);

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> operator | (I x, const N_bytes<r, word> &);

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &, I x);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator | (const Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator | (const N_bytes<r, word> &, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> &operator |=
    (Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> operator | (const Z_bytes<r, negativity::twos, word> &, I x);

    template <std::integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator | (I x, const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> &operator |= (Z_bytes<r, negativity::twos, word> &, I x);

    // bit shift
    template <endian r, Word word>
    N_bytes<r, word> operator << (const N_bytes<r, word> &, int);

    template <endian r, Word word>
    N_bytes<r, word> operator >> (const N_bytes<r, word> &, int);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator << (const Z_bytes<r, neg, word> &, int);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator >> (const Z_bytes<r, neg, word> &, int);

    template <endian r, Word word>
    N_bytes<r, word> &operator <<= (N_bytes<r, word> &, int64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator <<= (Z_bytes<r, neg, word> &, int64);

    template <endian r, Word word>
    N_bytes<r, word> &operator >>= (N_bytes<r, word> &, int64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator >>= (Z_bytes<r, neg, word> &, int64);

    // logical (only for Bitcoin style numbers)
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> operator ! (const Z_bytes<r, negativity::BC, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> operator && (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> operator || (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    // bit arithmetic with built-in types.
    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> operator & (const uint<r, size, word> &, I);

    template <std::integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator & (I, const uint<r, size, word> &);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> operator & (const sint<r, size, word> &, I);

    template <std::signed_integral I, endian r, size_t size, Word word>
    constexpr sint<r, size, word> operator & (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::unsigned_integral I>
    constexpr uint<r, size, word> operator & (const sint<r, size, word> &, I);

    template <std::unsigned_integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator & (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> &operator &= (uint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> &operator &= (sint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> operator ^ (const uint<r, size, word> &, I);

    template <std::integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator ^ (I, const uint<r, size, word> &);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> operator ^ (const sint<r, size, word> &, I);

    template <std::signed_integral I, endian r, size_t size, Word word>
    constexpr sint<r, size, word> operator ^ (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::unsigned_integral I>
    constexpr uint<r, size, word> operator ^ (const sint<r, size, word> &, I);

    template <std::unsigned_integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator ^ (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> &operator ^= (uint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> &operator ^= (sint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> operator | (const uint<r, size, word> &, I);

    template <std::integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator | (I, const uint<r, size, word> &);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> operator | (const sint<r, size, word> &, I);

    template <std::signed_integral I, endian r, size_t size, Word word>
    constexpr sint<r, size, word> operator | (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::unsigned_integral I>
    constexpr uint<r, size, word> operator | (const sint<r, size, word> &, I);

    template <std::unsigned_integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator | (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> &operator |= (uint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> &operator |= (sint<r, size, word> &, I);

    /*************************************************************/
    /*                        Arithmetic                         */
    /*************************************************************/

    // negation
    Z operator - (const N &);
    Z operator - (const Z &);

    // negation
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator - (const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator - (const Z_bytes<r, neg, word> &);

    // negate
    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator - (const bounded<u, r, x, word> &);

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

    template <std::integral I> Z operator / (const Z &, I);
    template <std::unsigned_integral I> N operator / (const N &, I);
    template <std::signed_integral I> Z operator / (const N &, I);

    N operator % (const Z &, const Z &);
    N operator % (const Z &, const N &);
    N operator % (const N &, const N &);

    template <std::integral I> I operator % (const Z &, I);
    template <std::integral I> I operator % (const N &, I);

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

    template <std::integral I> Z &operator += (Z &, I);
    template <std::integral I> Z &operator -= (Z &, I);
    template <std::integral I> Z &operator *= (Z &, I);
    template <std::integral I> Z &operator /= (Z &, I);

    // arithmetic
    template <endian r, Word word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator + (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator - (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator * (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator + (const Z_bytes<r, neg, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator - (const Z_bytes<r, neg, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator + (const N_bytes<r, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator - (const N_bytes<r, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator * (const Z_bytes<r, neg, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator * (const N_bytes<r, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, I);

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, I);

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, I);

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> operator + (I, const N_bytes<r, word> &);

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> operator - (I, const N_bytes<r, word> &);

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> operator * (I, const N_bytes<r, word> &);

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator + (const N_bytes<r, word> &, I);

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator - (const N_bytes<r, word> &, I);

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator * (const N_bytes<r, word> &, I);

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator + (I, const N_bytes<r, word> &);

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator - (I, const N_bytes<r, word> &);

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator * (I, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word, std::integral I>
    Z_bytes<r, neg, word> operator + (const Z_bytes<r, neg, word> &, I);

    template <endian r, negativity neg, Word word, std::integral I>
    Z_bytes<r, neg, word> operator - (const Z_bytes<r, neg, word> &, I);

    template <endian r, negativity neg, Word word, std::integral I>
    Z_bytes<r, neg, word> operator * (const Z_bytes<r, neg, word> &, I);

    template <std::integral I, endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator + (I, const Z_bytes<r, neg, word> &);

    template <std::integral I, endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator - (I, const Z_bytes<r, neg, word> &);

    template <std::integral I, endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator * (I, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator += (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    math::Z_bytes<r, word> &operator += (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator -= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    math::Z_bytes<r, word> &operator -= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, uint64);

    template <endian r, Word word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator += (Z_bytes<r, neg, word> &, int64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator -= (Z_bytes<r, neg, word> &, int64);

    template <endian r, Word word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator *= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    math::Z_bytes<r, word> &operator *= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator *= (Z_bytes<r, neg, word> &, int64);

    // plus
    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator + (const bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator += (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    // minus
    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator - (const bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator -= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    // times
    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> operator * (const bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    template <bool u, endian r, size_t size, Word word>
    constexpr bounded<u, r, size, word> &operator *= (bounded<u, r, size, word> &, const bounded<u, r, size, word> &);

    // basic arithmetic with automatic conversions to unsigned.
    template <endian r, size_t size, Word word>
    constexpr uint<r, size, word> &operator += (uint<r, size, word> &, const sint<r, size, word> &);

    template <endian r, size_t size, Word word>
    constexpr uint<r, size, word> &operator -= (uint<r, size, word> &, const sint<r, size, word> &);

    template <endian r, size_t size, Word word>
    constexpr uint<r, size, word> &operator *= (uint<r, size, word> &, const sint<r, size, word> &);

    // basic arithmetic with built-in types.
    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> operator + (const uint<r, size, word> &, I);

    template <std::integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator + (I, const uint<r, size, word> &);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> operator + (const sint<r, size, word> &, I);

    template <std::signed_integral I, endian r, size_t size, Word word>
    constexpr sint<r, size, word> operator + (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::unsigned_integral I>
    constexpr uint<r, size, word> operator + (const sint<r, size, word> &, I);

    template <std::unsigned_integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator + (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> &operator += (uint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> &operator += (sint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> operator - (const uint<r, size, word> &, I);

    template <std::integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator - (I, const uint<r, size, word> &);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> operator - (const sint<r, size, word> &, I);

    template <std::signed_integral I, endian r, size_t size, Word word>
    constexpr sint<r, size, word> operator - (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::unsigned_integral I>
    constexpr uint<r, size, word> operator - (const sint<r, size, word> &, I);

    template <std::unsigned_integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator - (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> &operator -= (uint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> &operator -= (sint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> operator * (const uint<r, size, word> &, I);

    template <std::integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator * (I, const uint<r, size, word> &);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> operator * (const sint<r, size, word> &, I);

    template <std::signed_integral I, endian r, size_t size, Word word>
    constexpr sint<r, size, word> operator * (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::unsigned_integral I>
    constexpr uint<r, size, word> operator * (const sint<r, size, word> &, I);

    template <std::unsigned_integral I, endian r, size_t size, Word word>
    constexpr uint<r, size, word> operator * (I, const sint<r, size, word> &);

    template <endian r, size_t size, Word word, std::integral I>
    constexpr uint<r, size, word> &operator *= (uint<r, size, word> &, I);

    template <endian r, size_t size, Word word, std::signed_integral I>
    constexpr sint<r, size, word> &operator *= (sint<r, size, word> &, I);

    // mod and divmod
    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator / (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    template <bool u, endian r, size_t x, Word word>
    constexpr bounded<u, r, x, word> operator % (const bounded<u, r, x, word> &, const bounded<u, r, x, word> &);

    // division
    template <endian r, Word word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator / (const Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator / (const N_bytes<r, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator / (const Z_bytes<r, neg, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator / (const Z_bytes<r, neg, word> &, int64);

    template <endian r, Word word>
    N_bytes<r, word> operator % (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    N_bytes<r, word> operator % (const Z_bytes<r, neg, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    N_bytes<r, word> operator % (
        const Z_bytes<r, negativity::twos, word> &,
        const Z_bytes<r, negativity::twos, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> operator % (
        const Z_bytes<r, negativity::BC, word> &,
        const Z_bytes<r, negativity::BC, word> &);

    // TODO what's with all these uint64s?
    template <endian r, Word word>
    uint64 operator % (const N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    uint64 operator % (const Z_bytes<r, neg, word> &, uint64);

    template <endian r, Word word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator /= (Z_bytes<r, neg, word> &, const Z_bytes<r, neg, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, uint64);

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator /= (Z_bytes<r, neg, word> &, int64);

    template <endian r, Word word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity neg, Word word>
    N_bytes<r, word> &operator %= (Z_bytes<r, neg, word> &, const N_bytes<r, word> &);

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> &operator %=
    (Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian r, Word word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, uint64);
}

namespace data::math::def {

    template <> struct abs<Z> {
        N operator () (const Z &);
    };

    template <> struct abs<N> {
        N operator () (const N &n);
    };

    template <endian r, Word word>
    struct min<number::N_bytes<r, word>> {
        const number::N_bytes<r, word> &operator () (const number::N_bytes<r, word> &, const number::N_bytes<r, word> &) const;
    };

    template <endian r, Word word>
    struct max<number::N_bytes<r, word>> {
        const number::N_bytes<r, word> &operator () (const number::N_bytes<r, word> &, const number::N_bytes<r, word> &) const;
    };

    template <endian r, negativity neg, Word word>
    struct min<number::Z_bytes<r, neg, word>> {
        const number::Z_bytes<r, neg, word> &operator () (const number::Z_bytes<r, neg, word> &, const number::Z_bytes<r, neg, word> &) const;
    };

    template <endian r, negativity neg, Word word>
    struct max<number::Z_bytes<r, neg, word>> {
        const number::Z_bytes<r, neg, word> &operator () (const number::Z_bytes<r, neg, word> &, const number::Z_bytes<r, neg, word> &) const;
    };

    template <negativity neg, hex_case zz>
    struct min<hex::integer<neg, zz>> {
        const hex::integer<neg, zz> &operator () (const hex::integer<neg, zz> &, const hex::integer<neg, zz> &) const;
    };

    template <negativity neg, hex_case zz>
    struct max<hex::integer<neg, zz>> {
        const hex::integer<neg, zz> &operator () (const hex::integer<neg, zz> &, const hex::integer<neg, zz> &) const;
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

    template <> struct identity<times<Z>, Z> {
        Z operator () ();
    };

    template <> struct bit_xor<N> {
        N operator () (const N &a, const N &b);
    };

    template <> struct bit_xor<Z> {
        Z operator () (const Z &, const Z &);
    };

    template <> struct div_2_pow<N> {
        N operator () (const N &a, uint32);
    };

    template <> struct div_2_pow<Z> {
        Z operator () (const Z &a, uint32);
    };

    template <endian r, Word word>
    struct div_2_pow<math::N_bytes<r, word>> {
        math::N_bytes<r, word> operator () (const math::N_bytes<r, word> &x, uint32 exp);
    };

    template <endian r, negativity neg, Word word>
    struct div_2_pow<math::number::Z_bytes<r, neg, word>> {
        math::number::Z_bytes<r, neg, word> operator () (const math::number::Z_bytes<r, neg, word> &x, uint32 exp);
    };

    template <> struct mod_2<N> {
        N operator () (const N &a);
    };

    template <> struct mod_2<Z> {
        N operator () (const Z &a);
    };

    template <GroupNumber Exp>
    struct pow<N, Exp> {
        N operator () (const N &x, const Exp &y);
    };

    template <GroupNumber Exp>
    struct pow<Z, Exp> {
        Z operator () (const Z &x, const Exp &y);
    };

    template <GroupNumber Exp> struct pow_mod<N, Exp, N> {
        N operator () (const N &x, const Exp &y, const nonzero<N> &z);
    };

    template <GroupNumber Exp> struct pow_mod<Z, Exp, N> {
        N operator () (const Z &x, const Exp &y, const nonzero<N> &z);
    };

    // conversions with built-in types.
    // in general, conversions throw if one number is outside the range of another.
    template <std::integral I> struct convert<N, I> {
        N operator () (I) const;
    };

    template <std::integral I> struct convert<Z, I> {
        Z operator () (I) const;
    };

    template <std::integral I> struct convert<I, N> {
        I operator () (const N &) const;
    };

    template <std::integral I> struct convert<I, Z> {
        I operator () (const Z &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word, std::integral I>
    struct convert<number::bounded<is_signed, r, size, word>, I> {
        constexpr number::bounded<is_signed, r, size, word> operator () (const I &) const;
    };

    template <std::integral I, bool is_signed, endian r, size_t size, Word word>
    struct convert<I, number::bounded<is_signed, r, size, word>> {
        constexpr I operator () (const number::bounded<is_signed, r, size, word> &) const;
    };

    template <data::endian Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct convert<N, boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        N operator () (const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &) const;
    };

    template <data::endian Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct convert<Z, boost::endian::endian_arithmetic<Order, T, n_bits, Align>> {
        Z operator () (const boost::endian::endian_arithmetic<Order, T, n_bits, Align> &) const;
    };

    template <data::endian Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct convert<boost::endian::endian_arithmetic<Order, T, n_bits, Align>, N> {
        boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (const N &) const;
    };

    template <data::endian Order, class T, std::size_t n_bits, boost::endian::align Align>
    struct convert<boost::endian::endian_arithmetic<Order, T, n_bits, Align>, Z> {
        boost::endian::endian_arithmetic<Order, T, n_bits, Align> operator () (const Z &) const;
    };

    template <endian r, negativity neg, Word word>
    struct convert<number::Z_bytes<r, neg, word>, Z> {
        number::Z_bytes<r, neg, word> operator () (const Z &) const;
    };

    template <endian r, negativity neg, Word word>
    struct convert<Z, number::Z_bytes<r, neg, word>> {
        Z operator () (const number::Z_bytes<r, neg, word> &) const;
    };

    template <endian r, Word word>
    struct convert<number::N_bytes<r, word>, N> {
        number::N_bytes<r, word> operator () (const N &) const;
    };

    template <endian r, Word word>
    struct convert<N, number::N_bytes<r, word>> {
        N operator () (const number::N_bytes<r, word> &) const;
    };

    template <endian r, Word word>
    struct convert<number::N_bytes<r, word>, Z> {
        number::N_bytes<r, word> operator () (const Z &) const;
    };

    template <endian r, Word word>
    struct convert<Z, number::N_bytes<r, word>> {
        Z operator () (const number::N_bytes<r, word> &) const;
    };

    template <endian r, Word word, std::integral I>
    struct convert<number::N_bytes<r, word>, I> {
        number::N_bytes<r, word> operator () (const I &) const;
    };

    template <std::integral I, endian r, Word word>
    struct convert<I, number::N_bytes<r, word>> {
        I operator () (const number::N_bytes<r, word> &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word>
    struct convert<number::bounded<is_signed, r, size, word>, Z> {
        number::bounded<is_signed, r, size, word> operator () (const Z &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word>
    struct convert<Z, number::bounded<is_signed, r, size, word>> {
        Z operator () (const number::bounded<is_signed, r, size, word> &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word>
    struct convert<number::bounded<is_signed, r, size, word>, N> {
        number::bounded<is_signed, r, size, word> operator () (const N &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word>
    struct convert<N, number::bounded<is_signed, r, size, word>> {
        N operator () (const number::bounded<is_signed, r, size, word> &) const;
    };

    template <> struct convert<N, dec_uint> {
        N operator () (const dec_uint &) const;
    };

    template <hex_case zz> struct convert<N, hex::uint<zz>> {
        N operator () (const hex::uint<zz> &) const;
    };

    template <> struct convert<Z, dec_int> {
        Z operator () (const dec_int &) const;
    };

    template <hex_case zz> struct convert<Z, hex::int2<zz>> {
        Z operator () (const hex::int2<zz> &) const;
    };

    template <hex_case zz> struct convert<Z, hex::intBC<zz>> {
        Z operator () (const hex::intBC<zz> &) const;
    };

    template <> struct convert<dec_uint, dec_int> {
        dec_uint operator () (const dec_int &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word, endian o, std::integral T, std::size_t n_bits>
    struct convert<boost::endian::endian_arithmetic<o, T, n_bits, boost::endian::align::no>, number::bounded<is_signed, r, size, word>> {
        constexpr boost::endian::endian_arithmetic<o, T, n_bits, boost::endian::align::no>
        operator () (const number::bounded<is_signed, r, size, word> &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word, endian o, std::integral T, std::size_t n_bits>
    struct convert<number::bounded<is_signed, r, size, word>, boost::endian::endian_arithmetic<o, T, n_bits, boost::endian::align::no>> {
        constexpr number::bounded<is_signed, r, size, word> operator () (const boost::endian::endian_arithmetic<o, T, n_bits, boost::endian::align::no> &) const;
    };

    // convert between any two types of N_bytes
    template <endian ToEndian, Word ToWord, endian FromEndian, Word FromWord>
    requires Unsame<number::N_bytes<ToEndian, ToWord>, number::N_bytes<FromEndian, FromWord>>
    struct convert<number::N_bytes<ToEndian, ToWord>, number::N_bytes<FromEndian, FromWord>> {
        number::N_bytes<ToEndian, ToWord> operator () (const number::N_bytes<FromEndian, FromWord> &from) const;
    };

    // convert between any two types of Z_bytes
    template <endian ToEndian, negativity ToNeg, Word ToWord, endian FromEndian, negativity FromNeg, Word FromWord>
    requires Unsame<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>>
    struct convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>> {
        number::Z_bytes<ToEndian, ToNeg, ToWord> operator () (const number::Z_bytes<FromEndian, FromNeg, FromWord> &from) const;
    };

    // convert from N_bytes to Z_bytes
    template <endian ToEndian, negativity ToNeg, Word ToWord, endian FromEndian, Word FromWord>
    struct convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::N_bytes<FromEndian, FromWord>> {
        number::Z_bytes<ToEndian, ToNeg, ToWord> operator () (const number::N_bytes<FromEndian, FromWord> &from) const;
    };

    template <endian ToEndian, negativity FromNeg, Word ToWord, endian FromEndian, Word FromWord>
    struct convert<number::N_bytes<ToEndian, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>> {
        number::N_bytes<ToEndian, ToWord> operator () (const number::Z_bytes<FromEndian, FromNeg, FromWord> &from) const;
    };

    template <bool is_signed, endian to_r, size_t to_size, Word to_word, endian from_r, negativity neg, Word from_word>
    struct convert<number::bounded<is_signed, to_r, to_size, to_word>, number::Z_bytes<from_r, neg, from_word>> {
        number::bounded<is_signed, to_r, to_size, to_word> operator () (const number::Z_bytes<from_r, neg, from_word> &) const;
    };

    template <bool is_signed, endian to_r, size_t to_size, Word to_word, endian from_r, Word from_word>
    struct convert<number::bounded<is_signed, to_r, to_size, to_word>, number::N_bytes<from_r, from_word>> {
        number::bounded<is_signed, to_r, to_size, to_word> operator () (const number::N_bytes<from_r, from_word> &) const;
    };

    template <endian to_r, negativity neg, Word to_word, bool is_signed, endian from_r, size_t size, Word from_word>
    struct convert<number::Z_bytes<to_r, neg, to_word>, number::bounded<is_signed, from_r, size, from_word>> {
        number::Z_bytes<to_r, neg, to_word> operator () (const number::bounded<is_signed, from_r, size, from_word> &) const;
    };

    template <endian to_r, Word to_word, bool is_signed, endian from_r, size_t size, Word from_word>
    struct convert<number::N_bytes<to_r, to_word>, number::bounded<is_signed, from_r, size, from_word>> {
        number::N_bytes<to_r, to_word> operator () (const number::bounded<is_signed, from_r, size, from_word> &) const;
    };

    template <bool a, endian r, size_t x, Word w, bool b, endian o, size_t y, Word u>
    struct convert<number::bounded<a, r, x, w>, number::bounded<b, o, y, u>> {
        number::bounded<a, r, x, w> operator () (const number::bounded<b, o, y, u> &) const;
    };

    template <bool is_signed, endian r, size_t size, Word word>
    struct size_in_base<number::bounded<is_signed, r, size, word>, 2> {
        constexpr size_t operator () (const number::bounded<is_signed, r, size, word> &);
    };

    template <endian r, Word word>
    struct size_in_base<number::N_bytes<r, word>, 2> {
        size_t operator () (const number::N_bytes<r, word> &);
    };

    template <endian r, negativity neg, Word word>
    struct size_in_base<number::Z_bytes<r, neg, word>, 2> {
        size_t operator () (const number::Z_bytes<r, neg, word> &);
    };

    template <> struct size_in_base<N, 2> {
        size_t operator () (const N &);
    };

    template <> struct size_in_base<Z, 2> {
        size_t operator () (const Z &);
    };

    template <> struct size_in_base<dec_uint, 2> {
        size_t operator () (const dec_uint &);
    };

    template <> struct size_in_base<dec_int, 2> {
        size_t operator () (const dec_int &);
    };

    template<negativity neg, hex_case zz>
    struct size_in_base<hex::integer<neg, zz>, 2> {
        size_t operator () (const hex::integer<neg, zz> &);
    };

    template <endian r, negativity neg, Word word = byte>
    using Zb = number::Z_bytes<r, neg, word>;

    template <endian r, Word word>
    struct is_zero<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct is_zero<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct is_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, Word word>
    struct is_negative<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct is_negative<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &x);
    };

    template <endian r, Word word>
    struct is_negative<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &x);
    };

    template <endian r, Word word>
    struct is_positive<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &x);
    };

    template <endian r, Word word>
    struct is_positive<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct is_positive<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, Word word>
    struct is_positive_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, Word word>
    struct is_negative_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, Word word>
    struct abs<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const N_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct abs<Z_bytes<r, word>> {
        N_bytes<r, word> operator () (const Z_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct abs<Z_bytes_BC<r, word>> {
        Z_bytes_BC<r, word> operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, negativity neg, Word word>
    struct times<Zb<r, neg, word>, Zb<r, neg, word>> {
        Zb<r, neg, word> operator ()
        (const Zb<r, neg, word> &a, const Zb<r, neg, word> &b);

        nonzero<Zb<r, neg, word>> operator ()
        (const nonzero<Zb<r, neg, word>> &a, const nonzero<Zb<r, neg, word>> &b);
    };

    template <endian r, negativity neg, Word word>
    struct inverse<plus<Zb<r, neg, word>>, Zb<r, neg, word>> {
        Zb<r, neg, word> operator () (const Zb<r, neg, word> &a, const Zb<r, neg, word> &b) {
            return b - a;
        }
    };

    template <endian r, Word word>
    struct divmod<N_bytes<r, word>, N_bytes<r, word>> {
        division<N_bytes<r, word>, N_bytes<r, word>> operator ()
        (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian r, Word word>
    struct divmod<Z_bytes<r, word>, N_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
        (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian r, Word word>
    struct divmod<Z_bytes<r, word>, Z_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
        (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b);
    };

    template <endian r, Word word>
    struct divmod<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> {
        division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> operator ()
        (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b);
    };

    template <endian r, Word word>
    struct sign<N_bytes<r, word>> {
        math::sign operator () (const N_bytes<r, word> &);
    };

    template <endian r, negativity neg, Word word>
    struct sign<Zb<r, neg, word>> {
        math::sign operator () (const Zb<r, neg, word> &);
    };

    template <endian r, Word word>
    struct bit_xor<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const N_bytes<r, word> &a, const N_bytes<r, word> &b);
    };

    template <endian r, Word word>
    struct bit_not<N_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const N_bytes<r, word> &n);
    };

    template <endian r, Word word>
    struct bit_xor<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &a, const math::Z_bytes<r, word> &b);
    };

    template <endian r, Word word>
    struct bit_xor<N_bytes<r, word>, math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const N_bytes<r, word> &, const math::Z_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct bit_xor<math::Z_bytes<r, word>, N_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &, const N_bytes<r, word> &);
    };

    template <endian r, Word word>
    struct bit_xor<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &, const math::Z_bytes_BC<r, word> &);
    };

    template <endian r, Word word>
    struct bit_not<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &);
    };

    // N_bytes and Z_bytes (two's) will use the default mul_2_pow but
    // Z_bytes_BC will not because it does not satisfy bit_number.
    template <endian r, Word word>
    struct mul_2_pow<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &, uint32 u);
    };

    template <endian r, Word word>
    struct mod_2<math::N_bytes<r, word>> {
        math::N_bytes<r, word> operator () (const math::N_bytes<r, word> &x);
    };

    template <endian r, Word word>
    struct mod_2<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x);
    };

    template <endian r, Word word>
    struct mod_2<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &x);
    };

    template <endian r, Word word>
    struct bit_or<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b);
    };

    template <endian r, Word word>
    struct bit_and<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b);
    };

    template <bool is_signed, endian r, size_t size, Word word>
    struct digit<number::bounded<is_signed, r, size, word>> {
        constexpr size_t operator () (const number::bounded<is_signed, r, size, word> &, int32 i);
    };

    template <endian r, Word word>
    struct digit<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &, int32);
    };

    template <endian r, negativity neg, Word word>
    struct digit<number::Z_bytes<r, neg, word>> {
        bool operator () (const number::Z_bytes<r, neg, word> &, int32 i);
    };

    template <negativity neg, hex_case zz>
    struct digit<hex::integer<neg, zz>> {
        bool operator () (const hex::integer<neg, zz> &, int32 i) const;
    };

    template <> struct digit<dec_uint> {
        size_t operator () (const dec_uint &, int32 i);
    };

    template <> struct digit<dec_int> {
        size_t operator () (const dec_int &, int32 i);
    };

}
