// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/encoding/endian.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/division.hpp>
#include <data/encoding/words.hpp>
#include <data/io/unimplemented.hpp>

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
        
        Z_bytes& operator=(const Z_bytes& n) {
            throw method::unimplemented{"Z_bytes::="};
        }
        
        Z_bytes& operator=(const Z_bytes<opposite>& n) {
            return operator=(n.reverse());
        }
        
        Z_bytes& operator=(const N_bytes<r>& n) {
            throw method::unimplemented{"Z_bytes::="};
        }
        
        Z_bytes& operator=(const N_bytes<opposite>& n) {
            return operator=(n.reverse());
        }
        
        Z_bytes& operator=(const N& n) {
            return operator=(Z_bytes{n});
        }
        
        Z_bytes& operator=(const Z& n) {
            return operator=(Z_bytes{n});
        }
        
        bool operator==(int64 i) const {
            return operator==(Z_bytes{i});
        }
        
        bool operator==(const Z_bytes&) const {
            throw method::unimplemented{"Z_bytes == Z_bytes"};
        }
        
        bool operator==(const Z_bytes<opposite>& n) const {
            return operator==(n.reverse());
        }
        
        bool operator==(const N_bytes<opposite>& n) const {
            return operator==(n.reverse());
        }
        
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
        
        bool operator<(int64 n) const {
            return operator<(Z_bytes<r>{n});
        }
        
        bool operator<(const Z_bytes& n) const;
        
        bool operator<(const Z_bytes<opposite>& n) const {
            return operator<(n.reverse());
        }
        
        bool operator<(const N_bytes<opposite>& n) const {
            return operator<(n.reverse());
        }
        
        bool operator<(const N&) const;
        
        bool operator<(const Z&) const;
        
        bool operator>(int64 n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z_bytes& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z_bytes<opposite>& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const N_bytes<opposite>& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const N& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z& n) const {
            return !operator<=(n);
        }
        
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
        
        Z_bytes& operator++() {
            operator+=(1);
            return *this;
        }
        
        Z_bytes& operator--() {
            operator+=(1);
            return *this;
        }
        
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
        
        Z_bytes operator+(const Z_bytes<opposite>& n) const {
            return operator+(n.reverse());
        }
        
        Z_bytes operator+(const N_bytes<opposite>& n) const {
            return operator+(n.reverse());
        }
        
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
        
        Z_bytes operator-(const int64 n) const {
            return operator-(Z_bytes{n});
        }
        
        Z_bytes operator-(const Z_bytes& n) const {
            throw method::unimplemented{"Z_bytes::-"};
        }
        
        Z_bytes operator-(const Z_bytes<opposite>& n) const {
            return operator-(n.reverse());
        }
        
        Z_bytes operator-(const N_bytes<opposite>& n) const {
            return operator-(n.reverse());
        }
        
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
        
        Z_bytes operator*(const Z_bytes<opposite>& n) const {
            return operator*(n.reverse());
        }
        
        Z_bytes operator*(const N_bytes<opposite>& n) const {
            return operator*(n.reverse());
        }
        
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
        
        Z_bytes operator/(const Z_bytes<opposite>& n) const {
            return operator/(n.reverse());
        }
        
        Z_bytes operator%(const Z_bytes<opposite>& n) const {
            return operator%(n.reverse());
        }
        
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
        
        Z_bytes operator<<(int64 x) const {
            throw method::unimplemented{"Z_bytes::<<"};
        }
        
        Z_bytes operator>>(int64 x) const {
            throw method::unimplemented{"Z_bytes::>>"};
        }
        
        Z_bytes& operator<<=(int64 x) {
            return operator=(operator<<(x));
        }
        
        Z_bytes& operator>>=(int64 x) {
            return operator=(operator>>(x));
        }
        
        N_bytes<r> abs() const;
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z_bytes(const bounded<indexed, size, o, true>& b) {
            throw method::unimplemented{"Z_bytes constructor bounded true"};
        }
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z_bytes(const bounded<indexed, size, o, false>& b) {
            throw method::unimplemented{"Z_bytes constructor bounded false"};
        }
        
        constexpr static math::number::integer<Z_bytes> is_integer{};
    private:
        Z_bytes(bytes_view, endian::order) {
            throw method::unimplemented{"Z_bytes read bytes and endian ordering."};
        }
        
        using word = endian::ordered<uint32, r>;
        using words_type = arithmetic::unfixed_words<uint32, r>;
        using methods = arithmetic::unoriented<words_type, word>;
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
std::ostream& operator<<(std::ostream& s, const data::math::number::Z_bytes<o>& n) {
    throw data::method::unimplemented{"std::ostream << Z_bytes"};
}

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

