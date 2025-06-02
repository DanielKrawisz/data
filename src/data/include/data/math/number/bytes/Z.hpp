// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/math/number/gmp/Z.hpp>
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
        static N_bytes read (view<word> x);

        // trim the number to its minimal representation.
        N_bytes &trim ();

        // get a representation of zero of any size.
        static N_bytes zero (size_t size = 0);
        
        N_bytes () : oriented<r, word> {} {}
        
        N_bytes (const uint64 x);
        
        explicit N_bytes (const std::string &x): N_bytes {read (x)} {}
        
        operator N_bytes<endian::opposite (r), word> () const;
        
        template <negativity c>
        explicit operator Z_bytes<r, c, word> () const;
        explicit operator double () const;
        
        explicit operator uint64 () const;
        
        explicit N_bytes (const math::N &);
        explicit operator math::N () const;

        N_bytes (bytestring<word> &&b): oriented<r, word> {b} {}

    };
    
    template <endian::order r, std::unsigned_integral word> struct Z_bytes<r, negativity::twos, word> : oriented<r, word> {
        
        Z_bytes () : oriented<r, word> {} {}
        
        Z_bytes (int64 x);
        Z_bytes (uint64);
        Z_bytes (int32);
        Z_bytes (uint32);
        
        static Z_bytes read (string_view x);
        static Z_bytes read (view<word> x);
        explicit Z_bytes (const std::string &x): Z_bytes {read (x)} {}
        explicit Z_bytes (const char *x) : Z_bytes {std::string {x}} {}
        
        operator Z_bytes<endian::opposite (r), negativity::twos, word> () const;
        operator Z_bytes<r, negativity::BC, word> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0);
        
        explicit operator int64 () const;
        
        explicit Z_bytes (const Z &);
        explicit operator Z () const;
        
        Z_bytes &trim ();

        Z_bytes (bytestring<word> &&b) : oriented<r, word> {b} {}

    };
    
    // for little endian, these are an implementation of bitcoin numbers.
    template <endian::order r, std::unsigned_integral word> struct Z_bytes<r, negativity::BC, word> : oriented<r, word> {

        //explicit Z_bytes (const std::string &);
        Z_bytes () : oriented<r, word> {} {}
        
        Z_bytes (bool);
        Z_bytes (int64 x);
        Z_bytes (uint64);
        Z_bytes (int32);
        Z_bytes (uint32);
        
        static Z_bytes read (string_view x);
        static Z_bytes read (view<word> x);
        explicit Z_bytes (string_view x): Z_bytes {read (x)} {}
        explicit Z_bytes (const std::string &x): Z_bytes {read (x)} {}
        explicit Z_bytes (const char *x) : Z_bytes {std::string {x}} {}
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0, bool negative = false);
        
        explicit operator int64 () const;
        explicit operator size_t () const;
        
        explicit Z_bytes (const Z &);
        explicit operator Z () const;

        Z_bytes &trim ();

        Z_bytes (bytestring<word> &&b): oriented<r, word> {b} {}

        explicit operator bool () const {
            return !data::is_zero (*this);
        }

        operator Z_bytes<endian::opposite (r), negativity::BC, word> () const;
        operator Z_bytes<r, negativity::twos, word> () const;
    };

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

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline increment<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return ++x;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline decrement<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return --x;
    }
    
    // some functions that can easily be implemented as other functions using conversions. 
    template <endian::order r, negativity c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a <=> Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) <=> b;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ~ (const N_bytes<r, word> &b) {
        return ~Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a ^ Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) ^ b;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) ^ Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator & (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) & b;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator & (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return Z_bytes<r, c, word> (b) & a;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator | (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) | b;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator | (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return Z_bytes<r, c, word> (b) | a;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &a, uint64 b) {
        return a & N_bytes<r, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator & (const N_bytes<r, word> &a, int64 b) {
        return a & Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator & (const Z_bytes<r, c, word> &a, uint64 b) {
        return a & N_bytes<r, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator | (const N_bytes<r, word> &a, uint64 b) {
        return a | N_bytes<r, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator | (const N_bytes<r, word> &a, int64 b) {
        return a | Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a + Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a - Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a + (-b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator * (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a * Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) + b;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) - b;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
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
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const Z_bytes<r, c, word> &a, int64 b) {
        return a + Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, int64 b) {
        return a - Z_bytes<r, c, word> (b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
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
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator ++ (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        ++x;
        return z;
    }
    
    // post increment
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator -- (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        --x;
        return z;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x + n;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x - n;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, int64 n) {
        return x = x + n;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, int64 n) {
        return x = x - n;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x * n;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const int64 n) {
        return x = x * n;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator /= (Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a = a / b;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator <<= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator >>= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x >> i;
    }

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::Z_bytes (int64 x):
        Z_bytes {Z_bytes (Z_bytes<r, negativity::twos, word> {x})} {}

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::Z_bytes (int32 x):
        Z_bytes {Z_bytes (Z_bytes<r, negativity::twos, word> {x})} {}
    
    // check if the number is negative, and then do bit shift on the absolute value.
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator << (const Z_bytes<r, negativity::BC, word> &x, int i) {
        return Z_bytes<r, negativity::BC, word> (Z_bytes<r, negativity::twos, word> (x) << i);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator >> (const Z_bytes<r, negativity::BC, word> &x, int i) {
        return Z_bytes<r, negativity::BC, word> (Z_bytes<r, negativity::twos, word> (x) >> i);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator - (const Z_bytes<r, negativity::twos, word> &x) {
        auto z = ~x;
        return ++z;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator %= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    N_bytes<r, word> inline &operator %= (Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline &operator %=
    (Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b) {
        return a = a % b;
    }
/*
    template <endian::order r, std::unsigned_integral word>
    uint64 &operator %= (N_bytes<r, word> &, uint64);*/

    template <endian::order r, negativity c, std::unsigned_integral word>
    uint64 &operator %= (Z_bytes<r, c, word> &a, uint64 b) {
        return a = a % b;
    }
    
}

namespace data {

    template <endian::order a, endian::order b, std::unsigned_integral word>
    bool inline identical (const math::number::N_bytes<a, word> &x, const math::number::N_bytes<b, word> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }

    template <endian::order a, math::negativity b, endian::order c, math::negativity d, std::unsigned_integral word>
    bool inline identical (const math::number::Z_bytes<a, b, word> &x, const math::number::Z_bytes<c, d, word> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }
    
}

namespace data::math {

    template <endian::order r, std::unsigned_integral word>
    signature inline sign<N_bytes<r, word>>::operator () (const math::N_bytes<r, word> &x) {
        return arithmetic::nones::sign (x.words ());
    }

    template <endian::order r, math::negativity c, std::unsigned_integral word>
    signature inline sign<math::number::Z_bytes<r, c, word>>::operator () (const math::number::Z_bytes<r, c, word> &x) {
        if constexpr (c == math::negativity::twos) return arithmetic::twos::sign (x.words ());
        if constexpr (c == math::negativity::BC) return arithmetic::BC::sign (x.words ());
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
        if (data::is_zero (x)) return number::N_bytes<r, word> {};
        number::Z_bytes<r, negativity::twos, word> z = data::is_negative (x) ? -x : x;
        auto n = number::N_bytes<r, word>::zero (z.size ());
        std::copy (z.begin (), z.end (), n.begin ());
        return n;
    }

    template <endian::order r, negativity zz, std::unsigned_integral word>
    number::Z_bytes<r, zz, word> inline times<number::Z_bytes<r, zz, word>>::operator ()
    (const number::Z_bytes<r, zz, word> &a, const number::Z_bytes<r, zz, word> &b) {
        return a * b;
    }

    template <endian::order r, negativity zz, std::unsigned_integral word>
    nonzero<number::Z_bytes<r, zz, word>> inline times<number::Z_bytes<r, zz, word>>::operator ()
        (const nonzero<number::Z_bytes<r, zz, word>> &a, const nonzero<number::Z_bytes<r, zz, word>> &b) {
        return a * b;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    number::Z_bytes<r, c, word> inline quadrance<number::Z_bytes<r, c, word>>::operator () (const number::Z_bytes<r, c, word> &x) {
        return x * x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline next (const N_bytes<r, word> &n) {
        return number::increment<N_bytes<r, word>> (n).Value;
    }
    
    template <endian::order r, std::unsigned_integral word>
    number::Z_bytes<r, negativity::BC, word> inline
    next (const number::Z_bytes<r, negativity::BC, word> &z) {
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
    
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_minimal (const N_bytes<r, word> &x) {
        return arithmetic::nones::is_minimal (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_minimal (const Z_bytes<r, negativity::twos, word> &x) {
        return arithmetic::twos::is_minimal (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline is_minimal (const Z_bytes<r, negativity::BC, word> &x) {
        return arithmetic::BC::is_minimal (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    size_t inline minimal_size (const N_bytes<r, word> &x) {
        return arithmetic::minimal_size<math::negativity::nones> (x.words ());
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    size_t inline minimal_size (const Z_bytes<r, c, word> &x) {
        return arithmetic::minimal_size<c> (x.words ());
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline extend (const N_bytes<r, word> &x, size_t size) {
        return arithmetic::extend<r, negativity::nones, word> (x, size);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline extend (const Z_bytes<r, c, word> &x, size_t size) {
        return arithmetic::extend<r, c, word> (x, size);
    }
    
    template <endian::order r, std::unsigned_integral word>
    bool inline operator == (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::nones::compare<r, word> (a, b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, negativity::twos, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return arithmetic::twos::compare<r, word> (a, b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b) {
        return arithmetic::BC::compare<r, word> (a, b);
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
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
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &x, int64 i) {
        return (x <=> Z_bytes<r, c, word> {i}) == 0;
    }

    template <endian::order r, negativity cl, negativity cr, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cr, word> &b) {
        return a == Z_bytes<r, cl, word> (b);
    }

    template <endian::order r, std::unsigned_integral word>
    bool inline operator == (const N_bytes<r, word> &a, const math::N &b) {
        return math::N (a) == b;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &a, const math::Z &b) {
        return math::Z (a) == b;
    }

    template <endian::order r, negativity cl, negativity cr, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cl, word> &b) {
        return a <=> Z_bytes<r, cl, word> (b);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator - (const N_bytes<r, word> &x) {
        auto z = Z_bytes<r, negativity::twos, word> (x);
        return -z;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> N_bytes<r, word>::read (view<word> b) {
        auto x = N_bytes<r, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>
    Z_bytes<r, negativity::twos, word>::read (view<word> b) {
        auto x = Z_bytes<r, negativity::twos, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> Z_bytes<r, negativity::BC, word>::read (view<word> b) {
        auto x = Z_bytes<r, negativity::BC, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline N_bytes<r, word>::zero (size_t size) {
        return arithmetic::zero<r, word> (size);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline Z_bytes<r, negativity::twos, word>::zero (size_t size) {
        return arithmetic::zero<r, word> (size);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline Z_bytes<r, negativity::BC, word>::zero (size_t size, bool negative) {
        return arithmetic::BC::zero<r, word> (size, negative);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator - (const Z_bytes<r, negativity::BC, word> &x) {
        return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::negate<r, word> (x));
    }

    namespace {
        template <endian::order r, std::unsigned_integral word>
        const Z_bytes<r, negativity::BC, word> &true_value () {
            static Z_bytes<r, negativity::BC, word> TrueValue = Z_bytes<r, negativity::BC, word>::read ("0x01");
            return TrueValue;
        }

        template <endian::order r, std::unsigned_integral word>
        const Z_bytes<r, negativity::BC, word> &false_value () {
            static Z_bytes<r, negativity::BC, word> FalseValue = Z_bytes<r, negativity::BC, word>::read ("0x");
            return FalseValue;
        };
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator !
    (const Z_bytes<r, negativity::BC, word> &x) {
        return bool (x) ? false_value<r, word> () : true_value<r, word> ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator &&
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &y) {
        return bool (x) && bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator ||
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &y) {
        return bool (x) || bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ~ (const Z_bytes<r, negativity::twos, word> &x) {
        auto z = extend (x, x.size () + 1);
        arithmetic::bit_negate<word> (z.words ().end (), z.words ().begin (), z.words ().begin ());
        return trim (z);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_and<r, negativity::nones, word> (a, b));
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator & (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return arithmetic::trim<r, c, word> (arithmetic::bit_and<r, c, word> (a, b));
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> operator | (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_or<r, negativity::nones, word> (a, b));
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator | (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return arithmetic::trim<r, c, word> (arithmetic::bit_or<r, c, word> (a, b));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ^ (const Z_bytes<r, negativity::twos, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        if (a.size () < b.size ()) return b ^ a;
        auto bt = extend (b, a.size ());
        auto x = Z_bytes<r, negativity::twos, word>::zero (a.size ());
        arithmetic::bit_xor<word> (x.end (), x.begin (), a.begin (),
            const_cast<const Z_bytes<r, negativity::twos, word> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator +
    (const Z_bytes<r, negativity::twos, word> &a, const Z_bytes<r, negativity::twos, word> &b) {

        bool an = data::is_negative (a);
        bool bn = data::is_negative (b);
        auto ax = data::abs (a);
        auto bx = data::abs (b);

        if (!an && !bn) return Z_bytes<r, negativity::twos, word> (ax + bx);
        if (an && bn) return -(ax + bx);

        return ax > bx ?
            (an ? -(ax - bx) : Z_bytes<r, negativity::twos, word> (ax - bx)) :
            (an ? Z_bytes<r, negativity::twos, word> (bx - ax) : -(bx - ax));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator +
    (const Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b) {
        return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::plus<r, word> (trim (a), trim (b)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator *
    (const Z_bytes<r, negativity::twos, word> &a, const Z_bytes<r, negativity::twos, word> &b)
    {
        bool an = data::is_negative (a);
        bool bn = data::is_negative (b);
        if ((an && bn) || (!an && !bn)) return Z_bytes<r, negativity::twos, word> (data::abs (a) * data::abs (b));
        return -(data::abs (a) * data::abs (b));
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator * (const Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b)
    {
        return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::times<r, word> (trim (a), trim (b)));
    }
    
    template <endian::order r, std::unsigned_integral word> template<negativity c>
    N_bytes<r, word>::operator Z_bytes<r, c, word> () const {
        Z_bytes<r, c, word> n = Z_bytes<r, c, word>::zero (this->size () + 1);
        std::copy (this->words ().begin (), this->words ().end (), n.words ().begin ());
        return n.trim ();
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    N_bytes<r, word>::N_bytes (const uint64 x) : oriented<r, word> {} {
        if constexpr (sizeof (uint64) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (uint64) / sizeof (word));
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint64) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }

        this->trim ();
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::twos, word>::Z_bytes (int64 x) : oriented<r, word> {} {
        if constexpr (sizeof (int64) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (int64) / sizeof (word));
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (int64) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }

        this->trim ();
    }

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::twos, word>::Z_bytes (int32 x) : oriented<r, word> {} {
        if constexpr (sizeof (int32) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (int32) / sizeof (word));
            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (int32) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }

        this->trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>::Z_bytes (uint64 x) : oriented<r, word> {} {
        if constexpr (sizeof (uint64) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (uint64) / sizeof (word) + 1);

            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint64) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }

        this->trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>::Z_bytes (uint32 x) : oriented<r, word> {} {
        if constexpr (sizeof (uint32) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (uint32) / sizeof (word) + 1);

            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint32) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
        this->trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word>::Z_bytes (uint64 x) : oriented<r, word> {} {
        if constexpr (sizeof (uint64) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (uint64) / sizeof (word) + 1);

            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint64) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }

        this->trim ();
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word>::Z_bytes (uint32 x) : oriented<r, word> {} {
        if constexpr (sizeof (uint32) <= sizeof (word)) {
            this->resize (1);
            *this->begin () = x;
        } else {
            this->resize (sizeof (uint32) / sizeof (word) + 1);

            data::arithmetic::Words<boost::endian::order::native, word> n {
                slice<word> {(word*) (&x), sizeof (uint32) / sizeof (word)}};

            std::copy (n.begin (), n.end (), this->words ().begin ());
        }
        this->trim ();
    }

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::Z_bytes (bool b) : oriented<r, word> {} {
        if (b) {
            this->resize (1);
            (*this)[0] = 1;
        }
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline trim (const N_bytes<r, word> &n) {
        return arithmetic::trim<r, math::negativity::nones, word> (n);
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline trim (const Z_bytes<r, c, word> &z) {
        return arithmetic::trim<r, c, word> (z);
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &N_bytes<r, word>::trim () {
        arithmetic::trim<r, negativity::nones, word> (*this);
        return *this;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline &Z_bytes<r, negativity::twos, word>::trim () {
        arithmetic::trim<r, negativity::twos, word> (*this);
        return *this;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline &Z_bytes<r, negativity::BC, word>::trim () {
        arithmetic::trim<r, negativity::BC, word> (*this);
        return *this;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word>::operator uint64 () const {
        if (*this > N_bytes {std::numeric_limits<uint64>::max ()}) throw std::invalid_argument {"value too big"};

        endian_integral<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return uint64 (xx);
    } 
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>::operator int64 () const {
        if (*this > std::numeric_limits<int64>::max ()) throw std::invalid_argument {"value too big"};
        if (*this < std::numeric_limits<int64>::min ()) throw std::invalid_argument {"value too small"};

        endian_integral<true, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return int64 (xx);
    } 
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::operator int64 () const {
        return int64 (Z_bytes<r, negativity::twos, word> (*this));
    } 

    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::operator size_t () const {
        if (data::is_negative (*this)) throw std::invalid_argument {"negative value"};
        if (*this > std::numeric_limits<size_t>::max ()) throw std::invalid_argument {"value too big"};

        constexpr const size_t size = sizeof (std::declval<size_t> ());

        endian_integral<false, endian::little, size> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (size, this->size ()), xx.begin ());

        return size_t (xx);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>::operator Z_bytes<endian::opposite (r), negativity::twos, word> () const {
        Z_bytes<endian::opposite (r), negativity::twos, word> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    inline Z_bytes<r, negativity::twos, word>::operator Z_bytes<r, negativity::BC, word> () const {
        if (data::is_negative (*this)) return -Z_bytes<r, negativity::BC, word> (data::abs (*this));
        return Z_bytes<r, negativity::BC, word> (data::abs (*this)).trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word>::operator Z_bytes<endian::opposite (r), negativity::BC, word> () const {
        Z_bytes<endian::opposite (r), negativity::BC, word> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    inline Z_bytes<r, negativity::BC, word>::operator Z_bytes<r, negativity::twos, word> () const {
        if (data::is_zero (*this)) return Z_bytes<r, negativity::twos, word>::zero ();
        if (data::is_negative (*this)) return -Z_bytes<r, negativity::twos, word> (data::abs (*this));
        auto z = Z_bytes<r, negativity::twos, word>::zero (this->size ());
        std::copy (this->begin (), this->end (), z.begin ());
        return z.trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator ++ (N_bytes<r, word> &x) {
        arithmetic::nones::increment<r> (arithmetic::trim<r, negativity::nones> (x));
        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -- (N_bytes<r, word> &x) {
        arithmetic::trim<r, negativity::nones> (arithmetic::nones::decrement<r> (x));
        return x;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator ++ (Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::twos) arithmetic::trim<r, c> (arithmetic::twos::increment<r> (x));
        if constexpr (c == negativity::BC) arithmetic::trim<r, c> (arithmetic::BC::increment<r> (x));
        return x;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -- (Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::twos) arithmetic::trim<r, c> (arithmetic::twos::decrement<r> (x));
        if constexpr (c == negativity::BC) arithmetic::trim<r, c> (arithmetic::BC::decrement<r> (x));
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
        Z_bytes<r, negativity::twos, word> shift_left (const Z_bytes<r, negativity::twos, word> &x, uint32 extended, uint32 i) {
            Z_bytes<r, negativity::twos, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian::order r, std::unsigned_integral word>
        Z_bytes<r, negativity::twos, word> shift_right (const Z_bytes<r, negativity::twos, word> &x, uint32 i) {
            Z_bytes<r, negativity::twos, word> z = x;
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
    Z_bytes<r, negativity::twos, word> inline operator << (const Z_bytes<r, negativity::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator >> (const Z_bytes<r, negativity::twos, word> &x, int i) {
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
    N_bytes<r, word>::N_bytes (const math::N &n) : N_bytes {} {
        size_t n_size = mpz_size (n.Value.MPZ);

        if (n_size == 0) return;

        if constexpr (same_as<word, GMP::gmp_uint>) {
            this->resize (n_size);
            std::copy (n.Value.begin (), n.Value.end (), this->words.begin ());
        } else if (sizeof (word) < sizeof (GMP::gmp_uint)) {
            this->resize (n_size * (sizeof (GMP::gmp_uint) / sizeof (word)));
            auto x = this->words ().begin ();
            for (const GMP::gmp_uint &limb : n.Value) {
                GMP::gmp_uint z = limb;
                for (int i = 0; i < sizeof (GMP::gmp_uint) / sizeof (word); i++) {
                    *x = static_cast<word> (z & std::numeric_limits<word>::max ());
                    x++;
                    z >>= (sizeof (word) * 8);
                }
            }
        } else throw exception {} << "unhandled case in N -> N_bytes";

        this->trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>::Z_bytes (const Z &z) {
        auto x = Z_bytes<r, negativity::twos, word> (N_bytes<r, word> {data::abs (z)});
        *this = z < 0 ? -x : x;
        this->trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word>::Z_bytes (const Z &z) {
        *this = Z_bytes<r, negativity::BC, word> (N_bytes<r, word> {data::abs (z)});
        if (z < 0) *this = -*this;
        this->trim ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word>::operator math::N () const {
        math::N x {};

        for (const word &b : this->words ().reverse ()) {
            x <<= sizeof (word) * 8;
            x += b;
        }

        return x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>::operator Z () const {
        math::N x = math::N (data::abs (*this));
        return data::is_negative (*this) ? -x : Z (x);
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word>::operator Z () const {
        auto ab = data::abs (*this);
        math::N x {};

        for (const byte &b : ab.words ().reverse ()) {
            x <<= 8;
            x += b;
        }

        return data::is_negative (*this) ? -x : Z (x);
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        return data::divide<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Quotient;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &j) {
        return data::divide<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {j}).Quotient;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const N_bytes<r, word> &j) {
        return data::divide<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, uint64 j) {
        return data::divide<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Quotient;
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, int64 j) {
        return data::divide<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        return data::divide<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }

    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const Z_bytes<r, negativity::twos, word> &x, const N_bytes<r, word> &j) {
        return data::divide<Z_bytes<r, negativity::twos, word>, N_bytes<r, word>>
        (x, nonzero<Z_bytes<r, negativity::twos, word>> {Z_bytes<r, negativity::twos, word> {j}}).Remainder;
    }

    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator %
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &j) {
        return data::divide<Z_bytes<r, negativity::BC, word>> (x, nonzero<Z_bytes<r, negativity::BC, word>> {j}).Remainder;
    }

    template <endian::order r, std::unsigned_integral word>
    uint64 inline operator % (const N_bytes<r, word> &x, uint64 j) {
        return uint64 (data::divide<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Remainder);
    }

    template <endian::order r, negativity c, std::unsigned_integral word>
    uint64 inline operator % (const Z_bytes<r, c, word> &x, uint64 j) {
        return uint64 (data::divide<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Remainder);
    }
}

#endif
