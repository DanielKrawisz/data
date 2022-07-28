// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_Z
#define DATA_MATH_NUMBER_BYTES_Z

#include <data/encoding/integer.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/division.hpp>
#include <data/math/abs.hpp>
#include <data/math/arithmetic.hpp>
#include <data/encoding/words.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/math/number/bytes/N.hpp>

#include <algorithm>

namespace data::math::number {
    
    template <endian::order r>
    Z_bytes<r> operator<<(const Z_bytes<r>&, int);
    
    template <endian::order r>
    Z_bytes<r> operator>>(const Z_bytes<r>&, int);
    
    template <endian::order r>
    Z_bytes<r> &operator<<=(const Z_bytes<r>&, int);
    
    template <endian::order r>
    Z_bytes<r> &operator>>=(const Z_bytes<r>&, int);
    
    template <endian::order r>
    struct Z_bytes : oriented<r, byte> {
        friend struct N_bytes<r>;
        
        Z_bytes();
        
        Z_bytes(const N_bytes<r>&);
        
        Z_bytes(int64 x);
        
        explicit Z_bytes(const Z& z);
        
        static Z_bytes read(string_view x);
        
        explicit Z_bytes(string_view s);
        
        Z_bytes(bytes_view b);
        
    private:
        
        Z_bytes(size_t size, byte fill) : oriented<r, byte>(size) {
            this->fill(fill);
        }
        
    public:
        static Z_bytes zero(size_t size) {
            return Z_bytes(size, 0x00);
        }
        
        math::sign sign() const {
            if (this->size() == 0) return math::zero;
            if (is_negative(*this)) return negative;
            if (is_zero(*this)) return math::zero;
            return positive;
        }
        
        Z_bytes operator~() const {
            Z_bytes z = extend(*this, this->size() + 1);
            data::arithmetic::bit_negate<byte>(z.end(), z.begin(), z.begin());
            return z.trim();
        }
        
        Z_bytes& operator++();
        Z_bytes& operator--();
        
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
        
        Z_bytes& operator+=(const Z_bytes& n) {
            return *this = *this + n;
        }
        
        Z_bytes operator-() const {
            return ++(~*this);
        }
        
        Z_bytes& operator-=(const Z_bytes& n) {
            return *this = *this - n;
        }
        
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
        
        Z_bytes &trim();
        
        explicit operator Z() const {
            return is_negative(*this) ? -N(data::abs(*this)) : Z(N(data::abs(*this)));
        }
        /*
        template <size_t size, endian::order o> 
        explicit Z_bytes(const bounded<size, o, true>& b) {
            resize(b.size());
            std::copy(b.words().begin(), b.words().end(), this->words().begin());
        }
        
        template <size_t size, endian::order o> 
        explicit Z_bytes(const bounded<size, o, false>& b) {
            *this = zero(b.size() + 1);
            std::copy(b.words().begin(), b.words().end(), this->words().begin());
        }*/
    };
    
    template <endian::order r> Z_bytes<r> inline trim(const Z_bytes<r> &x) {
        auto n = x;
        return n.trim();
    }
    
    template <endian::order r> bool inline is_negative(const Z_bytes<r> &x) {
        return arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_positive(const Z_bytes<r> &x) {
        return !is_zero(x) && !arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_zero(const Z_bytes<r> &x) {
        return arithmetic::ones_is_zero(x.words());
    }
    
    template <endian::order r> bool inline is_minimal(const Z_bytes<r> &x) {
        return arithmetic::ones_is_minimal(x.words());
    }
    
    template <endian::order r> size_t inline minimal_size(const Z_bytes<r> &x) {
        return arithmetic::ones_minimal_size(x.words());
    }
    
    template <endian::order r> 
    Z_bytes<r> inline operator+(const Z_bytes<r> &a, int64 b) {
        return a + Z_bytes<r>(b);
    }
    
    template <endian::order r> 
    Z_bytes<r> inline operator-(const Z_bytes<r> &a, int64 b) {
        return a - Z_bytes<r>(b);
    }
    
    template <endian::order r> 
    Z_bytes<r> inline operator*(const Z_bytes<r> &a, int64 b) {
        return a * Z_bytes<r>(b);
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes() : oriented<r, byte>{} {}
    
    template <endian::order r> Z_bytes<r>::Z_bytes(const N_bytes<r>& n) {
        this->resize(n.size() + 1);
        this->words()[-1] = 0;
        std::copy(n.words().begin(), n.words().end(), this->words().begin());
        this->trim();
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes(int64 x) : Z_bytes{} {
        this->resize(8);
        endian::arithmetic<r, true, 8> n{x};
        std::copy(n.begin(), n.end(), this->begin());
        this->trim();
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes(bytes_view x) : oriented<r, byte>{x} {}
    
    // First we write the Z as hex and then read it in again. 
    // A bit inefficient but it's really not that bad. 
    template <endian::order r> inline Z_bytes<r>::Z_bytes(const Z& z) : Z_bytes() {
        *this = z < 0 ? -N_bytes<r>(data::abs(z)) : Z_bytes<r>(N_bytes<r>(data::abs(z)));
        this->trim();
    }
    
    template <endian::order r> Z_bytes<r> Z_bytes<r>::read(string_view x) {
        if (x == "") return 0; 
        ptr<Z_bytes> b = encoding::integer::read<r>(x);
        if (b == nullptr) throw std::invalid_argument{"invalid integer string provided"};
        return Z_bytes<r>{*b};
    }
    
    template <endian::order r> Z_bytes<r>::Z_bytes(string_view s) : Z_bytes{read(s)} {}
    
    template <endian::order r> 
    bool inline operator==(const Z_bytes<r> &a, int64 b) {
        return a == Z_bytes<r>(b);
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const Z_bytes<r> &a, int64 b) {
        return a <=> Z_bytes<r>(b);
    }
    
    template <endian::order r> 
    bool inline operator==(const Z_bytes<r> &a, const Z_bytes<r> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const Z_bytes<r> &a, const Z_bytes<r> &b) {
        bool na = is_negative(a);
        bool nb = is_negative(b);
        
        if (na && nb) {
            auto ya = -b;
            auto yb = -a;
            return arithmetic::N_compare(ya.words(), yb.words());
        }
        
        if (!na && !nb) return arithmetic::N_compare(a.words(), b.words());
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <endian::order r> 
    Z_bytes<r> inline operator+(const Z_bytes<r> &a, const Z_bytes<r>& b) {
        return Z_bytes<r>(Z(a) + Z(b));
    }
    
    template <endian::order r> 
    Z_bytes<r> inline operator-(const Z_bytes<r> &a, const Z_bytes<r>& b) {
        return Z_bytes<r>(Z(a) - Z(b));
    }
    
    template <endian::order r> 
    Z_bytes<r> inline operator*(const Z_bytes<r> &a, const Z_bytes<r>& b) {
        return Z_bytes<r>(Z(a) * Z(b));
    }
    
    template <endian::order r> Z_bytes<r> &Z_bytes<r>::trim() {
        size_t size = minimal_size(*this);
        if (size == this->size()) return *this;
        auto n = Z_bytes<r>::zero(size);
        auto w = this->words();
        std::copy(w.begin(), w.begin() + size, n.words().begin());
        return *this = n;
    }
    
    template <endian::order r> Z_bytes<r> extend(const Z_bytes<r> &x, size_t size) {
        if (size < x.size()) {
            size_t min_size = minimal_size(x); 
            if (size < min_size) throw std::invalid_argument{"cannot extend smaller than minimal size"};
            return extend(trim(x), size);
        }
        
        if (size == x.size()) return x;
        
        Z_bytes<r> z;
        z.resize(size);
        byte extend_digit = is_negative(x) ? 0xff : 0x00;
        
        auto i = z.words().rbegin();
        for (int n = 0; n < size - x.size(); n++) {
            *i = extend_digit;
            i++;
        }
        
        std::copy(x.words().rbegin(), x.words().rend(), i);
        return z;
    }
    
    template <endian::order r>
    Z_bytes<r> inline operator<<(const Z_bytes<r> &z, int i) {
        auto n = z;
        return n <<= i;
    }
    
    template <endian::order r>
    Z_bytes<r> inline operator>>(const Z_bytes<r> &z, int i) {
        auto n = z;
        return n >>= i;
    }
    
    namespace {
        template <endian::order r>
        void inline bit_shift_left(Z_bytes<r> &z, uint32 i) {
            z = extend(z, z.size() + (i + 7) / 8);
            z.bit_shift_left(i);
        }
    }
    
    template <endian::order r>
    Z_bytes<r> inline &operator<<=(Z_bytes<r> &n, int i) {
        if (i < 0) n.bit_shift_right(-i);
        else bit_shift_left(n, i);
        return n = n.trim();
    }
    
    template <endian::order r>
    Z_bytes<r> inline &operator>>=(Z_bytes<r> &n, int i) {
        if (i < 0) bit_shift_left(n, -i);
        else n.bit_shift_right(i);
        return n = n.trim();
    }
    
    template <data::endian::order r>
    inline std::ostream& operator<<(std::ostream& o, const data::math::number::Z_bytes<r>& n) {
        if (o.flags() & std::ios::dec) return data::encoding::signed_decimal::write(o, n);
        if (o.flags() & std::ios::hex) return data::encoding::hexidecimal::write(o, n);
        return o;
    }
    
    template <data::endian::order r>
    Z_bytes<r>& Z_bytes<r>::operator++() {
        if (!is_negative(*this)) *this = extend(*this, this->size() + 1);
        data::arithmetic::plus<byte>(this->words().end(), this->words().begin(), 1, this->words().begin());
        return this->trim();
    }
    
    template <data::endian::order r>
    Z_bytes<r>& Z_bytes<r>::operator--() {
        if (!is_positive(*this)) *this = extend(*this, this->size() + 1);
        data::arithmetic::minus<byte>(this->words().end(), this->words().begin(), 1, this->words().begin());
        return this->trim();
    }
    
    template <endian::order r> Z_bytes<r> operator&(const Z_bytes<r> &a, const Z_bytes<r> &b) {
        if (a.size() < b.size()) return b & a;
        auto bt = extend(b, a.size());
        auto x = Z_bytes<r>::zero(a.size());
        data::arithmetic::bit_and<byte>(x.end(), x.begin(), a.begin(), const_cast<const Z_bytes<r>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r> Z_bytes<r> operator|(const Z_bytes<r> &a, const Z_bytes<r> &b) {
        if (a.size() < b.size()) return b | a;
        auto bt = extend(b, a.size());
        auto x = Z_bytes<r>::zero(a.size());
        data::arithmetic::bit_or<byte>(x.end(), x.begin(), a.begin(), const_cast<const Z_bytes<r>&>(bt).begin());
        return x.trim();
    }
    
}

namespace data::math {
    
    template <endian::order r> 
    N_bytes<r> abs<Z_bytes<r>>::operator()(const Z_bytes<r>& i) {
        Z_bytes<r> x = number::is_negative(i) ? -i : i;
        N_bytes<r> u;
        u.resize(x.size());
        std::copy(x.begin(), x.end(), u.begin());
        return u;
    }
}

namespace data {
    template <endian::order r> 
    math::sign inline sign(const math::Z_bytes<r> &n) {
        return math::arithmetic::Z_sign_ones(n.words());
    }
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r> 
    ptr<math::number::Z_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        bool negative = s[0] == '-';
        string_view positive = negative ? s.substr(1) : s; 
        auto z = std::make_shared<math::number::Z_bytes<r>>((math::number::Z_bytes<r>)(*decimal::read<r>(positive)));
        if (negative) *z = -*z;
        z->trim();
        return z;
    }
    
}

#endif

