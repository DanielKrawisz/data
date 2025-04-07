// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/power.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/encoding/integer.hpp>
#include <data/math/number/division.hpp>

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
    
    Z inline &operator ++ (Z &n) {
        __gmp_unary_increment::eval (n.MPZ);
        return n;
    }
    
    Z inline &operator -- (Z &n) {
        __gmp_unary_decrement::eval (n.MPZ);
        return n;
    }
    
    Z inline &operator += (Z &z, int64 n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, (long int) (n));
        return z;
    }
    
    Z inline &operator += (Z &z, const Z& n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator -= (Z &z, int64 n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, (long int) (n));
        return z;
    }
    
    Z inline &operator -= (Z &z, const Z &n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator *= (Z &z, int64 n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, (long int) (n));
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
    }
    
    Z inline operator - (const Z &n) {
        Z z {n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    
    Z inline operator + (const Z &z, int64 n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, (long int) (n));
        return sum;
    }
    
    Z inline operator + (int64 n, const Z &z) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, (long int) (n), z.MPZ);
        return sum;
    }
    
    Z inline operator + (const Z &z, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator - (const Z &z, int64 n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, z.MPZ, (long int) (n));
        return diff;
    }
    
    Z inline operator - (int64 n, const Z &z) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, (long int) (n), z.MPZ);
        return diff;
    }
    
    Z inline operator - (const Z &z, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, z.MPZ, n.MPZ);
        return diff;
    }
    
    Z inline operator * (const Z &z, int64 n) {
        Z prod;
        __gmp_binary_multiplies::eval (prod.MPZ, z.MPZ, (long int) (n));
        return prod;
    }
    
    Z inline operator * (int64 n, const Z &z) {
        Z prod;
        __gmp_binary_multiplies::eval (prod.MPZ, (long int) (n), z.MPZ);
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
    }
    bool inline operator == (const N &a, const N &b) {
        return a.Value == b.Value;
    }

    auto inline operator <=> (const N &a, const N &b) -> decltype (std::declval<Z> () <=> std::declval<Z> ()) {
        return a.Value <=> b.Value;
    }

    N inline operator + (const N &a, const N &b) {
        return N {a.Value + b.Value};
    }

    N inline operator - (const N &a, const N &b) {
        return N {a.Value < b.Value ? 0 : a.Value - b.Value};
    }

    N inline operator * (const N &a, const N &b) {
        return N {a.Value * b.Value};
    }

    Z inline operator + (const N &a, const Z &b) {
        return a.Value + b;
    }

    Z inline operator - (const N &a, const Z &b) {
        return a.Value - b;
    }

    Z inline operator * (const N &a, const Z &b) {
        return a.Value * b;
    }

    Z inline operator + (const Z &a, const N &b) {
        return a + b.Value;
    }

    Z inline operator - (const Z &a, const N &b) {
        return a - b.Value;
    }

    Z inline operator * (const Z &a, const N &b) {
        return a * b.Value;
    }

    Z inline operator - (const N &a) {
        return -a.Value;
    }

    // bit operations
    N inline operator | (const N &a, const N &b) {
        return N {a.Value | b};
    }

    N inline operator & (const N &a, const N &b) {
        return N {a.Value & b};
    }

    // divided by
    N inline operator / (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return N {divide<Z> {} (a.Value, nonzero {b.Value}).Quotient};
    }

    Z inline operator / (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return divide<Z> {} (a, nonzero {b.Value}).Quotient;
    }

    N inline operator / (const N &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return N {divide<Z> {} (a.Value, nonzero {Z (b)}).Quotient};
    }

    // mod
    N inline operator % (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return divide<N> {} (a, nonzero {b}).Remainder;
    }

    N inline operator % (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return divide<Z> {} (a, nonzero {b.Value}).Remainder;
    }

    uint64 inline operator % (const N &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return uint64 (divide<Z> {} (a.Value, nonzero {Z (b)}).Remainder);
    }

    // bit shift, which really just means
    // powers of two.
    N inline operator << (const N &a, int b) {
        return N {a.Value << b};
    }

    N inline operator >> (const N &a, int b) {
        return N {a.Value >> b};
    }

    // pre increment
    N inline &operator ++ (N &a) {
        ++a.Value;
        return a;
    }

    N inline &operator -- (N &a) {
        --a.Value;
        return a;
    }

    // post increment
    N inline operator ++ (N &a, int) {
        auto b = a;
        ++a;
        return b;
    }

    N inline operator -- (N &a, int) {
        auto b = a;
        --a;
        return b;
    }

    Z inline &operator += (Z &a, const N &b) {
        a += b.Value;
        return a;
    }

    Z inline &operator -= (Z &a, const N &b) {
        return a -= b.Value;
    }

    Z inline &operator *= (Z &a, const N &b) {
        a *= b.Value;
        return a;
    }

    N inline &operator &= (N &a, const N &b) {
        a.Value &= b.Value;
        return a;
    }

    N inline &operator |= (N &a, const N &b) {
        a.Value |= b.Value;
        return a;
    }

    N inline &operator ^= (N &a, uint64 b) {
        a.Value ^= b;
        return a;
    }

    N inline &operator /= (N &a, const N &b) {
        a.Value /= b.Value;
        return a;
    }

    N inline &operator /= (N &a, uint64 b) {
        a.Value /= b;
        return a;
    }

    N inline &operator %= (N &a, const N &b) {
        a.Value = a.Value % b;
        return a;
    }

    N inline &operator <<= (N &a, int i) {
        a.Value <<= i;
        return a;
    }

    N inline &operator >>= (N &a, int i) {
        a.Value >>= i;
        return a;
    }

    // exponential
    N inline operator ^ (const N &a, const N &b) {
        return data::pow<N, N> (a, b);
    }

    Z inline operator ^ (const Z &a, const N &b) {
        return data::pow<Z, N> (a, b);
    }

    N inline operator ^ (const N &a, uint64 b) {
        return data::pow<N, uint64> (a, b);
    }

    bool inline operator == (const uint64 &u, const N &n) {
        return Z (u) == n.Value;
    }

    std::weak_ordering inline operator <=> (const uint64 &u, const N &n) {
        return Z (u) <=> n.Value;
    }

    std::weak_ordering inline operator <=> (const int64 &i, const Z &n) {
        return Z (i) <=> n;
    }
}

namespace data::math {

    N inline abs<Z>::operator () (const Z &z) {
        return N {z < 0 ? -z : z};
    }

    N inline abs<N>::operator () (const N &n) {
        return n;
    }

    division<N, N> inline divide<N, N>::operator () (const N &a, const nonzero<N> &b) {
        return number::natural_divide (a, b.Value);
    }

    division<Z, N> inline divide<Z, N>::operator () (const Z &a, const nonzero<N> &b) {
        return number::integer_divide (a, b.Value);
    }

    division<Z, N> inline divide<Z, Z>::operator () (const Z &a, const nonzero<Z> &b) {
        return number::integer_divide (a, b.Value);
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

    nonzero<N> inline increment<N>::operator () (const N &n) {
        nonzero<N> x {n};
        ++x.Value;
        return x;
    }

    N inline decrement<N>::operator () (const nonzero<N> &n) {
        auto x = n.Value;
        return --x;
    }

    N inline decrement<N>::operator () (const N &n) {
        if (n == 0) return n;
        auto x = n;
        return --x;
    }
}

namespace data::math::number::GMP {

    N inline operator + (const N &n, uint64 u) {
        N sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return sum;
    }

    N inline operator - (const N &n, uint64 u) {
        if (n <= u) return 0;
        N diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return diff;
    }

    N inline operator * (const N &n, uint64 u) {
        N prod;
        __gmp_binary_multiplies::eval (prod.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return prod;
    }

    N inline operator + (uint64 u, const N & n) {
        N sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, (long unsigned int) (u), n.Value.MPZ);
        return sum;
    }

    N inline operator - (uint64 u, const N & n) {
        if (u <= n) return 0;
        N diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, (long unsigned int) (u), n.Value.MPZ);
        return diff;
    }

    N inline operator * (uint64 u, const N & n) {
        N prod;
        __gmp_binary_multiplies::eval (prod.Value.MPZ, (long unsigned int) (u), n.Value.MPZ);
        return prod;
    }

    N inline &operator += (N &n, const N &z) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N inline &operator -= (N &n, const N &z) {
        if (n <= z) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N inline &operator *= (N &n, const N &z) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N inline &operator += (N &n, const GMP::Z &z) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N inline &operator -= (N &n, const GMP::Z &z) {
        if (n <= z) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N inline &operator *= (N &n, const GMP::Z &z) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N inline &operator += (N &n, uint64 u) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return n;
    }

    N inline &operator -= (N &n, uint64 u) {
        if (n <= u) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return n;
    }

    N inline &operator *= (N &n, uint64 u) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return n;
    }
}


#endif
