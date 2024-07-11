// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/math/number/gmp/N.hpp>
#include <data/math/number/bytes/complements.hpp>

namespace data::math::number {
    
    // N_bytes is a representation of the natural numbers that
    // is also a string of bytes. It has an order parameter
    // representing endianness.
    template <endian::order r> struct N_bytes : oriented<r, byte> {

        // read in a standard number representation as a string.
        // decimal and hexidecimal are both accepted. In hexidecimal,
        // the number will be read to be identical with the hexidecimal
        // representation, including extra leading zeros.
        // for example, "0x00" and "0x" are both representations of zero
        // but different strings of bytes.
        static N_bytes read (string_view x);
        //explicit N_bytes (const std::string &);

        // read in the number as a string of bytes.
        static N_bytes read (bytes_view x);

        // trim the number to its minimal representation.
        N_bytes &trim ();

        // get a representation of zero of any size.
        static N_bytes zero (size_t size = 0);
        
        N_bytes () : oriented<r, byte> {} {}
        
        N_bytes (const uint64 x);
        
        explicit N_bytes (const std::string &x): N_bytes {read (x)} {}
        
        operator N_bytes<endian::opposite (r)> () const;
        
        template <complement c>
        explicit operator Z_bytes<r, c> () const;
        explicit operator double () const;
        
        explicit operator uint64 () const;
        
        explicit N_bytes (const N &);
        explicit operator N () const;

        N_bytes (bytestring<byte> &&b): oriented<r, byte> {b} {}

    };
    
    template <endian::order r> struct Z_bytes<r, complement::ones> : oriented<r, byte> {
        
        //explicit Z_bytes (const std::string &);
        Z_bytes () : oriented<r, byte> {} {}
        
        Z_bytes (int64 x);
        
        static Z_bytes read (string_view x);
        static Z_bytes read (bytes_view x);
        explicit Z_bytes (const std::string &x): Z_bytes {read (x)} {}
        
        operator Z_bytes<endian::opposite (r), complement::ones> () const;
        operator Z_bytes<r, complement::twos> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0);
        
        explicit operator int64 () const;
        
        explicit Z_bytes (const Z &);
        explicit operator Z () const;
        
        Z_bytes &trim ();

        Z_bytes (bytestring<byte> &&b) : oriented<r, byte> {b} {}

    };
    
    // for little endian, these are an implementation of bitcoin numbers.
    template <endian::order r> struct Z_bytes<r, complement::twos> : oriented<r, byte> {

        //explicit Z_bytes (const std::string &);
        Z_bytes () : oriented<r, byte> {} {}
        
        Z_bytes (int64 x);
        
        static Z_bytes read (string_view x);
        static Z_bytes read (bytes_view x);
        explicit Z_bytes (const std::string &x): Z_bytes {read (x)} {}
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0, bool negative = false);
        
        explicit operator int64 () const;
        
        explicit Z_bytes (const Z &);
        explicit operator Z () const;

        Z_bytes &trim ();

        Z_bytes (bytestring<byte> &&b): oriented<r, byte> {b} {}

        explicit operator bool () const {
            return !data::is_zero (*this);
        }

        operator Z_bytes<endian::opposite (r), complement::twos> () const;
        operator Z_bytes<r, complement::ones> () const;
    };

    template <endian::order r> nonzero<N_bytes<r>> inline increment<N_bytes<r>>::operator () (const N_bytes<r> &n) {
        return nonzero<N_bytes<r>> {n + 1};
    }

    template <endian::order r> N_bytes<r> inline decrement<N_bytes<r>>::operator () (const nonzero<N_bytes<r>> &n) {
        auto x = n.Value;
        return --x;
    }

    template <endian::order r> N_bytes<r> inline decrement<N_bytes<r>>::operator () (const N_bytes<r> &n) {
        if (data::is_zero (n)) return n;
        auto x = n;
        return --x;
    }

    template <endian::order r, complement c> Z_bytes<r, c> inline increment<Z_bytes<r, c>>::operator () (const Z_bytes<r, c> &n) {
        auto x = n;
        return ++x;
    }

    template <endian::order r, complement c> Z_bytes<r, c> inline decrement<Z_bytes<r, c>>::operator () (const Z_bytes<r, c> &n) {
        auto x = n;
        return --x;
    }
    
    // some functions that can easily be implemented as other functions using conversions. 
    template <endian::order r, complement c> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a <=> Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    std::weak_ordering inline operator <=> (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c> (a) <=> b;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator ~ (const N_bytes<r> &b) {
        return ~Z_bytes<r, complement::ones> (b);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator ^ (
        const Z_bytes<r, complement::ones> &a, const N_bytes<r> &b) {
        return a ^ Z_bytes<r, complement::ones> (b);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator ^ (
        const N_bytes<r> &a, const Z_bytes<r, complement::ones> &b) {
        return Z_bytes<r, complement::ones> (a) ^ b;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator ^ (const N_bytes<r> &a, const N_bytes<r> &b) {
        return Z_bytes<r, complement::ones> (a) ^ Z_bytes<r, complement::ones> (b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator & (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c> (a) & b;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator & (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return Z_bytes<r, c> (b) & a;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator | (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c> (a) | b;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator | (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return Z_bytes<r, c> (b) | a;
    }
    
    template <endian::order r> N_bytes<r> inline operator & (const N_bytes<r> &a, uint64 b) {
        return a & N_bytes<r> (b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator & (const N_bytes<r> &a, int64 b) {
        return a & Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator & (const Z_bytes<r, c> &a, uint64 b) {
        return a & N_bytes<r> (b);
    }
    
    template <endian::order r> N_bytes<r> inline operator | (const N_bytes<r> &a, uint64 b) {
        return a | N_bytes<r> (b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator | (const Z_bytes<r, c> &a, int64 b) {
        return a | Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator | (const N_bytes<r> &a, int64 b) {
        return a | Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator | (const Z_bytes<r, c> &a, uint64 b) {
        return a | N_bytes<r> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator + (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a + Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator - (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a - Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator - (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        return a + (-b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator * (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a * Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator + (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c> (a) + b;
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator - (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c> (a) - b;
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator * (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c> (a) * b;
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator + (const N_bytes<r> &a, uint64 b) {
        return a + N_bytes<r> (b);
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator - (const N_bytes<r> &a, uint64 b) {
        return a - N_bytes<r> (b);
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator * (const N_bytes<r> &a, uint64 b) {
        return a * N_bytes<r> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator + (const Z_bytes<r, c> &a, int64 b) {
        return a + Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator - (const Z_bytes<r, c> &a, int64 b) {
        return a - Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator * (const Z_bytes<r, c> &a, int64 b) {
        return a * Z_bytes<r, c> (b);
    }
    
    template <endian::order r>
    N_bytes<r> inline operator ++ (N_bytes<r> &x, int) {
        N_bytes<r> z = x;
        ++x;
        return z;
    }
    
    template <endian::order r>
    N_bytes<r> inline operator -- (N_bytes<r> &x, int) {
        N_bytes<r> z = x;
        --x;
        return z;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator += (N_bytes<r> &x, const N_bytes<r> &n) {
        return x = x + n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator -= (N_bytes<r> &x, const N_bytes<r> &n) {
        return x = x - n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator += (N_bytes<r> &x, uint64 n) {
        return x = x + n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator -= (N_bytes<r> &x, uint64 n) {
        return x = x - n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator *= (N_bytes<r> &x, const N_bytes<r> &n) {
        return x = x * n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator *= (N_bytes<r> &x, uint64 n) {
        return x = x * n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator <<= (N_bytes<r> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator >>= (N_bytes<r> &x, int64 i) {
        return x = x >> i;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline operator ++ (Z_bytes<r, c> &x, int) {
        Z_bytes<r, c> z = x;
        ++x;
        return z;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline operator -- (Z_bytes<r, c> &x, int) {
        Z_bytes<r, c> z = x;
        --x;
        return z;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator += (Z_bytes<r, c> &x, const Z_bytes<r, c>& n) {
        return x = x + n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator -= (Z_bytes<r, c> &x, const Z_bytes<r, c>& n) {
        return x = x - n;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline &operator += (Z_bytes<r, c> &x, int64 n) {
        return x = x + n;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline &operator -= (Z_bytes<r, c> &x, int64 n) {
        return x = x - n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator *= (Z_bytes<r, c> &x, const Z_bytes<r, c>& n) {
        return x = x * n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator *= (Z_bytes<r, c> &x, const int64 n) {
        return x = x * n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator <<= (Z_bytes<r, c> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator >>= (Z_bytes<r, c> &x, int64 i) {
        return x = x >> i;
    }
    
    template <endian::order r> inline Z_bytes<r, complement::twos>::Z_bytes (int64 x):
        Z_bytes {Z_bytes (Z_bytes<r, complement::ones> {x})} {}
    
    // check if the number is negative, and then do bit shift on the absolute value.
    template <endian::order r> 
    Z_bytes<r, complement::twos> inline operator << (const Z_bytes<r, complement::twos> &x, int i) {
        return Z_bytes<r, complement::twos> (Z_bytes<r, complement::ones> (x) << i);
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos> inline operator >> (const Z_bytes<r, complement::twos> &x, int i) {
        return Z_bytes<r, complement::twos> (Z_bytes<r, complement::ones> (x) >> i);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator - (const Z_bytes<r, complement::ones> &x) {
        auto z = ~x;
        return ++z;
    }
    
}

namespace data {

    template <endian::order a, endian::order b>
    bool inline identical (const math::number::N_bytes<a> &x, const math::number::N_bytes<b> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }

    template <endian::order a, math::number::complement b, endian::order c, math::number::complement d>
    bool inline identical (const math::number::Z_bytes<a, b> &x, const math::number::Z_bytes<c, d> &y) {
        return static_cast<bytes> (x) == static_cast<bytes> (y);
    }
    
}

namespace data::math {

    template <endian::order r> signature inline sign<N_bytes<r>>::operator () (const math::N_bytes<r> &x) {
        return math::number::arithmetic::nones::sign (x.words ());
    }

    template <endian::order r, math::number::complement c>
    signature inline sign<math::number::Z_bytes<r, c>>::operator () (const math::number::Z_bytes<r, c> &x) {
        if constexpr (c == math::number::complement::ones) return math::number::arithmetic::ones::sign (x.words ());
        if constexpr (c == math::number::complement::twos) return math::number::arithmetic::twos::sign (x.words ());
    }

    template <endian::order r> bool inline is_negative<N_bytes<r>>::operator () (const N_bytes<r> &) {
        return false;
    }
    
    template <endian::order r> bool inline is_positive<N_bytes<r>>::operator () (const N_bytes<r> &x) {
        return !data::is_zero (x);
    }
    
    template <endian::order r> N_bytes<r> inline abs<N_bytes<r>>::operator () (const N_bytes<r> &x) {
        return x;
    }
    
    template <endian::order r>
    number::Z_bytes<r, number::complement::twos> inline
    abs<number::Z_bytes<r, number::complement::twos>>::operator ()
    (const number::Z_bytes<r, number::complement::twos> &x)
    {
        return data::is_negative (x) ? -x : x;
    }
    
    template <endian::order r>
    number::N_bytes<r>
    abs<number::Z_bytes<r, number::complement::ones>>::operator ()
    (const number::Z_bytes<r, number::complement::ones> &x) {
        if (data::is_zero (x)) return number::N_bytes<r> {};
        number::Z_bytes<r, number::complement::ones> z = data::is_negative (x) ? -x : x;
        auto n = number::N_bytes<r>::zero (z.size ());
        std::copy (z.begin (), z.end (), n.begin ());
        return n;
    }

    template <endian::order r, number::complement zz>
    number::Z_bytes<r, zz> inline times<number::Z_bytes<r, zz>>::operator ()
    (const number::Z_bytes<r, zz> &a, const number::Z_bytes<r, zz> &b) {
        return a * b;
    }

    template <endian::order r, number::complement zz>
    nonzero<number::Z_bytes<r, zz>> inline times<number::Z_bytes<r, zz>>::operator ()
    (const nonzero<number::Z_bytes<r, zz>> &a, const nonzero<number::Z_bytes<r, zz>> &b) {
        return a * b;
    }
    
    template <endian::order r> N_bytes<r> inline quadrance<N_bytes<r>>::operator () (const N_bytes<r> &x) {
        return x * x;
    }
    
    template <endian::order r, number::complement c> auto inline 
    quadrance<number::Z_bytes<r, c>>::operator () (const number::Z_bytes<r, c> &x) -> decltype (data::abs (x)) {
        return data::abs (x) * data::abs (x);
    }
    
    template <endian::order r> N_bytes<r> inline next (const N_bytes<r> &n) {
        return number::increment<N_bytes<r>> (n).Value;
    }
    
    template <endian::order r> number::Z_bytes<r, number::complement::twos> inline next
    (const number::Z_bytes<r, number::complement::twos> &z)
    {
        return is_positive (z) ? -z : -z - 1;
    }
    
    template <endian::order r> bool inline is_negative<Z_bytes<r>>::operator () (const Z_bytes<r> &x) {
        return number::arithmetic::sign_bit (x.words ());
    }

    template <endian::order r> bool inline is_negative<Z_bytes_twos<r>>::operator () (const Z_bytes_twos<r> &x) {
        return !data::is_zero (x) && number::arithmetic::sign_bit (x.words ());
    }

    template <endian::order r> bool inline is_positive<Z_bytes<r>>::operator () (const Z_bytes<r> &x) {
        return !data::is_zero (x) && !number::arithmetic::sign_bit (x.words ());
    }

    template <endian::order r> bool inline is_positive<Z_bytes_twos<r>>::operator () (const Z_bytes_twos<r> &x) {
        return !data::is_zero (x) && !number::arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_zero<N_bytes<r>>::operator () (const N_bytes<r> &x) {
        return number::arithmetic::is_zero (x.words ());
    }
    
    template <endian::order r> bool inline is_zero<Z_bytes<r>>::operator () (const Z_bytes<r> &x) {
        return number::arithmetic::is_zero (x.words ());
    }
    
    template <endian::order r> bool inline is_zero<Z_bytes_twos<r>>::operator () (const Z_bytes_twos<r> &x) {
        return number::arithmetic::twos::is_zero (x.words ());
    }
    
    template <endian::order r> bool inline is_positive_zero<Z_bytes_twos<r>>::operator () (const Z_bytes_twos<r> &x) {
        return data::is_zero (x) && !number::arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_negative_zero<Z_bytes_twos<r>>::operator () (const Z_bytes_twos<r> &x) {
        return data::is_zero (x) && number::arithmetic::sign_bit (x.words ());
    }
    
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian::order r>
    bool inline is_minimal (const N_bytes<r> &x) {
        return arithmetic::nones::is_minimal (x.words ());
    }
    
    template <endian::order r>
    bool inline is_minimal (const Z_bytes<r, complement::ones> &x) {
        return arithmetic::ones::is_minimal (x.words ());
    }
    
    template <endian::order r>
    bool inline is_minimal (const Z_bytes<r, complement::twos> &x) {
        return arithmetic::twos::is_minimal (x.words ());
    }
    
    template <endian::order r>
    size_t inline minimal_size (const N_bytes<r> &x) {
        return arithmetic::minimal_size<math::number::complement::nones> (x.words ());
    }
    
    template <endian::order r, complement c>
    size_t inline minimal_size (const Z_bytes<r, c> &x) {
        return arithmetic::minimal_size<c> (x.words ());
    }
    
    template <endian::order r>
    N_bytes<r> inline extend (const N_bytes<r> &x, size_t size) {
        return arithmetic::extend<r, complement::nones, byte> (x, size);
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline extend (const Z_bytes<r, c> &x, size_t size) {
        return arithmetic::extend<r, c, byte> (x, size);
    }
    
    template <endian::order r> 
    bool inline operator == (const N_bytes<r> &a, const N_bytes<r> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, complement c> 
    bool inline operator == (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator <=> (const N_bytes<r> &a, const N_bytes<r> &b) {
        return arithmetic::nones::compare<r, byte> (a, b);
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b) {
        return arithmetic::ones::compare<r, byte> (a, b);
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, complement::twos> &a, const Z_bytes<r, complement::twos> &b) {
        return arithmetic::twos::compare<r, byte> (a, b);
    }
    
    template <endian::order r, complement c> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, c> &a, int64 i) {
        return a <=> Z_bytes<r, c> {i};
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator <=> (const N_bytes<r> &a, uint64 i) {
        return a <=> N_bytes<r> {i};
    }
    
    template <endian::order r> 
    bool inline operator == (const N_bytes<r> &x, uint64 i) {
        if (i < 0) return false;
        return arithmetic::compare<complement::nones> (x.words (), endian::arithmetic<false, r, 8> {i}.words ()) == 0;
    }
    
    template <endian::order r, complement c> 
    bool inline operator == (const Z_bytes<r, c> &x, int64 i) {
        return (x <=> Z_bytes<r, c> {i}) == 0;
    }

    template <endian::order r, complement cl, complement cr>
    bool inline operator == (const Z_bytes<r, cl> &a, const Z_bytes<r, cr> &b) {
        return a == Z_bytes<r, cl> (b);
    }

    template <endian::order r, complement cl, complement cr>
    std::weak_ordering inline operator <=> (const Z_bytes<r, cl> &a, const Z_bytes<r, cl> &b) {
        return a <=> Z_bytes<r, cl> (b);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator - (const N_bytes<r> &x) {
        auto z = Z_bytes<r, complement::ones> (x);
        return -z;
    }
    
    template <endian::order r> N_bytes<r> N_bytes<r>::read (bytes_view b) {
        auto x = N_bytes<r> {};
        x.resize (b.size ());
        std::copy (b.begin(), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> Z_bytes<r, complement::ones>::read (bytes_view b) {
        auto x = Z_bytes<r, complement::ones> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> Z_bytes<r, complement::twos>::read (bytes_view b) {
        auto x = Z_bytes<r, complement::ones> {};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r> 
    N_bytes<r> inline N_bytes<r>::zero (size_t size) {
        return arithmetic::zero<r, byte> (size);
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> inline Z_bytes<r, complement::ones>::zero (size_t size) {
        return arithmetic::zero<r, byte> (size);
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos> inline Z_bytes<r, complement::twos>::zero (size_t size, bool negative) {
        return arithmetic::twos::zero<r, byte> (size, negative);
    }
    
    template <endian::order r>
    Z_bytes<r, complement::twos> inline operator - (const Z_bytes<r, complement::twos> &x) {
        return arithmetic::trim<r, complement::twos, byte> (arithmetic::twos::negate<r, byte> (x));
    }

    namespace {
        template <endian::order r>
        const Z_bytes<r, complement::twos> &true_value () {
            static Z_bytes<r, complement::twos> TrueValue = Z_bytes<r, complement::twos>::read ("0x01");
            return TrueValue;
        }

        template <endian::order r>
        const Z_bytes<r, complement::twos> &false_value () {
            static Z_bytes<r, complement::twos> FalseValue = Z_bytes<r, complement::twos>::read ("0x");
            return FalseValue;
        };
    }

    template <endian::order r> Z_bytes<r, complement::twos> inline operator ! (const Z_bytes<r, complement::twos> &x) {
        return bool (x) ? false_value<r> () : true_value<r> ();
    }

    template <endian::order r> Z_bytes<r, complement::twos> inline operator &&
        (const Z_bytes<r, complement::twos> &x, const Z_bytes<r, complement::twos> &y) {
        return bool (x) && bool (y) ? true_value<r> () : false_value<r> ();
    }

    template <endian::order r> Z_bytes<r, complement::twos> inline operator ||
        (const Z_bytes<r, complement::twos> &x, const Z_bytes<r, complement::twos> &y) {
        return bool (x) || bool (y) ? true_value<r> () : false_value<r> ();
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> operator ~ (const Z_bytes<r, complement::ones> &x) {
        auto z = extend (x, x.size () + 1);
        arithmetic::bit_negate<byte> (z.words ().end (), z.words ().begin (), z.words ().begin ());
        return trim (z);
    }
    
    template <endian::order r> N_bytes<r> inline operator & (const N_bytes<r> &a, const N_bytes<r> &b) {
        return arithmetic::trim<r, complement::nones, byte> (arithmetic::bit_and<r, complement::nones, byte> (a, b));
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        return arithmetic::trim<r, c, byte> (arithmetic::bit_and<r, c, byte> (a, b));
    }
    
    template <endian::order r> N_bytes<r> operator | (const N_bytes<r> &a, const N_bytes<r> &b) {
        return arithmetic::trim<r, complement::nones, byte> (arithmetic::bit_or<r, complement::nones, byte> (a, b));
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        return arithmetic::trim<r, c, byte> (arithmetic::bit_or<r, c, byte> (a, b));
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> operator ^ (
        const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b) {
        if (a.size () < b.size ()) return b ^ a;
        auto bt = extend (b, a.size ());
        auto x = Z_bytes<r, complement::ones>::zero (a.size ());
        arithmetic::bit_xor<byte> (x.end (), x.begin (), a.begin (),
            const_cast<const Z_bytes<r, complement::ones> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> operator +
    (const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b)
    {

        bool an = data::is_negative (a);
        bool bn = data::is_negative (b);
        auto ax = data::abs (a);
        auto bx = data::abs (b);

        if (!an && !bn) return Z_bytes<r, complement::ones> (ax + bx);
        if (an && bn) return -(ax + bx);

        return ax > bx ?
            (an ? -(ax - bx) : Z_bytes<r, complement::ones> (ax - bx)) :
            (an ? Z_bytes<r, complement::ones> (bx - ax) : -(bx - ax));
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos> operator +
    (const Z_bytes<r, complement::twos> &a, const Z_bytes<r, complement::twos> &b)
    {
        return arithmetic::trim<r, complement::twos, byte> (arithmetic::twos::plus<r, byte> (trim (a), trim (b)));
    }
    
    template <endian::order r>
    Z_bytes<r, complement::ones> operator *
    (const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b)
    {
        bool an = data::is_negative (a);
        bool bn = data::is_negative (b);
        if ((an && bn) || (!an && !bn)) return Z_bytes<r, complement::ones> (data::abs (a) * data::abs (b));
        return -(data::abs (a) * data::abs (b));
    }

    template <endian::order r>
    Z_bytes<r, complement::twos> operator *
    (const Z_bytes<r, complement::twos> &a, const Z_bytes<r, complement::twos> &b)
    {
        return arithmetic::trim<r, complement::twos, byte> (arithmetic::twos::times<r, byte> (trim (a), trim (b)));
    }
    
    template <endian::order r> template<complement c> N_bytes<r>::operator Z_bytes<r, c> () const {
        Z_bytes<r, c> n = Z_bytes<r, c>::zero (this->size () + 1);
        std::copy (this->words ().begin (), this->words ().end (), n.words ().begin ());
        return n.trim ();
    }
    
    template <endian::order r> inline N_bytes<r>::N_bytes (const uint64 x) : oriented<r, byte> {} {
        this->resize (8);
        endian::arithmetic<false, r, 8> xx {x};
        std::copy (xx.begin (), xx.end (), this->begin ());
        this->trim ();
    }
    
    template <endian::order r> inline Z_bytes<r, complement::ones>::Z_bytes (int64 x) : oriented<r, byte> {} {
        this->resize (8);
        endian::arithmetic<true, r, 8> n {x};
        std::copy (n.begin (), n.end (), this->begin ());
        this->trim ();
    }

    template <endian::order r> N_bytes<r> inline trim (const N_bytes<r> &n) {
        return arithmetic::trim<r, math::number::complement::nones, byte> (n);
    }

    template <endian::order r, complement c> Z_bytes<r, c> inline trim (const Z_bytes<r, c> &z) {
        return arithmetic::trim<r, c, byte> (z);
    }
    
    template <endian::order r> N_bytes<r> inline &N_bytes<r>::trim () {
        arithmetic::trim<r, complement::nones, byte> (*this);
        return *this;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline &Z_bytes<r, complement::ones>::trim () {
        arithmetic::trim<r, complement::ones, byte> (*this);
        return *this;
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> inline &Z_bytes<r, complement::twos>::trim () {
        arithmetic::trim<r, complement::twos, byte> (*this);
        return *this;
    }
    
    template <endian::order r> N_bytes<r>::operator uint64 () const {
        if (*this > N_bytes {std::numeric_limits<uint64>::max ()}) throw std::invalid_argument {"value too big"};

        endian::arithmetic<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return uint64 (xx);
    } 
    
    template <endian::order r> Z_bytes<r, complement::ones>::operator int64 () const {
        if (*this > Z_bytes {std::numeric_limits<int64>::max ()}) throw std::invalid_argument {"value too big"};
        if (*this < Z_bytes {std::numeric_limits<int64>::min ()}) throw std::invalid_argument {"value too small"};

        endian::arithmetic<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin (),
            this->words ().begin () + std::min (static_cast<size_t> (8),
            this->size ()), xx.begin ());

        return int64 (xx);
    } 
    
    template <endian::order r> inline Z_bytes<r, complement::twos>::operator int64 () const {
        return int64 (Z_bytes<r, complement::ones> (*this));
    } 
    
    template <endian::order r> 
    Z_bytes<r, complement::ones>::operator Z_bytes<endian::opposite (r), complement::ones> () const {
        Z_bytes<endian::opposite (r), complement::ones> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian::order r> 
    inline Z_bytes<r, complement::ones>::operator Z_bytes<r, complement::twos> () const {
        if (data::is_negative (*this)) return -Z_bytes<r, complement::twos> (data::abs (*this));
        return Z_bytes<r, complement::twos> (data::abs (*this)).trim ();
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos>::operator Z_bytes<endian::opposite (r), complement::twos> () const {
        Z_bytes<endian::opposite (r), complement::twos> n;
        n.resize (this->size ());
        std::copy (this->words ().begin (), this->words ().end (), n.begin ());
        return n;
    }
    
    template <endian::order r> 
    inline Z_bytes<r, complement::twos>::operator Z_bytes<r, complement::ones> () const {
        if (data::is_zero (*this)) return Z_bytes<r, complement::ones>::zero ();
        if (data::is_negative (*this)) return -Z_bytes<r, complement::ones> (data::abs (*this));
        auto z = Z_bytes<r, complement::ones>::zero (this->size ());
        std::copy (this->begin (), this->end (), z.begin ());
        return z.trim ();
    }
    
    template <endian::order r> N_bytes<r> inline &operator ++ (N_bytes<r> &x) {
        arithmetic::nones::increment<r> (arithmetic::trim<r, complement::nones> (x));
        return x;
    }
    
    template <endian::order r> N_bytes<r> inline &operator -- (N_bytes<r> &x) {
        arithmetic::trim<r, complement::nones> (arithmetic::nones::decrement<r> (x));
        return x;
    }

    template <endian::order r, complement c> Z_bytes<r, c> inline &operator ++ (Z_bytes<r, c> &x) {
        if constexpr (c == complement::ones) arithmetic::trim<r, c> (arithmetic::ones::increment<r> (x));
        if constexpr (c == complement::twos) arithmetic::trim<r, c> (arithmetic::twos::increment<r> (x));
        return x;
    }

    template <endian::order r, complement c> Z_bytes<r, c> inline &operator -- (Z_bytes<r, c> &x) {
        if constexpr (c == complement::ones) arithmetic::trim<r, c> (arithmetic::ones::decrement<r> (x));
        if constexpr (c == complement::twos) arithmetic::trim<r, c> (arithmetic::twos::decrement<r> (x));
        return x;
    }
    
    namespace {
    
        template <endian::order r> 
        N_bytes<r> shift_left (const N_bytes<r> &x, uint32 extended, uint32 i) {
            N_bytes<r> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian::order r> 
        N_bytes<r> shift_right (const N_bytes<r> &x, uint32 i) {
            N_bytes<r> z = x;
            z.words ().bit_shift_right (i);
            return z;
        }
    
        template <endian::order r> 
        Z_bytes<r, complement::ones> shift_left (const Z_bytes<r, complement::ones> &x, uint32 extended, uint32 i) {
            Z_bytes<r, complement::ones> z = extend (x, x.size () + extended);
            z.words ().bit_shift_left (i);
            return z;
        }
        
        template <endian::order r> 
        Z_bytes<r, complement::ones> shift_right (const Z_bytes<r, complement::ones> &x, uint32 i) {
            Z_bytes<r, complement::ones> z = x;
            z.words ().bit_shift_right (i, x < 0);
            return z;
        }

        template <endian::order r>
        N_bytes<r> inline plus (const N_bytes<r> &a, const N_bytes<r> &b) {
            return arithmetic::nones::plus<r, byte> (a, b);
        }

        template <endian::order r>
        N_bytes<r> inline minus (const N_bytes<r> &a, const N_bytes<r> &b) {
            if (b > a) return N_bytes<r>::zero ();
            return arithmetic::nones::minus<r, byte> (a, b);
        }

        template <endian::order r>
        N_bytes<r> times (const N_bytes<r> &a, const N_bytes<r> &b) {
            N_bytes<r> n = N_bytes<r>::zero (a.size () + b.size () + 1);
            auto w = n.words ();
            arithmetic::times (w, a.words (), b.words ());
            return n;
        }
        
    }
    
    template <endian::order r>
    N_bytes<r> inline operator << (const N_bytes<r> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }

    template <endian::order r>
    N_bytes<r> inline operator >> (const N_bytes<r> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> inline operator << (const Z_bytes<r, complement::ones> &x, int i) {
        return trim (i < 0 ?
            shift_right (trim (x), static_cast<uint32> (-i)) :
            shift_left (trim (x), (i + 7) / 8, static_cast<uint32> (i)));
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> inline operator >> (const Z_bytes<r, complement::ones> &x, int i) {
        return trim (i < 0 ?
            shift_left (trim (x), (-i + 7) / 8, static_cast<uint32> (-i)) :
            shift_right (trim (x), static_cast<uint32> (i)));
    }
    
    template <endian::order r> N_bytes<r>::operator N_bytes<endian::opposite (r)> () const {
        N_bytes<endian::opposite (r)> z;
        z.resize (this->size ());
        std::copy (this->rbegin (), this->rend (), z.begin ());
        return z;
    }
    
    template <endian::order r> 
    N_bytes<r> operator + (const N_bytes<r> &a, const N_bytes<r> &b) {
        return trim (plus (trim (a), trim (b)));
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator - (const N_bytes<r> &a, const N_bytes<r> &b) {
        return trim (minus (trim (a), trim (b)));
    }
    
    template <endian::order r> N_bytes<r> inline operator * (const N_bytes<r> &a, const N_bytes<r> &b) {
        return trim (times (trim (a), trim (b)));
    }

    template <endian::order r> N_bytes<r>::N_bytes (const N &n) : N_bytes {} {
        for (auto i = n.Value.end () - 1; i >= n.Value.begin (); i--)
            *this = shift_left (*this, sizeof (mp_limb_t), sizeof (mp_limb_t) * 8) + *i;

        this->trim ();
    }
    
    template <endian::order r> Z_bytes<r, complement::ones>::Z_bytes (const Z &z) {
        auto x = Z_bytes<r, complement::ones> (N_bytes<r> {data::abs (z)});
        *this = z < 0 ? -x : x;
        this->trim ();
    }
    
    template <endian::order r> Z_bytes<r, complement::twos>::Z_bytes (const Z &z) {
        *this = Z_bytes<r, complement::twos> (N_bytes<r> {data::abs (z)});
        if (z < 0) *this = -*this;
        this->trim ();
    }
    
    template <endian::order r> N_bytes<r>::operator N () const {
        N x {};

        for (const byte &b : this->words ().reverse ()) {
            x <<= 8;
            x += b;
        }

        return x;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones>::operator Z () const {
        N x = N (data::abs (*this));
        return data::is_negative (*this) ? -x : Z (x);
    }
    
    template <endian::order r> Z_bytes<r, complement::twos>::operator Z () const {
        auto ab = data::abs (*this);
        N x {};

        for (const byte &b : ab.words ().reverse ()) {
            x <<= 8;
            x += b;
        }

        return data::is_negative (*this) ? -x : Z (x);
    }
}

#endif
