// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_BYTES
#define DATA_MATH_NUMBER_BYTES_BYTES

#include <data/arithmetic/negativity.hpp>

#include <data/math/group.hpp>

#include <data/complex.hpp>

#include <data/divmod.hpp>

#include <data/bytes.hpp>

namespace data::math::number {
    
    // an implementation of the natural numbers that is
    // encoded as a big or little endian sequence of bytes. 
    template <endian::order, std::unsigned_integral> struct N_bytes;

    template <endian::order r, std::unsigned_integral word> struct increment<N_bytes<r, word>> {
        nonzero<N_bytes<r, word>> operator () (const N_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word> struct decrement<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const nonzero<N_bytes<r, word>> &);
        N_bytes<r, word> operator () (const N_bytes<r, word> &);
    };
    
    // similar implementation of the integers. We have two's complement
    // and the sign-and-magnetude system used in Bitcoin. N_bytes works
    // as the absolute value of the two's complement types. Bitcoin numbers
    // work as their own number system without a type for the naturals.
    template <endian::order, neg, std::unsigned_integral> struct Z_bytes;
}

namespace data::math {

    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = number::Z_bytes<r, neg::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = number::Z_bytes<r, neg::BC, word>;

}

namespace data::math::number {

    // pre-increment and decreement
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
    
    // comparisons
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
    
    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N_bytes<r, word> &, uint64);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, c, word> &, int64);
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, uint64);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, c, word> &, int64);
    
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

    // negation
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator - (const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator - (const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator - (const Z_bytes<r, neg::BC, word> &);
    
    // arithmetic
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator *
        (const Z_bytes<r, neg::twos, word> &, const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator *
        (const Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, I);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, I);

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, I);

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator + (I, const N_bytes<r, word> &);

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator - (I, const N_bytes<r, word> &);

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator * (I, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, neg::twos, word> operator + (const N_bytes<r, word> &, I);

    template <endian::order r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, neg::twos, word> operator - (const N_bytes<r, word> &, I);

    template <endian::order r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, neg::twos, word> operator * (const N_bytes<r, word> &, I);

    template <std::signed_integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator + (I, const N_bytes<r, word> &);

    template <std::signed_integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator - (I, const N_bytes<r, word> &);

    template <std::signed_integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator * (I, const N_bytes<r, word> &);

    template <endian::order r, neg neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> operator + (const Z_bytes<r, neg, word> &, I);

    template <endian::order r, neg neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> operator - (const Z_bytes<r, neg, word> &, I);

    template <endian::order r, neg neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> operator * (const Z_bytes<r, neg, word> &, I);

    template <std::integral I, endian::order r, neg neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator + (I, const Z_bytes<r, neg, word> &);

    template <std::integral I, endian::order r, neg neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator - (I, const Z_bytes<r, neg, word> &);

    template <std::integral I, endian::order r, neg neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator * (I, const Z_bytes<r, neg, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator += (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator += (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator -= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, uint64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator += (Z_bytes<r, c, word> &, int64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -= (Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator *= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator *= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator *= (Z_bytes<r, c, word> &, int64);

    // power (for neg nones and twos, the ^ operator means bit xor).
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator ^
        (const Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> &operator ^=
        (Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);

    // division
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator % (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    N_bytes<r, word> operator % (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator % (
        const Z_bytes<r, neg::twos, word> &,
        const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator % (
        const Z_bytes<r, neg::BC, word> &,
        const Z_bytes<r, neg::BC, word> &);
    
    // functions having to do with size.
    template <endian::order r, std::unsigned_integral word>
    bool is_minimal (const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    bool is_minimal (const Z_bytes<r, neg::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    bool is_minimal (const Z_bytes<r, neg::BC, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    size_t minimal_size (const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    size_t minimal_size (const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> extend (const N_bytes<r, word> &, size_t);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> extend (const Z_bytes<r, c, word> &, size_t);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> trim (const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> trim (const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &o, const N_bytes<r, word> &n);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    std::ostream &operator << (std::ostream &o, const Z_bytes<r, c, word> &n);

    template <endian::order r, std::unsigned_integral word>
    std::istream &operator >> (std::istream &o, N_bytes<r, word> &n);
    
    template <endian::order r, neg c, std::unsigned_integral word>
    std::istream &operator >> (std::istream &o, Z_bytes<r, c, word> &n);

    template <endian::order r, neg c, std::unsigned_integral word>
    struct increment<Z_bytes<r, c, word>> {
        Z_bytes<r, c, word> operator () (const Z_bytes<r, c, word> &);
    };

    template <endian::order r, neg c, std::unsigned_integral word>
    struct decrement<Z_bytes<r, c, word>> {
        Z_bytes<r, c, word> operator () (const Z_bytes<r, c, word> &);
    };
    
}

namespace data::math::def {

    template <endian::order r, neg c, std::unsigned_integral word = byte>
    using Zb = number::Z_bytes<r, c, word>;
    
    template <endian::order r, std::unsigned_integral word>
    struct is_zero<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_zero<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };
    
    template <endian::order r, std::unsigned_integral word>
    struct is_negative<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_negative<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_negative<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &x);
    };
    
    template <endian::order r, std::unsigned_integral word>
    struct is_positive<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_positive<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_positive<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };
    
    template <endian::order r, std::unsigned_integral word>
    struct is_positive_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct is_negative_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };
    
    template <endian::order r, std::unsigned_integral word>
    struct abs<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const N_bytes<r, word> &);
    };
    
    template <endian::order r, std::unsigned_integral word>
    struct abs<Z_bytes<r, word>> {
        N_bytes<r, word> operator () (const Z_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct abs<Z_bytes_BC<r, word>> {
        Z_bytes_BC<r, word> operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian::order r, neg zz, std::unsigned_integral word>
    struct times<Zb<r, zz, word>, Zb<r, zz, word>> {
        Zb<r, zz, word> operator ()
            (const Zb<r, zz, word> &a, const Zb<r, zz, word> &b);

        nonzero<Zb<r, zz, word>> operator ()
            (const nonzero<Zb<r, zz, word>> &a, const nonzero<Zb<r, zz, word>> &b);
    };

    
    template <endian::order r, neg c, std::unsigned_integral word>
    struct inverse<plus<Zb<r, c, word>>, Zb<r, c, word>> {
        Zb<r, c, word> operator () (const Zb<r, c, word> &a, const Zb<r, c, word> &b) {
            return b - a;
        }
    };

    template <endian::order r, std::unsigned_integral word>
    struct divmod<N_bytes<r, word>, N_bytes<r, word>> {
        division<N_bytes<r, word>, N_bytes<r, word>> operator ()
            (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct divmod<Z_bytes<r, word>, N_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
            (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct divmod<Z_bytes<r, word>, Z_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
            (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct divmod<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> {
        division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> operator ()
            (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct sign<N_bytes<r, word>> {
        math::sign operator () (const N_bytes<r, word> &);
    };

    template <endian::order r, neg c, std::unsigned_integral word>
    struct sign<Zb<r, c, word>> {
        math::sign operator () (const Zb<r, c, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_xor<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const N_bytes<r, word> &a, const N_bytes<r, word> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_not<N_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const N_bytes<r, word> &n);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_xor<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &a, const math::Z_bytes<r, word> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_xor<N_bytes<r, word>, math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const N_bytes<r, word> &, const math::Z_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_xor<math::Z_bytes<r, word>, N_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &, const N_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_xor<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &, const math::Z_bytes_BC<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_not<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct mul_2_pow<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &, uint32 u);
    };

    template <endian::order r, std::unsigned_integral word>
    struct div_2<math::N_bytes<r, word>> {
        math::N_bytes<r, word> operator () (const math::N_bytes<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct div_2<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct div_2<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct mod_2<math::N_bytes<r, word>> {
        math::N_bytes<r, word> operator () (const math::N_bytes<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct mod_2<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct mod_2<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &x);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_or<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct bit_and<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b);
    };
    
}

namespace data {

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
    
    template <endian::order r, std::unsigned_integral word>
    uint64 operator % (const N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    uint64 operator % (const Z_bytes<r, c, word> &, uint64);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator /= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, uint64);

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator /= (Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> &operator %=
        (Z_bytes<r, neg::BC, word> &, const Z_bytes<r, neg::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, uint64);
    
}

namespace data::encoding {
    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = math::number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = math::number::Z_bytes<r, neg::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = math::number::Z_bytes<r, neg::BC, word>;
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number. 
namespace data::encoding::decimal {
    
    template <endian::order r, std::unsigned_integral word> maybe<N_bytes<r, word>> read (string_view s);
    
    struct string;
    
    template <endian::order r, std::unsigned_integral word> string write (const N_bytes<r, word> &z);
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const N_bytes<r, word> &n) {
        return o << static_cast<std::string> (write (n));
    }
    
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, neg n, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, n, word>> read (string_view);
    
    struct string;
    
    template <endian::order r, neg n, std::unsigned_integral word>
    string write (const math::number::Z_bytes<r, n, word> &);
    
    template <endian::order r, neg n, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, n, word> &);
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r, std::unsigned_integral word>
    maybe<oriented<r, word>> read (string_view);
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream &write (std::ostream &, const oriented<r, word> &, hex_case q = hex_case::lower);
    
    template <hex_case zz> struct string;
    
    template <hex_case zz, endian::order r, std::unsigned_integral word>
    string<zz> write (const oriented<r, word> &);
    
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
