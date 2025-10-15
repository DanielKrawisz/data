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
    
    template <neg n, hex_case zz> integer<n, zz> inline write (const Z &z) {
        std::stringstream ss;
        write (ss, z, zz, n);
        return integer<n, zz> {ss.str ()};
    }

    template <hex_case zz> integer<neg::nones, zz> inline write (const N &n) {
        std::stringstream ss;
        write (ss, n, zz);
        return integer<neg::nones, zz> {ss.str ()};
    }

}

namespace data::encoding::signed_decimal { 
    
    string inline write (const Z &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
    
}

namespace data::encoding::decimal {

    string inline write (const N &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }

}

namespace data::math::number::GMP {

    template <std::unsigned_integral I> N inline operator + (const N &n, I x) {
        N sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, n.Value.MPZ, (gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator + (I x, const N &n) {
        N sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, n.Value.MPZ, (gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator * (const N &n, I x) {
        N sum;
        __gmp_binary_multiplies::eval (sum.Value.MPZ, n.Value.MPZ, (gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator * (I x, const N &n) {
        N sum;
        __gmp_binary_multiplies::eval (sum.Value.MPZ, n.Value.MPZ, (gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator - (const N &n, I x) {
        if (n <= x) return 0;
        N diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, n.Value.MPZ, (gmp_uint) x);
        return diff;
    }

    template <std::unsigned_integral I> N inline operator - (I x, const N &n) {
        if (x <= n) return 0;
        N diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, (gmp_uint) x, n.Value.MPZ);
        return diff;
    }

    template <std::unsigned_integral I> N inline &operator += (N &u, I x) {
        __gmp_binary_plus::eval (u.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator -= (N &u, I x) {
        if (u <= x) u = 0;
        __gmp_binary_minus::eval (u.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator *= (N &u, I x) {
        __gmp_binary_multiplies::eval (u.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N operator & (I x, const N &u) {
        N result;
        __gmp_binary_and::eval (result.Value.MPZ, (gmp_uint) x, u.Value.MPZ);
        return result;
    }

    template <std::unsigned_integral I> N operator & (const N &u, I x) {
        N result;
        __gmp_binary_and::eval (result.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return result;
    }

    template <std::unsigned_integral I> N operator ^ (I x, const N &u) {
        N result;
        __gmp_binary_xor::eval (result.Value.MPZ, (gmp_uint) x, u.Value.MPZ);
        return result;
    }

    template <std::unsigned_integral I> N operator ^ (const N &u, I x) {
        N result;
        __gmp_binary_xor::eval (result.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return result;
    }

    template <std::unsigned_integral I> N operator | (I x, const N &u) {
        N result;
        __gmp_binary_ior::eval (result.Value.MPZ, (gmp_uint) x, u.Value.MPZ);
        return result;
    }

    template <std::unsigned_integral I> N operator | (const N &u, I x) {
        N result;
        __gmp_binary_ior::eval (result.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return result;
    }

    template <std::unsigned_integral I> N inline &operator &= (N &u, I x) {
        __gmp_binary_and::eval (u.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator |= (N &u, I x) {
        __gmp_binary_ior::eval (u.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator ^= (N &u, I x) {
        __gmp_binary_xor::eval (u.Value.MPZ, u.Value.MPZ, (gmp_uint) x);
        return u;
    }

    template <std::integral I> Z inline operator + (const Z &n, I x) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.MPZ, x);
        return sum;
    }

    template <std::integral I> Z inline operator + (I x, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.MPZ, x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (const Z &n, I x) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (I x, const Z &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (gmp_int) x);
        return sum;
    }

    template <std::unsigned_integral I> Z inline operator * (const Z &n, I x) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> Z inline operator * (I x, const Z &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (gmp_uint) x);
        return sum;
    }

    template <std::integral I> Z inline operator - (const Z &n, I x) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, n.MPZ, x);
        return diff;
    }

    template <std::integral I> Z inline operator - (I x, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, x, n.MPZ);
        return diff;
    }

    template <std::signed_integral I> Z inline operator + (const N &n, I x) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.Value.MPZ, (gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator + (I x, const N &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.Value.MPZ, (gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (const N &n, I x) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.Value.MPZ, (gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (I x, const N &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.Value.MPZ, (gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator - (const N &n, I x) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, n.Value.MPZ, (gmp_int) x);
        return diff;
    }

    template <std::signed_integral I> Z inline operator - (I x, const N &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, (gmp_int) x, n.Value.MPZ);
        return diff;
    }
    
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

    Z inline &operator -= (Z &z, int64 n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n);
        return z;
    }

    Z inline &operator *= (Z &z, int64 n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator += (Z &z, const Z& n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator -= (Z &z, const Z &n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n.MPZ);
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

    Z inline &operator ^= (Z &a, const Z &b) {
        __gmp_binary_xor::eval (a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline operator - (const Z &n) {
        Z z {n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    
    Z inline operator + (const Z &z, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator - (const Z &z, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, z.MPZ, n.MPZ);
        return diff;
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

    Z inline operator ^ (const Z &a, const Z &b) {
        Z x;
        __gmp_binary_xor::eval (x.MPZ, a.MPZ, b.MPZ);
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
        return N {a.Value | b.Value};
    }

    N inline operator & (const N &a, const N &b) {
        return N {a.Value & b.Value};
    }

    N inline operator ^ (const N &a, const N &b) {
        return N {a.Value ^ b.Value};
    }

    // divmodd by
    N inline operator / (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return N {def::divmod<Z> {} (a.Value, nonzero {b.Value}).Quotient};
    }

    Z inline operator / (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z> {} (a, nonzero {b.Value}).Quotient;
    }

    N inline operator / (const N &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return N {def::divmod<Z> {} (a.Value, nonzero {Z (b)}).Quotient};
    }

    // mod
    N inline operator % (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<N> {} (a, nonzero {b}).Remainder;
    }

    N inline operator % (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z> {} (a, nonzero {b.Value}).Remainder;
    }

    uint64 inline operator % (const N &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return uint64 (def::divmod<Z> {} (a.Value, nonzero {Z (b)}).Remainder);
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

namespace data::math::def {

    N inline abs<Z>::operator () (const Z &z) {
        return N {z < 0 ? -z : z};
    }

    N inline abs<N>::operator () (const N &n) {
        return n;
    }

    static_assert (proto_bit_number<N>);
    static_assert (proto_bit_number<Z>);

    static_assert (ring_integral<N>);
    static_assert (ring_integral<Z>);

    division<N, N> inline divmod<N, N>::operator () (const N &a, const nonzero<N> &b) {
        return math::number::natural_divmod (a, b.Value);
    }

    division<Z, N> inline divmod<Z, N>::operator () (const Z &a, const nonzero<N> &b) {
        return math::number::integer_natural_divmod (a, b.Value);
    }

    division<Z, N> inline divmod<Z, Z>::operator () (const Z &a, const nonzero<Z> &b) {
        return math::number::integer_divmod (a, b.Value);
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
