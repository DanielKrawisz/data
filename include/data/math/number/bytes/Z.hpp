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
#include <data/math/number/abs.hpp>
#include <algorithm>

namespace data::math::number {
    
    template <endian::order r> struct N_bytes;
    
    template <endian::order r>
    struct Z_bytes : bytes {
        
        Z_bytes() : bytes{} {}
        
        Z_bytes(const N_bytes<r>& n) : bytes{static_cast<const bytes&>(n)} {}
        
        Z_bytes(int64 x) : bytes{8} {
            *(int64*)(bytes::data()) = endian::native<int64, r>{}.from(x);
        }
        
        // TODO should be able to write directly
        explicit Z_bytes(const N& n) : bytes{} {
            n.write_bytes(static_cast<bytes&>(*this), r);
        }
        
        explicit Z_bytes(const Z& n) : bytes{} {
            n.write_bytes(static_cast<bytes&>(*this), r);
        }
        
        static Z_bytes read(string_view x) {
            return Z_bytes<r>{encoding::integer::read(x, r)};
        }
        
        explicit Z_bytes(string_view s) : Z_bytes{read(s)} {}
        
        Z_bytes(bytes_view b) : bytes{b} {}
        
    private:
        bool is_negative() const {
            if (size() == 0) return false;
            return operator[](r == endian::big ? 0 : size() - 1) >= 0x80;
        }
        
        bool is_zero() const {
            for (int i = 0; i < size(); i++) if (operator[](i) != 0) return false;
            return true;
        }
        
        Z_bytes(bytes_view b, endian::order o) : bytes{b.size()} {
            std::copy(b.begin(), b.end(), begin());
            if (o != r) std::reverse(begin(), end());
        }
        
        using word = boost::endian::endian_arithmetic<r, int32, 32>;
        using words_type = arithmetic::unfixed_words<int32, r>;
        using methods = arithmetic::unoriented<words_type, word>;
            
        words_type words() {
            return words_type{static_cast<slice<byte>>(*this)};
        }
        
        const words_type words() const {
            return words_type::make(static_cast<slice<byte>>(*this));
        }
        
        constexpr static endian::order opposite = endian::opposite(r);
        
        Z_bytes<opposite> reverse() const;
        
    public:
        math::sign sign() const {
            if (size() == 0) return math::zero;
            if (is_negative()) return math::negative;
            if (is_zero()) return math::zero;
            return math::positive;
        }
        
        explicit Z_bytes(const Z_bytes<opposite>&);
        
        explicit Z_bytes(const N_bytes<opposite>&);
        
        Z_bytes& operator=(const Z_bytes<opposite>& n) {
            return operator=(n.reverse());
        }
        
        Z_bytes& operator=(const N_bytes<r>& n) {
            return operator=(Z_bytes{static_cast<const bytes&>(n)});
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
        
        /*bool operator==(int64 i) const {
            return operator==(Z_bytes{i});
        }*/
        
        bool operator==(const Z_bytes& z) const {
            bool negative_left = is_negative();
            bool negative_right = z.is_negative();
            if (negative_left != negative_right) return false; 
            throw method::unimplemented{"Z_bytes == "};
        }
        
        /*bool operator==(const Z_bytes<opposite>& n) const {
            return operator==(n.reverse());
        }
        
        bool operator==(const N_bytes<opposite>& n) const {
            return operator==(n.reverse());
        }
        
        bool operator==(const Z&) const;
        
        bool operator==(const N&) const;*/
        
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
            return na != nz ? na : methods::less(std::max(size(), z.size()), words(), z.words());
        }
        
        bool operator<(const N_bytes<r>& z) const {
            return is_negative() ? true : methods::less(std::max(size(), z.size()), words(), z.words());
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
            return na != nz ? na : methods::less_equal(std::max(size(), z.size()), words(), z.words());
        }
        
        bool operator<=(const N_bytes<r>& z) const {
            return is_negative() ? true : methods::less_equal(std::max(size(), z.size()), words(), z.words());
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
        
        Z_bytes<r> abs() const {
            throw method::unimplemented{"Z_bytes::abs"};
        }
        
        Z_bytes trim() const;
        
        template <size_t size, endian::order o> 
        explicit Z_bytes(const bounded<size, o, true>& b) : Z_bytes{bytes_view(b), o} {}
        
        template <size_t size, endian::order o> 
        explicit Z_bytes(const bounded<size, o, false>& b) : Z_bytes{bytes_view(b), o} {}
    };
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::operator+(const Z_bytes& z) const {
        uint32 s = std::max(size(), z.size()) + 1;
        Z_bytes re{bytes{s}};
        methods::plus(s, words(), z.words(), re.words());
        return re.trim();
    }
    
    template <endian::order r>
    Z_bytes<r> Z_bytes<r>::operator-(const Z_bytes& z) const {
        uint32 s = std::max(size(), z.size()) + 1;
        Z_bytes re{bytes{s}};
        methods::minus(s, words(), z.words(), re.words());
        return re.trim();
    }
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::operator*(const Z_bytes& z) const {
        uint32 s = size() + z.size() + 1;
        Z_bytes re{bytes{s}};
        methods::times(s, words(), z.words(), re.words());
        return re.trim();
    }
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::trim() const {
        throw method::unimplemented{"Z_bytes::trim"};
    }

    template <endian::order r> 
    struct abs<Z_bytes<r>, Z_bytes<r>> {
        Z_bytes<r> operator()(const Z_bytes<r>& i) {
            return i.abs();
        }
    };

    template <endian::order r> 
    struct abs<N_bytes<r>, Z_bytes<r>> {
        N_bytes<r> operator()(const Z_bytes<r>& i) {
            return i.abs();
        }
    };

}

namespace data::encoding::hexidecimal {
    
    template <endian::order o>
    std::string write(const math::number::Z_bytes<o>& n) {
        throw method::unimplemented{"hexidecimal::write(Z_bytes)"};
    }
    
}

namespace data::encoding::decimal {
    
    template <endian::order o>
    std::string write(const math::number::Z_bytes<o>& n) {
        throw method::unimplemented{"decimal::write(Z_bytes)"};
    }
    
}

template <data::endian::order r>
inline std::ostream& operator<<(std::ostream& o, const data::math::number::Z_bytes<r>& n) {
    if (o.flags() & std::ios::hex) o << data::encoding::decimal::write(n);
    if (o.flags() & std::ios::dec) o << data::encoding::hexidecimal::write(n);
    return o;
}

#endif

