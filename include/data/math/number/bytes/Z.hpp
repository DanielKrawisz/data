// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/encoding/endian.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/division.hpp>
#include <data/encoding/words.hpp>

namespace data::math::number {
    
    template <endian::order r> struct N_bytes;
    
    template <endian::order r>
    struct Z_bytes {
        bytes Value;
        
        Z_bytes() : Value{} {}
        
        Z_bytes(const N_bytes<r>& n) : Value{n.Value} {}
        
        Z_bytes(int64 n);
        
        explicit Z_bytes(const N&);
        explicit Z_bytes(const Z&);
        
        static Z_bytes read(string_view x);
        
        explicit Z_bytes(string_view s) : Z_bytes{read(s)} {}
        
        explicit Z_bytes(bytes_view b) : Value{b} {}
        
        explicit operator bytes_view() const {
            return bytes_view(Value);
        }
        
        math::sign sign() const {
            return operator==(0) ? math::zero : operator<(0) ? math::negative : math::positive;
        }
        
        size_t size() const {
            return Value.size();
        }
        
        using index = uint32;
        
        byte& operator[](index i) {
            return Value[i];
        }
        
        const byte& operator[](index i) const {
            return Value[i];
        }
        
        bytes::iterator begin() {
            return Value.begin();
        }
        
        bytes::iterator end() {
            return Value.end();
        }
        
        bytes::const_iterator begin() const {
            return Value.begin();
        }
        
        bytes::const_iterator end() const {
            return Value.end();
        }
        
        constexpr static endian::order opposite = endian::opposite(r);
        
        explicit Z_bytes(const Z_bytes<opposite>&);
        
        explicit Z_bytes(const N_bytes<opposite>&);
        
        Z_bytes<opposite> reverse() const;
        
        Z_bytes& operator=(const Z_bytes& n);
        
        Z_bytes& operator=(const Z_bytes<opposite>& n);
        
        Z_bytes& operator=(const N_bytes<r>& n);
        
        Z_bytes& operator=(const N_bytes<opposite>& n);
        
        bool operator==(int64) const;
        
        bool operator==(const Z_bytes&) const;
        
        bool operator==(const Z_bytes<opposite>&) const;
        
        bool operator==(const N_bytes<opposite>&) const;
        
        bool operator==(const Z&) const;
        
        bool operator==(const N&) const;
        
        bool operator!=(int64 z) const {
            return !operator==(z);
        }
        
        bool operator!=(const Z_bytes& z) const {
            return !operator==(z);
        }
        
        bool operator!=(const Z_bytes<opposite>& z) const {
            return !operator==(z);
        }
        
        bool operator!=(const N_bytes<opposite>& z) const {
            return !operator==(z);
        }
        
        bool operator!=(const Z& z) const {
            return !operator==(z);
        }
        
        bool operator!=(const N& z) const {
            return !operator==(z);
        }
        
        bool operator<(int64 n) const;
        
        bool operator<(const Z_bytes& n) const;
        
        bool operator<(const Z_bytes<opposite>&) const;
        
        bool operator<(const N_bytes<opposite>&) const;
        
        bool operator<(const N&) const;
        
        bool operator<(const Z&) const;
        
        bool operator>(int64 n) const;
        
        bool operator>(const Z_bytes& n) const;
        
        bool operator>(const Z_bytes<opposite>&) const;
        
        bool operator>(const N_bytes<opposite>&) const;
        
        bool operator>(const N&) const;
        
        bool operator>(const Z&) const;
        
        bool operator<=(int64 n) const {
            return !operator>(n);
        }
        
        bool operator<=(const Z_bytes& n) const {
            return !operator>(n);
        }
        
        bool operator<=(const Z_bytes<opposite>& n) const {
            return !operator>(n);
        }
        
        bool operator<=(const N_bytes<opposite>& n) const {
            return !operator>(n);
        }
        
        bool operator<=(const N& n) const {
            return !operator>(n);
        }
        
        bool operator<=(const Z& n) const {
            return !operator>(n);
        }
        
        bool operator>=(int64 n) const {
            return !operator<(n);
        }
        
        bool operator>=(const Z_bytes& n) const {
            return !operator<(n);
        }
        
        bool operator>=(const N_bytes<opposite>& n) const {
            return !operator<(n);
        }
        
        bool operator>=(const Z_bytes<opposite>& n) const {
            return !operator<(n);
        }
        
        bool operator>=(const N& n) const {
            return !operator<(n);
        }
        
        bool operator>=(const Z& n) const {
            return !operator<(n);
        }
        
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
        
        Z_bytes operator+(int64 n) const;
        
        Z_bytes operator+(const Z_bytes&) const;
        
        Z_bytes operator+(const Z_bytes<opposite>&) const;
        
        Z_bytes operator+(const N_bytes<opposite>&) const;
        
        Z_bytes operator+(const Z&) const;
        
        Z_bytes operator+(const N&) const;
        
        Z_bytes& operator+=(int64 n) {
            return operator=(operator^(n));
        }
        
        Z_bytes& operator+=(const Z_bytes& n) {
            return operator=(operator^(n));
        }
        
        Z_bytes& operator+=(const Z_bytes<opposite>& n) {
            return operator=(operator^(n));
        }
        
        Z_bytes& operator+=(const N_bytes<opposite>& n) {
            return operator=(operator^(n));
        }
        
        Z_bytes& operator+=(const Z& n) {
            return operator=(operator^(n));
        }
        
        Z_bytes& operator+=(const N& n) {
            return operator=(operator^(n));
        }
        
        Z_bytes operator-(const int64 n) const;
        
        Z_bytes operator-(const Z_bytes&) const;
        
        Z_bytes operator-(const Z_bytes<opposite>&) const;
        
        Z_bytes operator-(const N_bytes<opposite>&) const;
        
        Z_bytes operator-(const Z&) const;
        
        Z_bytes operator-(const N&) const;
        
        Z_bytes operator-() const;
        
        Z_bytes& operator-=(int64 n) {
            return operator=(operator-(n));
        }
        
        Z_bytes& operator-=(const Z_bytes& n) {
            return operator=(operator-(n));
        }
        
        Z_bytes& operator-=(const Z_bytes<opposite>& n) {
            return operator=(operator-(n));
        }
        
        Z_bytes& operator-=(const N_bytes<opposite>& n) {
            return operator=(operator-(n));
        }
        
        Z_bytes& operator-=(const Z& n) {
            return operator=(operator-(n));
        }
        
        Z_bytes& operator-=(const N& n) {
            return operator=(operator-(n));
        }
        
        Z_bytes operator*(int64 n) const;
        
        Z_bytes operator*(const Z_bytes& z) const;
        
        Z_bytes operator*(const N_bytes<r>&) const;
        
        Z_bytes operator*(const Z_bytes<opposite>&) const;
        
        Z_bytes operator*(const N_bytes<opposite>&) const;
        
        Z_bytes operator*(const Z&) const;
        
        Z_bytes operator*(const N&) const;
        
        Z_bytes& operator*=(int64 n) {
            return operator=(operator*(n));
        }
        
        Z_bytes& operator*=(const Z_bytes& z) {
            return operator=(operator*(z));
        }
        
        Z_bytes& operator*=(const N_bytes<r>& n) {
            return operator=(operator*(n));
        }
        
        Z_bytes& operator*=(const Z_bytes<opposite>& z) {
            return operator=(operator*(z));
        }
        
        Z_bytes& operator*=(const N_bytes<opposite>& n) {
            return operator=(operator*(n));
        }
        
        Z_bytes& operator*=(const Z& z) {
            return operator=(operator*(z));
        }
        
        Z_bytes& operator*=(const N& n) {
            return operator=(operator*(n));
        }
        
        Z_bytes operator^(uint32 n) const;
        
        Z_bytes& operator^=(uint32 n) {
            return operator=(operator^(n));
        }
        
        division<Z_bytes> divide(const Z_bytes&) const;
        
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
            return operator=(operator%(z));
        }
        
        Z_bytes operator/(const Z_bytes<opposite>&) const;
        
        Z_bytes operator%(const Z_bytes<opposite>&) const;
        
        Z_bytes& operator/=(const Z_bytes<opposite>& z) {
            return operator=(operator/(z));
        }
        
        Z_bytes& operator%=(const Z_bytes<opposite>& z) {
            return operator=(operator%(z));
        }
        
        Z_bytes operator/(const Z&) const;
        
        Z_bytes operator%(const Z&) const;
        
        Z_bytes& operator/=(const Z& z) {
            return operator=(operator/(z));
        }
        
        Z_bytes& operator%=(const Z& z) {
            return operator=(operator%(z));
        }
        
        Z_bytes operator<<(int64 x) const;
        
        Z_bytes operator>>(int64 x) const;
        
        Z_bytes& operator<<=(int64 x) {
            return operator=(operator<<(x));
        }
        
        Z_bytes& operator>>=(int64 x) {
            return operator=(operator>>(x));
        }
        
        N_bytes<r> abs() const;
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z_bytes(const bounded<indexed, size, o, true>& b);
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z_bytes(const bounded<indexed, size, o, false>& b);
        
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

namespace data::math::number {
    
    template <endian::order r>
    Z_bytes<r>::Z_bytes(const N& n) : Value{n.write(r)} {}
    
    template <endian::order r>
    Z_bytes<r>::Z_bytes(const Z& n) : Value{n.write(r)} {}
    
    template <endian::order r>
    Z_bytes<r> Z_bytes<r>::read(string_view s) {
        throw method::unimplemented{"Z_bytes::read"};
    }

}

#endif

