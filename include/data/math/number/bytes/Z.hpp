// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/encoding/integer.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/division.hpp>
#include <data/encoding/words.hpp>
#include <algorithm>

namespace data::math::number {
    
    template <endian::order r> struct N_bytes;
    
    template <endian::order r>
    struct Z_bytes {
        bytes Value;
        
        Z_bytes() : Value{} {}
        
        Z_bytes(const N_bytes<r>& n) : Value{n.Value} {}
        
        Z_bytes(int64 x) {
            Value.resize(8);
            *(int64*)(Value.data()) = endian::native<int64, r>{}.from(x);
        }
        
        explicit Z_bytes(const N& n) : Value{n.write(r)} {}
        explicit Z_bytes(const Z& n) : Value{n.write(r)} {}
        
        static Z_bytes read(string_view x) {
            return Z_bytes<r>{encoding::integer::read(x, r)};
        }
        
        explicit Z_bytes(string_view s) : Z_bytes{read(s)} {}
        
        explicit Z_bytes(bytes_view b) : Value{b} {}
        
        explicit operator bytes_view() const {
            return bytes_view(Value);
        }
        
    private:
        bool is_negative() const {
            if (Value.size() == 0) return false;
            return Value[r == endian::big ? 0 : Value.size() - 1] >= 0x80;
        }
        
        bool is_zero() const {
            for (int i = 0; i < Value.size(); i++) if (Value[i] != 0) return false;
            return true;
        }
        
        Z_bytes(bytes_view b, endian::order o) {
            Value.resize(b.size());
            std::copy(b.begin(), b.end(), Value.begin());
            if (o != r) std::reverse(Value.begin(), Value.end());
        }
        
        using word = endian::ordered<int32, r>;
        using words_type = arithmetic::unfixed_words<int32, r>;
        using methods = arithmetic::unoriented<words_type, word>;
            
        words_type words() {
            return words_type{slice<byte>{Value}};
        }
        
        const words_type words() const {
            return words_type::make(slice<byte>::make(Value));
        }
        
        constexpr static endian::order opposite = endian::opposite(r);
        
        Z_bytes<opposite> reverse() const;
        
    public:
        math::sign sign() const {
            if (Value.size() == 0) return math::zero;
            if (is_negative()) return math::negative;
            if (is_zero()) return math::zero;
            return math::positive;
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
        
        explicit Z_bytes(const Z_bytes<opposite>&);
        
        explicit Z_bytes(const N_bytes<opposite>&);
        
        Z_bytes& operator=(const Z_bytes& n) {
            Value = n.Value;
            return *this;
        }
        
        Z_bytes& operator=(const Z_bytes<opposite>& n) {
            return operator=(n.reverse());
        }
        
        Z_bytes& operator=(const N_bytes<r>& n) {
            Value = n.Value;
            return *this;
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
        
        bool operator==(const Z_bytes& z) const {
            bool negative_left = is_negative();
            bool negative_right = z.is_negative();
            if (negative_left != negative_right) return false; 
            throw method::unimplemented{"Z_bytes == "};
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
        
        bool operator<(const Z_bytes& z) const {
            bool na = is_negative();
            bool nz = z.is_negative();
            return na != nz ? na : methods::less(std::max(Value.size(), z.Value.size()), words(), z.words());
        }
        
        bool operator<(const N_bytes<r>& z) const {
            return is_negative() ? true : methods::less(std::max(Value.size(), z.Value.size()), words(), z.words());
        }
        
        bool operator<(const Z_bytes<opposite>& n) const {
            return operator<(n.reverse());
        }
        
        bool operator<(const N_bytes<opposite>& n) const {
            return operator<(n.reverse());
        }
        
        bool operator<(const N& n) const {
            return Z{*this} < n;
        }
        
        bool operator<(const Z& n) const {
            return Z{*this} < n;
        }
        
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
            return operator<=(Z_bytes<r>{n});
        }
        
        bool operator<=(const Z_bytes& z) const {
            bool na = is_negative();
            bool nz = z.is_negative();
            return na != nz ? na : methods::less_equal(std::max(Value.size(), z.Value.size()), words(), z.words());
        }
        
        bool operator<=(const N_bytes<r>& z) const {
            return is_negative() ? true : methods::less_equal(std::max(Value.size(), z.Value.size()), words(), z.words());
        }
        
        bool operator<=(const N_bytes<opposite>& n) const {
            return operator<=(n.reverse());
        }
        
        bool operator<=(const N& n) const {
            return Z{*this} <= n;
        }
        
        bool operator<=(const Z& n) const {
            return Z{*this} <= n;
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
            return operator=(operator+(n));
        }
        
        Z_bytes& operator+=(const Z_bytes& n) {
            return operator=(operator+(n));
        }
        
        Z_bytes& operator+=(const Z_bytes<opposite>& n) {
            return operator=(operator+(n));
        }
        
        Z_bytes& operator+=(const N_bytes<opposite>& n) {
            return operator=(operator+(n));
        }
        
        Z_bytes& operator+=(const Z& n) {
            return operator=(operator+(n));
        }
        
        Z_bytes& operator+=(const N& n) {
            return operator=(operator+(n));
        }
        
        Z_bytes operator-(const int64 n) const {
            return operator-(Z_bytes{n});
        }
        
        Z_bytes operator-(const Z_bytes& z) const;
        
        Z_bytes operator-(const Z_bytes<opposite>& n) const {
            return operator-(n.reverse());
        }
        
        Z_bytes operator-(const N_bytes<opposite>& n) const {
            return operator-(n.reverse());
        }
        
        Z_bytes operator-(const Z& z) const {
            return Z{*this} - z;
        }
        
        Z_bytes operator-(const N& z) const {
            return Z{*this} - z;
        }
        
        Z_bytes operator-() const {
            return Z_bytes{0} - *this;
        }
        
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
        
        Z_bytes operator*(const Z_bytes&) const;
        
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
        
        Z_bytes operator/(const Z& z) const {
            return Z{*this} / z;
        }
        
        Z_bytes operator%(const Z& z) const {
            return Z{*this} % z;
        }
        
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
        
        Z_bytes trim() const;
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z_bytes(const bounded<indexed, size, o, true>& b) : Z_bytes{bytes_view(b), o} {}
        
        template <typename indexed, size_t size, endian::order o> 
        explicit Z_bytes(const bounded<indexed, size, o, false>& b) : Z_bytes{bytes_view(b), o} {}
        
        constexpr static math::number::integer<Z_bytes> is_integer{};
    };
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::operator+(const Z_bytes& z) const {
        uint32 size = std::max(Value.size(), z.Value.size()) + 1;
        Z_bytes re{};
        re.Value.resize(size);
        methods::plus(size, words(), z.words(), re.words());
        return re.trim();
    }
    
    template <endian::order r>
    Z_bytes<r> Z_bytes<r>::operator-(const Z_bytes& z) const {
        uint32 size = std::max(Value.size(), z.Value.size()) + 1;
        Z_bytes re{};
        re.Value.resize(size);
        methods::minus(size, words(), z.words(), re.words());
        return re.trim();
    }
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::operator*(const Z_bytes& z) const {
        uint32 size = Value.size() + z.Value.size() + 1;
        Z_bytes re{};
        re.Value.resize(size);
        methods::times(size, words(), z.words(), re.words());
        return re.trim();
    }
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::trim() const {
        if (!is_negative()) return Z_bytes{N_bytes<r>{Value}.trim().Value};
        throw method::unimplemented{"Z_bytes::trim"};
    }

}

namespace data::encoding::hexidecimal {
    
    template <endian::order o>
    string write(const math::number::Z_bytes<o>& n) {
        throw method::unimplemented{"hexidecimal::write(Z_bytes)"};
    }
    
}

namespace data::encoding::decimal {
    
    template <endian::order o>
    string write(const math::number::Z_bytes<o>& n) {
        throw method::unimplemented{"decimal::write(Z_bytes)"};
    }
    
}

template <data::endian::order r>
std::ostream& operator<<(std::ostream& o, const data::math::number::Z_bytes<r>& n) {
    if (o.flags() & std::ios::hex) o << data::encoding::decimal::write(n);
    if (o.flags() & std::ios::dec) o << data::encoding::hexidecimal::write(n);
    return o;
}

#endif

