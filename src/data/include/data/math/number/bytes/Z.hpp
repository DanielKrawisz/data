// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/math/number/bytes/bytes.hpp>
#include <data/arithmetic/complementary.hpp>

namespace data::math::number {
    
    // N_bytes is a representation of the natural numbers that
    // is also a string of bytes. It has an order parameter
    // representing endianness.
    template <endian::order r, std::unsigned_integral word> struct N_bytes : oriented<r, word> {

        // read in a standard number representation as a string.
        // decimal and hexidecimal are both accepted. In hexidecimal,
        // the number will be read to be identical with the hexidecimal
        // representation, including extra leading zeros.
        // for example, "0x00" and "0x" are both representations of zero
        // but different strings of bytes.
        static N_bytes read (string_view x);
        //explicit N_bytes (const std::string &);

        // read in the number as a string of bytes.
        static N_bytes read (slice<const word> x);

        // trim the number to its minimal representation.
        N_bytes &trim ();

        // get a representation of zero of any size.
        static N_bytes zero (size_t size = 0);
        
        N_bytes () : oriented<r, word> {} {}

        // construct from any number literal
        template <std::signed_integral I> N_bytes (I);
        template <std::unsigned_integral I> N_bytes (I);

        explicit N_bytes (string_view x): N_bytes {read (x)} {}
        explicit N_bytes (slice<const word> x): N_bytes {read (x)} {}
        
        operator N_bytes<endian::opposite (r), word> () const;

        template <neg c>
        explicit operator Z_bytes<r, c, word> () const;
        explicit operator double () const;
        
        explicit operator uint64 () const;
        explicit operator int64 () const;

        N_bytes (bytestring<word> &&b): oriented<r, word> {b} {}

    };
    
    template <endian::order r, std::unsigned_integral word> struct Z_bytes<r, neg::twos, word> : oriented<r, word> {
        
        Z_bytes () : oriented<r, word> {} {}

        // construct from any number literal.
        template <std::integral I> Z_bytes (I);
        
        // string constructors.
        static Z_bytes read (string_view x);
        static Z_bytes read (slice<const word> x);
        explicit Z_bytes (string_view x) : Z_bytes {read (x)} {}
        explicit Z_bytes (slice<const word> x): Z_bytes {read (x)} {}
        
        operator Z_bytes<endian::opposite (r), neg::twos, word> () const;
        operator Z_bytes<r, neg::BC, word> () const;

        explicit operator N_bytes<r, word> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0);
        
        explicit operator int64 () const;
        
        Z_bytes &trim ();

        Z_bytes (bytestring<word> &&b) : oriented<r, word> {b} {}\

    };
    
    // for little endian, these are an implementation of bitcoin numbers.
    template <endian::order r, std::unsigned_integral word> struct Z_bytes<r, neg::BC, word> : oriented<r, word> {

        //explicit Z_bytes (const std::string &);
        Z_bytes () : oriented<r, word> {} {}
        
        Z_bytes (bool);

        // construct from any number literal.
        template <std::signed_integral I> Z_bytes (I);
        template <std::unsigned_integral I> Z_bytes (I);
        
        // string constructors.
        static Z_bytes read (string_view x);
        static Z_bytes read (slice<const word> x);
        explicit Z_bytes (string_view x): Z_bytes {read (x)} {}
        explicit Z_bytes (slice<const word> x): Z_bytes {read (x)} {}

        // we need this constructor because the compiler gets confused
        // with the bool constructor when we try to use string literals.
        Z_bytes (const char *x): Z_bytes {string_view {x, std::strlen (x)}} {}

        explicit operator N_bytes<r, word> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0, bool negative = false);
        
        explicit operator int64 () const;
        explicit operator size_t () const;

        Z_bytes &trim ();

        Z_bytes (bytestring<word> &&b): oriented<r, word> {b} {}

        explicit operator bool () const {
            return !data::is_zero (*this);
        }

        operator Z_bytes<endian::opposite (r), neg::BC, word> () const;
        operator Z_bytes<r, neg::twos, word> () const;
    };

    // increment and decrement
    template <endian::order r, std::unsigned_integral word>
    nonzero<N_bytes<r, word>> inline increment<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        nonzero<N_bytes<r, word>> v {n};
        ++v.Value;
        return v;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline decrement<N_bytes<r, word>>::operator () (const nonzero<N_bytes<r, word>> &n) {
        auto x = n.Value;
        return --x;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline decrement<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        if (data::is_zero (n)) return n;
        auto x = n;
        return --x;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline increment<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return ++x;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline decrement<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return --x;
    }
    
    // some functions that can easily be implemented as other functions using conversions. 

    template <endian::order r, neg c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a <=> Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) <=> b;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator ~ (const N_bytes<r, word> &b) {
        return ~Z_bytes<r, neg::twos, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator ^ (
        const Z_bytes<r, neg::twos, word> &a, const N_bytes<r, word> &b) {
        return a ^ Z_bytes<r, neg::twos, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator ^ (
        const N_bytes<r, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        return Z_bytes<r, neg::twos, word> (a) ^ b;
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator & (
        const Z_bytes<r, neg::twos, word> &a, const N_bytes<r, word> &b) {
        return a & Z_bytes<r, neg::twos, word> (b);
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator & (
        const N_bytes<r, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        return Z_bytes<r, neg::twos, word> (a) & b;
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator | (
        const Z_bytes<r, neg::twos, word> &a, const N_bytes<r, word> &b) {
        return a | Z_bytes<r, neg::twos, word> (b);
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator | (
        const N_bytes<r, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        return Z_bytes<r, neg::twos, word> (a) | b;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a + Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a - Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a + (-b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator * (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a * Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) + b;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) - b;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator * (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) * b;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator + (const N_bytes<r, word> &a, uint64 b) {
        return a + N_bytes<r, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator - (const N_bytes<r, word> &a, uint64 b) {
        return a - N_bytes<r, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator * (const N_bytes<r, word> &a, uint64 b) {
        return a * N_bytes<r, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const Z_bytes<r, c, word> &a, int64 b) {
        return a + Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, int64 b) {
        return a - Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator * (const Z_bytes<r, c, word> &a, int64 b) {
        return a * Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator ++ (N_bytes<r, word> &x, int) {
        N_bytes<r, word> z = x;
        ++x;
        return z;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator -- (N_bytes<r, word> &x, int) {
        N_bytes<r, word> z = x;
        --x;
        return z;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator += (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x + n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -= (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x - n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator += (N_bytes<r, word> &x, uint64 n) {
        return x = x + n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -= (N_bytes<r, word> &x, uint64 n) {
        return x = x - n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator *= (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x * n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator *= (N_bytes<r, word> &x, uint64 n) {
        return x = x * n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator <<= (N_bytes<r, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator >>= (N_bytes<r, word> &x, int64 i) {
        return x = x >> i;
    }
    
    // post increment
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator ++ (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        ++x;
        return z;
    }
    
    // post increment
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator -- (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        --x;
        return z;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x + n;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x - n;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, int64 n) {
        return x = x + n;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, int64 n) {
        return x = x - n;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x * n;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const int64 n) {
        return x = x * n;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator /= (Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a = a / b;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator <<= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator >>= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x >> i;
    }
    
    // check if the number is negative, and then do bit shift on the absolute value.
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator << (const Z_bytes<r, neg::BC, word> &x, int i) {
        return Z_bytes<r, neg::BC, word> (Z_bytes<r, neg::twos, word> (x) << i);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator >> (const Z_bytes<r, neg::BC, word> &x, int i) {
        return Z_bytes<r, neg::BC, word> (Z_bytes<r, neg::twos, word> (x) >> i);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator - (const Z_bytes<r, neg::twos, word> &x) {
        auto z = ~x;
        return ++z;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator %= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    N_bytes<r, word> inline &operator %= (Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline &operator %=
    (Z_bytes<r, neg::BC, word> &a, const Z_bytes<r, neg::BC, word> &b) {
        return a = a % b;
    }
/*
    template <endian::order r, std::unsigned_integral word>
    uint64 &operator %= (N_bytes<r, word> &, uint64);*/

    template <endian::order r, neg c, std::unsigned_integral word>
    uint64 &operator %= (Z_bytes<r, c, word> &a, uint64 b) {
        return a = a % b;
    }

    template <endian::order r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &u, I x) {
        return u & N_bytes<r, word> {x};
    }

    template <std::unsigned_integral I, endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator & (I x, const N_bytes<r, word> &u) {
        return u & N_bytes<r, word> {x};
    }

    template <endian::order r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg::twos, word> inline operator & (const Z_bytes<r, neg::twos, word> &u, I x) {
        return u & Z_bytes<r, neg::twos, word> {x};
    }

    template <std::integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator & (I x, const Z_bytes<r, neg::twos, word> &u) {
        return u & Z_bytes<r, neg::twos, word> {x};
    }

    template <endian::order r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, neg::twos, word> inline operator & (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, neg::twos, word> {x} & Z_bytes<r, neg::twos, word> {u};}

    template <std::signed_integral I, endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator & (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, neg::twos, word> {x} & Z_bytes<r, neg::twos, word> {u};
    }
    
}

namespace data {

    template <endian::order a, endian::order b, std::unsigned_integral word>
    bool inline identical (const math::number::N_bytes<a, word> &x, const math::number::N_bytes<b, word> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }

    template <endian::order a, neg b, endian::order c, neg d, std::unsigned_integral word>
    bool inline identical (const math::number::Z_bytes<a, b, word> &x, const math::number::Z_bytes<c, d, word> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }
    
}

namespace data::math::def {

    template <endian::order r, std::unsigned_integral word>
    math::sign inline sign<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return arithmetic::nones::sign (x.words ());
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    math::sign inline sign<Zb<r, c, word>>::operator () (const Zb<r, c, word> &x) {
        if constexpr (c == neg::twos) return arithmetic::twos::sign (x.words ());
        if constexpr (c == neg::BC) return arithmetic::BC::sign (x.words ());
    }

    template <endian::order r, std::unsigned_integral word>
    bool inline is_negative<N_bytes<r, word>>::operator () (const N_bytes<r, word> &) {
        return false;
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_positive<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return !data::is_zero (x);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline abs<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes_BC<r, word> inline abs<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_negative (x) ? -x : x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> abs<Z_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &x) {
        if (data::is_zero (x)) return N_bytes<r, word> {};
        Zb<r, neg::twos, word> z = data::is_negative (x) ? -x : x;
        auto n = N_bytes<r, word>::zero (z.size ());
        std::copy (z.begin (), z.end (), n.begin ());
        return n;
    }

    template <endian::order r, neg zz, std::unsigned_integral word>
    Zb<r, zz, word> inline times<Zb<r, zz, word>>::operator ()
    (const Zb<r, zz, word> &a, const Zb<r, zz, word> &b) {
        return a * b;
    }

    template <endian::order r, neg zz, std::unsigned_integral word>
    nonzero<Zb<r, zz, word>> inline times<Zb<r, zz, word>>::operator ()
        (const nonzero<Zb<r, zz, word>> &a, const nonzero<Zb<r, zz, word>> &b) {
        return a * b;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Zb<r, neg::BC, word> inline
    next (const Zb<r, neg::BC, word> &z) {
        return is_positive (z) ? -z : -z - 1;
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_negative<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return arithmetic::sign_bit (x.words ());
    }

    template <endian::order r, std::unsigned_integral word>
    bool inline is_negative<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return !data::is_zero (x) && arithmetic::sign_bit (x.words ());
    }

    template <endian::order r, std::unsigned_integral word>
    bool inline is_positive<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return !data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }

    template <endian::order r, std::unsigned_integral word>
    bool inline is_positive<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return !data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_zero<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return arithmetic::is_zero (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_zero<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return arithmetic::is_zero (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return arithmetic::BC::is_zero (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_positive_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_negative_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_zero (x) && arithmetic::sign_bit (x.words ());
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline bit_xor<N_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a ^ b;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_not<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        return ~n;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_xor<math::Z_bytes<r, word>>::operator ()
        (const math::Z_bytes<r, word> &a, const math::Z_bytes<r, word> &b) {
        return a ^ b;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_xor<N_bytes<r, word>, math::Z_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const math::Z_bytes<r, word> &b) {
        return math::Z_bytes<r, word> {a} ^ b;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_xor<math::Z_bytes<r, word>, N_bytes<r, word>>::operator ()
    (const math::Z_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a ^ math::Z_bytes<r, word> {b};
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_not<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x) {
        auto a = x;
        arithmetic::bit_negate<word> (a.end (), a.begin (), x.begin ());
        return a;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline mul_2_pow<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &a, uint32 u) {
        return a << u;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline mod_2<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x) {
        if (data::is_zero (x)) return 0;
        return *x.words ().begin () & 1;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_xor<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_xor (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_xor<word> (bb.end (), bb.begin (), const_cast<const decltype (bb) &> (bb).begin (), a.begin ());
        return bb;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_or<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_or (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_or<word> (bb.end (), bb.begin (), const_cast<const decltype (bb) &> (bb).begin (), a.begin ());
        return bb;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_and<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_and (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_and<word> (bb.end (), bb.begin (), const_cast<const decltype (bb) &> (bb).begin (), a.begin ());
        return bb;
    }

    template <endian::order r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline div_2<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &z) {
        return trim ((z < 0 ? data::increment (z): z) >> 1);
    }
    
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_minimal (const N_bytes<r, word> &x) {
        return arithmetic::nones::is_minimal (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_minimal (const Z_bytes<r, neg::twos, word> &x) {
        return arithmetic::twos::is_minimal (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_minimal (const Z_bytes<r, neg::BC, word> &x) {
        return arithmetic::BC::is_minimal (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    size_t inline minimal_size (const N_bytes<r, word> &x) {
        return arithmetic::minimal_size<neg::nones> (x.words ());
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    size_t inline minimal_size (const Z_bytes<r, c, word> &x) {
        return arithmetic::minimal_size<c> (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline extend (const N_bytes<r, word> &x, size_t size) {
        return arithmetic::extend<r, neg::nones, word> (x, size);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline extend (const Z_bytes<r, c, word> &x, size_t size) {
        return arithmetic::extend<r, c, word> (x, size);
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline operator == (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::nones::compare<r, word> (a, b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        return arithmetic::twos::compare<r, word> (a, b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, neg::BC, word> &a, const Z_bytes<r, neg::BC, word> &b) {
        return arithmetic::BC::compare<r, word> (a, b);
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, c, word> &a, int64 i) {
        return a <=> Z_bytes<r, c, word> {i};
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, uint64 i) {
        return a <=> N_bytes<r, word> {i};
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline operator == (const N_bytes<r, word> &x, uint64 i) {
        return x == N_bytes<r, word> {i};
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &x, int64 i) {
        return (x <=> Z_bytes<r, c, word> {i}) == 0;
    }

    template <endian::order r, neg cl, neg cr, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cr, word> &b) {
        return a == Z_bytes<r, cl, word> (b);
    }

    template <endian::order r, neg cl, neg cr, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cl, word> &b) {
        return a <=> Z_bytes<r, cl, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator - (const N_bytes<r, word> &x) {
        auto z = Z_bytes<r, neg::twos, word> (x);
        return -z;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> N_bytes<r, word>::read (slice<const word> b) {
        auto x = N_bytes<r, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word>
    Z_bytes<r, neg::twos, word>::read (slice<const word> b) {
        auto x = Z_bytes<r, neg::twos, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> Z_bytes<r, neg::BC, word>::read (slice<const word> b) {
        auto x = Z_bytes<r, neg::BC, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline N_bytes<r, word>::zero (size_t size) {
        return arithmetic::zero<r, word> (size);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline Z_bytes<r, neg::twos, word>::zero (size_t size) {
        return arithmetic::zero<r, word> (size);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline Z_bytes<r, neg::BC, word>::zero (size_t size, bool negative) {
        return arithmetic::BC::zero<r, word> (size, negative);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator - (const Z_bytes<r, neg::BC, word> &x) {
        return arithmetic::trim<r, neg::BC, word> (arithmetic::BC::negate<r, word> (x));
    }

    namespace {
        template <endian::order r, std::unsigned_integral word>
        const Z_bytes<r, neg::BC, word> &true_value () {
            static Z_bytes<r, neg::BC, word> TrueValue = Z_bytes<r, neg::BC, word>::read ("0x01");
            return TrueValue;
        }

        template <endian::order r, std::unsigned_integral word>
        const Z_bytes<r, neg::BC, word> &false_value () {
            static Z_bytes<r, neg::BC, word> FalseValue = Z_bytes<r, neg::BC, word>::read ("0x");
            return FalseValue;
        };
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator !
    (const Z_bytes<r, neg::BC, word> &x) {
        return bool (x) ? false_value<r, word> () : true_value<r, word> ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator &&
    (const Z_bytes<r, neg::BC, word> &x, const Z_bytes<r, neg::BC, word> &y) {
        return bool (x) && bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator ||
    (const Z_bytes<r, neg::BC, word> &x, const Z_bytes<r, neg::BC, word> &y) {
        return bool (x) || bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ~ (const Z_bytes<r, neg::twos, word> &x) {
        auto z = extend (x, x.size () + 1);
        arithmetic::bit_negate<word> (z.words ().end (), z.words ().begin (), z.words ().begin ());
        return trim (z);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::trim<r, neg::nones, word> (arithmetic::bit_and<r, neg::nones, word> (a, b));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator & (const Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        return arithmetic::trim<r, neg::twos, word> (arithmetic::bit_and<r, neg::twos, word> (a, b));
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator | (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::trim<r, neg::nones, word> (arithmetic::bit_or<r, neg::nones, word> (a, b));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator | (const Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        return arithmetic::trim<r, neg::twos, word> (arithmetic::bit_or<r, neg::twos, word> (a, b));
    }

    // bit xor
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator ^ (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) return b ^ a;
        auto bt = extend (b, a.size ());
        auto x = N_bytes<r, word>::zero (a.size ());
        arithmetic::bit_xor<word> (x.end (), x.begin (), a.begin (),
            const_cast<const N_bytes<r, word> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator ^ (const Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        if (a.size () < b.size ()) return b ^ a;
        auto bt = extend (b, a.size ());
        auto x = Z_bytes<r, neg::twos, word>::zero (a.size ());
        arithmetic::bit_xor<word> (x.end (), x.begin (), a.begin (),
            const_cast<const Z_bytes<r, neg::twos, word> &> (bt).begin ());
        return x.trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> &operator ^= (Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_xor<word> (a.end (), a.begin (), a.begin (), b.begin ());
        return a.trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        arithmetic::bit_xor<word> (a.end (), a.begin (), a.begin (), b.begin ());
        return a.trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator +
    (const Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b) {

        bool an = data::is_negative (a);
        bool bn = data::is_negative (b);
        auto ax = data::abs (a);
        auto bx = data::abs (b);

        if (!an && !bn) return Z_bytes<r, neg::twos, word> (ax + bx);
        if (an && bn) return -(ax + bx);

        return ax > bx ?
            (an ? -(ax - bx) : Z_bytes<r, neg::twos, word> (ax - bx)) :
            (an ? Z_bytes<r, neg::twos, word> (bx - ax) : -(bx - ax));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator +
    (const Z_bytes<r, neg::BC, word> &a, const Z_bytes<r, neg::BC, word> &b) {
        return arithmetic::trim<r, neg::BC, word> (arithmetic::BC::plus<r, word> (trim (a), trim (b)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> operator *
    (const Z_bytes<r, neg::twos, word> &a, const Z_bytes<r, neg::twos, word> &b)
    {
        bool an = data::is_negative (a);
        bool bn = data::is_negative (b);
        if ((an && bn) || (!an && !bn)) return Z_bytes<r, neg::twos, word> (data::abs (a) * data::abs (b));
        return -(data::abs (a) * data::abs (b));
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> operator * (const Z_bytes<r, neg::BC, word> &a, const Z_bytes<r, neg::BC, word> &b)
    {
        return arithmetic::trim<r, neg::BC, word> (arithmetic::BC::times<r, word> (trim (a), trim (b)));
    }
    
    template <endian::order r, std::unsigned_integral word> template<neg c>
    N_bytes<r, word>::operator Z_bytes<r, c, word> () const {
        Z_bytes<r, c, word> n = Z_bytes<r, c, word>::zero (this->size () + 1);
        std::copy (this->words ().begin (), this->words ().end (), n.words ().begin ());
        return n.trim ();
    }

    template <std::integral I, typename ZZ, typename word>
    void initialize_bytes (I x, ZZ &n) {
        if constexpr (sizeof (I) <= sizeof (word)) {
            n.resize (1);
            *n.begin () = x;
        } else if constexpr (sizeof (I) % sizeof (word) != 0) {
            throw exception {} << "We do not know how to handle this case; init value size is " << sizeof (I) << " and word size is " << sizeof (word);
        } else {
            n.resize (sizeof (I) / sizeof (word));
            data::arithmetic::Words<boost::endian::order::native, word> w {
                slice<word> {(word*) (&x), sizeof (I) / sizeof (word)}};
            std::copy (w.begin (), w.end (), n.words ().begin ());
        }
        n.trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    template <std::signed_integral I> inline
    N_bytes<r, word>::N_bytes (I x) {
        if (x < 0) throw exception {} << "Unsigned type cannot be less than zero";
        initialize_bytes<I, N_bytes<r, word>, word> (x, *this);
    }

    template <endian::order r, std::unsigned_integral word>
    template <std::unsigned_integral I> inline
    N_bytes<r, word>::N_bytes (I x) {
        initialize_bytes<I, N_bytes<r, word>, word> (x, *this);
    }

    template <endian::order r, std::unsigned_integral word>
    template <std::integral I> inline
    Z_bytes<r, neg::twos, word>::Z_bytes (I x) {
        initialize_bytes<I, Z_bytes<r, neg::twos, word>, word> (x, *this);
    }

    template <std::unsigned_integral I, typename ZZ, typename word>
    void initialize_bytes_BC (I x, ZZ &n, bool sign_bit = false) {
        if constexpr (sizeof (I) <= sizeof (word)) {
            n.resize (2);
            *n.words().begin () = x;
        } else if constexpr (sizeof (I) % sizeof (word) != 0) {
            throw exception {} << "We do not know how to handle this case; init value size is " << sizeof (I) << " and word size is " << sizeof (word);
        } else {
            n.resize (sizeof (I) / sizeof (word) + 1);
            data::arithmetic::Words<boost::endian::order::native, word> w {
                slice<word> {(word*) (&x), sizeof (I) / sizeof (word)}};
            std::copy (w.begin (), w.end (), n.words ().begin ());
        }

        *(n.words ().end () - 1) = sign_bit ?
            static_cast<word> (std::numeric_limits<std::make_signed_t<word>>::min ()):
            0;

        n.trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    template <std::signed_integral I> inline
    Z_bytes<r, neg::BC, word>::Z_bytes (I x) {
        using U = std::make_unsigned_t<I>;
        if (x == std::numeric_limits<I>::min ())
            initialize_bytes_BC<U, Z_bytes<r, neg::BC, word>, word>
                (static_cast<U> (x), *this, true);
        else if (x < 0)
            initialize_bytes_BC<U, Z_bytes<r, neg::BC, word>, word>
                (static_cast<U> (-x), *this, true);
        else initialize_bytes_BC<U, Z_bytes<r, neg::BC, word>, word>
                (static_cast<U> (x), *this, false);
    }

    template <endian::order r, std::unsigned_integral word>
    template <std::unsigned_integral I> inline
    Z_bytes<r, neg::BC, word>::Z_bytes (I x) {
        initialize_bytes_BC<I, Z_bytes<r, neg::BC, word>, word> (x, *this, false);
    }

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, neg::BC, word>::Z_bytes (bool b) : oriented<r, word> {} {
        if (b) {
            this->resize (1);
            (*this)[0] = 1;
        }
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline trim (const N_bytes<r, word> &n) {
        return arithmetic::trim<r, neg::nones, word> (n);
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline trim (const Z_bytes<r, c, word> &z) {
        return arithmetic::trim<r, c, word> (z);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &N_bytes<r, word>::trim () {
        arithmetic::trim<r, neg::nones, word> (*this);
        return *this;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline &Z_bytes<r, neg::twos, word>::trim () {
        arithmetic::trim<r, neg::twos, word> (*this);
        return *this;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline &Z_bytes<r, neg::BC, word>::trim () {
        arithmetic::trim<r, neg::BC, word> (*this);
        return *this;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word>::operator uint64 () const {
        if (*this > N_bytes {std::numeric_limits<uint64>::max ()})
            throw exception {} << "value too big";

        endian::integral<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return uint64 (xx);
    } 
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word>::operator int64 () const {
        if (*this > std::numeric_limits<int64>::max ())
            throw std::invalid_argument {"value too big"};

        if (*this < std::numeric_limits<int64>::min ())
            throw std::invalid_argument {"value too small"};

        endian::integral<true, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return int64 (xx);
    } 
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, neg::BC, word>::operator int64 () const {
        return int64 (Z_bytes<r, neg::twos, word> (*this));
    } 

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, neg::BC, word>::operator size_t () const {
        if (data::is_negative (*this)) throw std::invalid_argument {"negative value"};
        if (*this > std::numeric_limits<size_t>::max ()) throw std::invalid_argument {"value too big"};

        constexpr const size_t size = sizeof (std::declval<size_t> ());

        endian::integral<false, endian::little, size> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (size, this->size ()), xx.begin ());

        return size_t (xx);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word>::operator Z_bytes<endian::opposite (r), neg::twos, word> () const {
        Z_bytes<endian::opposite (r), neg::twos, word> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    inline Z_bytes<r, neg::twos, word>::operator Z_bytes<r, neg::BC, word> () const {
        if (data::is_negative (*this)) return -Z_bytes<r, neg::BC, word> (data::abs (*this));
        return Z_bytes<r, neg::BC, word> (data::abs (*this)).trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word>::operator Z_bytes<endian::opposite (r), neg::BC, word> () const {
        Z_bytes<endian::opposite (r), neg::BC, word> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    inline Z_bytes<r, neg::BC, word>::operator Z_bytes<r, neg::twos, word> () const {
        if (data::is_zero (*this)) return Z_bytes<r, neg::twos, word>::zero ();
        if (data::is_negative (*this)) return -Z_bytes<r, neg::twos, word> (data::abs (*this));
        auto z = Z_bytes<r, neg::twos, word>::zero (this->size ());
        std::copy (this->begin (), this->end (), z.begin ());
        return z.trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator ++ (N_bytes<r, word> &x) {
        arithmetic::nones::increment<r> (arithmetic::trim<r, neg::nones> (x));
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -- (N_bytes<r, word> &x) {
        arithmetic::trim<r, neg::nones> (arithmetic::nones::decrement<r> (x));
        return x;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator ++ (Z_bytes<r, c, word> &x) {
        if constexpr (c == neg::twos) arithmetic::trim<r, c> (arithmetic::twos::increment<r> (x));
        if constexpr (c == neg::BC) arithmetic::trim<r, c> (arithmetic::BC::increment<r> (x));
        return x;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -- (Z_bytes<r, c, word> &x) {
        if constexpr (c == neg::twos) arithmetic::trim<r, c> (arithmetic::twos::decrement<r> (x));
        if constexpr (c == neg::BC) arithmetic::trim<r, c> (arithmetic::BC::decrement<r> (x));
        return x;
    }
    
    namespace {
    
        template <endian::order r, std::unsigned_integral word>
        N_bytes<r, word> shift_left (const N_bytes<r, word> &x, uint32 extended, uint32 i) {
            N_bytes<r, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian::order r, std::unsigned_integral word>
        N_bytes<r, word> shift_right (const N_bytes<r, word> &x, uint32 i) {
            N_bytes<r, word> z = x;
            z.words ().bit_shift_right (i);
            return z;
        }
    
        template <endian::order r, std::unsigned_integral word>
        Z_bytes<r, neg::twos, word> shift_left (const Z_bytes<r, neg::twos, word> &x, uint32 extended, uint32 i) {
            Z_bytes<r, neg::twos, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian::order r, std::unsigned_integral word>
        Z_bytes<r, neg::twos, word> shift_right (const Z_bytes<r, neg::twos, word> &x, uint32 i) {
            Z_bytes<r, neg::twos, word> z = x;
            z.words ().bit_shift_right (i, x < 0);
            return z;
        }

        template <endian::order r, std::unsigned_integral word>
        N_bytes<r, word> inline plus (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            return arithmetic::nones::plus<r, word> (a, b);
        }

        template <endian::order r, std::unsigned_integral word>
        N_bytes<r, word> inline minus (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            if (b > a) return N_bytes<r, word>::zero ();
            return arithmetic::nones::minus<r, word> (a, b);
        }

        template <endian::order r, std::unsigned_integral word>
        N_bytes<r, word> times (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            N_bytes<r, word> n = N_bytes<r, word>::zero (a.size () + b.size () + 1);
            auto w = n.words ();
            arithmetic::times (w, a.words (), b.words ());
            return n;
        }
        
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator << (const N_bytes<r, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator >> (const N_bytes<r, word> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator << (const Z_bytes<r, neg::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline operator >> (const Z_bytes<r, neg::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word>::operator N_bytes<endian::opposite (r), word> () const {
        N_bytes<endian::opposite (r), word> z;
        z.resize (this->size ());
        std::copy (this->rbegin (), this->rend (), z.begin ());
        return z;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (plus (trim (a), trim (b)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator - (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (minus (trim (a), trim (b)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator * (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (times (trim (a), trim (b)));
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Quotient;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &j) {
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {j}).Quotient;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const N_bytes<r, word> &j) {
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, uint64 j) {
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Quotient;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, int64 j) {
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const Z_bytes<r, neg::twos, word> &x, const N_bytes<r, word> &j) {
        return data::divmod<Z_bytes<r, neg::twos, word>, N_bytes<r, word>>
        (x, nonzero<Z_bytes<r, neg::twos, word>> {Z_bytes<r, neg::twos, word> {j}}).Remainder;
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline operator %
    (const Z_bytes<r, neg::BC, word> &x, const Z_bytes<r, neg::BC, word> &j) {
        return data::divmod<Z_bytes<r, neg::BC, word>> (x, nonzero<Z_bytes<r, neg::BC, word>> {j}).Remainder;
    }

    template <endian::order r, std::unsigned_integral word>
    uint64 inline operator % (const N_bytes<r, word> &x, uint64 j) {
        return uint64 (data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Remainder);
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    uint64 inline operator % (const Z_bytes<r, c, word> &x, uint64 j) {
        return uint64 (data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Remainder);
    }
}

#endif
