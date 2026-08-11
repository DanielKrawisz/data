// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <compare>

#include <data/sign.hpp>
#include <data/abs.hpp>
#include <data/arithmetic.hpp>
#include <data/divmod.hpp>
#include <data/increment.hpp>
#include <data/math/algebra.hpp>

#include <data/arithmetic.hpp>

#include <data/encoding/hex.hpp>
#include <data/arithmetic/negativity.hpp>

#include <string>

namespace data::math::number::GMP {

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
    using Z = math::number::GMP::Z;
    using N = math::number::GMP::N;
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

}

namespace data::math::number {

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
}

namespace data::encoding::decimal {
    struct string;
    string write (const N &);
    
    std::ostream &write (std::ostream &, const N &);
    
}

namespace data::encoding::hexidecimal {
    template <neg, hex_case> struct integer;
    
    template <hex_case zz> integer<neg::nones, zz> write (const N &);
    template <neg n, hex_case zz> integer<n, zz> write (const Z &);

    std::ostream &write (std::ostream &, const N &, hex_case = hex_case::lower);
    std::ostream &write (std::ostream &, const Z &, hex_case = hex_case::lower, neg = neg::twos);
    
}

namespace data::encoding::signed_decimal {
    struct string;
    string write (const Z &);
    
    std::ostream &write (std::ostream &, const Z &);
}


#endif
