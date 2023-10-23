// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/math/number/gmp/N.hpp>
#include <data/cross.hpp>
#include <data/math/arithmetic.hpp>

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

        // read in the number as a string of bytes.
        static N_bytes read (bytes_view x);

        // trim the number to its minimal representation.
        N_bytes trim () const;

        // get a representation of zero of any size.
        static N_bytes zero (size_t size = 0);
        
        N_bytes () : oriented<r, byte> {} {}
        
        N_bytes (const uint64 x);
        
        explicit N_bytes (const string &x): N_bytes {read (x)} {}
        
        operator N_bytes<endian::opposite (r)> () const;
        
        template <complement c>
        explicit operator Z_bytes<r, c> () const;
        explicit operator double () const;
        
        explicit operator uint64 () const;
        
        explicit N_bytes (const N &);
        explicit operator N () const;

    private:
        N_bytes (size_t size, byte fill) : oriented<r, byte>(size, fill) {}
    };
    
    template <endian::order r> struct Z_bytes<r, complement::ones> : oriented<r, byte> {
        
        Z_bytes () : oriented<r, byte> {} {}
        
        Z_bytes (int64 x);
        
        static Z_bytes read (string_view x);
        static Z_bytes read (bytes_view x);
        explicit Z_bytes (const string &x): Z_bytes {read (x)} {}
        
        operator Z_bytes<endian::opposite (r), complement::ones> () const;
        operator Z_bytes<r, complement::twos> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0);
        
        explicit operator int64 () const;
        
        explicit Z_bytes (const Z &);
        explicit operator Z () const;
        
        Z_bytes trim () const;
        
    private:
        Z_bytes (size_t size, byte fill) : oriented<r, byte> (size, fill) {}
    };
    
    template <endian::order r> struct Z_bytes<r, complement::twos> : oriented<r, byte> {
        Z_bytes () : oriented<r, byte> {} {}
        
        Z_bytes (int64 x);
        
        static Z_bytes read (string_view x);
        static Z_bytes read (bytes_view x);
        explicit Z_bytes (const string &x): Z_bytes {read (x)} {}
        
        operator Z_bytes<endian::opposite (r), complement::twos> () const;
        operator Z_bytes<r, complement::ones> () const;
        
        explicit operator double () const;
        
        static Z_bytes zero (size_t size = 0, bool negative = false);
        
        explicit operator int64 () const;
        
        explicit Z_bytes (const Z &);
        explicit operator Z () const;
        
        Z_bytes trim () const;
        
    private:
        Z_bytes (size_t size, byte fill) : oriented<r, byte> (size, fill) {}
    };
    
    template <endian::order r> N_bytes<r> inline trim (const N_bytes<r> &n) {
        return n.trim ();
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline trim (const Z_bytes<r, c> &z) {
        return z.trim ();
    }
    
    // some functions that can easily be implemented as other functions using conversions. 
    template <endian::order r, complement c> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a <=> Z_bytes<r, c> (b);
    }
    
    template <endian::order r, complement c> 
    std::weak_ordering inline operator <=> (const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) <=> b;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator ~ (const N_bytes<r> &b) {
        return ~Z_bytes<r, complement::ones> (b);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator^(
        const Z_bytes<r, complement::ones> &a, const N_bytes<r> &b) {
        return a ^ Z_bytes<r, complement::ones> (b);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator^(
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

    template <endian::order r, complement c> Z_bytes<r, c> inline operator & (const Z_bytes<r, c> &a, int64 b) {
        return a & Z_bytes<r, c> (b);
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
    Z_bytes<r, c> inline operator -(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
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
        return a * Z_bytes<r, c>(b);
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
    N_bytes<r> inline &operator += (N_bytes<r> &x, const N_bytes<r>& n) {
        return x = x + n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator -= (N_bytes<r> &x, const N_bytes<r>& n) {
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
    N_bytes<r> inline &operator *= (N_bytes<r> &x, const N_bytes<r>& n) {
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
        auto n = is_negative (x);
        throw method::unimplemented {" twos << "};
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos> inline operator >> (const Z_bytes<r, complement::twos> &x, int i) {
        auto n = is_negative (x);
        throw method::unimplemented {" twos >> "};
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline operator - (const Z_bytes<r, complement::ones> &x) {
        auto z = ~x;
        return ++z;
    }
    
}

namespace data {
    
    template <endian::order r> math::number::N_bytes<r> inline increment (const math::number::N_bytes<r> &n) {
        auto x = n;
        return ++x;
    }
    
    template <endian::order r, math::number::complement c> 
    math::number::Z_bytes<r, c> inline increment (const math::number::Z_bytes<r, c> &n) {
        auto x = n;
        return ++x; 
    }
    
    template <endian::order r> math::number::N_bytes<r> inline decrement (const math::number::N_bytes<r> &n) {
        auto x = n;
        return --x;
    }
    
    template <endian::order r, math::number::complement c> 
    math::number::Z_bytes<r, c> inline decrement (const math::number::Z_bytes<r, c> &n) {
        auto x = n;
        return --x;
    }
    
    template <endian::order r> math::sign inline sign (const math::N_bytes<r> &x) {
        return math::arithmetic::N_sign (x.words ());
    }
    
    template <endian::order r, math::number::complement c> math::sign inline sign (const math::number::Z_bytes<r, c> &x) {
        return math::is_zero (x) ? math::zero : math::arithmetic::sign_bit (x.words ()) ? math::negative : math::positive;
    }
    
}

namespace data::math {
    template <endian::order r> bool inline is_negative (const N_bytes<r> &) {
        return false;
    }
    
    template <endian::order r> bool inline is_positive (const N_bytes<r> &x) {
        return !is_zero (x);
    }
    
    template <endian::order r> N_bytes<r> inline abs<N_bytes<r>>::operator () (const N_bytes<r> &x) {
        return x;
    }
    
    template <endian::order r> number::Z_bytes<r, number::complement::twos> inline 
    abs<number::Z_bytes<r, number::complement::twos>>::operator () (const number::Z_bytes<r, number::complement::twos> &x) {
        return is_negative (x) ? -x : x;
    }
    
    template <endian::order r> number::N_bytes<r> 
    abs<number::Z_bytes<r, number::complement::ones>>::operator () (const number::Z_bytes<r, number::complement::ones> &x) {
        if (is_zero (x)) return number::N_bytes<r> {};
        number::Z_bytes<r, number::complement::ones> z = is_negative (x) ? -x : x;
        auto n = number::N_bytes<r>::zero (z.size ());
        std::copy (z.begin (), z.end (), n.begin ());
        return n;
    }
    
    template <endian::order r> N_bytes<r> inline quadrance<N_bytes<r>>::operator () (const N_bytes<r> &x) {
        return x * x;
    }
    
    template <endian::order r, number::complement c> auto inline 
    quadrance<number::Z_bytes<r, c>>::operator () (const number::Z_bytes<r, c> &x) -> decltype (data::abs (x)) {
        return data::abs (x) * data::abs (x);
    }
    
    template <endian::order r> N_bytes<r> inline next(const N_bytes<r> &n) {
        return increment (n);
    }
    
    template <endian::order r> number::Z_bytes<r, number::complement::twos> inline next(
        const number::Z_bytes<r, number::complement::twos> &z) {
        return is_positive (z) ? -z : -z - 1;
    }
    
    template <endian::order r> bool inline is_negative (const Z_bytes<r> &x) {
        return arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_negative (const Z_bytes_twos<r> &x) {
        return !is_zero (x) && arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_positive (const Z_bytes<r> &x) {
        return !is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_positive (const Z_bytes_twos<r> &x) {
        return !is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_zero (const N_bytes<r> &x) {
        return arithmetic::ones_is_zero (x.words ());
    }
    
    template <endian::order r> bool inline is_zero (const Z_bytes<r> &x) {
        return arithmetic::ones_is_zero (x.words ());
    }
    
    template <endian::order r> bool inline is_zero (const Z_bytes_twos<r> &x) {
        return arithmetic::twos_is_zero (x.words ());
    }
    
    template <endian::order r> bool inline is_positive_zero (const Z_bytes_twos<r> &x) {
        return is_zero (x) && !arithmetic::sign_bit (x.words ());
    }
    
    template <endian::order r> bool inline is_negative_zero (const Z_bytes_twos<r> &x) {
        return is_zero (x) && arithmetic::sign_bit (x.words ());
    }
    
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian::order r> bool inline is_minimal (const N_bytes<r> &x) {
        return arithmetic::N_is_minimal (x.words ());
    }
    
    template <endian::order r> bool inline is_minimal (const Z_bytes<r, complement::ones> &x) {
        return arithmetic::ones_is_minimal (x.words ());
    }
    
    template <endian::order r> bool inline is_minimal (const Z_bytes<r, complement::twos> &x) {
        return arithmetic::twos_is_minimal (x.words ());
    }
    
    template <endian::order r> size_t inline minimal_size (const N_bytes<r> &x) {
        return arithmetic::N_minimal_size (x.words ());
    }
    
    template <endian::order r> size_t inline minimal_size (const Z_bytes<r, complement::ones> &x) {
        return arithmetic::ones_minimal_size (x.words ());
    }
    
    template <endian::order r> size_t inline minimal_size (const Z_bytes<r, complement::twos> &x) {
        return arithmetic::twos_minimal_size (x.words ());
    }
    
    template <endian::order r> N_bytes<r> extend (const N_bytes<r> &x, size_t size) {
        if (size < x.size ()) {
            size_t min_size = minimal_size (x);
            if (size < min_size) throw std::invalid_argument {"cannot extend smaller than minimal size"};
            return extend (trim (x), size);
        }
        
        if (size == x.size ()) return x;
        
        N_bytes<r> z;
        z.resize (size);
        
        auto i = z.words ().rbegin ();
        for (int n = 0; n < size - x.size (); n++) {
            *i = 0;
            i++;
        }
        
        std::copy (x.words ().rbegin (), x.words ().rend (), i);
        return z;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> extend (const Z_bytes<r, complement::ones> &x, size_t size) {
        if (size < x.size ()) {
            size_t min_size = minimal_size (x);
            if (size < min_size) throw std::invalid_argument {"cannot extend smaller than minimal size"};
            return extend (trim(x), size);
        }
        
        if (size == x.size ()) return x;
        
        Z_bytes<r, complement::ones> z;
        z.resize (size);
        byte extend_digit = is_negative (x) ? 0xff : 0x00;
        
        auto i = z.words ().rbegin ();
        for (int n = 0; n < size - x.size (); n++) {
            *i = extend_digit;
            i++;
        }
        
        std::copy (x.words ().rbegin (), x.words ().rend (), i);
        return z;
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> extend(const Z_bytes<r, complement::twos> &x, size_t size) {
        if (size < x.size ()) {
            size_t min_size = minimal_size(x); 
            if (size < min_size) throw std::invalid_argument{"cannot extend smaller than minimal size"};
            return extend(trim (x), size);
        }
        
        if (size == x.size ()) return x;
        
        auto z = Z_bytes<r, complement::twos>::zero (size);
        
        if (x.size () == 0) return z;
        
        auto xw = x.words ();
        auto zw = z.words ();
        std::copy (xw.begin (), xw.end () - 1, zw.begin ());
        byte last = xw[-1];
        
        zw[-1] = last & 0x80 ? 0x80 : 0x00;
        zw[x.size () - 1] = last & 0x7f;
        return z;
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
        return arithmetic::N_compare (a.words (), b.words());
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b) {
        bool na = is_negative (a);
        bool nb = is_negative (b);
        
        if (na && nb) {
            auto ya = -b;
            auto yb = -a;
            return arithmetic::N_compare (ya.words(), yb.words());
        }
        
        if (!na && !nb) return arithmetic::N_compare (a.words(), b.words());
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator <=> (const Z_bytes<r, complement::twos> &a, const Z_bytes<r, complement::twos> &b) {
        
        sign na = data::sign (a);
        sign nb = data::sign (b);
        
        if (na == zero) switch (nb) {
            case positive: return std::weak_ordering::less;
            case negative: return std::weak_ordering::greater;
            default: return std::weak_ordering::equivalent;
        }
        
        if (nb == zero) return na == positive ? std::weak_ordering::greater : std::weak_ordering::less;
        
        if (na == positive && nb == positive) return arithmetic::N_compare (a.words (), b.words ());

        if (na == negative && nb == negative) {
            auto ya = -b;
            auto yb = -a;
            return arithmetic::N_compare (ya.words (), yb.words ());
        }
        
        return na == negative ? std::weak_ordering::less : std::weak_ordering::greater;
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
        return arithmetic::N_compare (x.words (), endian::arithmetic<false, r, 8> {i}.words ()) == 0;
    }
    
    template <endian::order r, complement c> 
    bool inline operator == (const Z_bytes<r, c> &x, int64 i) {
        return (x <=> Z_bytes<r, c> {i}) == 0;
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
        auto x = Z_bytes<r, complement::ones>{};
        x.resize (b.size ());
        std::copy (b.begin (), b.end (), x.begin ());
        return x;
    }
    
    template <endian::order r> 
    N_bytes<r> inline N_bytes<r>::zero (size_t size) {
        return N_bytes {size, 0x00};
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> inline Z_bytes<r, complement::ones>::zero (size_t size) {
        return Z_bytes {size, 0x00};
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos> Z_bytes<r, complement::twos>::zero (size_t size, bool negative) {
        if (size == 0 && negative) throw std::invalid_argument {"invalid size for negative zero"};
        auto z = Z_bytes (size, 0x00);
        if (negative) z.words ()[-1] = 0x80;
        return z;
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> inline operator - (const Z_bytes<r, complement::twos> &x) {
        if (x.size () == 0) return x;
        auto z = x;
        arithmetic::flip_sign_bit (z.words ());
        return z;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> operator ~ (const Z_bytes<r, complement::ones> &x) {
        auto z = extend (x, x.size () + 1);
        data::arithmetic::bit_negate<byte> (z.words ().end (), z.words ().begin (), z.words ().begin ());
        return trim (z);
    }
    
    template <endian::order r> N_bytes<r> operator & (const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a.size () < b.size ()) return b & a;
        auto bt = extend (b, a.size ());
        auto x = N_bytes<r>::zero (a.size ());
        data::arithmetic::bit_and<byte> (x.end (), x.begin (), a.begin (),
            const_cast<const N_bytes<r> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        if (a.size () < b.size ()) return b & a;
        auto bt = extend (b, a.size ());
        auto x = Z_bytes<r, c>::zero (a.size ());
        data::arithmetic::bit_and<byte> (x.end (), x.begin (), a.begin (),
            const_cast<const Z_bytes<r, c> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r> N_bytes<r> operator | (const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a.size () < b.size ()) return b | a;
        auto bt = extend (b, a.size ());
        auto x = N_bytes<r>::zero (a.size ());
        data::arithmetic::bit_or<byte> (x.end (), x.begin (), a.begin (), const_cast<const N_bytes<r>&> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        if (a.size () < b.size ()) return b | a;
        auto bt = extend (b, a.size ());
        auto x = Z_bytes<r, c>::zero (a.size ());
        data::arithmetic::bit_or<byte> (x.end (), x.begin (), a.begin (),
            const_cast<const Z_bytes<r, c> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> operator ^ (
        const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b) {
        if (a.size () < b.size ()) return b ^ a;
        auto bt = extend (b, a.size ());
        auto x = Z_bytes<r, complement::ones>::zero (a.size ());
        data::arithmetic::bit_xor<byte> (x.end (), x.begin (), a.begin (),
            const_cast<const Z_bytes<r, complement::ones> &> (bt).begin ());
        return x.trim ();
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> operator + (const Z_bytes<r, complement::ones> &a, const Z_bytes<r, complement::ones> &b) {

        bool an = is_negative (a);
        bool bn = is_negative (b);
        auto ax = data::abs (a);
        auto bx = data::abs (b);

        if (!an && !bn) return Z_bytes<r, complement::ones> (ax + bx);
        if (an && bn) return -(ax + bx);

        return trim (ax > bx ?
            (an ? -(ax - bx) : Z_bytes<r, complement::ones> (ax - bx)) :
            (an ? Z_bytes<r, complement::ones> (bx - ax) : -(bx - ax)));
    }
    
    template <endian::order r> 
    Z_bytes<r, complement::twos> operator + (const Z_bytes<r, complement::twos> &a, const Z_bytes<r, complement::twos> &b) {
        throw method::unimplemented {"Z_bytes + Z_bytes"};
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator * (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        bool an = is_negative (a);
        bool bn = is_negative (b);
        if ((an && bn) || (!an && !bn)) return Z_bytes<r, c> (data::abs (a) * data::abs (b));
        return -(data::abs (a) * data::abs (b));
    }
    
    template <endian::order r> template<complement c> N_bytes<r>::operator Z_bytes<r, c> () const {
        Z_bytes<r, c> n = Z_bytes<r, c>::zero (this->size () + 1);
        std::copy (this->words ().begin (), this->words ().end(), n.words().begin());
        return n.trim();
    }
    
    template <endian::order r> inline N_bytes<r>::N_bytes (const uint64 x) : oriented<r, byte> {} {
        this->resize (8);
        endian::arithmetic<false, r, 8> xx {x};
        std::copy (xx.begin (), xx.end (), this->begin ());
        *this = this->trim ();
    }
    
    template <endian::order r> inline Z_bytes<r, complement::ones>::Z_bytes (int64 x) : oriented<r, byte> {} {
        this->resize (8);
        endian::arithmetic<true, r, 8> n {x};
        std::copy (n.begin (), n.end (), this->begin ());
        *this = this->trim ();
    }
    
    template <endian::order r> N_bytes<r> N_bytes<r>::trim () const {
        size_t size = minimal_size (*this);
        if (size == this->size ()) return *this;
        auto n = N_bytes<r>::zero (size);
        auto w = this->words ();
        std::copy (w.begin (), w.begin () + size, n.words ().begin ());
        return n;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> Z_bytes<r, complement::ones>::trim () const {
        size_t size = minimal_size (*this);
        if (size == this->size ()) return *this;
        auto n = Z_bytes<r, complement::ones>::zero (size);
        auto w = this->words ();
        std::copy (w.begin (), w.begin() + size, n.words ().begin ());
        return n;
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> Z_bytes<r, complement::twos>::trim () const {
        size_t size = minimal_size (*this);
        if (size == this->size ()) return *this;
        auto n = Z_bytes<r, complement::twos>::zero (size);
        auto w = this->words ();
        std::copy (w.begin (), w.begin() + size, n.words ().begin ());
        if (size != 0) n.words ()[-1] |= (this->words ()[-1] & 0x80);
        return n;
    }
    
    template <endian::order r> N_bytes<r>::operator uint64 () const {
        if (*this > N_bytes{std::numeric_limits<uint64>::max ()}) throw std::invalid_argument {"value too big"};
        endian::arithmetic<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin (), this->words ().begin () + std::min (static_cast<size_t> (8), this->size ()), xx.begin ());
        return uint64 (xx);
    } 
    
    template <endian::order r> Z_bytes<r, complement::ones>::operator int64 () const {
        if (*this > Z_bytes {std::numeric_limits<int64>::max ()}) throw std::invalid_argument {"value too big"};
        if (*this < Z_bytes {std::numeric_limits<int64>::min ()}) throw std::invalid_argument {"value too small"};
        endian::arithmetic<false, endian::little, 8> xx {0};
        std::copy (this->words ().begin(), this->words ().begin () + std::min (static_cast<size_t> (8), this->size ()), xx.begin ());
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
        if (is_negative (*this)) return -Z_bytes<r, complement::twos> (data::abs (*this));
        return Z_bytes<r, complement::twos> (data::abs (*this));
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
        if (is_negative (*this)) return -Z_bytes<r, complement::ones> (data::abs (*this));
        auto z = Z_bytes<r, complement::ones>::zero (this->size ());
        std::copy (this->begin (), this->end (), z.begin ());
        return z;
    }
    
    template <endian::order r> N_bytes<r> &operator ++ (N_bytes<r> &x) {
        x = trim (x);

        auto o = x.words ().begin ();
        auto i = x.words ().begin ();
        auto remainder = data::arithmetic::plus<byte> (x.words ().end (), o, 1, i);
        
        if (remainder != 0) {
            x = extend (x, x.size () + 1);
            x.words ()[-1] = remainder;
        }
        
        return x;
    }
    
    template <endian::order r> N_bytes<r> inline &operator -- (N_bytes<r> &x) {
        auto xx = x.words ().begin ();
        auto xy = x.words ().begin ();
        if (!is_zero (x)) data::arithmetic::minus<byte> (x.words ().end (), xx, 1, xy);
        return x = trim (x);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> &operator ++ (Z_bytes<r, complement::ones> &x) {
        x = extend (x, x.size () + 1);

        auto oit = x.words ().begin ();
        auto iit = x.words ().begin ();

        auto remainder = data::arithmetic::plus<byte> (x.words ().end (), oit, 1, iit);
        return x = trim (x);
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> &operator -- (Z_bytes<r, complement::ones> &x) {
        if (is_zero (x)) return x = Z_bytes<r, complement::ones> {-1};

        x = extend (x, x.size () + 1);

        auto xx = x.words ().begin ();
        auto xy = x.words ().begin ();
        auto remainder = data::arithmetic::minus<byte> (x.words ().end (), xx, 1, xy);
        
        return x = trim (x);
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> &operator ++ (Z_bytes<r, complement::twos> &x) {
        if (is_zero (x)) return x = Z_bytes<r, complement::twos> {1};
        if (arithmetic::sign_bit (x.words ())) return x = -decrement (-x);

        auto oit = x.words ().begin ();
        auto iit = x.words ().begin ();
        
        auto remainder = data::arithmetic::plus<byte> (x.words ().end (), oit, 1, iit);
        if (remainder != 0) {
            x = extend (x, x.size () + 1);
            x.words ()[-1] = remainder;
        }
        
        return x = trim (x);
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> &operator -- (Z_bytes<r, complement::twos> &x) {
        if (is_zero (x)) return x = Z_bytes<r, complement::twos> {-1};
        if (arithmetic::sign_bit (x.words ())) return x = -increment (-x);
        
        auto xx = x.words ().begin ();
        auto xy = x.words ().begin ();
        data::arithmetic::minus<byte> (x.words ().end (), xx, 1, xy);
        return x = trim (x);
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
        N_bytes<r> plus (const N_bytes<r> &a, const N_bytes<r> &b) {
            N_bytes<r> n = N_bytes<r>::zero (std::max (a.size (), b.size ()) + 1);
            auto w = n.words ();
            data::math::arithmetic::plus (w, a.words (), b.words ());
            return n;
        }

        template <endian::order r>
        N_bytes<r> minus (const N_bytes<r> &a, const N_bytes<r> &b) {
            if (b > a) return N_bytes<r>::zero ();
            N_bytes<r> n = N_bytes<r>::zero (a.size () + 1);
            auto w = n.words ();
            data::math::arithmetic::minus (w, a.words (), b.words ());
            return n;
        }

        template <endian::order r>
        N_bytes<r> times (const N_bytes<r> &a, const N_bytes<r> &b) {
            N_bytes<r> n = N_bytes<r>::zero (a.size () + b.size () + 1);
            auto w = n.words ();
            data::math::arithmetic::times (w, a.words (), b.words ());
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

        *this = this->trim ();
    }
    
    template <endian::order r> Z_bytes<r, complement::ones>::Z_bytes (const Z &z) {
        auto x = Z_bytes<r, complement::ones> (N_bytes<r> {data::abs (z)});
        *this = z < 0 ? -x : x;

    }
    
    template <endian::order r> Z_bytes<r, complement::twos>::Z_bytes (const Z &z) {
        auto x = Z_bytes<r, complement::twos> (N_bytes<r> {data::abs (z)});
        *this = z < 0 ? -x : x;
    }
    
    template <endian::order r> N_bytes<r>::operator N () const {
        N x {};
        for (const byte &b : this->words ()) {
            x << 8;
            x += b;
        }
        return x;
    }
    
    template <endian::order r> Z_bytes<r, complement::ones>::operator Z () const {
        N x = N (data::abs (*this));
        return is_negative (*this) ? -x : Z (x);
    }
    
    template <endian::order r> Z_bytes<r, complement::twos>::operator Z () const {
        auto ab = data::abs (*this);
        N x {};
        for (const byte &b : ab.words ()) {
            x << 8;
            x += b;
        }
        return is_negative (*this) ? -x : Z (x);
    }
}

#endif
