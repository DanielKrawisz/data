// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/number/gmp/mpz.hpp>
#include <data/encoding/integer.hpp>

namespace data::encoding::hexidecimal { 
    
    template <math::number::complement n, hex_case zz> integer<n, zz> inline write (const math::Z &z) {
        std::stringstream ss;
        write (ss, z, zz, n);
        return integer<n, zz> {ss.str ()};
    }

    template <hex_case zz> integer<complement::nones, zz> inline write (const math::N &n) {
        std::stringstream ss;
        write (ss, n, zz);
        return integer<complement::nones, zz> {ss.str ()};
    }

}

namespace data::encoding::signed_decimal { 
    
    string inline write (const math::Z &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
    
}

namespace data::encoding::decimal {

    string inline write (const math::N &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }

}

namespace data::math::number::GMP {
    /*
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
    
    Z inline &operator *= (Z &z, const Z &n) {
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
    }*/
    
    Z inline operator - (const Z &n) {
        Z z {n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    /*
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
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, z.MPZ, n);
        return diff;
    }
    
    Z inline operator - (int64 n, const Z &z) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, n, z.MPZ);
        return diff;
    }
    
    Z inline operator - (const Z &z, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, z.MPZ, n.MPZ);
        return diff;
    }
    
    Z inline operator * (const Z &z, int64 n) {
        Z prod;
        __gmp_binary_multiplies::eval (prod.MPZ, z.MPZ, n);
        return prod;
    }
    
    Z inline operator * (int64 n, const Z &z) {
        Z prod;
        __gmp_binary_multiplies::eval (prod.MPZ, n, z.MPZ);
        return prod;
    }
    
    Z inline operator * (const Z &z, const Z &n) {
        Z prod;
        __gmp_binary_multiplies::eval (prod.MPZ, z.MPZ, n.MPZ);
        return prod;
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
    }*/
}

namespace data::math {

    N inline abs<Z>::operator () (const Z &z) {
        return N {z < 0 ? -z : z};
    }
}

namespace data::math::number {

    Z inline increment<Z>::operator () (const Z &z) {
        auto n = z;
        return ++n;
    }

    Z inline decrement<Z>::operator () (const Z &z) {
        auto n = z;
        return --n;
    }
/*
    N<GMP::Z> inline operator + (const N<GMP::Z> &n, uint64 u) {
        N<GMP::Z> sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, n.Value.MPZ, u);
        return sum;
    }

    N<GMP::Z> inline operator - (const N<GMP::Z> &n, uint64 u) {
        if (n <= u) return 0;
        N<GMP::Z> diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, n.Value.MPZ, u);
        return diff;
    }

    N<GMP::Z> inline operator * (const N<GMP::Z> &n, uint64 u) {
        N<GMP::Z> prod;
        __gmp_binary_multiplies::eval (prod.Value.MPZ, n.Value.MPZ, u);
        return prod;
    }

    N<GMP::Z> inline operator + (uint64 u, const N<GMP::Z> & n) {
        N<GMP::Z> sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, u, n.Value.MPZ);
        return sum;
    }

    N<GMP::Z> inline operator - (uint64 u, const N<GMP::Z> & n) {
        if (u <= n) return 0;
        N<GMP::Z> diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, u, n.Value.MPZ);
        return diff;
    }

    N<GMP::Z> inline operator * (uint64 u, const N<GMP::Z> & n) {
        N<GMP::Z> prod;
        __gmp_binary_multiplies::eval (prod.Value.MPZ, u, n.Value.MPZ);
        return prod;
    }

    N<GMP::Z> inline &operator += (N<GMP::Z> &n, const N<GMP::Z> &z) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N<GMP::Z> inline &operator -= (N<GMP::Z> &n, const N<GMP::Z> &z) {
        if (n <= z) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N<GMP::Z> inline &operator *= (N<GMP::Z> &n, const N<GMP::Z> &z) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N<GMP::Z> inline &operator += (N<GMP::Z> &n, const GMP::Z &z) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N<GMP::Z> inline &operator -= (N<GMP::Z> &n, const GMP::Z &z) {
        if (n <= z) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N<GMP::Z> inline &operator *= (N<GMP::Z> &n, const GMP::Z &z) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N<GMP::Z> inline &operator += (N<GMP::Z> &n, uint64 u) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, u);
        return n;
    }

    N<GMP::Z> inline &operator -= (N<GMP::Z> &n, uint64 u) {
        if (n <= u) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, u);
        return n;
    }

    N<GMP::Z> inline &operator *= (N<GMP::Z> &n, uint64 u) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, u);
        return n;
    }*/
}


#endif
