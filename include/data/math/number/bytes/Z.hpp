// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/math/number/bytes/bytes.hpp>
#include <data/arithmetic/complementary.hpp>

#include <data/exception.hpp>

namespace data::math::number {
    
    // N_bytes is a representation of the natural numbers that
    // is also a string of bytes. It has an order parameter
    // representing endianness.
    template <endian r, std::unsigned_integral word> struct N_bytes : oriented<r, word> {

        // read in a standard number representation as a string.
        // decimal and hexidecimal are both accepted. In hexidecimal,
        // the number will be read to be identical with the hexidecimal
        // representation, including extra leading zeros.
        // for example, "0x00" and "0x" are both representations of zero
        // but different strings of bytes.
        static N_bytes read (string_view x);

        // read in the number as a string of bytes.
        static N_bytes read (slice<const word> x);

        // trim the number to its minimal representation.
        N_bytes &trim ();

        // get a representation of zero of any size.
        static N_bytes zero (size_t size = 0);
        
        N_bytes () : oriented<r, word> {} {}

        // construct from any number literal
        template <std::integral I> N_bytes (I);

        explicit N_bytes (string_view x): N_bytes {read (x)} {}
        explicit N_bytes (slice<const word> x): N_bytes {read (x)} {}

        explicit operator double () const;
        
        explicit operator uint64 () const;
        explicit operator int64 () const;

        // TODO we need this intstead of the above two operators.
        /*
        // cast to any built in type
        template <std::integral I>
        explicit operator I () const;*/

        explicit N_bytes (bytestring<word> &&b): oriented<r, word> {b} {}

    };
    
    template <endian r, std::unsigned_integral word>
    struct Z_bytes<r, negativity::twos, word> : oriented<r, word> {
        
        Z_bytes () : oriented<r, word> {} {}

        // construct from any number literal.
        template <std::integral I> Z_bytes (I);

        Z_bytes (const N_bytes<r, word> &n): Z_bytes {convert<Z_bytes<r, negativity::twos, word>> (n)} {}
        
        // string constructors.
        static Z_bytes read (string_view x);
        static Z_bytes read (slice<const word> x);

        explicit Z_bytes (string_view x) : Z_bytes {read (x)} {}

        explicit Z_bytes (slice<const word> x): Z_bytes {read (x)} {}

        // TODO get rid of this and replace with a constructor.
        operator Z_bytes<r, negativity::BC, word> () const;

        explicit operator N_bytes<r, word> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0);
        
        // cast to any built in type
        template <std::integral I>
        explicit operator I () const;
        
        Z_bytes &trim ();

        explicit Z_bytes (bytestring<word> &&b) : oriented<r, word> {b} {}

    };
    
    // for little endian, these are an implementation of bitcoin numbers.
    template <endian r, std::unsigned_integral word>
    struct Z_bytes<r, negativity::BC, word> : oriented<r, word> {

        //explicit Z_bytes (const std::string &);
        Z_bytes () : oriented<r, word> {} {}
        
        Z_bytes (bool);

        // construct from any number literal.
        template <std::integral I> Z_bytes (I);

        Z_bytes (const N_bytes<r, word> &n): Z_bytes {convert<Z_bytes<r, negativity::BC, word>> (n)} {}
        
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
        
        // cast to any built-in type.
        template <std::integral I>
        explicit operator I () const;

        explicit operator bool () const {
            return !data::is_zero (*this);
        }

        // TODO get rid of these and replace with a constructor.
        operator Z_bytes<encoding::endian::opposite (r), negativity::BC, word> () const;
        operator Z_bytes<r, negativity::twos, word> () const;

        Z_bytes &trim ();

        Z_bytes (bytestring<word> &&b): oriented<r, word> {b} {}
    };

    // increment and decrement
    template <endian r, std::unsigned_integral word>
    nonzero<N_bytes<r, word>> inline increment<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        nonzero<N_bytes<r, word>> v {n};
        ++v.Value;
        return v;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline decrement<N_bytes<r, word>>::operator () (const nonzero<N_bytes<r, word>> &n) {
        auto x = n.Value;
        return --x;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline decrement<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        if (data::is_zero (n)) return n;
        auto x = n;
        return --x;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline increment<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return ++x;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline decrement<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return --x;
    }
    
    // some functions that can easily be implemented as other functions using conversions. 

    template <endian r, negativity c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a <=> Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) <=> b;
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ~ (const N_bytes<r, word> &b) {
        return ~Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a ^ Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) ^ b;
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator & (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a & Z_bytes<r, negativity::twos, word> (b);
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator & (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) & b;
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator | (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a | Z_bytes<r, negativity::twos, word> (b);
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator | (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) | b;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a + Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a - Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a + (-b);
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator * (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a * Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator + (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) + b;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator - (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) - b;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator * (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) * b;
    }

    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> inline operator + (const N_bytes<r, word> &u, I x) {
        return u + N_bytes<r, word> (x);
    }

    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> inline operator - (const N_bytes<r, word> &u, I x) {
        return u - N_bytes<r, word> (x);
    }

    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> inline operator * (const N_bytes<r, word> &u, I x) {
        return u * N_bytes<r, word> (x);
    }

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator + (I x, const N_bytes<r, word> &u) {
        return u + N_bytes<r, word> (x);
    }

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator - (I x, const N_bytes<r, word> &u) {
        return N_bytes<r, word> (x) - u;
    }

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator * (I x, const N_bytes<r, word> &u) {
        return u * N_bytes<r, word> (x);
    }

    template <endian r, negativity neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> inline operator + (const Z_bytes<r, neg, word> &z, I x) {
        return z + Z_bytes<r, neg, word> {x};
    }

    template <endian r, negativity neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> inline operator - (const Z_bytes<r, neg, word> &z, I x) {
        return z - Z_bytes<r, neg, word> {x};
    }

    template <endian r, negativity neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> inline operator * (const Z_bytes<r, neg, word> &z, I x) {
        return z * Z_bytes<r, neg, word> {x};
    }

    template <std::integral I, endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> inline operator + (I x, const Z_bytes<r, neg, word> &z) {
        return Z_bytes<r, neg, word> {x} + z;
    }

    template <std::integral I, endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> inline operator - (I x, const Z_bytes<r, neg, word> &z) {
        return Z_bytes<r, neg, word> {x} - z;
    }

    template <std::integral I, endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> inline operator * (I x, const Z_bytes<r, neg, word> &z) {
        return Z_bytes<r, neg, word> {x} * z;
    }

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator + (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {u} + Z_bytes<r, negativity::twos, word> {x};
    }

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator - (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {u} - Z_bytes<r, negativity::twos, word> {x};
    }

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator * (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {u} * Z_bytes<r, negativity::twos, word> {x};
    }

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator + (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} + Z_bytes<r, negativity::twos, word> {u};
    }

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator - (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} - Z_bytes<r, negativity::twos, word> {u};
    }

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator * (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} * Z_bytes<r, negativity::twos, word> {u};
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator ++ (N_bytes<r, word> &x, int) {
        N_bytes<r, word> z = x;
        ++x;
        return z;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator -- (N_bytes<r, word> &x, int) {
        N_bytes<r, word> z = x;
        --x;
        return z;
    }
    
    // TODO these next several operations could be more optimized.
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator += (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x + n;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -= (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x - n;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator += (N_bytes<r, word> &x, uint64 n) {
        return x = x + n;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -= (N_bytes<r, word> &x, uint64 n) {
        return x = x - n;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator *= (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x * n;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator *= (N_bytes<r, word> &x, uint64 n) {
        return x = x * n;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator <<= (N_bytes<r, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator >>= (N_bytes<r, word> &x, int64 i) {
        return x = x >> i;
    }
    
    // post increment
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator ++ (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        ++x;
        return z;
    }
    
    // post increment
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator -- (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        --x;
        return z;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x + n;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x - n;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, int64 n) {
        return x = x + n;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, int64 n) {
        return x = x - n;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x * n;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const int64 n) {
        return x = x * n;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator /= (Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a = a / b;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator <<= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator >>= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x >> i;
    }
    
    // check if the number is negative, and then do bit shift on the absolute value.
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator << (const Z_bytes<r, negativity::BC, word> &x, int n) {
        if (n < 0) return x >> -n;
        if (n == 0) return x;
        if (is_zero (x)) return Z_bytes<r, negativity::BC, word> {};

        bool neg = is_negative (x);
        Z_bytes<r, negativity::BC, word> result = neg ? -x : x;
        result = extend (result, result.size () + ((n + 7) / 8));
        result.words ().bit_shift_left (static_cast<uint32> (n));
        return neg ? -result : trim (result);
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator >> (const Z_bytes<r, negativity::BC, word> &x, int n) {
        if (n < 0) return x << -n;
        if (n == 0) return x;
        if (is_zero (x)) return Z_bytes<r, negativity::BC, word> {};

        bool neg = is_negative (x);
        Z_bytes<r, negativity::BC, word> result = neg ? -x : x;
        result.words ().bit_shift_right (static_cast<uint32> (n));
        return neg ? -result : trim (result);
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator - (const Z_bytes<r, negativity::twos, word> &x) {
        auto z = ~x;
        return ++z;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator %= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    N_bytes<r, word> inline &operator %= (Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline &operator %=
    (Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b) {
        return a = a % b;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    uint64 &operator %= (Z_bytes<r, c, word> &a, uint64 b) {
        return a = a % b;
    }

    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &u, I x) {
        return u & N_bytes<r, word> {x};
    }

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator & (I x, const N_bytes<r, word> &u) {
        return u & N_bytes<r, word> {x};
    }

    template <endian r, std::unsigned_integral word, std::integral I>
    Z_bytes<r, negativity::twos, word> inline operator & (const Z_bytes<r, negativity::twos, word> &u, I x) {
        return u & Z_bytes<r, negativity::twos, word> {x};
    }

    template <std::integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator & (I x, const Z_bytes<r, negativity::twos, word> &u) {
        return u & Z_bytes<r, negativity::twos, word> {x};
    }

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator & (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {x} & Z_bytes<r, negativity::twos, word> {u};}

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator & (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} & Z_bytes<r, negativity::twos, word> {u};
    }
    
}

namespace data {

    template <endian a, endian b, std::unsigned_integral word>
    bool inline identical (const math::number::N_bytes<a, word> &x, const math::number::N_bytes<b, word> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }

    template <endian a, negativity b, endian c, negativity d, std::unsigned_integral word>
    bool inline identical (const math::number::Z_bytes<a, b, word> &x, const math::number::Z_bytes<c, d, word> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }
    
}

namespace data::math::def {

    template <endian r, std::unsigned_integral word>
    math::sign inline sign<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return arithmetic::nones::sign (x.words ());
    }

    template <endian r, negativity c, std::unsigned_integral word>
    math::sign inline sign<Zb<r, c, word>>::operator () (const Zb<r, c, word> &x) {
        if constexpr (c == negativity::twos) return arithmetic::twos::sign (x.words ());
        if constexpr (c == negativity::BC) return arithmetic::BC::sign (x.words ());
    }

    template <endian r, std::unsigned_integral word>
    bool inline is_negative<N_bytes<r, word>>::operator () (const N_bytes<r, word> &) {
        return false;
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_positive<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return !data::is_zero (x);
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline abs<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return x;
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes_BC<r, word> inline abs<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_negative (x) ? -x : x;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> abs<Z_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &x) {
        if (data::is_zero (x)) return N_bytes<r, word> {};
        Zb<r, negativity::twos, word> z = data::is_negative (x) ? -x : x;
        auto n = N_bytes<r, word>::zero (z.size ());
        std::copy (z.begin (), z.end (), n.begin ());
        return n;
    }

    template <endian r, negativity zz, std::unsigned_integral word>
    Zb<r, zz, word> inline times<Zb<r, zz, word>>::operator ()
    (const Zb<r, zz, word> &a, const Zb<r, zz, word> &b) {
        return a * b;
    }

    template <endian r, negativity zz, std::unsigned_integral word>
    nonzero<Zb<r, zz, word>> inline times<Zb<r, zz, word>>::operator ()
        (const nonzero<Zb<r, zz, word>> &a, const nonzero<Zb<r, zz, word>> &b) {
        return a * b;
    }
    
    template <endian r, std::unsigned_integral word>
    Zb<r, negativity::BC, word> inline
    next (const Zb<r, negativity::BC, word> &z) {
        return is_positive (z) ? -z : -z - 1;
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_negative<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return arithmetic::sign_bit (x.words ());
    }

    template <endian r, std::unsigned_integral word>
    bool inline is_negative<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return !data::is_zero (x) && arithmetic::sign_bit (x.words ());
    }

    template <endian r, std::unsigned_integral word>
    bool inline is_positive<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return !data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }

    template <endian r, std::unsigned_integral word>
    bool inline is_positive<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return !data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_zero<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return arithmetic::is_zero (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_zero<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return arithmetic::is_zero (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return arithmetic::BC::is_zero (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_positive_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_negative_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_zero (x) && arithmetic::sign_bit (x.words ());
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline bit_xor<N_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a ^ b;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_not<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        return ~n;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_xor<math::Z_bytes<r, word>>::operator ()
        (const math::Z_bytes<r, word> &a, const math::Z_bytes<r, word> &b) {
        return a ^ b;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_xor<N_bytes<r, word>, math::Z_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const math::Z_bytes<r, word> &b) {
        return math::Z_bytes<r, word> {a} ^ b;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline bit_xor<math::Z_bytes<r, word>, N_bytes<r, word>>::operator ()
    (const math::Z_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a ^ math::Z_bytes<r, word> {b};
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_not<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x) {
        auto a = x;
        arithmetic::bit_negate<word> (a.begin (), a.end (), x.begin ());
        return a;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline mul_2_pow<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &a, uint32 u) {
        return a << u;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_xor<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_xor (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_xor<word> (bb.begin (), bb.end (), bb.begin (), a.begin ());
        return bb;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_or<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_or (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_or<word> (bb.begin (), bb.end (), bb.begin (), a.begin ());
        return bb;
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline bit_and<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_and (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_and<word> (bb.begin (), bb.end (), bb.begin (), a.begin ());
        return bb;
    }

    template <endian r, std::unsigned_integral word>
    math::N_bytes<r, word> inline div_2_pow<math::N_bytes<r, word>>::operator () (const math::N_bytes<r, word> &x, uint32 exp) {
        return bit_div_2_pow_unsigned_and_BC (x, exp);
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline div_2_pow<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x, uint32 exp) {
        return bit_div_2_pow_unsigned_and_BC (x, exp);
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline div_2_pow<math::Z_bytes<r, word>>::operator () (const math::Z_bytes<r, word> &x, uint32 exp) {
        return bit_div_2_pow_twos (x, exp);
    }

    template <endian r, std::unsigned_integral word>
    math::N_bytes<r, word> inline mod_2<math::N_bytes<r, word>>::operator () (const math::N_bytes<r, word> &x) {
        return bit_mod_2_positive_mod (x);
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes_BC<r, word> inline mod_2<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x) {
        if (data::is_zero (x)) return 0;
        word z = *x.words ().begin () & 1;
        return data::is_negative (x) ?
            math::Z_bytes_BC<r, word> {-static_cast<std::make_signed_t<word>> (z)}:
            math::Z_bytes_BC<r, word> {z};
    }

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> inline mod_2<math::Z_bytes<r, word>>::operator () (const math::Z_bytes<r, word> &x) {
        return bit_mod_2_positive_mod (x);
    }

    template <endian r, std::unsigned_integral word>
    struct bit_shift_right<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x, uint32 i) const {
            return x >> i;
        }
    };

    template <endian r, std::unsigned_integral word>
    struct bit_shift_left<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x, uint32 i) const {
            return x << i;
        }
    };

    // convert between any two types of N_bytes
    template <endian ToEndian, std::unsigned_integral ToWord,
        endian FromEndian, std::unsigned_integral FromWord>
    number::N_bytes<ToEndian, ToWord>
    convert<number::N_bytes<ToEndian, ToWord>, number::N_bytes<FromEndian, FromWord>>::operator ()
    (const number::N_bytes<FromEndian, FromWord> &from) const {
        number::N_bytes<ToEndian, ToWord> result;

        constexpr std::size_t from_bits = sizeof (FromWord) * CHAR_BIT;
        constexpr std::size_t to_bits   = sizeof (ToWord) * CHAR_BIT;

        const auto trimmed = trim (from);
        const auto source = trimmed.words ();

        if constexpr (to_bits == from_bits) {
            result.resize (source.size ());

            auto dst = result.words ().begin();

            for (const auto word : source)
                *dst++ = static_cast<ToWord> (word);
        } else if constexpr (to_bits > from_bits) {
            constexpr std::size_t words_per_destination = to_bits / from_bits;

            result.resize (
                (source.size () + words_per_destination - 1)
                    / words_per_destination
            );

            auto dst = result.words ().begin ();

            ToWord value = 0;
            std::size_t shift = 0;

            for (const auto word : source) {
                value |= static_cast<ToWord> (word) << shift;
                shift += from_bits;

                if (shift == to_bits) {
                    *dst++ = value;
                    value = 0;
                    shift = 0;
                }
            }

            if (shift != 0)
                *dst = value;
        } else {
            constexpr std::size_t words_per_source = from_bits / to_bits;

            result.resize (source.size () * words_per_source);

            auto dst = result.words ().begin ();

            for (const auto word : source) {
                FromWord value = word;

                for (std::size_t i = 0; i < words_per_source; ++i) {
                    *dst++ = static_cast<ToWord> (value);
                    value >>= to_bits;
                }
            }
        }

        return trim (result);
    }

    // convert between any two types of Z_bytes
    template <endian ToEndian, negativity ToNeg, std::unsigned_integral ToWord,
        endian FromEndian, negativity FromNeg, std::unsigned_integral FromWord>
    number::Z_bytes<ToEndian, ToNeg, ToWord>
    convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>>::operator ()
        (const number::Z_bytes<FromEndian, FromNeg, FromWord> &from) const {

        if constexpr (ToNeg != FromNeg)
            return math::convert<number::Z_bytes<ToEndian, ToNeg, ToWord>> (from.operator number::Z_bytes<FromEndian, ToNeg, FromWord> ());

        number::Z_bytes<ToEndian, ToNeg, ToWord> result;

        constexpr std::size_t from_bits = sizeof (FromWord) * CHAR_BIT;
        constexpr std::size_t to_bits   = sizeof (ToWord) * CHAR_BIT;

        const auto trimmed = trim (from);
        const auto source = trimmed.words ();

        if constexpr (to_bits == from_bits) {
            result.resize (source.size ());

            auto dst = result.words ().begin();

            for (const auto word : source)
                *dst++ = static_cast<ToWord> (word);
        } else if constexpr (to_bits > from_bits) {
            constexpr std::size_t words_per_destination = to_bits / from_bits;

            result.resize (
                (source.size () + words_per_destination - 1)
                    / words_per_destination
            );

            auto dst = result.words ().begin ();

            ToWord value = 0;
            std::size_t shift = 0;

            for (const auto word : source) {
                value |= static_cast<ToWord> (word) << shift;
                shift += from_bits;

                if (shift == to_bits) {
                    *dst++ = value;
                    value = 0;
                    shift = 0;
                }
            }

            if (shift != 0)
                *dst = value;
        } else {
            constexpr std::size_t words_per_source = from_bits / to_bits;

            result.resize (source.size () * words_per_source);

            auto dst = result.words ().begin ();

            for (const auto word : source) {
                FromWord value = word;

                for (std::size_t i = 0; i < words_per_source; ++i) {
                    *dst++ = static_cast<ToWord> (value);
                    value >>= to_bits;
                }
            }
        }

        return trim (result);
    }

    // convert from N_bytes to Z_bytes
    template <endian ToEndian, negativity ToNeg, std::unsigned_integral ToWord,
        endian FromEndian, std::unsigned_integral FromWord>
    number::Z_bytes<ToEndian, ToNeg, ToWord> inline
    convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::N_bytes<FromEndian, FromWord>>::operator ()
    (const number::N_bytes<FromEndian, FromWord> &from) const {

        number::Z_bytes<ToEndian, ToNeg, ToWord> result;
        number::N_bytes<ToEndian, ToWord> input;

        constexpr std::size_t from_bits = sizeof (FromWord) * CHAR_BIT;
        constexpr std::size_t to_bits   = sizeof (ToWord) * CHAR_BIT;

        if constexpr (to_bits == from_bits) {
            input = trim (from);
        } else {
            input = math::convert<number::N_bytes<ToEndian, ToWord>> (from);
        }

        result = number::Z_bytes<ToEndian, ToNeg, ToWord>::zero (input.size () + 1);

        std::copy (input.words ().begin (), input.words ().end (), result.words ().begin ());
        return number::trim (result);
    }
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian r, std::unsigned_integral word>
    bool inline is_minimal (const N_bytes<r, word> &x) {
        return arithmetic::nones::is_minimal (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_minimal (const Z_bytes<r, negativity::twos, word> &x) {
        return arithmetic::twos::is_minimal (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline is_minimal (const Z_bytes<r, negativity::BC, word> &x) {
        return arithmetic::BC::is_minimal (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    size_t inline minimal_size (const N_bytes<r, word> &x) {
        return arithmetic::minimal_size<negativity::nones> (x.words ());
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    size_t inline minimal_size (const Z_bytes<r, c, word> &x) {
        return arithmetic::minimal_size<c> (x.words ());
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline extend (const N_bytes<r, word> &x, size_t size) {
        return N_bytes<r, word> (std::move (arithmetic::extend<r, negativity::nones, word> (x, size)));
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline extend (const Z_bytes<r, c, word> &x, size_t size) {
        return Z_bytes<r, c, word> (std::move (arithmetic::extend<r, c, word> (x, size)));
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline operator == (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return arithmetic::nones::compare<r, word> (a, b);
    }
    
    template <endian r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, negativity::twos, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return arithmetic::twos::compare<r, word> (a, b);
    }
    
    template <endian r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b) {
        return arithmetic::BC::compare<r, word> (a, b);
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, c, word> &a, int64 i) {
        return a <=> Z_bytes<r, c, word> {i};
    }
    
    template <endian r, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const N_bytes<r, word> &a, uint64 i) {
        return a <=> N_bytes<r, word> {i};
    }
    
    template <endian r, std::unsigned_integral word>
    bool inline operator == (const N_bytes<r, word> &x, uint64 i) {
        return x == N_bytes<r, word> {i};
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, c, word> &x, int64 i) {
        return (x <=> Z_bytes<r, c, word> {i}) == 0;
    }

    template <endian r, negativity cl, negativity cr, std::unsigned_integral word>
    bool inline operator == (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cr, word> &b) {
        return a == Z_bytes<r, cl, word> (b);
    }

    template <endian r, negativity cl, negativity cr, std::unsigned_integral word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cl, word> &b) {
        return a <=> Z_bytes<r, cl, word> (b);
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator - (const N_bytes<r, word> &x) {
        auto z = Z_bytes<r, negativity::twos, word> (x);
        return -z;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> N_bytes<r, word>::read (slice<const word> b) {
        auto x = N_bytes<r, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word>
    Z_bytes<r, negativity::twos, word>::read (slice<const word> b) {
        auto x = Z_bytes<r, negativity::twos, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> Z_bytes<r, negativity::BC, word>::read (slice<const word> b) {
        auto x = Z_bytes<r, negativity::BC, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline N_bytes<r, word>::zero (size_t size) {
        return N_bytes<r, word> (std::move (arithmetic::zero<r, word> (size)));
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline Z_bytes<r, negativity::twos, word>::zero (size_t size) {
        return Z_bytes<r, negativity::twos, word> (std::move (arithmetic::zero<r, word> (size)));
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline Z_bytes<r, negativity::BC, word>::zero (size_t size, bool negative) {
        return arithmetic::BC::zero<r, word> (size, negative);
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator - (const Z_bytes<r, negativity::BC, word> &x) {
        return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::negate<r, word> (x));
    }

    namespace {
        template <endian r, std::unsigned_integral word>
        const Z_bytes<r, negativity::BC, word> &true_value () {
            static Z_bytes<r, negativity::BC, word> TrueValue = Z_bytes<r, negativity::BC, word>::read ("0x01");
            return TrueValue;
        }

        template <endian r, std::unsigned_integral word>
        const Z_bytes<r, negativity::BC, word> &false_value () {
            static Z_bytes<r, negativity::BC, word> FalseValue = Z_bytes<r, negativity::BC, word>::read ("0x");
            return FalseValue;
        };
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator !
    (const Z_bytes<r, negativity::BC, word> &x) {
        return bool (x) ? false_value<r, word> () : true_value<r, word> ();
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator &&
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &y) {
        return bool (x) && bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator ||
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &y) {
        return bool (x) || bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator ~ (const Z_bytes<r, negativity::twos, word> &x) {
        auto z = trim (x);
        if (is_zero (z)) z = extend (z, 1);
        arithmetic::bit_negate<word> (z.begin (), z.end (), z.begin ());
        return z;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        N_bytes<r, word> result {std::move (arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_and<r, negativity::nones, word> (a, b)))};
        result.trim ();
        return result;
    }

    template <endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator & (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if constexpr (neg == negativity::twos)
            return Z_bytes<r, neg, word> {std::move (arithmetic::trim<r, neg, word> (arithmetic::bit_and<r, neg, word> (a, b)))};
        else return Z_bytes<r, neg, word> {std::move (arithmetic::bit_and<r, neg, word> (a, b))};
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_and<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        a.trim ();
        return a;
    }

    template <endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> &operator &= (Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_and<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        if constexpr (neg == negativity::twos) a.trim ();
        return a;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator | (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        N_bytes<r, word> result {std::move (arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_or<r, negativity::nones, word> (a, b)))};
        result.trim ();
        return result;
    }

    template <endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> inline operator | (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if constexpr (neg == negativity::twos)
            return Z_bytes<r, neg, word> {std::move (arithmetic::trim<r, neg, word> (arithmetic::bit_or<r, neg, word> (a, b)))};
        else return Z_bytes<r, neg, word> {std::move (arithmetic::bit_or<r, neg, word> (a, b))};
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_or<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        a.trim ();
        return a;
    }

    template <endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> &operator |= (Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_or<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        if constexpr (neg == negativity::twos) a.trim ();
        return a;
    }

    // bit xor
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator ^ (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        N_bytes<r, word> result {std::move (arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_xor<r, negativity::nones, word> (a, b)))};
        result.trim ();
        return result;
    }

    template <endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator ^ (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if constexpr (neg == negativity::twos)
            return Z_bytes<r, neg, word> {std::move (arithmetic::trim<r, neg, word> (arithmetic::bit_xor<r, neg, word> (a, b)))};
        else return Z_bytes<r, neg, word> {std::move (arithmetic::bit_xor<r, neg, word> (a, b))};
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_xor<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        a.trim ();
        return a;
    }

    template <endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> &operator ^= (Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_xor<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        if constexpr (neg == negativity::twos) a.trim ();
        return a;
    }

    // basic arithmetic
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {

        if constexpr (c == negativity::twos) {
            bool an = data::is_negative (a);
            bool bn = data::is_negative (b);
            auto ax = data::abs (a);
            auto bx = data::abs (b);

            if (!an && !bn) return Z_bytes<r, negativity::twos, word> (ax + bx);
            if (an && bn) return -(ax + bx);

            return ax > bx ?
                (an ? -(ax - bx) : Z_bytes<r, negativity::twos, word> (ax - bx)) :
                (an ? Z_bytes<r, negativity::twos, word> (bx - ax) : -(bx - ax));
        } else if constexpr (c == negativity::BC) {
            return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::plus<r, word> (trim (a), trim (b)));
        } else throw exception {} << "invalid neg " << c;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        if constexpr (c == negativity::twos) {
            bool an = data::is_negative (a);
            bool bn = data::is_negative (b);
            if ((an && bn) || (!an && !bn)) return Z_bytes<r, negativity::twos, word> (data::abs (a) * data::abs (b));
            return -(data::abs (a) * data::abs (b));
        } else if constexpr (c == negativity::BC) {
            return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::times<r, word> (trim (a), trim (b)));
        } else throw exception {} << "invalid neg " << c;
    }

    // set a number to the value of a built-in type using a minimal representation.
    #pragma GCC push_options
    #pragma GCC optimize("O1")
    // we need these directives here for this particular function on gcc or else the tests fail.
    template <std::integral Int, negativity Negativity, typename ZZ, std::unsigned_integral Word>
    void initialize_bytes (Int input, ZZ &location) {
        if constexpr (Negativity == negativity::nones && std::signed_integral<Int>)
            if (input < 0) throw out_of_range {} << "write negative value to unsigned type " << input;

        if (input == 0) return;

        using X = std::make_signed_t<Word>;
        using UInt = std::make_unsigned_t<Int>;

        // sign bit is always zero other than when n == BC and i is negative.
        Word sign_bit;

        // val contains information that can be copied directly into the result.
        // for n == BC, we need to take the absolute value before we can copy.
        // for other cases, x is the same as i.
        Int val;

        // set the correct values for val and sign_bit.
        if constexpr (Negativity == negativity::BC && std::signed_integral<Int>) {
            if (input > 0) {
                sign_bit = 0;
                val = input;
            } else {
                sign_bit = static_cast<Word> (numeric_limits<X>::min ());
                val = input == static_cast<Int> (numeric_limits<std::make_signed_t<Int>>::min ()) ? input : -input;
            }
        } else {
            sign_bit = 0;
            val = input;
        };

        if constexpr (sizeof (Int) < sizeof (Word)) {
            location.resize (1);

            if constexpr (Negativity == negativity::BC)
                // cast to unsigned and then to word.
                *location.words ().begin () = sign_bit | static_cast<Word> (static_cast<UInt> (val));
            else if constexpr (std::signed_integral<Int>)
                // cast to signed then to word
                *location.words ().begin () = static_cast<Word> (static_cast<X> (val));
            else *location.words ().begin () = static_cast<Word> (val);

        } else if constexpr (sizeof (Int) == sizeof (Word)) {
            // the size of the number to initialize.
            size_t number_size;

            // we need to resize to 2 in the following cases:
            //   * n == BC and I is signed and equal to min value.
            //   * n is signed and I is unsigned and bigger than the max value if it were signed.
            if constexpr (Negativity != negativity::nones && std::unsigned_integral<Int>)
                number_size = val > static_cast<Int> (numeric_limits<std::make_signed_t<Int>>::max ()) ? 2 : 1;
            else if constexpr (Negativity == negativity::BC)
                number_size = val == static_cast<Int> (numeric_limits<std::make_signed_t<Int>>::min ()) ? 2 : 1;
            else number_size = 1;

            location.resize (number_size);
            *location.words ().begin () = static_cast<Word> (val);

            if constexpr (Negativity == negativity::BC) *(location.words ().end () - 1) |= sign_bit;

        } else if constexpr (sizeof (Int) % sizeof (Word) != 0) {
            // should not really happen.
            throw exception {} <<
                "We do not know how to handle this case; init value size is " <<
                sizeof (Int) << " and word size is " << sizeof (Word);
        } else if constexpr (Same<Word, byte>) {

            const auto bits_per_word = sizeof (Word) * CHAR_BIT;

            // bit width that val takes up in the result, not including sign.
            size_t width; {

                // For sign-and-magnitude, mb is the magnitude of val.
                // For two's-complement, this is the bitwise complement of a negative val.
                // In the latter case, its bit width determines whether an additional word
                // is required for sign extension.
                UInt mb; {
                    if constexpr (Negativity == negativity::twos && std::signed_integral<Int>)
                        mb = static_cast<UInt> (val < 0 ? ~val : val);
                    else mb = static_cast<UInt> (val);
                }

                width = std::bit_width (mb);
            }

            // words_to_copy is the number of words needed to represent the
            // significant bits of the input in the representation we are using.
            // built-in division discards remainders, but we need a division
            // that rounds up if there is a remainder.
            const auto words_to_copy = (width + bits_per_word - 1) / bits_per_word;

            // the size of the number to initialize. For signed representations,
            // an additional word may be required to represent the sign without
            // changing the value. In nones (unsigned) representation, no sign
            // word is needed. In the signed representations (ones, twos, and BC),
            // an additional word is needed when the significant bits exactly
            // fill a word, leaving no room for the sign bit.
            size_t number_size; {
                if constexpr (Negativity == negativity::nones) number_size = words_to_copy;
                else number_size = width / bits_per_word + 1;
            }

            location.resize (number_size);

            // write val in little endian order for copying into location.
            endian_integral<std::signed_integral<Int>, endian::little, sizeof (val)> e {val};
            std::copy (e.begin (), e.begin () + words_to_copy, location.words ().begin ());

            // For two's-complement, a negative value requires an additional
            // sign-extension word only when number_size exceeds words_to_copy.
            if constexpr (Negativity == negativity::twos) {
                if (val < 0 && number_size > words_to_copy)
                    *(location.words ().end () - 1) = static_cast<Word> (X {-1});
            // set sign bit
            } else if constexpr (Negativity == negativity::BC)
                *(location.words ().end () - 1) |= sign_bit;

        // the general case, not perfectly efficient but good enough.
        } else {

            // this case could be more efficient and not use trim
            // just like the above case but we don't bother because
            // this case just isn't that important.
            if constexpr (Negativity == negativity::BC) {
                location.resize (sizeof (Int) / sizeof (Word) + 1);

                *(location.words ().end () - 1) = sign_bit;
            } else {
                location.resize (sizeof (Int) / sizeof (Word));
            }

            data::arithmetic::Words<endian::native, Word> e {
                slice<Word> {(Word*) (&val), sizeof (Int) / sizeof (Word)}};
            std::copy (e.begin (), e.end (), location.words ().begin ());

            location.trim ();
        }
    }

    template <endian r, std::unsigned_integral word>
    template <std::integral I> inline
    N_bytes<r, word>::N_bytes (I x) {
        initialize_bytes<I, negativity::nones, N_bytes<r, word>, word> (x, *this);
    }

    template <endian r, std::unsigned_integral word>
    template <std::integral I> inline
    Z_bytes<r, negativity::twos, word>::Z_bytes (I x) {
        initialize_bytes<I, negativity::twos, Z_bytes<r, negativity::twos, word>, word> (x, *this);
    }

    template <endian r, std::unsigned_integral word>
    template <std::integral I> inline
    Z_bytes<r, negativity::BC, word>::Z_bytes (I x) {
        initialize_bytes<I, negativity::BC, Z_bytes<r, negativity::BC, word>, word> (x, *this);
    }

    template <endian r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::Z_bytes (bool b): Z_bytes<r, negativity::BC, word> {} {
        if (b) this->push_back (1);
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline trim (const N_bytes<r, word> &n) {
        return N_bytes<r, word> (std::move (arithmetic::trim<r, negativity::nones, word> (n)));
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline trim (const Z_bytes<r, c, word> &z) {
        return Z_bytes<r, c, word> (std::move (arithmetic::trim<r, c, word> (z)));
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &N_bytes<r, word>::trim () {
        arithmetic::trim<r, negativity::nones, word> (*this);
        return *this;
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline &Z_bytes<r, negativity::twos, word>::trim () {
        arithmetic::trim<r, negativity::twos, word> (*this);
        return *this;
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline &Z_bytes<r, negativity::BC, word>::trim () {
        arithmetic::trim<r, negativity::BC, word> (*this);
        return *this;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word>::operator uint64 () const {
        if (*this > N_bytes {std::numeric_limits<uint64>::max ()})
            throw exception {} << "value too big";

        endian_integral<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return uint64 (xx);
    } 
    
    template <endian r, std::unsigned_integral word>
    template <std::integral I>
    Z_bytes<r, negativity::twos, word>::operator I () const {

        if (*this > std::numeric_limits<I>::max ())
            throw std::invalid_argument {"value too big"};

        if (*this < std::numeric_limits<I>::min ())
            throw std::invalid_argument {"value too small"};

        if (this->size () == 0) return 0;

        if constexpr (Same<word, byte>) {
            endian_integral<std::signed_integral<I>, endian::little, sizeof (I)> xx {0};

            std::copy (this->words ().begin (),
                this->words ().begin () +
                    std::min (static_cast<size_t> (sizeof (I)),
                        this->size ()),
                xx.begin ());

            return I (xx);
        } else if constexpr (Same<I, word>) {
            return this->words () [0];
        } else throw unimplemented {"Z_bytes to signed integral"};
    } 
    
    template <endian r, std::unsigned_integral word>
    template <std::integral I>
    inline Z_bytes<r, negativity::BC, word>::operator I () const {

        if (*this > std::numeric_limits<I>::max ())
            throw std::invalid_argument {"value too big"};

        if (*this < std::numeric_limits<I>::min ())
            throw std::invalid_argument {"value too small"};

        if (this->size () == 0) return 0;

        bool neg = is_negative (*this);

        Z_bytes neg_if;
        if (neg) neg_if = -*this;

        const Z_bytes &mag = neg ? neg_if : *this;

        I result;

        if constexpr (Same<word, byte>) {
            endian_integral<std::signed_integral<I>, endian::little, sizeof (I)> xx {0};

            std::copy (this->words ().begin (),
                this->words ().begin () +
                    std::min (static_cast<size_t> (sizeof (I)),
                        this->size ()),
                xx.begin ());

            result = I (xx);
        } else if constexpr (Same<I, word>) {
            result = mag->words () [0];
        } else throw unimplemented {"Z_bytes to signed integral"};

        return neg ? -result : result;
    } 
    
    template <endian r, std::unsigned_integral word>
    inline Z_bytes<r, negativity::twos, word>::operator Z_bytes<r, negativity::BC, word> () const {
        if (data::is_negative (*this)) return -Z_bytes<r, negativity::BC, word> (data::abs (*this));
        return Z_bytes<r, negativity::BC, word> (data::abs (*this)).trim ();
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word>::operator Z_bytes<encoding::endian::opposite (r), negativity::BC, word> () const {
        Z_bytes<encoding::endian::opposite (r), negativity::BC, word> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian r, std::unsigned_integral word>
    inline Z_bytes<r, negativity::BC, word>::operator Z_bytes<r, negativity::twos, word> () const {
        if (data::is_zero (*this)) return Z_bytes<r, negativity::twos, word>::zero ();
        if (data::is_negative (*this)) return -Z_bytes<r, negativity::twos, word> (data::abs (*this));
        auto z = Z_bytes<r, negativity::twos, word>::zero (this->size ());
        std::copy (this->begin (), this->end (), z.begin ());
        return z.trim ();
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator ++ (N_bytes<r, word> &x) {
        arithmetic::nones::increment<r> (arithmetic::trim<r, negativity::nones> (x));
        return x;
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline &operator -- (N_bytes<r, word> &x) {
        arithmetic::trim<r, negativity::nones> (arithmetic::nones::decrement<r> (x));
        return x;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator ++ (Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::twos) arithmetic::trim<r, c> (arithmetic::twos::increment<r> (x));
        if constexpr (c == negativity::BC) arithmetic::trim<r, c> (arithmetic::BC::increment<r> (x));
        return x;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline &operator -- (Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::twos) arithmetic::trim<r, c> (arithmetic::twos::decrement<r> (x));
        if constexpr (c == negativity::BC) arithmetic::trim<r, c> (arithmetic::BC::decrement<r> (x));
        return x;
    }
    
    namespace {
    
        template <endian r, std::unsigned_integral word>
        N_bytes<r, word> shift_left (const N_bytes<r, word> &x, uint32 extended, uint32 i) {
            N_bytes<r, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian r, std::unsigned_integral word>
        N_bytes<r, word> shift_right (const N_bytes<r, word> &x, uint32 i) {
            N_bytes<r, word> z = x;
            z.words ().bit_shift_right (i);
            return z;
        }
    
        template <endian r, std::unsigned_integral word>
        Z_bytes<r, negativity::twos, word> shift_left (const Z_bytes<r, negativity::twos, word> &x, uint32 extended, uint32 i) {
            Z_bytes<r, negativity::twos, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian r, std::unsigned_integral word>
        Z_bytes<r, negativity::twos, word> shift_right (const Z_bytes<r, negativity::twos, word> &x, uint32 i) {
            Z_bytes<r, negativity::twos, word> z = x;
            z.words ().bit_shift_right (i, x < 0);
            return z;
        }

        template <endian r, std::unsigned_integral word>
        N_bytes<r, word> inline plus (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            return N_bytes<r, word> (std::move (arithmetic::nones::plus<r, word> (a, b)));
        }

        template <endian r, std::unsigned_integral word>
        N_bytes<r, word> inline minus (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            if (b > a) return N_bytes<r, word>::zero ();
            return N_bytes<r, word> (std::move (arithmetic::nones::minus<r, word> (a, b)));
        }

        template <endian r, std::unsigned_integral word>
        N_bytes<r, word> times (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            N_bytes<r, word> n = N_bytes<r, word>::zero (a.size () + b.size () + 1);
            auto w = n.words ();
            arithmetic::times (w, a.words (), b.words ());
            return n;
        }
        
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator << (const N_bytes<r, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator >> (const N_bytes<r, word> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator << (const Z_bytes<r, negativity::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }
    
    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline operator >> (const Z_bytes<r, negativity::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (plus (trim (a), trim (b)));
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator - (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (minus (trim (a), trim (b)));
    }
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator * (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (times (trim (a), trim (b)));
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Quotient;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {j}).Quotient;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, uint64 j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Quotient;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, int64 j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const Z_bytes<r, negativity::twos, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline operator %
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod (x, nonzero {j}).Remainder;
    }

    template <endian r, std::unsigned_integral word>
    uint64 inline operator % (const N_bytes<r, word> &x, uint64 j) {
        if (j == 0) throw math::division_by_zero {};
        return uint64 (data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Remainder);
    }

    template <endian r, negativity c, std::unsigned_integral word>
    uint64 inline operator % (const Z_bytes<r, c, word> &x, uint64 j) {
        if (j == 0) throw math::division_by_zero {};
        return uint64 (data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Remainder);
    }

    template <endian r, std::unsigned_integral word>
    writer<word> inline &operator << (writer<word> &w, const N_bytes<r, word> &n) {
        return w << static_cast<const bytestring<word> &> (n);
    }

    template <endian r, negativity c, std::unsigned_integral word>
    writer<word> inline &operator << (writer<word> &w, const Z_bytes<r, c, word> &n) {
        return w << static_cast<const bytestring<word> &> (n);
    }

    template <endian r, std::unsigned_integral word>
    reader<word> inline &operator >> (reader<word> &rr, N_bytes<r, word> &n) {
        rr.read (n.data (), n.size ());
        return rr;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    reader<word> inline &operator >> (reader<word> &rr, Z_bytes<r, c, word> &n) {
        rr.read (n.data (), n.size ());
        return rr;
    }
}

#endif
