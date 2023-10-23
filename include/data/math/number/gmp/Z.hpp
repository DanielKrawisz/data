// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/integer.hpp>
#include <data/encoding/integer.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::number::GMP {
    
    struct N;
    
    struct Z {
        mpz_t MPZ;
        
        Z (): Z {0} {}
        
        Z (const N &);
        
        virtual ~Z () {
            mpz_clear (MPZ);
        }
        
        Z (gmp_int n) : MPZ {} {
            mpz_init_set_si (MPZ, n);
        }
        
        static Z read (string_view x);
        
        explicit Z (const string &x) : Z {read (x)} {};
        
        Z (const Z &n) {
            mpz_init (MPZ);
            mpz_set (MPZ, n.MPZ);
        }
        
        Z (Z&& n) : Z {} {
            mpz_swap (MPZ, n.MPZ);
        }
        
        Z& operator = (const Z &n) {
            mpz_set (MPZ, n.MPZ);
            return *this;
        }
        
        Z& operator = (Z &&n) {
            mpz_swap (MPZ, n.MPZ);
            return *this;
        }
        
        size_t size () const {
            return GMP::size (MPZ[0]);
        }
        
        using index = uint32;
        
        mp_limb_t &operator [] (index i) {
            if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
            return *(MPZ[0]._mp_d + i);
        }
        
        const mp_limb_t &operator [] (index i) const {
            if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
            return *(MPZ[0]._mp_d + i);
        }
        
        mp_limb_t *begin () {
            return MPZ[0]._mp_d;
        }
        
        mp_limb_t *end () {
            return MPZ[0]._mp_d + MPZ[0]._mp_alloc;
        }
        
        const mp_limb_t *begin () const {
            return MPZ[0]._mp_d;
        }
        
        const mp_limb_t *end () const {
            return MPZ[0]._mp_d + MPZ[0]._mp_alloc;
        };
        
        explicit operator int64 () const;
        
        explicit operator double () const {
            return mpz_get_d (MPZ);
        }
        
        Z operator ^ (uint32 n) const {
            Z pow {};
            mpz_pow_ui (pow.MPZ, MPZ, n);
            return pow;
        }
        
        Z& operator ^= (uint32 n) {
            mpz_pow_ui (MPZ, MPZ, n);
            return *this;
        }
        
        division<Z, N> divide (const Z &z) const;
        
        template <endian::order o> 
        explicit Z (const N_bytes<o> &b) : Z (bytes_view (b), o) {}
        
    };
    
    bool inline operator == (const Z &a, const Z &b) {
        return a <=> b == 0;
    }
    
    bool inline operator == (const Z &a, int64 b) {
        return a <=> b == 0;
    }
    
    std::weak_ordering inline operator <=> (const Z &a, const Z &b) {
        auto cmp = mpz_cmp (a.MPZ, b.MPZ);
        return cmp < 0 ? std::weak_ordering::less : 
            cmp > 0 ? std::weak_ordering::greater : std::weak_ordering::equivalent;
    }
    
    std::weak_ordering inline operator <=> (const Z &a, int64 b) {
        auto cmp = mpz_cmp_si (a.MPZ, b);
        return cmp < 0 ? std::weak_ordering::less : 
            cmp > 0 ? std::weak_ordering::greater : std::weak_ordering::equivalent;
    }
    
    Z inline &operator /= (Z &a, const Z& z) {
        return a = a / z;
    }
    
    Z inline operator ++ (Z &n, int) {
        Z z = n;
        ++ (n);
        return z;
    }
    
    Z inline operator -- (Z &n, int) {
        Z z = n;
        ++ (n);
        return z;
    }
    
}

namespace data::math {
    
    Z inline identity<plus<Z>, Z>::operator () () {
        return 0;
    }
    
    Z inline identity<times<Z>, Z>::operator () () {
        return 1;
    }
    
    Z inline inverse<plus<Z>, Z>::operator () (const Z &a, const Z &b) {
        return b - a;
    }
}

namespace data::encoding::hexidecimal { 
    
    template <math::number::complement n, hex_case zz> integer<n, zz> inline write (const math::Z &z) {
        std::stringstream ss;
        write (ss, z, zz, n);
        return integer<n, zz> {ss.str ()};
    }
    
}

namespace data::encoding::signed_decimal { 
    
    string inline write (const math::Z &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
    
}

namespace data::math::number::GMP {
    
    Z inline &operator ++ (Z &n) {
        __gmp_unary_increment::eval (n.MPZ);
        return n;
    }
    
    Z inline &operator -- (Z &n) {
        __gmp_unary_decrement::eval (n.MPZ);
        return n;
    }
    
    Z inline &operator += (Z &z, int64 n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator += (Z &z, const Z& n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator -= (Z &z, int64 n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator -= (Z &z, const Z &n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator *= (Z &z, int64 n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator *= (Z &z, const Z& n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator &= (Z &a, const Z &b) {
        __gmp_binary_and::eval (a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline &operator |= (Z &a, const Z &b) {
        __gmp_binary_ior::eval (a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline operator - (const Z &n) {
        Z z {n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    
    Z inline operator + (const Z &z, int64 n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, n);
        return sum;
    }
    
    Z inline operator + (int64 n, const Z &z) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n, z.MPZ);
        return sum;
    }
    
    Z inline operator + (const Z &z, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator - (const Z &z, int64 n) {
        Z sum;
        __gmp_binary_minus::eval (sum.MPZ, z.MPZ, n);
        return sum;
    }
    
    Z inline operator - (int64 n, const Z &z) {
        Z sum;
        __gmp_binary_minus::eval(sum.MPZ, n, z.MPZ);
        return sum;
    }
    
    Z inline operator - (const Z &z, const Z &n) {
        Z sum;
        __gmp_binary_minus::eval (sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator * (const Z &z, int64 n) {
        Z sum;
        __gmp_binary_multiplies::eval(sum.MPZ, z.MPZ, n);
        return sum;
    }
    
    Z inline operator * (int64 n, const Z &z) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n, z.MPZ);
        return sum;
    }
    
    Z inline operator * (const Z &z, const Z &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator & (const Z &a, const Z &b) {
        Z x;
        __gmp_binary_and::eval (x.MPZ, a.MPZ, b.MPZ);
        return x;
    }
    
    Z inline operator | (const Z &a, const Z &b) {
        Z x;
        __gmp_binary_ior::eval (x.MPZ, a.MPZ, b.MPZ);
        return x;
    }
    
    Z inline operator << (const Z &a, int x) {
        Z n;
        __gmp_binary_lshift::eval (n.MPZ, a.MPZ, x);
        return n;
    }
    
    Z inline operator >> (const Z &a, int x) {
        Z n;
        __gmp_binary_rshift::eval (n.MPZ, a.MPZ, x);
        return n;
    }
    
    Z inline &operator <<= (Z &a, int x) {
        __gmp_binary_lshift::eval (a.MPZ, a.MPZ, x);
        return a;
    }
    
    Z inline &operator >>= (Z &a, int x) {
        __gmp_binary_rshift::eval (a.MPZ, a.MPZ, x);
        return a;
    }
}

#endif
