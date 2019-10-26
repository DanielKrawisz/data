// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/integer.hpp>

namespace data::math::number::gmp {
    
    struct N;
    
    struct Z {
        mpz_t MPZ;
        
        Z() {
            MPZ[0] = MPZInvalid;
        }
        
        Z(const N&);
        
        bool valid() const {
            return gmp::valid(MPZ[0]);
        }
        
        virtual ~Z() {
            if (valid()) mpz_clear(MPZ);
        }
        
        Z(gmp_int n) : MPZ{} {
            mpz_init_set_si(MPZ, n);
        }
        
        static Z read(string_view x);
        
        explicit Z(string_view x) : Z{read(x)} {};
        
        Z(const Z& n) {
            mpz_init(MPZ);
            mpz_set(MPZ, n.MPZ);
        }
        
        Z(Z&& n) : Z{} {
            mpz_swap(MPZ, n.MPZ);
        }
        
        Z& operator=(const Z& n) {
            if (!valid()) mpz_init(MPZ);
            mpz_set(MPZ, n.MPZ);
            return *this;
        }
        
        math::sign sign() const {
            return gmp::sign(MPZ[0]);
        }
        
        size_t size() const {
            return gmp::size(MPZ[0]);
        }
        
        using index = uint32;
        
        mp_limb_t& operator[](index i) {
            if (i >= MPZ[0]._mp_alloc) throw std::out_of_range{"Z"};
            return *(MPZ[0]._mp_d + i);
        }
        
        const mp_limb_t& operator[](index i) const {
            if (i >= MPZ[0]._mp_alloc) throw std::out_of_range{"Z"};
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
        
        bool operator==(int64 z) const {
            return __gmp_binary_equal::eval(MPZ, (signed long int)(z));
        }
        
        bool operator==(const Z& z) const {
            if (!valid() && !z.valid()) return true;
            return __gmp_binary_equal::eval(MPZ, z.MPZ);
        }
        
        bool operator==(const N&) const;
        
        bool operator!=(int64 z) const {
            return !operator==(z);
        }
        
        bool operator!=(const Z& z) const {
            return !operator==(z);
        }
        
        bool operator!=(const N&) const;
        
        bool operator<(int64 n) const {
            return __gmp_binary_less::eval(MPZ, (signed long int)(n));
        }
        
        bool operator<(const Z& n) const {
            return __gmp_binary_less::eval(MPZ, n.MPZ);
        }
        
        bool operator<(const N&) const;
        
        bool operator>(int64 n) const {
            return __gmp_binary_greater::eval(MPZ, (signed long int)(n));
        }
        
        bool operator>(const Z& n) const {
            return __gmp_binary_greater::eval(MPZ, n.MPZ);
        }
        
        bool operator>(const N&) const;
        
        bool operator<=(int64 n) const {
            return !operator>(n);
        }
        
        bool operator<=(const Z& n) const {
            return !operator>(n);
        }
        
        bool operator<=(const N&) const;
        
        bool operator>=(int64 n) const {
            return !operator<(n);
        }
        
        bool operator>=(const Z& n) const {
            return !operator<(n);
        }
        
        bool operator>=(const N&) const;
        
        Z& operator+=(int64 n) {
            __gmp_binary_plus::eval(MPZ, MPZ, (signed long int)(n));
            return *this;
        }
        
        Z& operator+=(const Z& n) {
            __gmp_binary_plus::eval(MPZ, MPZ, n.MPZ);
            return *this;
        }
        
        Z& operator+=(const N&);
        
        Z operator+(int64 n) const {
            Z sum{};
            __gmp_binary_plus::eval(sum.MPZ, MPZ, (signed long int)(n));
            return sum;
        }
        
        Z operator+(const Z& n) const {
            Z sum{};
            __gmp_binary_plus::eval(sum.MPZ, MPZ, n.MPZ);
            return sum;
        }
        
        Z operator+(const N&) const;
        
        Z operator-(const gmp_int n) const {
            Z sum{};
            __gmp_binary_minus::eval(sum.MPZ, MPZ, n);
            return sum;
        }
        
        Z& operator-=(const Z& n) {
            __gmp_binary_minus::eval(MPZ, MPZ, n.MPZ);
            return *this;
        }
        
        Z operator-(const Z& n) const {
            Z sum{};
            __gmp_binary_minus::eval(sum.MPZ, MPZ, n.MPZ);
            return sum;
        }
        
        Z& operator-=(const N&);
        
        Z operator-(const N&) const {
            throw method::unimplemented{};
        }
        
        Z operator-() const {
            throw method::unimplemented{};
        }
        
        Z& operator++() {
            __gmp_unary_increment::eval(MPZ);
            return *this;
        }
        
        Z& operator--() {
            __gmp_unary_decrement::eval(MPZ);
            return *this;
        }
        
        Z operator++(int) {
            Z z = *this;
            ++(*this);
            return z;
        }
        
        Z operator--(int) {
            Z z = *this;
            ++(*this);
            return z;
        }
        
        Z operator*(int64 n) const {
            Z prod{};
            __gmp_binary_multiplies::eval(prod.MPZ, MPZ, (signed long int)(n));
            return prod;
        }
        
        Z operator*(const Z& z) const {
            Z prod{};
            __gmp_binary_multiplies::eval(prod.MPZ, MPZ, z.MPZ);
            return prod;
        }
        
        Z operator*(const N&) const;
        
        Z& operator*=(int64 n) {
            __gmp_binary_multiplies::eval(MPZ, MPZ, (signed long int)(n));
            return *this;
        }
        
        Z& operator*=(const Z& z) {
            __gmp_binary_multiplies::eval(MPZ, MPZ, z.MPZ);
            return *this;
        }
        
        Z& operator*=(const N&);
        
        Z operator^(uint32 n) const {
            Z pow{};
            mpz_pow_ui(pow.MPZ, MPZ, n);
            return pow;
        }
        
        Z& operator^=(uint32 n) {
            mpz_pow_ui(MPZ, MPZ, n);
            return *this;
        }
        
        division<Z> divide(const Z& z) const {
            division<Z> qr{};
            mpz_fdiv_qr(qr.Quotient.MPZ, qr.Remainder.MPZ, MPZ, z.MPZ);
            return qr;
        }
        
        division<Z> divide(const N&) const;
        
        bool operator|(const Z& z) const {
            return divide(z).Remainder == 0;
        }
        
        Z operator/(const Z& z) const {
            return divide(z).Quotient;
        }
        
        Z operator%(const Z& z) const {
            return divide(z).Remainder;
        }
        
        Z& operator/=(const Z& z) {
            Z q = operator/(z);
            return operator=(q);
        }
        
        Z& operator%=(const Z& z) {
            Z r = operator%(z);
            return operator=(r);
        }
        
        Z operator<<(int64 x) const {
            Z n{};
            __gmp_binary_lshift::eval(&n.MPZ[0], &MPZ[0], x);
            return n;
        }
        
        Z operator>>(int64 x) const {
            Z n{};
            __gmp_binary_rshift::eval(&n.MPZ[0], &MPZ[0], x);
            return n;
        }
        
        Z& operator<<=(int64 x) {
            __gmp_binary_lshift::eval(&MPZ[0], &MPZ[0], x);
            return *this;
        }
        
        Z& operator>>=(int64 x) {
            __gmp_binary_lshift::eval(&MPZ[0], &MPZ[0], x);
            return *this;
        }
        
        N abs() const;
        
        constexpr static math::number::integer<Z> is_integer{};
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z(const bounded<indexed, size, o, true>& b) : Z{0}{
            typename bounded<indexed, size, o, true>::words_type w = b.words();
            for (int i = bounded<indexed, size, o, true>::words_type::last; i > 0; i--) {
                operator+=(uint32(int32(w[i])));
                operator<<=(32);
            }
            operator+=(uint32(int32(w[0])));
            if (b < 0) operator-=(bounded<indexed, size, o, true>::modulus()); 
        }
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z(const bounded<indexed, size, o, false>& b) : Z{0}{
            typename bounded<indexed, size, o, false>::words_type w = b.words();
            for (int i = bounded<indexed, size, o, false>::words_type::last; i > 0; i--) {
                operator+=(uint32(w[i]));
                operator<<=(32);
            }
            operator+=(uint32(w[0]));
        }
        
    };
    
    N square(const Z& z);
    
    N abs(const Z& z);
    
    N norm(const Z& z);

}

namespace data::encoding::hexidecimal {
    
    string write(const math::number::gmp::Z& n);
    
}

namespace data::encoding::decimal {
    
    string write(const math::number::gmp::Z& n);
    
}

std::ostream& operator<<(std::ostream& o, const data::math::number::gmp::Z& n);

#endif
