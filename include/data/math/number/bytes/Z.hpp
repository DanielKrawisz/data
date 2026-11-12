// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/bytes.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include <data/arithmetic/complementary.hpp>

#include <data/exception.hpp>

namespace data::math::number {
    
    // N_bytes is a representation of the natural numbers that
    // is also a string of bytes. It has an order parameter
    // representing endianness.
    template <endian r, Word word> struct Z_bytes<r, negativity::nones, word> : oriented<r, word> {

        // read in a standard number representation as a string.
        // decimal and hexidecimal are both accepted. In hexidecimal,
        // the number will be read to be identical with the hexidecimal
        // representation, including extra leading zeros.
        // for example, "0x00" and "0x" are both representations of zero
        // but different strings of bytes.
        static Z_bytes read (string_view x);

        // read in the number as a string of bytes.
        static Z_bytes read (slice<const word> x);

        // trim the number to its minimal representation.
        Z_bytes &trim ();

        // get a representation of zero of any size.
        static Z_bytes zero (size_t size = 0);
        
        Z_bytes () : oriented<r, word> {} {}

        // implicitly convert from fixed size types.
        // this is important so that we can write literals
        // that will be automatically converted. We throw
        // if we try to construct with a negative number.
        template <std::integral I> Z_bytes (I x): Z_bytes {convert<Z_bytes> (x)} {}

        template <endian Order, std::integral T, std::size_t n_bits, alignment Align>
        Z_bytes (const endian_arithmetic<Order, T, n_bits, Align> &x): Z_bytes {convert<Z_bytes> (x)} {}

        template <bool is_signed, endian o, size_t size, Word w>
        Z_bytes (const bounded<is_signed, o, size, w> &n): Z_bytes {convert<Z_bytes> (n)} {}

        // Implicitly convert from unsigned N_bytes number types.
        template <data::endian o, Word w> requires Unsame<N_bytes<o, w>, Z_bytes>
        Z_bytes (const N_bytes<o, w> &n): Z_bytes {convert<Z_bytes> (n)} {}

        // explicitly convert from signed Z_bytes types.

        template <endian o, negativity neg, Word w> requires (neg != negativity::nones)
        explicit Z_bytes (const Z_bytes<o, neg, w> &n): Z_bytes {convert<Z_bytes> (n)} {}

        explicit Z_bytes (string_view x): Z_bytes {read (x)} {}
        explicit Z_bytes (slice<const word> x): Z_bytes {read (x)} {}

        template <hex_case zz> explicit Z_bytes (const hex::uint<zz> &);
        explicit Z_bytes (const dec_uint &u): Z_bytes {read (u)} {}

        explicit operator double () const;
        
        template <std::integral I> explicit operator I () const;

        explicit Z_bytes (bytestring<word> &&b): oriented<r, word> {b} {}

    };
    
    template <endian r, Word word>
    struct Z_bytes<r, negativity::twos, word> : oriented<r, word> {
        
        Z_bytes () : oriented<r, word> {} {}

        // construct from any number literal.
        template <std::integral I> Z_bytes (I x): Z_bytes {convert<Z_bytes> (x)} {};

        template <data::endian Order, std::integral T, std::size_t n_bits, alignment Align>
        Z_bytes (const endian_arithmetic<Order, T, n_bits, Align> &x): Z_bytes {convert<Z_bytes> (x)} {}

        template <bool is_signed, data::endian o, size_t size, Word w>
        Z_bytes (const bounded<is_signed, o, size, w> &n): Z_bytes {convert<Z_bytes> (n)} {}

        Z_bytes (const N_bytes<r, word> &n): Z_bytes {convert<Z_bytes<r, negativity::twos, word>> (n)} {}
        
        // string constructors.
        static Z_bytes read (string_view x);
        static Z_bytes read (slice<const word> x);

        explicit Z_bytes (string_view x) : Z_bytes {read (x)} {}

        explicit Z_bytes (slice<const word> x): Z_bytes {read (x)} {}

        template <hex_case zz> explicit Z_bytes (const hex::int2<zz> &);
        explicit Z_bytes (const dec_uint &u): Z_bytes {read (u)} {}
        explicit Z_bytes (const dec_int &u): Z_bytes {read (u)} {}

        // TODO get rid of this and replace with a constructor.
        operator Z_bytes<r, negativity::BC, word> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0);
        
        // cast to any built in type
        template <std::integral I>
        explicit operator I () const;
        
        Z_bytes &trim ();

        explicit Z_bytes (bytestring<word> &&b) : oriented<r, word> {b} {}

        explicit Z_bytes (const hex_string &x): oriented<r, word> {x} {}

    };
    
    // for little endian, these are an implementation of bitcoin numbers.
    template <endian r, Word word>
    struct Z_bytes<r, negativity::BC, word> : oriented<r, word> {

        //explicit Z_bytes (const std::string &);
        Z_bytes () : oriented<r, word> {} {}
        
        Z_bytes (bool);

        // construct from any number literal.
        template <std::integral I> Z_bytes (I x): Z_bytes {convert<Z_bytes> (x)} {}

        Z_bytes (const N_bytes<r, word> &n): Z_bytes {convert<Z_bytes<r, negativity::BC, word>> (n)} {}
        
        // string constructors.
        static Z_bytes read (string_view x);
        static Z_bytes read (slice<const word> x);
        explicit Z_bytes (string_view x): Z_bytes {read (x)} {}
        explicit Z_bytes (slice<const word> x): Z_bytes {read (x)} {}

        template <hex_case zz> explicit Z_bytes (const hex::intBC<zz> &);
        explicit Z_bytes (const dec_uint &u): Z_bytes {read (u)} {}
        explicit Z_bytes (const dec_int &u): Z_bytes {read (u)} {}

        // we need this constructor because the compiler gets confused
        // with the bool constructor when we try to use string literals.
        Z_bytes (const char *x): Z_bytes {string_view {x, std::strlen (x)}} {}
        
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
    template <endian r, Word word>
    nonzero<N_bytes<r, word>> inline increment<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        nonzero<N_bytes<r, word>> v {n};
        ++v.Value;
        return v;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline decrement<N_bytes<r, word>>::operator () (const nonzero<N_bytes<r, word>> &n) {
        auto x = n.Value;
        return --x;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline decrement<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        if (data::is_zero (n)) return n;
        auto x = n;
        return --x;
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline increment<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return ++x;
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline decrement<Z_bytes<r, c, word>>::operator () (const Z_bytes<r, c, word> &n) {
        auto x = n;
        return --x;
    }
    
    // some functions that can easily be implemented as other functions using conversions. 
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator ~ (const N_bytes<r, word> &b) {
        return ~Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a ^ Z_bytes<r, negativity::twos, word> (b);
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator ^ (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) ^ b;
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator & (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a & Z_bytes<r, negativity::twos, word> (b);
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator & (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) & b;
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator | (
        const Z_bytes<r, negativity::twos, word> &a, const N_bytes<r, word> &b) {
        return a | Z_bytes<r, negativity::twos, word> (b);
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator | (
        const N_bytes<r, word> &a, const Z_bytes<r, negativity::twos, word> &b) {
        return Z_bytes<r, negativity::twos, word> (a) | b;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator + (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a + Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a - Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator - (const Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a + (-b);
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator * (const Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a * Z_bytes<r, c, word> (b);
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator + (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) + b;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator - (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) - b;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator * (const N_bytes<r, word> &a, const Z_bytes<r, c, word> &b) {
        return Z_bytes<r, c, word> (a) * b;
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline operator + (const N_bytes<r, word> &u, I x) {
        return u + N_bytes<r, word> (x);
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline operator - (const N_bytes<r, word> &u, I x) {
        return u - N_bytes<r, word> (x);
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline operator * (const N_bytes<r, word> &u, I x) {
        return u * N_bytes<r, word> (x);
    }

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> inline operator + (I x, const N_bytes<r, word> &u) {
        return u + N_bytes<r, word> (x);
    }

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> inline operator - (I x, const N_bytes<r, word> &u) {
        return N_bytes<r, word> (x) - u;
    }

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> inline operator * (I x, const N_bytes<r, word> &u) {
        return u * N_bytes<r, word> (x);
    }

    template <endian r, negativity neg, Word word, std::integral I>
    Z_bytes<r, neg, word> inline operator + (const Z_bytes<r, neg, word> &z, I x) {
        return z + Z_bytes<r, neg, word> {x};
    }

    template <endian r, negativity neg, Word word, std::integral I>
    Z_bytes<r, neg, word> inline operator - (const Z_bytes<r, neg, word> &z, I x) {
        return z - Z_bytes<r, neg, word> {x};
    }

    template <endian r, negativity neg, Word word, std::integral I>
    Z_bytes<r, neg, word> inline operator * (const Z_bytes<r, neg, word> &z, I x) {
        return z * Z_bytes<r, neg, word> {x};
    }

    template <std::integral I, endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> inline operator + (I x, const Z_bytes<r, neg, word> &z) {
        return Z_bytes<r, neg, word> {x} + z;
    }

    template <std::integral I, endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> inline operator - (I x, const Z_bytes<r, neg, word> &z) {
        return Z_bytes<r, neg, word> {x} - z;
    }

    template <std::integral I, endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> inline operator * (I x, const Z_bytes<r, neg, word> &z) {
        return Z_bytes<r, neg, word> {x} * z;
    }

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator + (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {u} + Z_bytes<r, negativity::twos, word> {x};
    }

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator - (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {u} - Z_bytes<r, negativity::twos, word> {x};
    }

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator * (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {u} * Z_bytes<r, negativity::twos, word> {x};
    }

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator + (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} + Z_bytes<r, negativity::twos, word> {u};
    }

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator - (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} - Z_bytes<r, negativity::twos, word> {u};
    }

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator * (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} * Z_bytes<r, negativity::twos, word> {u};
    }

    template <endian r, Word word>
    N_bytes<r, word> inline &operator += (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x + n;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline &operator -= (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x - n;
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline &operator *= (N_bytes<r, word> &x, const N_bytes<r, word> &n) {
        return x = x * n;
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline &operator <<= (N_bytes<r, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline &operator >>= (N_bytes<r, word> &x, int64 i) {
        return x = x >> i;
    }
    
    // post increment
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator ++ (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        ++x;
        return z;
    }
    
    // post increment
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator -- (Z_bytes<r, c, word> &x, int) {
        Z_bytes<r, c, word> z = x;
        --x;
        return z;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator += (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x + n;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator -= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x - n;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator *= (Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &n) {
        return x = x * n;
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator /= (Z_bytes<r, c, word> &a, const Z_bytes<r, c, word> &b) {
        return a = a / b;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator <<= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator >>= (Z_bytes<r, c, word> &x, int64 i) {
        return x = x >> i;
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline &operator += (N_bytes<r, word> &x, I n) {
        return x = x + n;
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline &operator -= (N_bytes<r, word> &x, I n) {
        return x = x - n;
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline &operator *= (N_bytes<r, word> &x, I n) {
        return x = x * n;
    }

    template <endian r, Word word, std::integral I>
    math::Z_bytes<r, word> inline &operator += (math::Z_bytes<r, word> &x, I n) {
        return x = x + n;
    }

    template <endian r, Word word, std::integral I>
    math::Z_bytes<r, word> inline &operator -= (math::Z_bytes<r, word> &x, I n) {
        return x = x - n;
    }

    template <endian r, Word word, std::integral I>
    math::Z_bytes<r, word> inline &operator *= (math::Z_bytes<r, word> &x, I n) {
        return x = x * n;
    }

    template <endian r, Word word, std::integral I>
    Z_bytes_BC<r, word> inline &operator += (Z_bytes_BC<r, word> &x, I n) {
        return x = x + n;
    }

    template <endian r, Word word, std::integral I>
    Z_bytes_BC<r, word> inline &operator -= (Z_bytes_BC<r, word> &x, I n) {
        return x = x - n;
    }

    template <endian r, Word word, std::integral I>
    Z_bytes_BC<r, word> inline &operator *= (Z_bytes_BC<r, word> &x, I n) {
        return x = x * n;
    }
    
    // check if the number is negative, and then do bit shift on the absolute value.
    template <endian r, Word word>
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
    
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> operator >> (const Z_bytes<r, negativity::BC, word> &x, int n) {
        if (n < 0) return x << -n;
        if (n == 0) return x;
        if (is_zero (x)) return Z_bytes<r, negativity::BC, word> {};

        bool neg = is_negative (x);
        Z_bytes<r, negativity::BC, word> result = neg ? -x : x;
        result.words ().bit_shift_right (static_cast<uint32> (n));
        return neg ? -result : trim (result);
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator - (const Z_bytes<r, negativity::twos, word> &x) {
        auto z = ~x;
        return ++z;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline &operator %= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian r, negativity c, Word word>
    N_bytes<r, word> inline &operator %= (Z_bytes<r, c, word> &a, const N_bytes<r, word> &b) {
        return a = a % b;
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline &operator %=
    (Z_bytes<r, negativity::BC, word> &a, const Z_bytes<r, negativity::BC, word> &b) {
        return a = a % b;
    }

    template <endian r, negativity c, Word word>
    uint64 &operator %= (Z_bytes<r, c, word> &a, uint64 b) {
        return a = a % b;
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &u, I x) {
        return u & N_bytes<r, word> {x};
    }

    template <std::unsigned_integral I, endian r, Word word>
    N_bytes<r, word> inline operator & (I x, const N_bytes<r, word> &u) {
        return u & N_bytes<r, word> {x};
    }

    template <endian r, Word word, std::integral I>
    Z_bytes<r, negativity::twos, word> inline operator & (const Z_bytes<r, negativity::twos, word> &u, I x) {
        return u & Z_bytes<r, negativity::twos, word> {x};
    }

    template <std::integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator & (I x, const Z_bytes<r, negativity::twos, word> &u) {
        return u & Z_bytes<r, negativity::twos, word> {x};
    }

    template <endian r, Word word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> inline operator & (const N_bytes<r, word> &u, I x) {
        return Z_bytes<r, negativity::twos, word> {x} & Z_bytes<r, negativity::twos, word> {u};}

    template <std::signed_integral I, endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator & (I x, const N_bytes<r, word> &u) {
        return Z_bytes<r, negativity::twos, word> {x} & Z_bytes<r, negativity::twos, word> {u};
    }
    
}

namespace data {

    template <endian a, negativity b, endian c, negativity d, Word word>
    bool inline identical (const math::number::Z_bytes<a, b, word> &x, const math::number::Z_bytes<c, d, word> &y) {
        if constexpr (a != c || b != d) return false;
        else return static_cast<const bytestring<word> &> (x) == static_cast<const bytestring<word> &> (y);
    }
    
}

namespace data::math::def {

    template <endian r, Word word>
    math::sign inline sign<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return arithmetic::sign<negativity::nones> (x.words ());
    }

    template <endian r, negativity c, Word word>
    math::sign inline sign<Zb<r, c, word>>::operator () (const Zb<r, c, word> &x) {
        return arithmetic::sign<c> (x.words ());
    }

    template <endian r, Word word>
    bool inline is_negative<N_bytes<r, word>>::operator () (const N_bytes<r, word> &) {
        return false;
    }
    
    template <endian r, Word word>
    bool inline is_positive<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return !data::is_zero (x);
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline abs<N_bytes<r, word>>::operator () (const N_bytes<r, word> &x) {
        return x;
    }
    
    template <endian r, Word word>
    Z_bytes_BC<r, word> inline abs<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_negative (x) ? -x : x;
    }
    
    template <endian r, Word word>
    N_bytes<r, word> abs<Z_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &x) {
        if (data::is_zero (x)) return N_bytes<r, word> {};
        Zb<r, negativity::twos, word> z = data::is_negative (x) ? -x : x;
        auto n = N_bytes<r, word>::zero (z.size ());
        std::copy (z.begin (), z.end (), n.begin ());
        return n;
    }

    template <endian r, negativity zz, Word word>
    Zb<r, zz, word> inline times<Zb<r, zz, word>>::operator ()
    (const Zb<r, zz, word> &a, const Zb<r, zz, word> &b) {
        return a * b;
    }

    template <endian r, negativity zz, Word word>
    nonzero<Zb<r, zz, word>> inline times<Zb<r, zz, word>>::operator ()
        (const nonzero<Zb<r, zz, word>> &a, const nonzero<Zb<r, zz, word>> &b) {
        return a * b;
    }
    
    template <endian r, Word word>
    Zb<r, negativity::BC, word> inline
    next (const Zb<r, negativity::BC, word> &z) {
        return is_positive (z) ? -z : -z - 1;
    }
    
    template <endian r, Word word>
    bool inline is_negative<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return arithmetic::sign_bit (x.words ());
    }

    template <endian r, Word word>
    bool inline is_negative<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return !data::is_zero (x) && arithmetic::sign_bit (x.words ());
    }

    template <endian r, Word word>
    bool inline is_positive<Z_bytes<r, word>>::operator () (const Z_bytes<r, word> &x) {
        return !data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }

    template <endian r, Word word>
    bool inline is_positive<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return !data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian r, negativity neg, Word word>
    bool inline is_zero<number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &x) {
        return arithmetic::is_zero<neg> (x.words ());
    }
    
    template <endian r, Word word>
    bool inline is_positive_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian r, Word word>
    bool inline is_negative_zero<Z_bytes_BC<r, word>>::operator () (const Z_bytes_BC<r, word> &x) {
        return data::is_zero (x) && arithmetic::sign_bit (x.words ());
    }

    template <endian r, Word word>
    N_bytes<r, word> inline bit_xor<N_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a ^ b;
    }

    template <endian r, Word word>
    math::Z_bytes<r, word> inline bit_not<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n) {
        return ~n;
    }

    template <endian r, Word word>
    math::Z_bytes<r, word> inline bit_xor<math::Z_bytes<r, word>>::operator ()
        (const math::Z_bytes<r, word> &a, const math::Z_bytes<r, word> &b) {
        return a ^ b;
    }

    template <endian r, Word word>
    math::Z_bytes<r, word> inline bit_xor<N_bytes<r, word>, math::Z_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const math::Z_bytes<r, word> &b) {
        return math::Z_bytes<r, word> {a} ^ b;
    }

    template <endian r, Word word>
    math::Z_bytes<r, word> inline bit_xor<math::Z_bytes<r, word>, N_bytes<r, word>>::operator ()
    (const math::Z_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return a ^ math::Z_bytes<r, word> {b};
    }

    template <endian r, Word word>
    math::Z_bytes_BC<r, word> inline bit_not<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x) {
        auto a = x;
        arithmetic::bit_negate<word> (a.begin (), a.end (), x.begin ());
        return a;
    }

    template <endian r, Word word>
    math::Z_bytes_BC<r, word> inline mul_2_pow<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &a, uint32 u) {
        return a << u;
    }

    template <endian r, Word word>
    math::Z_bytes_BC<r, word> inline bit_xor<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_xor (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_xor<word> (bb.begin (), bb.end (), bb.begin (), a.begin ());
        return bb;
    }

    template <endian r, Word word>
    math::Z_bytes_BC<r, word> inline bit_or<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_or (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_or<word> (bb.begin (), bb.end (), bb.begin (), a.begin ());
        return bb;
    }

    template <endian r, Word word>
    math::Z_bytes_BC<r, word> inline bit_and<math::Z_bytes_BC<r, word>>::operator ()
    (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b) {
        if (a.size () < b.size ()) return data::bit_and (b, a);
        auto bb = extend (b, a.size ());
        arithmetic::bit_and<word> (bb.begin (), bb.end (), bb.begin (), a.begin ());
        return bb;
    }

    template <endian r, Word word>
    math::N_bytes<r, word> inline div_2_pow<math::N_bytes<r, word>>::operator () (const math::N_bytes<r, word> &x, uint32 exp) {
        return bit_div_2_pow (x, exp);
    }

    template <endian r, negativity neg, Word word>
    math::number::Z_bytes<r, neg, word> inline
    div_2_pow<math::number::Z_bytes<r, neg, word>>::operator () (const math::number::Z_bytes<r, neg, word> &x, uint32 exp) {
        return bit_div_2_pow (x, exp);
    }

    template <endian r, Word word>
    math::N_bytes<r, word> inline mod_2<math::N_bytes<r, word>>::operator () (const math::N_bytes<r, word> &x) {
        return bit_mod_2_positive_mod (x);
    }

    template <endian r, Word word>
    math::Z_bytes_BC<r, word> inline mod_2<math::Z_bytes_BC<r, word>>::operator () (const math::Z_bytes_BC<r, word> &x) {
        if (data::is_zero (x)) return 0;
        word z = *x.words ().begin () & 1;
        return data::is_negative (x) ?
            math::Z_bytes_BC<r, word> {-static_cast<std::make_signed_t<word>> (z)}:
            math::Z_bytes_BC<r, word> {z};
    }

    template <endian r, Word word>
    math::Z_bytes<r, word> inline mod_2<math::Z_bytes<r, word>>::operator () (const math::Z_bytes<r, word> &x) {
        return bit_mod_2_positive_mod (x);
    }

    template <endian r, Word word>
    struct bit_shift_right<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x, uint32 i) const {
            return x >> i;
        }
    };

    template <endian r, Word word>
    struct bit_shift_left<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x, uint32 i) const {
            return x << i;
        }
    };

    template <endian r, Word word>
    size_t inline size_in_base<N_bytes<r, word>, 2>::operator () (const N_bytes<r, word> &x) {
        return arithmetic::bit_width<negativity::nones> (x.words ());
    }

    template <endian r, negativity c, Word word>
    size_t inline size_in_base<number::Z_bytes<r, c, word>, 2>::operator () (const number::Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::twos) return arithmetic::bit_width<c> (x.words ());
        else return arithmetic::bit_width<negativity::BC> (x.words ());
    }

    template <endian r, Word word>
    const N_bytes<r, word> inline &min<N_bytes<r, word>>::operator () (const N_bytes<r, word> &a, const N_bytes<r, word> &b) const {
        return a < b ? a : b;
    }

    template <endian r, Word word>
    const N_bytes<r, word> inline &max<N_bytes<r, word>>::operator () (const N_bytes<r, word> &a, const N_bytes<r, word> &b) const {
        return a > b ? a : b;
    }

    template <endian r, negativity neg, Word word>
    const number::Z_bytes<r, neg, word> inline &min<number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &a, const number::Z_bytes<r, neg, word> &b) const {
        return a < b ? a : b;
    }

    template <endian r, negativity neg, Word word>
    const number::Z_bytes<r, neg, word> inline &max<number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &a, const number::Z_bytes<r, neg, word> &b) const {
        return a > b ? a : b;
    }

    template <endian r, Word word>
    bool inline digit<N_bytes<r, word>>::operator () (const N_bytes<r, word> &n, int32 i) {
        return arithmetic::get_digit<negativity::nones> (n.words (), i);
    }

    template <endian r, negativity neg, Word word>
    bool inline digit<number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &n, int32 i) {
        return arithmetic::get_digit<neg> (n.words (), i);
    }
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian r, negativity neg, Word word>
    bool inline is_minimal (const Z_bytes<r, neg, word> &x) {
        return arithmetic::is_minimal<neg> (x.words ());
    }
    
    template <endian r, Word word>
    size_t inline minimal_size (const N_bytes<r, word> &x) {
        return arithmetic::minimal_size<negativity::nones> (x.words ());
    }
    
    template <endian r, negativity c, Word word>
    size_t inline minimal_size (const Z_bytes<r, c, word> &x) {
        return arithmetic::minimal_size<c> (x.words ());
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline extend (const N_bytes<r, word> &x, size_t size) {
        return N_bytes<r, word> (std::move (arithmetic::extend<r, negativity::nones, word> (x, size)));
    }
    
    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline extend (const Z_bytes<r, c, word> &x, size_t size) {
        return Z_bytes<r, c, word> (std::move (arithmetic::extend<r, c, word> (x, size)));
    }
    
    template <endian r, negativity neg, Word word>
    std::weak_ordering inline operator <=> (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        return arithmetic::compare<neg> (a.words (), b.words ());
    }
    
    template <endian r, negativity c, Word word, std::integral I>
    std::weak_ordering inline operator <=> (const Z_bytes<r, c, word> &a, I i) {
        if constexpr (c == negativity::nones && std::signed_integral<I>)
            if (i < 0) return std::weak_ordering::greater;
        return a <=> Z_bytes<r, c, word> {i};
    }
    
    template <endian r, negativity c, Word word, std::integral I>
    bool inline operator == (const Z_bytes<r, c, word> &x, I i) {
        if constexpr (c == negativity::nones && std::signed_integral<I>)
            if (i < 0) return false;
        return (x <=> Z_bytes<r, c, word> {i}) == 0;
    }

    template <endian r, negativity cl, negativity cr, Word word> requires (cl != cr)
    bool inline operator == (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cr, word> &b) {
        return a == Z_bytes<r, cl, word> (b);
    }

    template <endian r, negativity cl, negativity cr, Word word> requires (cl != cr)
    std::weak_ordering inline operator <=> (const Z_bytes<r, cl, word> &a, const Z_bytes<r, cr, word> &b) {
        return a <=> Z_bytes<r, cl, word> (b);
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator - (const N_bytes<r, word> &x) {
        auto z = Z_bytes<r, negativity::twos, word> (x);
        return -z;
    }
    
    template <endian r, Word word>
    N_bytes<r, word> N_bytes<r, word>::read (slice<const word> b) {
        auto x = N_bytes<r, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word>
    Z_bytes<r, negativity::twos, word>::read (slice<const word> b) {
        auto x = Z_bytes<r, negativity::twos, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> Z_bytes<r, negativity::BC, word>::read (slice<const word> b) {
        auto x = Z_bytes<r, negativity::BC, word> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline N_bytes<r, word>::zero (size_t size) {
        return N_bytes<r, word> (std::move (arithmetic::zero<r, word> (size)));
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline Z_bytes<r, negativity::twos, word>::zero (size_t size) {
        return Z_bytes<r, negativity::twos, word> (std::move (arithmetic::zero<r, word> (size)));
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline Z_bytes<r, negativity::BC, word>::zero (size_t size, bool negative) {
        return arithmetic::BC::zero<r, word> (size, negative);
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline operator - (const Z_bytes<r, negativity::BC, word> &x) {
        return arithmetic::trim<r, negativity::BC, word> (arithmetic::BC::negate<r, word> (x));
    }

    namespace {
        template <endian r, Word word>
        const Z_bytes<r, negativity::BC, word> &true_value () {
            static Z_bytes<r, negativity::BC, word> TrueValue = Z_bytes<r, negativity::BC, word>::read ("0x01");
            return TrueValue;
        }

        template <endian r, Word word>
        const Z_bytes<r, negativity::BC, word> &false_value () {
            static Z_bytes<r, negativity::BC, word> FalseValue = Z_bytes<r, negativity::BC, word>::read ("0x");
            return FalseValue;
        };
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline operator !
    (const Z_bytes<r, negativity::BC, word> &x) {
        return bool (x) ? false_value<r, word> () : true_value<r, word> ();
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline operator &&
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &y) {
        return bool (x) && bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline operator ||
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &y) {
        return bool (x) || bool (y) ? true_value<r, word> () : false_value<r, word> ();
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> operator ~ (const Z_bytes<r, negativity::twos, word> &x) {
        auto z = trim (x);
        if (is_zero (z)) z = extend (z, 1);
        arithmetic::bit_negate<word> (z.begin (), z.end (), z.begin ());
        return z;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline operator & (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return N_bytes<r, word> (std::move (arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_and<r, negativity::nones, word> (a, b))));
    }

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator & (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if constexpr (neg == negativity::twos)
            return Z_bytes<r, neg, word> {std::move (arithmetic::trim<r, neg, word> (arithmetic::bit_and<r, neg, word> (a, b)))};
        else return Z_bytes<r, neg, word> {std::move (arithmetic::bit_and<r, neg, word> (a, b))};
    }

    template <endian r, Word word>
    N_bytes<r, word> &operator &= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_and<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        a.trim ();
        return a;
    }

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator &= (Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_and<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        if constexpr (neg == negativity::twos) a.trim ();
        return a;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline operator | (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        N_bytes<r, word> result {std::move (arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_or<r, negativity::nones, word> (a, b)))};
        result.trim ();
        return result;
    }

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> inline operator | (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if constexpr (neg == negativity::twos)
            return Z_bytes<r, neg, word> {std::move (arithmetic::trim<r, neg, word> (arithmetic::bit_or<r, neg, word> (a, b)))};
        else return Z_bytes<r, neg, word> {std::move (arithmetic::bit_or<r, neg, word> (a, b))};
    }

    template <endian r, Word word>
    N_bytes<r, word> &operator |= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_or<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        a.trim ();
        return a;
    }

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator |= (Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_or<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        if constexpr (neg == negativity::twos) a.trim ();
        return a;
    }

    // bit xor
    template <endian r, Word word>
    N_bytes<r, word> inline operator ^ (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        N_bytes<r, word> result {std::move (arithmetic::trim<r, negativity::nones, word> (arithmetic::bit_xor<r, negativity::nones, word> (a, b)))};
        result.trim ();
        return result;
    }

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> operator ^ (const Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if constexpr (neg == negativity::twos)
            return Z_bytes<r, neg, word> {std::move (arithmetic::trim<r, neg, word> (arithmetic::bit_xor<r, neg, word> (a, b)))};
        else return Z_bytes<r, neg, word> {std::move (arithmetic::bit_xor<r, neg, word> (a, b))};
    }

    template <endian r, Word word>
    N_bytes<r, word> &operator ^= (N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_xor<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        a.trim ();
        return a;
    }

    template <endian r, negativity neg, Word word>
    Z_bytes<r, neg, word> &operator ^= (Z_bytes<r, neg, word> &a, const Z_bytes<r, neg, word> &b) {
        if (a.size () < b.size ()) a = extend (a, b.size ());
        auto bt = extend (b, a.size ());
        arithmetic::bit_xor<word> (a.begin (), a.end (), a.begin (), bt.begin ());
        if constexpr (neg == negativity::twos) a.trim ();
        return a;
    }

    // basic arithmetic
    template <endian r, negativity c, Word word>
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
    
    template <endian r, negativity c, Word word>
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

    template <endian r, Word word> inline
    Z_bytes<r, negativity::BC, word>::Z_bytes (bool b): Z_bytes<r, negativity::BC, word> {} {
        if (b) this->push_back (1);
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline trim (const Z_bytes<r, c, word> &z) {
        return Z_bytes<r, c, word> (std::move (arithmetic::trim<r, c, word> (z)));
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::nones, word> inline &Z_bytes<r, negativity::nones, word>::trim () {
        arithmetic::trim<r, negativity::nones, word> (*this);
        return *this;
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline &Z_bytes<r, negativity::twos, word>::trim () {
        arithmetic::trim<r, negativity::twos, word> (*this);
        return *this;
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline &Z_bytes<r, negativity::BC, word>::trim () {
        arithmetic::trim<r, negativity::BC, word> (*this);
        return *this;
    }
    
    template <endian r, Word word> template <std::integral I>
    inline N_bytes<r, word>::operator I () const {
        return convert<I> (*this);
    } 
    
    template <endian r, Word word>
    template <std::integral I>
    inline Z_bytes<r, negativity::twos, word>::operator I () const {
        return convert<I> (*this);
    } 
    
    template <endian r, Word word>
    template <std::integral I>
    inline Z_bytes<r, negativity::BC, word>::operator I () const {
        return convert<I> (*this);
    } 
    
    template <endian r, Word word>
    inline Z_bytes<r, negativity::twos, word>::operator Z_bytes<r, negativity::BC, word> () const {
        if (data::is_negative (*this)) return -Z_bytes<r, negativity::BC, word> (data::abs (*this));
        return Z_bytes<r, negativity::BC, word> (data::abs (*this)).trim ();
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word>::operator Z_bytes<encoding::endian::opposite (r), negativity::BC, word> () const {
        Z_bytes<encoding::endian::opposite (r), negativity::BC, word> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian r, Word word>
    inline Z_bytes<r, negativity::BC, word>::operator Z_bytes<r, negativity::twos, word> () const {
        if (data::is_zero (*this)) return Z_bytes<r, negativity::twos, word>::zero ();
        if (data::is_negative (*this)) return -Z_bytes<r, negativity::twos, word> (data::abs (*this));
        auto z = Z_bytes<r, negativity::twos, word>::zero (this->size ());
        std::copy (this->begin (), this->end (), z.begin ());
        return z.trim ();
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator ++ (Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::nones) arithmetic::trim<r, c> (arithmetic::nones::increment<r> (x));
        if constexpr (c == negativity::twos) arithmetic::trim<r, c> (arithmetic::twos::increment<r> (x));
        if constexpr (c == negativity::BC) arithmetic::trim<r, c> (arithmetic::BC::increment<r> (x));
        return x;
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline &operator -- (Z_bytes<r, c, word> &x) {
        if constexpr (c == negativity::nones) arithmetic::trim<r, c> (arithmetic::nones::decrement<r> (x));
        if constexpr (c == negativity::twos) arithmetic::trim<r, c> (arithmetic::twos::decrement<r> (x));
        if constexpr (c == negativity::BC) arithmetic::trim<r, c> (arithmetic::BC::decrement<r> (x));
        return x;
    }
    
    namespace {
    
        template <endian r, Word word>
        N_bytes<r, word> shift_left (const N_bytes<r, word> &x, uint32 extended, uint32 i) {
            N_bytes<r, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian r, Word word>
        N_bytes<r, word> shift_right (const N_bytes<r, word> &x, uint32 i) {
            N_bytes<r, word> z = x;
            z.words ().bit_shift_right (i);
            return z;
        }
    
        template <endian r, Word word>
        Z_bytes<r, negativity::twos, word> shift_left (const Z_bytes<r, negativity::twos, word> &x, uint32 extended, uint32 i) {
            Z_bytes<r, negativity::twos, word> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian r, Word word>
        Z_bytes<r, negativity::twos, word> shift_right (const Z_bytes<r, negativity::twos, word> &x, uint32 i) {
            Z_bytes<r, negativity::twos, word> z = x;
            z.words ().bit_shift_right (i, x < 0);
            return z;
        }

        template <endian r, Word word>
        N_bytes<r, word> inline plus (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            return N_bytes<r, word> (std::move (arithmetic::nones::plus<r, word> (a, b)));
        }

        template <endian r, Word word>
        N_bytes<r, word> inline minus (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            if (b > a) return N_bytes<r, word>::zero ();
            return N_bytes<r, word> (std::move (arithmetic::nones::minus<r, word> (a, b)));
        }

        template <endian r, Word word>
        N_bytes<r, word> times (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
            N_bytes<r, word> n = N_bytes<r, word>::zero (a.size () + b.size () + 1);
            auto w = n.words ();
            arithmetic::times (w, a.words (), b.words ());
            return n;
        }
        
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline operator << (const N_bytes<r, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + (sizeof (word) * CHAR_BIT) - 1) / (sizeof (word) * CHAR_BIT), static_cast<uint32> (i)));
    }

    template <endian r, Word word>
    N_bytes<r, word> inline operator >> (const N_bytes<r, word> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + (sizeof (word) * CHAR_BIT) - 1) / (sizeof (word) * CHAR_BIT), static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator << (const Z_bytes<r, negativity::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }
    
    template <endian r, Word word>
    Z_bytes<r, negativity::twos, word> inline operator >> (const Z_bytes<r, negativity::twos, word> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian r, Word word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (plus (trim (a), trim (b)));
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline operator - (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (minus (trim (a), trim (b)));
    }
    
    template <endian r, Word word>
    N_bytes<r, word> inline operator * (const N_bytes<r, word> &a, const N_bytes<r, word> &b) {
        return trim (times (trim (a), trim (b)));
    }

    template <endian r, Word word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Quotient;
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {j}).Quotient;
    }

    template <endian r, negativity c, Word word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian r, Word word, std::unsigned_integral I>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, I j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Quotient;
    }

    template <endian r, negativity c, Word word, std::integral I>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, I j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline operator % (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }

    template <endian r, Word word>
    N_bytes<r, word> inline operator % (const Z_bytes<r, negativity::twos, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }

    template <endian r, Word word>
    Z_bytes<r, negativity::BC, word> inline operator %
    (const Z_bytes<r, negativity::BC, word> &x, const Z_bytes<r, negativity::BC, word> &j) {
        if (j == 0) throw math::division_by_zero {};
        return data::divmod (x, nonzero {j}).Remainder;
    }

    template <endian r, Word word>
    uint64 inline operator % (const N_bytes<r, word> &x, uint64 j) {
        if (j == 0) throw math::division_by_zero {};
        return uint64 (data::divmod<N_bytes<r, word>> (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Remainder);
    }

    template <endian r, negativity c, Word word>
    uint64 inline operator % (const Z_bytes<r, c, word> &x, uint64 j) {
        if (j == 0) throw math::division_by_zero {};
        return uint64 (data::divmod<Z_bytes<r, c, word>> (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Remainder);
    }

    template <endian r, negativity c, Word word>
    writer<word> inline &operator << (writer<word> &w, const Z_bytes<r, c, word> &n) {
        return w << static_cast<const bytestring<word> &> (n);
    }

    template <endian r, negativity c, Word word>
    reader<word> inline &operator >> (reader<word> &rr, Z_bytes<r, c, word> &n) {
        rr.read (n.data (), n.size ());
        return rr;
    }

    template <endian r, Word word> template <hex_case zz> inline Z_bytes<r, negativity::nones, word>::Z_bytes (const hex::uint<zz> &u) {
        *this = math::N_bytes<r, word>::zero ((u.size () / 2 - 1 + sizeof (word) - 1) / sizeof (word));
        encoding::hex::decode (u.begin () + 2, u.end (), this->words ().rbegin ());
    }

    template <endian r, Word word> template <hex_case zz> inline Z_bytes<r, negativity::twos, word>::Z_bytes (const hex::int2<zz> &u) {
        *this = math::number::Z_bytes<r, negativity::twos, word>::zero ((u.size () / 2 - 1 + sizeof (word) - 1) / sizeof (word));
        encoding::hex::decode (u.begin () + 2, u.end (), this->words ().rbegin ());
    }

    template <endian r, Word word> template <hex_case zz> inline Z_bytes<r, negativity::BC, word>::Z_bytes (const hex::intBC<zz> &u) {
        *this = math::number::Z_bytes<r, negativity::BC, word>::zero ((u.size () / 2 - 1 + sizeof (word) - 1) / sizeof (word));
        encoding::hex::decode (u.begin () + 2, u.end (), this->words ().rbegin ());
    }
}
