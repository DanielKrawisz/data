// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/encoding/integer.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/division.hpp>
#include <data/math/number/abs.hpp>
#include <data/math/arithmetic.hpp>
#include <data/encoding/endian/words.hpp>

#include <algorithm>

namespace data::math::number {
    
    template <endian::order r> struct N_bytes;
    
    template <endian::order r>
    struct Z_bytes : bytes {
        friend struct N_bytes<r>;
        
        Z_bytes();
        
        Z_bytes(const N_bytes<r>&);
        
        Z_bytes(int64 x);
        
        explicit Z_bytes(const Z& z);
        
        explicit Z_bytes(const N& n);
        
        static Z_bytes read(string_view x);
        
        explicit Z_bytes(string_view s);
        
        Z_bytes(bytes_view b);
        
    private:
        
        bool is_negative() const {
            if (size() == 0) return false;
            return operator[](r == endian::big ? 0 : size() - 1) >= 0x80;
        }
        
        bool is_zero() const {
            for (int i = 0; i < size(); i++) if (operator[](i) != 0) return false;
            return true;
        }
        
        Z_bytes(size_t size, byte fill) : bytes(size) {
            this->fill(fill);
        }
        
    public:
        static Z_bytes zero(size_t size) {
            return Z_bytes(size, 0x00);
        }
        
        math::sign sign() const {
            if (size() == 0) return math::zero;
            if (is_negative()) return math::negative;
            if (is_zero()) return math::zero;
            return math::positive;
        }
        
        bool operator==(const Z_bytes& z) const;
        
        bool operator!=(const Z_bytes& z) const {
            return !operator==(z);
        }
        
        bool operator<(const Z_bytes& z) const;
        
        bool operator>(const Z_bytes& n) const;
        
        bool operator<=(const Z_bytes& z) const {
            return !operator>(z);
        }
        
        bool operator>=(const Z_bytes& z) const {
            return !operator<(z);
        }
        
        Z_bytes operator~() const {
            Z_bytes z(*this);
            arithmetic::bit_negate(z.end(), z.begin(), z.begin());
            return z;
        }
        
        Z_bytes& operator++() {
            operator+=(1);
            return *this;
        }
        
        Z_bytes& operator--() {
            operator-=(1);
            return *this;
        }
        
        Z_bytes operator++(int) const {
            Z_bytes z = *this;
            ++(*this);
            return z;
        }
        
        Z_bytes operator--(int) const {
            Z_bytes z = *this;
            --(*this);
            return z;
        }
        
        Z_bytes operator+(const Z_bytes&) const;
        
        Z_bytes& operator+=(const Z_bytes& n) {
            return operator=(operator+(n));
        }
        
        Z_bytes operator-(const Z_bytes& z) const;
        
        Z_bytes operator-() const {
            return ++(~*this);
        }
        
        Z_bytes& operator-=(const Z_bytes& n) {
            return operator=(operator-(n));
        }
        
        Z_bytes operator*(const Z_bytes&) const;
        
        Z_bytes& operator*=(const Z_bytes& z) {
            return operator=(operator*(z));
        }
        
        Z_bytes operator^(uint32 n) const;
        
        Z_bytes& operator^=(uint32 n) {
            return operator=(operator^(n));
        }
        
        division<Z_bytes> divide(const Z_bytes& z) const {
            return integer::divide<Z_bytes>(*this, z);
        }
        
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
        
        data::arithmetic::digits<r> digits() {
            return data::arithmetic::digits<r>{slice<byte>(*this)};
        }
        
        const data::arithmetic::digits<r> digits() const {
            return data::arithmetic::digits<r>{slice<byte>(*const_cast<Z_bytes*>(this))};
        }
        
    private:
        // TODO use words type eventually. 
        using words_type = data::arithmetic::digits<r>;
        
        words_type words() {
            return digits();
        }
        
        const words_type words() const {
            return digits();
        }
    };
    
    template <endian::order r> Z_bytes<r>::Z_bytes() : bytes{} {}
    
    template <endian::order r> Z_bytes<r>::Z_bytes(const N_bytes<r>& n) {
        typename data::arithmetic::digits<r>::iterator it;
        if (n.digits()[0] <= 0x80) {
            this->resize(n.size() + 1);
            this->operator[](0) = 0;
            it = this->digits().begin();
            it++;
        } else {
            this->resize(n.size());
            it = this->digits().begin();
            it++;
        }
        std::copy(n.begin(), n.end(), it);
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes(int64 x) : Z_bytes{} {
        this->resize(8);
        endian::arithmetic<r, true, 8> n{x};
        std::copy(n.begin(), n.end(), this->begin());
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes(bytes_view x) : bytes{x} {}
    
    // First we write the Z as hex and then read it in again. 
    // A bit inefficient but it's really not that bad. 
    template <endian::order r>  Z_bytes<r>::Z_bytes(const Z& z) : Z_bytes() {
        if (!z.valid()) throw std::invalid_argument{"invalid Z provided"};
        *this = Z_bytes(data::encoding::hexidecimal::write(z));
    }
    
    template <endian::order r>  Z_bytes<r>::Z_bytes(const N& n) : Z_bytes(Z(n)) {}
    
    template <endian::order r> Z_bytes<r> Z_bytes<r>::read(string_view x) {
        if (x == "") return 0; 
        ptr<Z_bytes> b = encoding::integer::read<r>(x);
        if (b == nullptr) throw std::invalid_argument{"invalid integer string provided"};
        return Z_bytes<r>{*b};
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes(string_view s) : Z_bytes{read(s)} {}
    
    template <typename it> bool check_non_zero(it x, size_t size) {
        bool r = false;
        for (int i = 0; i < size; i++) {
            if (*x != 0) r = true;
            x++;
        }
        return r;
    }
    
    template <endian::order r> 
    bool Z_bytes<r>::operator<(const Z_bytes& z) const {
        math::sign na = sign();
        math::sign nz = z.sign();
        if (na != nz) return na < nz;
        if (na == math::zero) return false;
        auto i = begin();
        auto j = z.begin();
        if (size() > z.size()) {
            if (check_non_zero(i, size() - z.size())) return na == math::negative;
        } else {
            if (check_non_zero(j, z.size() - size())) return na == math::positive;
        }
        return arithmetic::less(end(), i, j);
    }
    
    template <endian::order r> 
    bool Z_bytes<r>::operator>(const Z_bytes& z) const {
        math::sign na = sign();
        math::sign nz = z.sign();
        if (na != nz) return na > nz;
        if (na == math::zero) return false;
        auto i = begin();
        auto j = z.begin();
        auto b = size() > z.size() ? i : j;
        if (size() > z.size()) {
            if (check_non_zero(i, size() - z.size())) return na == math::positive;
        } else {
            if (check_non_zero(j, z.size() - size())) return na == math::negative;
        }
        return arithmetic::greater(end(), i, j);
    }
    
    
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::operator+(const Z_bytes& z) const {
        std::cout << "plus operator" << std::endl;
        Z_bytes re(std::max(size(), z.size()) + 1, 0);
        arithmetic::plus(re.words(), this->words(), z.words());
        return re.trim();
    }
    
    /*
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
    */
    template <endian::order r> 
    Z_bytes<r> Z_bytes<r>::trim() const {
        byte fill = is_negative() ? 0xff : 0x00;
        int extra_room = 0;
        while (extra_room < size() && operator[](extra_room) == fill) extra_room++;
        Z_bytes<r> ru(size() - extra_room, fill);
        if (r == endian::big) std::copy(this->begin() + extra_room, this->end(), ru.begin());
        else std::copy(this->begin(), this->begin() + size() - extra_room, ru.begin());
        return ru;
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
    
    template <endian::order r>
    bool Z_bytes<r>::operator==(const Z_bytes& z) const {
        bool negative_left = is_negative();
        bool negative_right = z.is_negative();
        if (negative_left != negative_right) return false; 
        size_t left_size = size();
        size_t right_size = z.size();
        size_t left_begin;
        size_t right_begin;
        size_t left_end;
        size_t right_end;
        size_t min_size = std::min(left_size, right_size);
        if (r == endian::big) {
            left_begin = 0;
            right_begin = 0;
            left_end = min_size;
            right_end = min_size;
        } else {
            left_end = left_size;
            right_end = right_size;
            if (left_size > right_size) {
                left_begin = left_end - min_size;
                right_begin = 0;
            } else {
                left_begin = 0;
                right_begin = right_end - min_size;
            }
        }
        
        return operator bytes_view().substr(left_begin, left_end) == bytes_view(z).substr(right_begin, right_end);
    }

}

namespace data::math {
    // Declare that the plus and times operation on Z are commutative. 
    template <endian::order r> struct commutative<data::plus<math::number::Z_bytes<r>>, math::number::Z_bytes<r>> {};
    template <endian::order r> struct associative<data::plus<math::number::Z_bytes<r>>, math::number::Z_bytes<r>> {};
    template <endian::order r> struct commutative<data::times<math::number::Z_bytes<r>>, math::number::Z_bytes<r>> {};
    template <endian::order r> struct associative<data::times<math::number::Z_bytes<r>>, math::number::Z_bytes<r>> {};
}

template <data::endian::order r>
inline std::ostream& operator<<(std::ostream& o, const data::math::number::Z_bytes<r>& n) {
    if (o.flags() & std::ios::dec) return data::encoding::integer::write(o, n);
    if (o.flags() & std::ios::hex) return data::encoding::hexidecimal::write(o, n);
    return o;
}

#endif

