// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_N
#define DATA_MATH_NUMBER_GMP_N

#include <limits>
#include <sstream>
#include <data/math/number/natural.hpp>
#include <data/math/abs.hpp>
#include <data/math/number/gmp/Z.hpp>

namespace data::math::number::GMP {
    
    //struct Q;
    
    struct N {
        Z Value;
        
        N () : Value {} {}
        
        N (gmp_uint n) : Value {} {
            mpz_init_set_ui (Value.MPZ, n);
        }
        
        N (const string &x) : N {read (x)} {}
        
        static N read (string_view x);
        
        template <size_t size>
        explicit N (decimal<size> d) : N {std::string {d.Value}} {}
        
        explicit operator uint64 () const;
        
        explicit operator double () const {
            return double (Value);
        }
        
        N operator ^ (uint32 n) const {
            return N {Value ^ n};
        }
        
        N &operator ^= (uint32 n) {
            Value ^= n;
            return *this;
        };
        
        math::division<N> divide (const N& n) const {
            auto div = Value.divide (n.Value);
            return math::division<N> {N {div.Quotient}, N {div.Remainder}};
        }
        
        template <endian::order o>
        explicit N (const N_bytes<o>& n) : N {bytes_view (n), o} {}
        /*
        template <endian::order o, size_t size> 
        explicit N(const bounded<false, o, size>& b) : Value{b} {}*/
        
    private:
        explicit N (const Z& z) : Value {z} {}
        
        N (bytes_view, endian::order);
        
        void write_bytes (bytes&, endian::order) const;
        
        friend struct Z;
        template <endian::order o> friend struct N_bytes;
    };
    
    bool inline operator == (const N &a, const N &b) {
        return a <=> b == 0;
    }
    
    std::weak_ordering inline operator <=> (const N &a, const N &b) {
        return a.Value <=> b.Value;
    }
    
    bool inline operator == (const Z &a, const N &b) {
        return a <=> b == 0;
    }
    
    std::weak_ordering inline operator <=> (const Z &a, const N &b) {
        return a <=> b.Value;
    }
    
    bool inline operator == (const N &a, uint64 b) {
        return a <=> b == 0;
    }
    
    std::weak_ordering inline operator <=> (const N &a, int64 b) {
        return a.Value <=> b;
    }
    
    Z inline operator / (const Z &a, const Z &z) {
        return a.divide (z).Quotient;
    }
    
    N inline operator % (const Z &a, const N &z) {
        return a.divide (z).Remainder;
    }
    
    uint64 inline operator % (const N &a, uint64 b) {
        return uint64 (a.divide(b).Remainder);
    }
    
    uint64 inline operator % (const Z &a, uint64 b) {
        return uint64 (a.divide (b).Remainder);
    }
    
    N inline operator / (const N &a, const N &n) {
        return a.divide (n).Quotient;
    }
    
    N inline operator / (const N &a, uint64 b) {
        return a.divide (N {b}).Quotient;
    }
    
    N inline operator % (const N &a, const N &n) {
        return a.divide (n).Remainder;
    }
    
    N inline &operator %= (N &a, const N &n) {
        return a = a.divide (n).Remainder;
    }
    
    N inline &operator /= (N &a, const N& n) {
        return a = a / n;
    }
    
    N inline &operator ++ (N &n) {
        ++n.Value;
        return n;
    }
    
    N inline &operator -- (N &n) {
        if (n.Value != 0) --n.Value; 
        return n;
    }
        
    N inline operator ++ (N &n, int) {
        N z = n;
        ++n;
        return z;
    }
    
    N inline operator -- (N &n, int) {
        N z = n;
        --n;
        return z;
    }
    
    N inline &operator += (N &z, uint64 n) {
        __gmp_binary_plus::eval (z.Value.MPZ, z.Value.MPZ, n);
        return z;
    }
    
    N inline &operator += (N &z, const N &n) {
        __gmp_binary_plus::eval (z.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return z;
    }
    
    Z inline &operator += (Z &z, const N& n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n.Value.MPZ);
        return z;
    }
    
    N inline &operator -= (N &z, int64 n) {
        __gmp_binary_minus::eval (z.Value.MPZ, z.Value.MPZ, n);
        return z;
    }
    
    N inline &operator -= (N &z, const N &n) {
        __gmp_binary_minus::eval (z.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return z;
    }
    
    Z inline &operator -= (Z &z, const N &n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n.Value.MPZ);
        return z;
    }
    
    N inline &operator *= (N &z, uint64 n) {
        __gmp_binary_multiplies::eval (z.Value.MPZ, z.Value.MPZ, n);
        return z;
    }
    
    N inline &operator *= (N &z, const N &n) {
        __gmp_binary_multiplies::eval (z.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return z;
    }
    
    Z inline &operator *= (Z &z, const N& n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, n.Value.MPZ);
        return z;
    }
    
    N inline operator & (const N &a, const N &b) {
        N n;
        __gmp_binary_and::eval (n.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return n;
    }
    
    N inline &operator &= (N &a, const N &b) {
        __gmp_binary_and::eval (a.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return a;
    }
    
    N inline &operator |= (N &a, const N &b) {
        __gmp_binary_ior::eval (a.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return a;
    }
    
    Z inline &operator &= (Z &a, const N &b) {
        __gmp_binary_and::eval (a.MPZ, a.MPZ, b.Value.MPZ);
        return a;
    }
    
    Z inline &operator |= (Z &a, const N &b) {
        __gmp_binary_ior::eval (a.MPZ, a.MPZ, b.Value.MPZ);
        return a;
    }
    
    Z inline operator - (const N &n) {
        return -n.Value;
    }
    
    N inline operator + (const N &z, uint64 n) {
        N sum {};
        __gmp_binary_plus::eval (sum.Value.MPZ, z.Value.MPZ, n);
        return sum;
    }
    
    N inline operator + (int64 n, const N &z) {
        N sum {};
        __gmp_binary_plus::eval (sum.Value.MPZ, n, z.Value.MPZ);
        return sum;
    }
    
    N inline operator + (const N &z, const N &n) {
        N sum{};
        __gmp_binary_plus::eval(sum.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator + (const Z &z, const N &n) {
        Z sum {};
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, n.Value.MPZ);
        return sum;
    }
    
    N inline operator - (const N &z, const N &n) {
        if (z <= n) return N {0};
        N sum {};
        __gmp_binary_minus::eval (sum.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return sum;
    }
    
    N inline operator-(const N &z, uint64 n) {
        if (z <= n) return N{0};
        N sum{};
        __gmp_binary_minus::eval(sum.Value.MPZ, z.Value.MPZ, n);
        return sum;
    }
    
    N inline operator - (int64 n, const N &z) {
        if (z >= n) return N {0};
        N sum {};
        __gmp_binary_minus::eval (sum.Value.MPZ, n, z.Value.MPZ);
        return sum;
    }
    
    Z inline operator + (const N &z, const Z &n) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, z.Value.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator - (const Z &z, const N &n) {
        Z sum {};
        __gmp_binary_minus::eval (sum.MPZ, z.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator - (const N &z, const Z &n) {
        Z sum {};
        __gmp_binary_minus::eval (sum.MPZ, z.Value.MPZ, n.MPZ);
        return sum;
    }
    
    N inline operator * (const N &z, uint64 n) {
        N sum {};
        __gmp_binary_multiplies::eval (sum.Value.MPZ, z.Value.MPZ, n);
        return sum;
    }
    
    N inline operator * (int64 n, const N &z) {
        N sum {};
        __gmp_binary_multiplies::eval (sum.Value.MPZ, n, z.Value.MPZ);
        return sum;
    }
    
    N inline operator * (const N &z, const N &n) {
        N sum {};
        __gmp_binary_multiplies::eval (sum.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator * (const Z &z, const N &n) {
        Z sum {};
        __gmp_binary_multiplies::eval (sum.MPZ, z.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator * (const N &z, const Z &n) {
        Z sum {};
        __gmp_binary_multiplies::eval (sum.MPZ, z.Value.MPZ, n.MPZ);
        return sum;
    }
    
    N inline operator << (const N &a, int x) {
        N n {};
        __gmp_binary_lshift::eval (n.Value.MPZ, a.Value.MPZ, x);
        return n;
    }
    
    N inline operator >> (const N &a, int x) {
        N n {};
        __gmp_binary_rshift::eval (n.Value.MPZ, a.Value.MPZ, x);
        return n;
    }
    
    N inline &operator <<= (N &a, int x) {
        __gmp_binary_lshift::eval (a.Value.MPZ, a.Value.MPZ, x);
        return a;
    }
    
    N inline &operator >>= (N &a, int x) {
        __gmp_binary_rshift::eval (a.Value.MPZ, a.Value.MPZ, x);
        return a;
    }
    
    division<Z, N> inline Z::divide (const Z& z) const {
        division<Z, N> qr {};
        mpz_fdiv_qr (qr.Quotient.MPZ, qr.Remainder.Value.MPZ, MPZ, z.MPZ);
        return qr;
    }
    
}

// Declare associativity and commutivity of operators + and * on N. 
namespace data::math {
    
    N inline identity<plus<N>, N>::operator () () {
        return 1;
    }
    
    N inline identity<times<N>, N>::operator () () {
        return 0;
    }
    
    N inline abs<N>::operator () (const N &i) {
        return i;
    }
    
    N inline abs<Z>::operator () (const Z &i) {
        N n;
        __gmp_abs_function::eval (n.Value.MPZ, i.MPZ);
        return n;
    }
}

namespace data::math::number {
    
    N inline square (const N &n) {
        return n * n;
    }
    
    N inline square (const Z &z) {
        return data::abs (z) * data::abs (z);
    }
    
}
    

namespace data::encoding::hexidecimal { 
    
    template <hex_case zz> integer<complement::nones, zz> inline write (const math::N &n) {
        std::stringstream ss;
        write (ss, n, zz);
        return integer<complement::nones, zz> {ss.str ()};
    }
    
}

namespace data::encoding::decimal {
    
    string inline write (const math::N &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
    
}

namespace data {
    
    math::sign inline sign (const math::N &z) {
        return math::number::GMP::sign (z.Value.MPZ[0]);
    }
    
    math::sign inline sign (const math::Z &z) {
        return math::number::GMP::sign (z.MPZ[0]);
    }
    
    math::N inline square (const math::N &n) {
        return n * n;
    }
    
    math::N inline square (const math::Z &z) {
        auto n = abs (z);
        return n * n;
    }
    
    math::N inline increment (const math::N &n) {
        auto x = n;
        return ++x;
    }
    
    math::N inline decrement (const math::N &n) {
        auto x = n;
        return --x;
    }
    
    math::Z inline increment (const math::Z &n) {
        auto x = n;
        return ++x;
    }
    
    math::Z inline decrement (const math::Z &n) {
        auto x = n;
        return --x;
    }
    
}

#endif
