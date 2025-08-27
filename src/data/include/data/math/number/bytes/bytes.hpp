// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_BYTES
#define DATA_MATH_NUMBER_BYTES_BYTES

#include <data/bytes.hpp>

namespace data::math::number {

    // TODO we want a new concept for unsigned_integral that
    // will include std::unsigned_integral as well as our boost
    // endian unsigned numbers and our bounded numbers.
    
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
    template <endian::order, negativity, std::unsigned_integral> struct Z_bytes;

    template <endian::order r, negativity c, std::unsigned_integral word>
    struct increment<Z_bytes<r, c, word>> {
        Z_bytes<r, c, word> operator () (const Z_bytes<r, c, word> &);
    };

    template <endian::order r, negativity c, std::unsigned_integral word>
    struct decrement<Z_bytes<r, c, word>> {
        Z_bytes<r, c, word> operator () (const Z_bytes<r, c, word> &);
    };
}

namespace data::math {

    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = number::Z_bytes<r, negativity::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = number::Z_bytes<r, negativity::BC, word>;

}

namespace data::math::number {

    // pre-increment and decreement
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator ++ (N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator ++ (Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator -- (N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -- (Z_bytes<r, c, word> &);

    // post-increment and decrement
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator ++ (N_bytes<r, word> &, int);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator ++ (Z_bytes<r, c, word> &, int);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator -- (N_bytes<r, word> &, int);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator -- (Z_bytes<r, c, word> &, int);
    
    // comparisons
    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity cl, negativity cr, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, cl, word> &, const Z_bytes<r, cr, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, negativity::twos, word> &, const Z_bytes<r, negativity::twos, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian::order r, negativity cl, negativity cr, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, cl, word> &, const Z_bytes<r, cl, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N_bytes<r, word> &, uint64);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    bool operator == (const Z_bytes<r, c, word> &, int64);
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering operator <=> (const N_bytes<r, word> &, uint64);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    std::weak_ordering operator <=> (const Z_bytes<r, c, word> &, int64);
    
    // bit negate
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ~ (const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator ~ (const Z_bytes<r, c, word> &);
    
    // bit xor
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ^ (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ^
        (const Z_bytes<r, negativity::twos, word> &, const Z_bytes<r, negativity::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator ^
        (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ^ (const Z_bytes<r, negativity::twos, word> &, const N_bytes<r, word> &);
    
    // bit and 
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator & (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator & (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator & (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator & (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator & (const N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator & (const Z_bytes<r, c, word> &, uint64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator &= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator &= (N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator &= (Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator &= (Z_bytes<r, c, word> &, uint64);
    
    // bit or 
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator | (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator | (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator | (const N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator | (const Z_bytes<r, c, word> &, uint64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator |= (Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator |= (Z_bytes<r, c, word> &, uint64);

    // bit shift
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator << (const N_bytes<r, word> &, int);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator >> (const N_bytes<r, word> &, int);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator << (const Z_bytes<r, c, word> &, int);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator >> (const Z_bytes<r, c, word> &, int);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator <<= (N_bytes<r, word> &, int64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator <<= (Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator >>= (N_bytes<r, word> &, int64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator >>= (Z_bytes<r, c, word> &, int64);
    
    // negation
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator - (const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator - (const Z_bytes<r, negativity::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator - (const Z_bytes<r, negativity::BC, word> &);

    // logical
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator ! (const Z_bytes<r, negativity::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator && (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator || (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);
    
    // arithmetic
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator *
        (const Z_bytes<r, negativity::twos, word> &, const Z_bytes<r, negativity::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator *
        (const Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, uint64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, uint64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, uint64);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, int64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, int64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator += (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator += (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator -= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, uint64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator += (Z_bytes<r, c, word> &, int64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -= (Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator *= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator *= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator *= (Z_bytes<r, c, word> &, int64);

    // division
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator % (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    N_bytes<r, word> operator % (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator % (
        const Z_bytes<r, negativity::twos, word> &,
        const Z_bytes<r, negativity::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator % (
        const Z_bytes<r, negativity::BC, word> &,
        const Z_bytes<r, negativity::BC, word> &);
    
    // functions having to do with size.
    template <endian::order r, std::unsigned_integral word>
    bool is_minimal (const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    bool is_minimal (const Z_bytes<r, negativity::twos, word> &);

    template <endian::order r, std::unsigned_integral word>
    bool is_minimal (const Z_bytes<r, negativity::BC, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    size_t minimal_size (const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    size_t minimal_size (const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> extend (const N_bytes<r, word> &, size_t);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> extend (const Z_bytes<r, c, word> &, size_t);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> trim (const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> trim (const Z_bytes<r, c, word> &);
    
}

namespace data::math {
    
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
    struct negate<N_bytes<r, word>> {
        Z_bytes<r, word> operator () (const N_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct negate<Z_bytes<r, word>> {
        Z_bytes<r, word> operator () (const Z_bytes<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct abs<Z_bytes_BC<r, word>> {
        Z_bytes_BC<r, word> operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian::order r, std::unsigned_integral word>
    struct negate<Z_bytes_BC<r, word>> {
        Z_bytes_BC<r, word> operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian::order r, negativity zz, std::unsigned_integral word>
    struct times<number::Z_bytes<r, zz, word>, number::Z_bytes<r, zz, word>> {
        number::Z_bytes<r, zz, word> operator ()
            (const number::Z_bytes<r, zz, word> &a, const number::Z_bytes<r, zz, word> &b);

        nonzero<number::Z_bytes<r, zz, word>> operator ()
            (const nonzero<number::Z_bytes<r, zz, word>> &a, const nonzero<number::Z_bytes<r, zz, word>> &b);
    };
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    struct quadrance<number::Z_bytes<r, c, word>> {
        number::Z_bytes<r, c, word> operator () (const number::Z_bytes<r, c, word> &);
    };
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    struct inverse<plus<number::Z_bytes<r, c, word>>, number::Z_bytes<r, c, word>> {
        number::Z_bytes<r, c, word> operator () (const number::Z_bytes<r, c, word> &a, const number::Z_bytes<r, c, word> &b) {
            return b - a;
        }
    };

    template <endian::order r, std::unsigned_integral word>
    struct divide<N_bytes<r, word>, N_bytes<r, word>> {
        division<N_bytes<r, word>, N_bytes<r, word>> operator ()
            (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct divide<Z_bytes<r, word>, N_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
            (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct divide<Z_bytes<r, word>, Z_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
            (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct divide<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> {
        division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> operator ()
            (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b);
    };

    template <endian::order r, std::unsigned_integral word>
    struct sign<N_bytes<r, word>> {
        signature operator () (const math::N_bytes<r, word> &);
    };

    template <endian::order r, math::negativity c, std::unsigned_integral word>
    struct sign<math::number::Z_bytes<r, c, word>> {
        signature operator () (const math::number::Z_bytes<r, c, word> &);
    };
    
}

namespace data {
    
    template <endian::order a, endian::order b, std::unsigned_integral word>
    bool identical (const math::number::N_bytes<a, word> &, const math::number::N_bytes<b, word> &);

    template <endian::order a, math::negativity b, endian::order c, math::negativity d, std::unsigned_integral word>
    bool identical (const math::number::Z_bytes<a, b, word> &, const math::number::Z_bytes<c, d, word> &);

}

namespace data::math::number {
    
    template <endian::order r, std::unsigned_integral word>
    uint64 operator % (const N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    uint64 operator % (const Z_bytes<r, c, word> &, uint64);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator /= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, uint64);

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator /= (Z_bytes<r, c, word> &, int64);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian::order r, negativity c, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> &operator %=
        (Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, uint64);
    
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number. 
namespace data::encoding::decimal {
    
    template <endian::order r, std::unsigned_integral word> maybe<math::N_bytes<r, word>> read (string_view s);
    
    struct string;
    
    template <endian::order r, std::unsigned_integral word> string write (const math::N_bytes<r, word> &z);
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::N_bytes<r, word> &n) {
        return o << static_cast<std::string> (write (n));
    }
    
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, math::negativity n, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, n, word>> read (string_view);
    
    struct string;
    
    template <endian::order r, math::negativity n, std::unsigned_integral word>
    string write (const math::number::Z_bytes<r, n, word> &);
    
    template <endian::order r, math::negativity n, std::unsigned_integral word>
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
    
    template <endian::order r, math::negativity c, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, c, word>> read (string_view s);
    
    template <endian::order r, math::negativity c, std::unsigned_integral word>
    std::ostream &write (std::ostream &, const math::number::Z_bytes<r, c, word> &);
    
}

#endif
