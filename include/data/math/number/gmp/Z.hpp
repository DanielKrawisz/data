// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/number/gmp/mpz.hpp>
#include <data/cross.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::number::GMP {
    
    struct N;
    
    struct Z {
        mpz_t MPZ;
        
        Z(): Z{0} {}
        
        explicit Z(const N&);
        
        virtual ~Z() {
            mpz_clear(MPZ);
        }
        
        explicit Z(gmp_int n) : MPZ{} {
            mpz_init_set_si(MPZ, n);
        }
        
        static Z read(string_view x);
        
        Z(const Z& n) {
            mpz_init(MPZ);
            mpz_set(MPZ, n.MPZ);
        }
        
        Z(Z&& n) : Z{} {
            mpz_swap(MPZ, n.MPZ);
        }
        
        Z& operator=(const Z& n) {
            mpz_set(MPZ, n.MPZ);
            return *this;
        }
        
        Z& operator=(Z&& n) {
            mpz_swap(MPZ, n.MPZ);
            return *this;
        }
        
        static Z zero(size_t size = 0, bool negative = false);
        
        size_t size() const {
            return GMP::size(MPZ[0]);
        }
        
        using index = uint32;
        
        mp_limb_t& operator[](index i) {
            if (static_cast<int>(i) >= MPZ[0]._mp_alloc) throw std::out_of_range{"Z"};
            return *(MPZ[0]._mp_d + i);
        }
        
        const mp_limb_t& operator[](index i) const {
            if (static_cast<int>(i) >= MPZ[0]._mp_alloc) throw std::out_of_range{"Z"};
            return *(MPZ[0]._mp_d + i);
        }
        
        mp_limb_t* begin() {
            return MPZ[0]._mp_d;
        }
        
        mp_limb_t* end() {
            return MPZ[0]._mp_d + MPZ[0]._mp_alloc;
        }
        
        const mp_limb_t* begin() const {
            return MPZ[0]._mp_d;
        }
        
        const mp_limb_t* end() const {
            return MPZ[0]._mp_d + MPZ[0]._mp_alloc;
        };
        
        explicit operator int64() const;
        
        explicit operator double() const {
            return mpz_get_d(MPZ);
        }
        
        template <endian::order o, complement c> 
        explicit Z(const Z_bytes<o, c>& b): Z(data::abs(b).words()) {
            if (is_negative(b)) MPZ[0]._mp_size = -MPZ[0]._mp_size;
        }
        
        template <endian::order o> 
        explicit Z(const N_bytes<o>& b) : Z(bytes_view(b), o) {}
        
        explicit Z(const uint<o, size>& b) : Z(bytes_view(b), o) {}
        
        template<endian::order r> 
        explicit operator Z_bytes<r, twos>() const {
            Z_bytes<r, twos> z = Z_bytes<r, twos>::zero(sizeof(mp_limb_t) * this->size() + 1, math::sign(*this) == negative);
            auto zi = z.words().begin();
            for (auto i = this->begin(); i != this->end(); i++) {
                endian::arithmetic<r, false, sizeof(mp_limb_t)> zz{*i};
                std::copy(zz.words().begin(), zz.words().end(), zi);
            }
            return trim(z);
        }
        
        template<endian::order r> 
        explicit operator Z_bytes<r, ones>() const {
            return Z_bytes<r, ones>(Z_bytes<r, twos>(*this));
        }
        
    private:
        
        explicit operator uint64() const;
        
        template <endian::order r> 
        Z(encoding::words<r, byte> w) : Z{0} {
            int i;
            for (auto i = w.rbegin(); i != w.rend(); i++) {
                *this <<= 8;
                *this += *i;
            }
        }
        
    };
    
    bool inline operator==(const Z &a, const Z &b) {
        return __gmp_binary_equal::eval(a.MPZ, b.MPZ);
    }
    
    std::weak_ordering inline operator<=>(const Z &a, const Z &b) {
        return std::weak_order(mpz_cmp(a.MPZ, b.MPZ), 0);
    }
    
    bool inline operator==(const Z &a, int64 b) {
        return __gmp_binary_equal::eval(a.MPZ, b);
    }
    
    std::weak_ordering inline operator<=>(const Z &a, int64 b) {
        return std::weak_order(mpz_cmp_si(a.MPZ, b), 0);
    }
    
    bool inline is_negative_zero(const Z &z) {
        return is_zero(z) && z.MPZ[0]._mp_size < 0;
    }
    
    bool inline is_positive_zero(const Z &z) {
        return is_zero(z) && z.MPZ[0]._mp_size >= 0;
    }
    
    Z inline &operator++(Z &z) {
        __gmp_unary_increment::eval(z.MPZ);
        return z;
    }
    
    Z inline &operator--(Z &z) {
        __gmp_unary_decrement::eval(z.MPZ);
        return z;
    }
    
    Z inline operator+(const Z &z, int64 n) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, z.MPZ, (signed long int)(n));
        return sum;
    }
    
    Z inline operator+(const Z &a, const Z &b) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, a.MPZ, b.MPZ);
        return sum;
    }
    
    Z inline operator*(const Z& a, int64 n) {
        Z prod{};
        __gmp_binary_multiplies::eval(prod.MPZ, a.MPZ, (signed long int)(n));
        return prod;
    }
    
    Z inline operator*(const Z& a, const Z& b) {
        Z prod{};
        __gmp_binary_multiplies::eval(prod.MPZ, a.MPZ, b.MPZ);
        return prod;
    }
    
    Z inline operator-(const Z& a, const gmp_int n) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, a.MPZ, n);
        return sum;
    }
    
    Z inline operator-(const Z& a, const Z& b) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, a.MPZ, b.MPZ);
        return sum;
    }
    
    Z inline operator-(const Z& n) {
        Z z{n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    
    Z inline operator<<(const Z& z, int x) {
        Z n{};
        __gmp_binary_lshift::eval(&n.MPZ[0], &z.MPZ[0], x);
        return n;
    }
    
    Z inline operator>>(const Z& z, int x) {
        Z n{};
        __gmp_binary_rshift::eval(&n.MPZ[0], &z.MPZ[0], x);
        return n;
    }
    
    Z inline operator++(Z &a, int) {
        Z z = a;
        ++a;
        return z;
    }
    
    Z inline operator--(Z &a, int) {
        Z z = a;
        ++a;
        return z;
    }
    
    Z inline &operator+=(Z &a, int64 n) {
        __gmp_binary_plus::eval(a.MPZ, a.MPZ, (signed long int)(n));
        return a;
    }
    
    Z inline &operator+=(Z &a, const Z& n) {
        __gmp_binary_plus::eval(a.MPZ, a.MPZ, n.MPZ);
        return a;
    }
    
    Z inline &operator/=(Z &a, const Z& z) {
        return a = a / z;
    }
    
}

namespace data::math {
    
    Z inline identity<plus<Z>, Z>::operator()() {
        return 0;
    }
    
    Z inline identity<times<Z>, Z>::operator()() {
        return 1;
    }
    
    Z inline inverse<plus<Z>, Z>::operator()(const Z &a, const Z &b) {
        return b - a;
    }
}

namespace data {
    
    math::sign inline sign(const math::Z &z) {
        return math::number::GMP::sign(z.MPZ[0]);
    }
    
}

namespace data::encoding::hexidecimal { 
    
    std::string inline write(const math::Z& n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
    Z inline &operator-=(Z &a, const Z& n) {
        __gmp_binary_minus::eval(a.MPZ, a.MPZ, n.MPZ);
        return a;
    }
    
    Z inline &operator-=(Z &a, int64 n) {
        __gmp_binary_minus::eval(a.MPZ, a.MPZ, (signed long int)(n));
        return a;
    }
    
    Z inline &operator*=(Z &a, const Z& z) {
        __gmp_binary_multiplies::eval(a.MPZ, a.MPZ, z.MPZ);
        return a;
    }
    
    Z inline &operator*=(Z &a, int64 n) {
        __gmp_binary_multiplies::eval(a.MPZ, a.MPZ, (signed long int)(n));
        return a;
    }
    
    Z inline operator^(Z &a, uint64 n) {
        Z pow{};
        mpz_pow_ui(pow.MPZ, a.MPZ, n);
        return pow;
    }
    
    Z inline &operator^=(Z &a, uint64 n) {
        mpz_pow_ui(a.MPZ, a.MPZ, n);
        return a;
    }
    
    Z inline &operator<<=(Z &a, int x) {
        __gmp_binary_lshift::eval(&a.MPZ[0], &a.MPZ[0], x);
        return a;
    }
    
    Z inline &operator>>=(Z &a, int x) {
        __gmp_binary_rshift::eval(&a.MPZ[0], &a.MPZ[0], x);
        return a;
    }

}

namespace data::encoding::signed_decimal { 
    
    std::string inline write(const math::Z& n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
}

namespace data::math::number::GMP {
    
    Z inline &operator++(Z &n) {
        __gmp_unary_increment::eval(n.MPZ);
        return n;
    }
    
    Z inline &operator--(Z &n) {
        __gmp_unary_decrement::eval(n.MPZ);
        return n;
    }
    
    Z inline &operator+=(Z &z, int64 n) {
        __gmp_binary_plus::eval(z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator+=(Z &z, const Z& n) {
        __gmp_binary_plus::eval(z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator-=(Z &z, int64 n) {
        __gmp_binary_minus::eval(z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator-=(Z &z, const Z &n) {
        __gmp_binary_minus::eval(z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator*=(Z &z, int64 n) {
        __gmp_binary_multiplies::eval(z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator*=(Z &z, const Z& n) {
        __gmp_binary_multiplies::eval(z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator&=(Z &a, const Z &b) {
        __gmp_binary_and::eval(a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline &operator|=(Z &a, const Z &b) {
        __gmp_binary_ior::eval(a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline operator-(const Z &n) {
        Z z{n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    
    Z inline operator+(const Z &z, int64 n) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, z.MPZ, n);
        return sum;
    }
    
    Z inline operator+(int64 n, const Z &z) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, n, z.MPZ);
        return sum;
    }
    
    Z inline operator+(const Z &z, const Z &n) {
        Z sum{};
        __gmp_binary_plus::eval(sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator-(const Z &z, int64 n) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, z.MPZ, n);
        return sum;
    }
    
    Z inline operator-(int64 n, const Z &z) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, n, z.MPZ);
        return sum;
    }
    
    Z inline operator-(const Z &z, const Z &n) {
        Z sum{};
        __gmp_binary_minus::eval(sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator*(const Z &z, int64 n) {
        Z sum{};
        __gmp_binary_multiplies::eval(sum.MPZ, z.MPZ, n);
        return sum;
    }
    
    Z inline operator*(int64 n, const Z &z) {
        Z sum{};
        __gmp_binary_multiplies::eval(sum.MPZ, n, z.MPZ);
        return sum;
    }
    
    Z inline operator*(const Z &z, const Z &n) {
        Z sum{};
        __gmp_binary_multiplies::eval(sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator&(const Z &a, const Z &b) {
        Z x;
        __gmp_binary_and::eval(x.MPZ, a.MPZ, b.MPZ);
        return x;
    }
    
    Z inline operator|(const Z &a, const Z &b) {
        Z x;
        __gmp_binary_ior::eval(x.MPZ, a.MPZ, b.MPZ);
        return x;
    }
    
    Z inline operator<<(const Z &a, int x) {
        Z n{};
        __gmp_binary_lshift::eval(n.MPZ, a.MPZ, x);
        return n;
    }
    
    Z inline operator>>(const Z &a, int x) {
        Z n{};
        __gmp_binary_rshift::eval(n.MPZ, a.MPZ, x);
        return n;
    }
    
    Z inline &operator<<=(Z &a, int x) {
        __gmp_binary_lshift::eval(a.MPZ, a.MPZ, x);
        return a;
    }
    
    Z inline &operator>>=(Z &a, int x) {
        __gmp_binary_rshift::eval(a.MPZ, a.MPZ, x);
        return a;
    }
}

#endif
