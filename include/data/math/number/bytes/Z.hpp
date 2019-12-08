// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/encoding/endian.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/division.hpp>

namespace data::math::number {
    
    template <endian::order o> struct N_bytes;
    
    template <endian::order o>
    struct Z_bytes {
        bytes Value;
        
        Z_bytes();
        
        Z_bytes(const N_bytes<o>&);
        
        Z_bytes(int64 n);
        
        static Z_bytes read(string_view x);
        
        explicit Z_bytes(string_view x);
        
        Z_bytes(const Z_bytes& n);
        
        Z_bytes(Z_bytes&& n);
        
        Z_bytes& operator=(const Z_bytes& n);
        
        math::sign sign() const;
        
        size_t size() const;
        
        using index = uint32;
        
        byte* begin();
        
        byte* end();
        
        const byte* begin() const;
        
        const byte* end() const;
        
        bool operator==(int64 z) const;
        
        bool operator==(const Z_bytes& z) const;
        
        bool operator==(const N_bytes<o>&) const;
        
        bool operator!=(int64 z) const;
        
        bool operator!=(const Z_bytes& z) const {
            return !operator==(z);
        }
        
        bool operator!=(const N_bytes<o>&) const;
        
        bool operator<(int64 n) const;
        
        bool operator<(const Z_bytes& n) const;
        
        bool operator<(const N_bytes<o>&) const;
        
        bool operator>(int64 n) const;
        
        bool operator>(const Z_bytes& n) const;
        
        bool operator>(const N_bytes<o>&) const;
        
        bool operator<=(int64 n) const;
        
        bool operator<=(const Z_bytes& n) const {
            return !operator>(n);
        }
        
        bool operator<=(const N_bytes<o>&) const;
        
        bool operator>=(int64 n) const {
            return !operator<(n);
        }
        
        bool operator>=(const Z_bytes& n) const {
            return !operator<(n);
        }
        
        bool operator>=(const N_bytes<o>&) const;
        
        Z_bytes& operator+=(int64 n);
        
        Z_bytes& operator+=(const Z_bytes& n);
        
        Z_bytes& operator+=(const N_bytes<o>&);
        
        Z_bytes operator+(int64 n) const;
        
        Z_bytes operator+(const Z_bytes& n) const;
        
        Z_bytes operator+(const N_bytes<o>&) const;
        
        Z_bytes operator-(const int64 n) const;
        
        Z_bytes& operator-=(const Z_bytes& n);
        
        Z_bytes operator-(const Z_bytes& n) const;
        
        Z_bytes& operator-=(const N_bytes<o>&);
        
        Z_bytes operator-(const N_bytes<o>&) const;
        
        Z_bytes operator-() const;
        
        Z_bytes& operator++();
        
        Z_bytes& operator--();
        
        Z_bytes operator++(int) {
            Z_bytes z = *this;
            ++(*this);
            return z;
        }
        
        Z_bytes operator--(int) {
            Z_bytes z = *this;
            ++(*this);
            return z;
        }
        
        Z_bytes operator*(int64 n) const;
        
        Z_bytes operator*(const Z_bytes& z) const;
        
        Z_bytes operator*(const N_bytes<o>&) const;
        
        Z_bytes& operator*=(int64 n);
        
        Z_bytes& operator*=(const Z_bytes& z);
        
        Z_bytes& operator*=(const N_bytes<o>&);
        
        Z_bytes operator^(uint32 n) const;
        
        Z_bytes& operator^=(uint32 n);
        
        division<Z_bytes> divide(const Z_bytes& z) const;
        
        division<Z_bytes> divide(const N_bytes<o>&) const;
        
        bool operator|(const Z_bytes& z) const {
            return divide(z).Remainder == 0;
        }
        
        Z_bytes operator/(const Z_bytes& z) const {
            return divide(z).Quotient;
        }
        
        Z_bytes operator%(const Z_bytes& z) const {
            return divide(z).Remainder;
        }
        
        Z_bytes& operator/=(const Z_bytes& z) {
            Z_bytes q = operator/(z);
            return operator=(q);
        }
        
        Z_bytes& operator%=(const Z_bytes& z) {
            Z_bytes r = operator%(z);
            return operator=(r);
        }
        
        Z_bytes operator<<(int64 x) const;
        
        Z_bytes operator>>(int64 x) const;
        
        Z_bytes& operator<<=(int64 x);
        
        Z_bytes& operator>>=(int64 x);
        
        N_bytes<o> abs() const;
        
        constexpr static math::number::integer<Z_bytes> is_integer{};
        
    };

}

namespace data::encoding::hexidecimal {
    
    template <endian::order o>
    string write(const math::number::Z_bytes<o>& n);
    
}

namespace data::encoding::decimal {
    
    template <endian::order o>
    string write(const math::number::Z_bytes<o>& n);
    
}

template <data::endian::order o>
std::ostream& operator<<(std::ostream& s, const data::math::number::Z_bytes<o>& n);

#endif

