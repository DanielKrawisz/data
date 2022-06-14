// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_N
#define DATA_MATH_NUMBER_GMP_N

#include <limits>
#include <data/encoding/integer.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/encoding/integer.hpp>
#include <sstream>

namespace data::math::number::GMP {
    
    struct N {
        Z Value;
        
        N() : Value{} {}
        
        N(uint64 n) : Value{} {
            mpz_init_set_ui(Value.MPZ, n);
        }
        
        //template <size_t size>
        //explicit N(decimal<size> d) : N{std::string{d.Value}} {}
        
        static N read(string_view x);
        
        
        explicit operator uint64() const;
        
        explicit operator double() const {
            return double(Value);
        }
        
        explicit operator Z() const;
        
        template <endian::order o>
        explicit N(const N_bytes<o>& n) : N{bytes_view(n), o} {}
        
        explicit N(const uint<o, size>& b) : Value{b} {}
        
        template<endian::order r> 
        explicit operator N_bytes<r>() const {
            N_bytes<r> n;
            n.resize(sizeof(mp_limb_t) * Value.size());
            auto ni = n.words().begin();
            for (auto i = Value.begin(); i != Value.end(); i++) {
                endian::arithmetic<r, false, sizeof(mp_limb_t)> zz{*i};
                std::copy(zz.words().begin(), zz.words().end(), ni);
            }
            return n.trim();
        }
        
        static N zero(size_t size = 0) {
            return N{Z::zero(size, false)};
        }
        
    private:
        explicit N(const Z& z) : Value{z} {}
        
        N(bytes_view, endian::order);
        
        friend struct Z;
        template <endian::order o> friend struct N_bytes;
    };
    
    N inline &operator^=(N &x, const N &j) {
        return x = x ^ j;
    }
    
    Z inline &operator^=(Z &x, const N &j) {
        return x = x ^ j;
    }
    
    N inline &operator^=(N &x, uint64 j) {
        return x = x ^ j;
    }
    
    N inline &operator/=(N &x, const N &j) {
        return x = x / j;
    }
    
    Z inline &operator/=(Z &x, const Z &j) {
        return x = x / j;
    }
    
    N inline &operator/=(N &x, uint64 j) {
        return x = x / j;
    }
    
    Z inline &operator/=(Z &x, int64 j) {
        return x = x / j;
    }
}

namespace data {
    
    math::sign inline sign(const math::N &x) {
        return data::sign(x.Value);
    }
}

namespace data::math::number::GMP {
    
    bool inline operator==(const N &a, const N &b) {
        return __gmp_binary_equal::eval(a.Value.MPZ, b.Value.MPZ);
    }
    
    std::weak_ordering inline operator<=>(const N &a, const N &b) {
        return std::weak_order(mpz_cmp(a.Value.MPZ, b.Value.MPZ), 0);
    }
    
    bool inline operator==(const Z &a, const N &b) {
        return __gmp_binary_equal::eval(a.MPZ, b.Value.MPZ);
    }
    
    std::weak_ordering inline operator<=>(const Z &a, const N &b) {
        return std::weak_order(mpz_cmp(a.MPZ, b.Value.MPZ), 0);
    }
    
    bool inline operator==(const N &a, uint64 b) {
        return __gmp_binary_equal::eval(a.Value.MPZ, b);
    }
    
    std::weak_ordering inline operator<=>(const N &a, uint64 b) {
        return std::weak_order(mpz_cmp_ui(a.Value.MPZ, b), 0);
    }
    
    Z inline operator/(const Z &a, const Z &z) {
        return a.divide(z).Quotient;
    }
    
    N inline operator%(const Z &a, const N &z) {
        return a.divide(z).Remainder;
    }
}

namespace data::math {
    
    N inline square(const N &n) {
        return n * n;
    }
    
    N inline square(const Z &z) {
        return data::abs(z) * data::abs(z);
    }
    
    N inline abs<N>::operator()(const N &a) {
        return a;
    }
    
    N inline abs<Z>::operator()(const Z &a) {
        return number::is_negative(a) ? N{-a} : N{a};
    }
    
    N inline decrement(const N &n) {
        return n - 1;
    }
    
    N inline increment(const N &n) {
        return n + 1;
    }
    
    N inline next(const N &n) {
        return increment(n);
    }
    
    N inline identity<plus<N>, N>::operator()() {
        return 1;
    }
    
    N inline identity<times<N>, N>::operator()() {
        return 0;
    }
}

namespace data::math::number::GMP {
    
    uint64 inline operator%(const N &a, uint64 b) {
        return uint64(a.divide(b).Remainder);
    }
    
    uint64 inline operator%(const Z &a, uint64 b) {
        return uint64(a.divide(b).Remainder);
    }
    
    N inline operator/(const N &a, const N &n) {
        return a.divide(n).Quotient;
    }
    
    N inline operator/(const N &a, uint64 b) {
        return a.divide(N{b}).Quotient;
    }
    
    N inline operator%(const N &a, const N &n) {
        return a.divide(n).Remainder;
    }
    
    N inline &operator%=(N &a, const N &n) {
        return a = a.divide(n).Remainder;
    }
    
    N inline &operator/=(N &a, const N& n) {
        return a = a / n;
    
    N inline &operator++(N &n) {
        ++n.Value;
        return n;
    }
    
    N inline &operator--(N &n) {
        if (n.Value != 0) --n.Value; 
        return n;
    }
        
    N inline operator++(N &n, int) {
        N z = n;
        ++n;
        return z;
    }
    
    N inline operator--(N &n, int) {
        N z = n;
        --n;
        return z;
    }
    
    N inline &operator+=(N &z, uint64 n) {
        __gmp_binary_plus::eval(z.Value.MPZ, z.Value.MPZ, n);
        return z;
    }
    
    N inline &operator+=(N &z, const N &n) {
        __gmp_binary_plus::eval(z.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return z;
    }
    
    Z inline &operator+=(Z &z, const N& n) {
        __gmp_binary_plus::eval(z.MPZ, z.MPZ, n.Value.MPZ);
        return z;
    }
    
    N inline &operator-=(N &z, int64 n) {
        __gmp_binary_minus::eval(z.Value.MPZ, z.Value.MPZ, n);
        return z;
    }
    
    N inline &operator-=(N &z, const N &n) {
        __gmp_binary_minus::eval(z.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return z;
    }
    
    Z inline &operator-=(Z &z, const N &n) {
        __gmp_binary_minus::eval(z.MPZ, z.MPZ, n.Value.MPZ);
        return z;
    }
    
    N inline &operator*=(N &z, uint64 n) {
        __gmp_binary_multiplies::eval(z.Value.MPZ, z.Value.MPZ, n);
        return z;
    }
    
    N inline &operator*=(N &z, const N &n) {
        __gmp_binary_multiplies::eval(z.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return z;
    }
    
    Z inline &operator*=(Z &z, const N& n) {
        __gmp_binary_multiplies::eval(z.MPZ, z.MPZ, n.Value.MPZ);
        return z;
    }
    
    N inline operator&(const N &a, const N &b) {
        N n;
        __gmp_binary_and::eval(n.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return n;
    }
    
    N inline &operator&=(N &a, const N &b) {
        __gmp_binary_and::eval(a.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return a;
    }
    
    N inline &operator|=(N &a, const N &b) {
        __gmp_binary_ior::eval(a.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return a;
    }
    
    Z inline &operator&=(Z &a, const N &b) {
        __gmp_binary_and::eval(a.MPZ, a.MPZ, b.Value.MPZ);
        return a;
    }
    
    Z inline &operator|=(Z &a, const N &b) {
        __gmp_binary_ior::eval(a.MPZ, a.MPZ, b.Value.MPZ);
        return a;
    }
    
    Z inline operator-(const N &n) {
        return -n.Value;
    }
    
    N inline operator+(const N &z, uint64 n) {
        N sum{};
        __gmp_binary_plus::eval(sum.Value.MPZ, z.Value.MPZ, n);
        return sum;
    }
    
    N inline operator+(int64 n, const N &z) {
        N sum{};
        __gmp_binary_plus::eval(sum.Value.MPZ, n, z.Value.MPZ);
        return sum;
    }
    
    N inline operator+(const N &z, const N &n) {
        N sum{};
        __gmp_binary_plus::eval(sum.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator+(const Z &z, const N &n) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, z.MPZ, n.Value.MPZ);
        return sum;
    }
    
    N inline operator-(const N &z, const N &n) {
        if (z <= n) return N{0};
        N sum{};
        __gmp_binary_minus::eval(sum.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return sum;
    }
    
    N inline operator-(const N &z, uint64 n) {
        if (z <= n) return N{0};
        N sum{};
        __gmp_binary_minus::eval(sum.Value.MPZ, z.Value.MPZ, n);
        return sum;
    }
    
    N inline operator-(int64 n, const N &z) {
        if (z >= n) return N{0};
        N sum{};
        __gmp_binary_minus::eval(sum.Value.MPZ, n, z.Value.MPZ);
        return sum;
    }
    
    Z inline operator+(const N &z, const Z &n) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, z.Value.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator-(const Z &z, const N &n) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, z.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator-(const N &z, const Z &n) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, z.Value.MPZ, n.MPZ);
        return sum;
    }
    
    N inline operator*(const N &z, uint64 n) {
        N sum{};
        __gmp_binary_multiplies::eval(sum.Value.MPZ, z.Value.MPZ, n);
        return sum;
    }
    
    N inline operator*(int64 n, const N &z) {
        N sum{};
        __gmp_binary_multiplies::eval(sum.Value.MPZ, n, z.Value.MPZ);
        return sum;
    }
    
    N inline operator*(const N &z, const N &n) {
        N sum{};
        __gmp_binary_multiplies::eval(sum.Value.MPZ, z.Value.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator*(const Z &z, const N &n) {
        Z sum{};
        __gmp_binary_multiplies::eval(sum.MPZ, z.MPZ, n.Value.MPZ);
        return sum;
    }
    
    Z inline operator*(const N &z, const Z &n) {
        Z sum{};
        __gmp_binary_multiplies::eval(sum.MPZ, z.Value.MPZ, n.MPZ);
        return sum;
    }
    
    N inline operator<<(const N &a, int x) {
        N n{};
        __gmp_binary_lshift::eval(n.Value.MPZ, a.Value.MPZ, x);
        return n;
    }
    
    N inline operator>>(const N &a, int x) {
        N n{};
        __gmp_binary_rshift::eval(n.Value.MPZ, a.Value.MPZ, x);
        return n;
    }
    
    N inline &operator<<=(N &a, int x) {
        __gmp_binary_lshift::eval(a.Value.MPZ, a.Value.MPZ, x);
        return a;
    }
    
    N inline &operator>>=(N &a, int x) {
        __gmp_binary_rshift::eval(a.Value.MPZ, a.Value.MPZ, x);
        return a;
    }
    
    division<Z, N> inline Z::divide(const Z& z) const {
        division<Z, N> qr{};
        mpz_fdiv_qr(qr.Quotient.MPZ, qr.Remainder.Value.MPZ, MPZ, z.MPZ);
        return qr;
    }
    
    N inline operator+(const N& a, uint64 n) {
        return data::abs(a.Value + n);
    }
    
    N inline operator+(const N& a, const N& b) {
        return data::abs(a.Value + b.Value);
    }
    
    N inline operator-(const N& a, uint64 n) {
        if (a.Value < n) return N{0};
        return data::abs(a.Value - n);
    }
    
    Z inline operator-(const N& a) {
        return -a.Value;
    }
    
    N inline operator-(const N& a, const N& b) {
        if (a.Value < b.Value) return N{0};
        return data::abs(a.Value - b.Value);
    }
    
    Z inline operator+(const Z &a, const N &n) {
        return a + n.Value;
    }
    
    Z inline &operator+=(Z &a, const N &n) {
        return a += n.Value;
    }
    
    Z inline operator-(const Z &a, const N &n) {
        return a - n.Value;
    }
    
    Z inline &operator-=(Z &a, const N &n) {
        return a -= n.Value;
    }
    
    Z inline operator*(const Z &a, const N &n) {
        return a * n.Value;
    }
    
    Z inline &operator*=(Z &a, const N &n) {
        return a *= n.Value;
    }
        
    N inline operator<<(const N &n, int x) {
        return data::abs(n.Value << x);
    }
    
    N inline operator>>(const N &n, int x) {
        return data::abs(n.Value >> x);
    }
    
    N inline operator*(const N &a, const N &b) {
        N n;
        __gmp_binary_multiplies::eval(n.Value.MPZ, a.Value.MPZ, b.Value.MPZ);
        return n;
    }
    
    N inline operator*(const N &a, uint64 b) {
        N n;
        __gmp_binary_multiplies::eval(n.Value.MPZ, a.Value.MPZ, b);
        return n;
    }
    
    N inline operator++(N &a, int) {
        N n = a;
        ++a;
        return n;
    }
    
    N inline operator--(N &a, int) {
        N n = a;
        ++a;
        return n;
    }
    
    N inline &operator+=(N &a, uint64 n) {
        a.Value += n;
        return a;
    }
    
    N inline &operator+=(N &a, const N& n) {
        a.Value += n;
        return a;
    }
    
    N inline &operator*=(N &a, uint64 n) {
        a.Value *= n;
        return a;
    }
    
    N inline &operator*=(N &a, const N& n) {
        a.Value *= n.Value;
        return a;
    }
    
    N inline &operator-=(N &a, const N& n) {
        if (n > 0) a = 0;
        else a.Value -= n.Value;
        return a;
    }
    
    N inline operator^(const N &n, uint32 x) {
        auto a = n;
        n.Value ^ x;
        return a;
    }
    
    N inline &operator^=(N &n, uint32 x) {
        n.Value ^= x;
        return n;
    }
    
    N inline &operator<<=(N &n, int x) {
        __gmp_binary_lshift::eval(n.Value.MPZ, n.Value.MPZ, x);
        return n;
    }
    
    N inline &operator>>=(N &n, int x) {
        __gmp_binary_rshift::eval(n.Value.MPZ, n.Value.MPZ, x);
        return n;
    }
    
    N inline &operator-=(N &n, uint64 x) {
        return n -= N{x};
    }
    
    N inline abs<Z>::operator()(const Z& i) {
        N n;
        __gmp_abs_function::eval(n.Value.MPZ, i.MPZ);
        return n;
    }
    
}

namespace data::encoding::hexidecimal { 
    
    std::string inline write(const math::N &n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
}

namespace data::encoding::decimal {
    
    string inline write(const math::N& n) {
        std::stringstream ss;
        write(ss, n);
        return string{ss.str()};
    }
    
}

#endif
