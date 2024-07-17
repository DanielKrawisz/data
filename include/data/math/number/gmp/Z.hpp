// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/integer.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::GMP {
    
    struct N;

    inline Z::Z (): Z {0} {}

    inline Z::~Z () {
        mpz_clear (MPZ);
    }

    inline Z::Z (gmp_int n) : MPZ {} {
        mpz_init_set_si (MPZ, n);
    }

    inline Z::Z (const std::string &x) : Z {read (x)} {};

    inline Z::Z (const Z &n) {
        mpz_init (MPZ);
        mpz_set (MPZ, n.MPZ);
    }

    inline Z::Z (Z &&n) : Z {} {
        mpz_swap (MPZ, n.MPZ);
    }

    Z inline &Z::operator = (const Z &n) {
        mpz_set (MPZ, n.MPZ);
        return *this;
    }

    Z inline &Z::operator = (Z &&n) {
        mpz_swap (MPZ, n.MPZ);
        return *this;
    }

    size_t inline Z::size () const {
        return GMP::size (MPZ[0]);
    }

    mp_limb_t inline &Z::operator [] (Z::index i) {
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
        return *(MPZ[0]._mp_d + i);
    }

    const mp_limb_t inline &Z::operator [] (Z::index i) const {
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
        return *(MPZ[0]._mp_d + i);
    }

    mp_limb_t inline *Z::begin () {
        return MPZ[0]._mp_d;
    }

    mp_limb_t inline *Z::end () {
        return MPZ[0]._mp_d + MPZ[0]._mp_alloc;
    }

    const mp_limb_t inline *Z::begin () const {
        return MPZ[0]._mp_d;
    }

    const mp_limb_t inline *Z::end () const {
        return MPZ[0]._mp_d + MPZ[0]._mp_alloc;
    };

    inline Z::operator double () const {
        return mpz_get_d (MPZ);
    }

    Z inline Z::operator ^ (uint32 n) const {
        Z pow {};
        mpz_pow_ui (pow.MPZ, MPZ, n);
        return pow;
    }

    Z inline &Z::operator ^= (uint32 n) {
        mpz_pow_ui (MPZ, MPZ, n);
        return *this;
    }
    
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
    math::signature inline sign<Z>::operator () (const Z &z) {
        return math::number::GMP::sign (z.MPZ[0]);
    }
    
    Z inline identity<plus<Z>, Z>::operator () () {
        return 0;
    }
    
    Z inline identity<times<Z>, Z>::operator () () {
        return 1;
    }
    
    Z inline inverse<plus<Z>, Z>::operator () (const Z &a, const Z &b) {
        return b - a;
    }

    Z inline times<Z>::operator () (const Z &a, const Z &b) {
        return a * b;
    }

    nonzero<Z> inline times<Z>::operator () (const nonzero<Z> &a, const nonzero<Z> &b) {
        return a * b;
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
        __gmp_binary_multiplies::eval (sum.MPZ, z.MPZ, n);
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
