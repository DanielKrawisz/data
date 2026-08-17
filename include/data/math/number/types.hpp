// Copyright (c) 2019-2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_TYPES
#define DATA_MATH_NUMBER_TYPES

#include <data/encoding/endian.hpp>
#include <data/arithmetic/negativity.hpp>
#include <data/arithmetic.hpp>

// Declare number types.

namespace data::math::number {

    // bounded numbers that resemble
    // satisfies range<word> and integral.
    template <bool u, endian::order, size_t size, std::unsigned_integral word> struct bounded;

    // an implementation of the natural numbers that is
    // encoded as a big or little endian sequence of bytes.
    template <endian::order, std::unsigned_integral> struct N_bytes;

    // similar implementation of the integers. We have two's complement
    // and the sign-and-magnetude system used in Bitcoin. N_bytes works
    // as the absolute value of the two's complement types. Bitcoin numbers
    // work as their own number system without a type for the naturals.
    template <endian::order, neg, std::unsigned_integral> struct Z_bytes;

    struct Z;
    struct N;

    bool operator == (const Z &, const Z &);
    std::strong_ordering operator <=> (const Z &, const Z &);

    bool operator == (const N &, const N &);
    std::strong_ordering operator <=> (const N &, const N &);

    template <std::integral I> bool operator == (const N &, I);
    template <std::integral I> bool operator == (I, const N &);

    template <std::integral I> bool operator == (const Z &, I);
    template <std::integral I> bool operator == (I, const Z &);

    template <std::signed_integral I> std::strong_ordering operator <=> (const N &, I);
    template <std::signed_integral I> std::strong_ordering operator <=> (I, const N &);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (const N &, I);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (I, const N &);

    template <std::signed_integral I> std::strong_ordering operator <=> (const Z &, I);
    template <std::signed_integral I> std::strong_ordering operator <=> (I, const Z &);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (const Z &, I);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (I, const Z &);

    Z operator ~ (const N &);
    Z operator ~ (const Z &);

    Z operator - (const N &);
    Z operator - (const Z &);

    Z operator + (const Z &, const Z &);
    Z operator - (const Z &, const Z &);
    Z operator * (const Z &, const Z &);

    N operator + (const N &, const N &);
    N operator - (const N &, const N &);
    N operator * (const N &, const N &);

    Z operator | (const Z &, const Z &);
    Z operator & (const Z &, const Z &);
    Z operator ^ (const Z &, const Z &);

    N operator | (const N &, const N &);
    N operator & (const N &, const N &);
    N operator ^ (const N &, const N &);

    template <std::integral I> Z operator + (I, const Z &);
    template <std::integral I> Z operator + (const Z &, I);

    template <std::signed_integral I> Z operator - (I, const Z &);
    template <std::signed_integral I> Z operator - (const Z &, I);

    template <std::unsigned_integral I> Z operator - (I, const Z &);
    template <std::unsigned_integral I> Z operator - (const Z &, I);

    template <std::signed_integral I> Z operator * (I, const Z &);
    template <std::signed_integral I> Z operator * (const Z &, I);

    template <std::unsigned_integral I> Z operator * (I, const Z &);
    template <std::unsigned_integral I> Z operator * (const Z &, I);

    template <std::signed_integral I> Z operator + (I, const N &);
    template <std::signed_integral I> Z operator + (const N &, I);

    template <std::signed_integral I> Z operator - (I, const N &);
    template <std::signed_integral I> Z operator - (const N &, I);

    template <std::signed_integral I> Z operator * (I, const N &);
    template <std::signed_integral I> Z operator * (const N &, I);

    template <std::unsigned_integral I> N operator + (I, const N &);
    template <std::unsigned_integral I> N operator + (const N &, I);

    template <std::unsigned_integral I> N operator - (I, const N &);
    template <std::unsigned_integral I> N operator - (const N &, I);

    template <std::unsigned_integral I> N operator * (I, const N &);
    template <std::unsigned_integral I> N operator * (const N &, I);

    template <std::unsigned_integral I> N operator & (I, const N &);
    template <std::unsigned_integral I> N operator & (const N &, I);

    template <std::unsigned_integral I> N operator ^ (I, const N &);
    template <std::unsigned_integral I> N operator ^ (const N &, I);

    template <std::unsigned_integral I> N operator | (I, const N &);
    template <std::unsigned_integral I> N operator | (const N &, I);

    Z operator / (const Z &, const Z &);
    N operator / (const N &, const N &);

    Z operator / (const Z &, int64);
    N operator / (const N &, uint64);

    N operator % (const Z &, const Z &);
    N operator % (const Z &, const N &);
    N operator % (const N &, const N &);

    uint64 operator % (const Z &, uint64);
    uint64 operator % (const N &, uint64);

    Z &operator ++ (Z &);
    Z &operator -- (Z &);

    Z operator ++ (Z &, int);
    Z operator -- (Z &, int);

    N &operator ++ (N &);
    N &operator -- (N &);

    N operator ++ (N &, int);
    N operator -- (N &, int);

    Z operator << (const Z &, int);
    Z operator >> (const Z &, int);

    N operator << (const N &, int);
    N operator >> (const N &, int);

    std::ostream &operator << (std::ostream &o, const Z &n);
    std::ostream &operator << (std::ostream &o, const N &n);

    std::istream &operator >> (std::istream &i, Z &z);
    std::istream &operator >> (std::istream &i, N &n);

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

}

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

namespace data::encoding {
    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = math::number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = math::number::Z_bytes<r, neg::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = math::number::Z_bytes<r, neg::BC, word>;
}

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

#endif
