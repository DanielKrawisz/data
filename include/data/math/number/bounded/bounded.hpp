// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <data/cross.hpp>
#include <data/math/group.hpp>
#include <data/math/abs.hpp>
#include <data/math/sign.hpp>
#include <data/encoding/halves.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/encoding/words.hpp>

namespace data::math::number {
    
    // satisfies range<byte>
    template <bool is_signed, endian::order, size_t size> struct bounded;
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator==(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator!=(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator<=(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator>=(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator~(const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator&(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator|(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator^(const bounded<is_signed, r, size>&, const bounded<is_signed, r, size>&);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator<<(const bounded<is_signed, r, size>&, int);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator>>(const bounded<is_signed, r, size>&, int);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> &operator<<=(bounded<is_signed, r, size>&, int);
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> &operator>>=(bounded<is_signed, r, size>&, int);
}

namespace data::math { 
    template <endian::order r, size_t size>
    using uint = typename number::bounded<false, r, size>;
    
    template <endian::order r, size_t size>
    using sint = typename number::bounded<true, r, size>;
    
    template <size_t size>
    using uint_little = typename number::bounded<false, endian::little, size>;
    
    template <size_t size>
    using uint_big = typename number::bounded<false, endian::big, size>;
    
    template <size_t size>
    using sint_little = typename number::bounded<true, endian::little, size>;
    
    template <size_t size>
    using sint_big = typename number::bounded<true, endian::big, size>;

    // plus and times are associative and commutative
    // for both bounded and bytes. 
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<times<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<times<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
}

namespace data::math::number {
    
    template <endian::order r, size_t size>
    sint<r, size> operator-(const sint<r, size>&);
    
    template <endian::order r, size_t size>
    uint<r, size> operator+(const uint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator+(const sint<r, size>&, const sint<r, size>&);
    
    template <endian::order r, size_t size>
    uint<r, size> operator-(const uint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator-(const sint<r, size>&, const sint<r, size>&);
    
    template <endian::order r, size_t size>
    uint<r, size> operator*(const uint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator*(const sint<r, size>&, const sint<r, size>&);

    template <data::endian::order r, size_t size>
    std::ostream& operator<<(std::ostream& s, const sint<r, size>& n);

    template <data::endian::order r, size_t size>
    std::ostream& operator<<(std::ostream& s, const uint<r, size>& n);
    
    template <endian::order r, size_t size>
    bool operator==(const uint<r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator!=(const uint<r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator==(const sint<r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bool operator!=(const sint<r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bool operator>=(const uint<r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator<=(const uint<r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator>=(const sint<r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bool operator<=(const sint<r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    uint<r, size> operator+(const uint<r, size>&, uint64);
    
    template <endian::order r, size_t size>
    sint<r, size> operator+(const sint<r, size>&, int64);
    
    template <endian::order r, size_t size>
    uint<r, size> operator-(const uint<r, size>&, uint64);
    
    template <endian::order r, size_t size>
    sint<r, size> operator-(const sint<r, size>&, int64);
    
    template <bool u, endian::order r, size_t x> bool is_zero(const bounded<u, r, x> &);
    
    template <endian::order r, size_t x> bool is_negative(const uint<r, x> &);
    template <endian::order r, size_t x> bool is_negative(const sint<r, x> &);
    
    template <endian::order r, size_t x> bool is_positive(const uint<r, x> &);
    template <endian::order r, size_t x> bool is_positive(const sint<r, x> &);
    
}
    
namespace data::encoding::hexidecimal {
    
    template <bool is_signed, endian::order r, size_t size>
    std::string write(const math::number::bounded<is_signed, r, size>& n);

}

namespace data::encoding::decimal {
    
    template <bool is_signed, endian::order r, size_t size>
    std::string write(const math::number::bounded<is_signed, r, size>& n);
    
}

namespace data::math::number {
    
    template <endian::order r, size_t size>
    struct bounded<false, r, size> : byte_array<size> {
        
        bounded() : byte_array<size>() {
            this->fill(0x00);
        }
        
        bounded(const uint64 x);
        
        bounded(const byte_array<size>& b);
        
        explicit bounded(slice<byte, size> x);
        
        // The string can be a hex string or a representation of a number. 
        explicit bounded(string_view s);
        static bounded read(string_view s) {
            return bounded{s};
        }
        
        math::sign sign() const;
        
        bool operator>(const bounded&) const;
        bool operator<(const bounded&) const;
        
        bounded& operator^=(const bounded&);
        
        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
        bounded& operator*=(const bounded&);
        
        bounded& operator+=(uint64);
        bounded& operator-=(uint64);
        bounded& operator*=(uint64);
        
        bounded& operator++();
        bounded& operator--();
        
        bounded operator++(int);
        bounded operator--(int);
        
        math::division<bounded> divide(const bounded&) const;
        
        bounded operator/(const bounded& n) const {
            return divide(n).Quotient;
        }
        
        bounded operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
        static bounded max();
        static bounded min();
        static N_bytes<r> modulus();
        
        using words_type = encoding::words<r, byte>;
        
        words_type words() {
            return words_type{slice<byte>(*this)};
        }
        
        const words_type words() const {
            return words_type{slice<byte>(*const_cast<bounded*>(this))};
        }
        
    private:
        explicit bounded(const N_bytes<r>& n) : bounded{} {
            if (n.size() <= size) {
                if (r == endian::little) {
                    std::copy(n.begin(), n.end(), byte_array<size>::begin());
                } else {
                    std::copy(n.begin(), n.end(), byte_array<size>::begin() + (size - n.size()));
                }
            }
            if (n > N_bytes<r> {max()}) throw std::out_of_range{"N_bytes too big"};
            if (r == endian::little) {
                std::copy(n.begin(), n.begin() + size, byte_array<size>::begin());
            } else {
                std::copy(n.begin() + (n.size() - size), n.end(), byte_array<size>::begin());
            }
        }
        
        bounded(const sint<r, size>&) {
            throw method::unimplemented{"bounded<size, o, false>{bounded<size, o, true>}"};
        }
        
        friend struct abs<sint<r, size>>;
    };
    
    template <endian::order r, size_t size>
    struct bounded<true, r, size> : byte_array<size> {
        
        bounded() : byte_array<size>{} {
            this->fill(0);
        }
        
        bounded(const int64 x);
        
        bounded(const byte_array<size>& x) : byte_array<size>{x} {}
        
        bounded(const bounded<false, r, size>&);
        
        explicit bounded(string_view s);
        static bounded read(string_view s) {
            return bounded{s};
        }
        
        explicit bounded(slice<byte, size>);
        
        bounded& operator=(const bounded& d) {
            bytes::operator=(static_cast<const bytes&>(d));
            return *this;
        }
        
        math::sign sign() const;
        
        bool operator>(const bounded&) const;
        bool operator<(const bounded&) const;
        
        bounded& operator^=(const bounded<size, r, false>&);

        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
        bounded& operator*=(const bounded&);

        bounded& operator+=(int64);
        bounded& operator-=(int64);
        bounded& operator*=(int64);
        
        static bounded max();
        static bounded min();

        static N_bytes<r> modulus() {
            return uint<r, size>::modulus();
        }

        bounded& operator++();
        bounded& operator--();
        
        bounded operator++(int);
        bounded operator--(int);

        math::division<bounded> divide(const bounded&) const;
        
        bounded operator/(const bounded& n) const {
            return divide(n).Quotient;
        }

        bounded operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
        using words_type = encoding::words<r, byte>;
        
        words_type words() {
            return words_type{slice<byte>(*this)};
        }
        
        const words_type words() const {
            return words_type{slice<byte>(*const_cast<bounded*>(this))};
        }
        
    private:
        explicit bounded(const Z_bytes<r>& z) {
            if (z > Z_bytes<r> {max()} || z < Z_bytes<r> {min()}) throw std::out_of_range{"Z_bytes too big"};
            throw method::unimplemented{"bounded{Z_bytes}"};
        }
    };
    
    template <endian::order r, size_t x> bool inline is_positive(const uint<r, x> &n) {
        return !is_zero(n);
    }
    
    template <endian::order r, size_t x> bool inline is_positive(const sint<r, x> &n) {
        return !is_negative(n) && !is_zero(n);
    }
    
    template <endian::order r, size_t x> bool inline is_negative(const uint<r, x> &n) {
        return false;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator!=(const bounded<is_signed, r, size> &a, const bounded<is_signed, r, size> &b) {
        return !(a == b);
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator<=(const bounded<is_signed, r, size> &a, const bounded<is_signed, r, size> &b) {
        return !(a > b);
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator>=(const bounded<is_signed, r, size> &a, const bounded<is_signed, r, size> &b) {
        return !(a < b);
    }
    
    template <data::endian::order r, size_t size>
    inline std::ostream& operator<<(std::ostream& s, const sint<r, size>& n) {
        return s << Z_bytes<r>{n};
    }

    template <data::endian::order r, size_t size>
    inline std::ostream& operator<<(std::ostream& s, const uint<r, size>& n) {
        return s << N_bytes<r>{n};
    }

    template <endian::order o, size_t size>
    inline math::sign uint<o, size>::sign() const {
        return *this == 0 ? math::zero : math::positive;
    }
        
    template <endian::order o, size_t size>
    inline math::sign sint<o, size>::sign() const {
        if (*this == 0) return math::zero;
        return (words()[-1]) < 0x80 ? math::positive : math::negative;
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded(const uint64 x) : byte_array<size>{byte_array<size>::fill(0)} {
        endian::arithmetic<endian::big, false, 8> n{x};
        std::copy(n.begin(), n.end(), words().begin());
    }
    
    template <endian::order r, size_t size>
    sint<r, size>::bounded(const int64 x) : byte_array<size>{x < 0 ? byte_array<size>::fill(0xff) : byte_array<size>::fill(0x00)} {
        endian::arithmetic<endian::big, true, 8> n{x};
        std::copy(n.begin(), n.end(), words().begin());
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded(const byte_array<size>& b) : byte_array<size>{b} {}
        
    template <endian::order r, size_t size>
    uint<r, size>::bounded(slice<byte, size> x) {
        std::copy(x.begin(), x.end(), this->begin());
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded(string_view s) : bounded{} {
        
        ptr<N_bytes<endian::little>> dec = encoding::natural::read<endian::little>(s);
        if (dec != nullptr) {
            if (dec->size() <= size) {
                std::copy(dec->begin(), dec->end(), words().begin());
                return; 
            } else throw std::invalid_argument{"decimal number too big"};
        }
        
        ptr<bytes> hex = encoding::hex::read(s);
        if (hex != nullptr) {
            if (hex->size() == size) {
                std::copy(hex->begin(), hex->end(), this->begin());
                return;
            } else throw std::invalid_argument{"hex string has the wrong size."};
        }
            
        throw std::invalid_argument{"format is unrecognized."};
        
    }
    
    template <endian::order r, size_t size>
    sint<r, size>::bounded(string_view s) : bounded{} {
        ptr<Z_bytes<endian::little>> dec = encoding::integer::read<endian::little>(s);
        if (dec != nullptr) {
            if (dec->size() <= size) {
                std::copy(dec->begin(), dec->end(), words().begin());
                return;
            } else throw std::invalid_argument{"decimal number has too many digits"};
        }
        
        ptr<bytes> hex = encoding::hex::read(s);
        if (hex != nullptr) {
            if (hex->size() == size) {
                std::copy(hex->begin(), hex->end(), this->begin());
                return;
            } else throw std::invalid_argument{"hex string has the wrong size."};
        }
            
        throw std::invalid_argument{"format is unrecognized."};
        
    }
    
    template <endian::order r, size_t size>
    bool uint<r, size>::operator<(const bounded &b) const {
        return data::arithmetic::less(words().end(), words().begin(), b.words().begin());
    }
    
    template <endian::order r, size_t size>
    bool uint<r, size>::operator>(const bounded &b) const {
        return data::arithmetic::greater(words().end(), words().begin(), b.words().begin());
    }
    
    template <endian::order o, size_t size>
    bool sint<o, size>::operator<(const sint<o, size> &b) const {
        math::sign sa = sign();
        math::sign sb = b.sign();
        if (sa == sb) return data::arithmetic::less(words().rend(), words().rbegin(), b.words().rbegin());
        return sa < sb;
    }
    
    template <endian::order o, size_t size>
    bool sint<o, size>::operator>(const sint<o, size> &b) const {
        math::sign sa = sign();
        math::sign sb = b.sign();
        if (sa == sb) return data::arithmetic::greater(words().rend(), words().rbegin(), b.words().rbegin());
        return sa > sb;
    }
    
    template <endian::order r, size_t size>
    uint<r, size>& uint<r, size>::operator++() {
        data::arithmetic::plus<byte>(this->words().end(), this->words().begin(), 1, this->words().begin());
        return *this;
    }
        
    template <endian::order r, size_t size>
    uint<r, size>& uint<r, size>::operator--() {
        data::arithmetic::minus<byte>(this->words().end(), this->words().begin(), 1, this->words().begin());
        return *this;
    }
    
    template <endian::order r, size_t size>
    uint<r, size> uint<r, size>::operator++(int) {
        bounded n = *this;
        ++(*this);
        return n;
    }
    
    template <endian::order r, size_t size>
    uint<r, size> uint<r, size>::operator--(int) {
        bounded n = *this;
        ++(*this);
        return n;
    }
    
    template <endian::order r, size_t size>
    sint<r, size>& sint<r, size>::operator++() {
        data::arithmetic::plus<byte>(this->words().rend(), this->words().rbegin(), 1, this->words().rbegin());
        return *this;
    }
    
    template <endian::order r, size_t size>
    bounded<true, r, size>& sint<r, size>::operator--() {
        data::arithmetic::minus<byte>(this->words().rend(), this->words().rbegin(), 1, this->words().rbegin());
        return *this;
    }
    
    template <endian::order r, size_t size>  
    bounded<true, r, size> sint<r, size>::operator++(int) {
        bounded z = *this;
        ++(*this);
        return z;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> sint<r, size>::operator--(int) {
        bounded z = *this;
        --(*this);
        return z;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline operator<<(const bounded<is_signed, r, size> &z, int i) {
        auto n = z;
        return n <<= i;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline operator>>(const bounded<is_signed, r, size> &z, int i) {
        auto n = z;
        return n >>= i;
    }
    
    namespace {
        template <endian::order r, size_t size>
        void shift_right(byte_array<size> &n, uint32 i, byte fill) {
            if (r == endian::big) data::arithmetic::bit_shift_right(n.rbegin(), n.rend(), i, fill);
            else data::arithmetic::bit_shift_right(n.begin(), n.end(), i, fill);
        }
        
        template <endian::order r, size_t size>
        void shift_left(byte_array<size> &n, uint32 i, byte fill) {
            if (r == endian::big) data::arithmetic::bit_shift_left(n.begin(), n.end(), i, fill);
            else data::arithmetic::bit_shift_left(n.rbegin(), n.rend(), i, fill);
        }
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline &operator<<=(bounded<is_signed, r, size> &n, int i) {
        (i < 0 ? shift_right<r, size> : shift_left<r, size>)(n, i, is_negative(n));
        return n; 
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline &operator>>=(bounded<is_signed, r, size> &n, int i) {
        (i < 0 ? shift_left<r, size> : shift_right<r, size>)(n, i, is_negative(n));
        return n; 
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator-(const sint<r, size>& a) {
        return ++(~a);
    }
    
    template <endian::order o, size_t size>
    uint<o, size> uint<o, size>::min() {
        bounded b{};
        for (int i = 0; i <= size; i++) b[i] = 0x00;
        return b;
    }
    
    template <endian::order o, size_t size>
    uint<o, size> uint<o, size>::max() {
        bounded b{};
        for (int i = 0; i <= size; i++) b[i] = 0xff;
        return b;
    }

    template <endian::order o, size_t size>
    sint<o, size> sint<o, size>::min() {
        bounded b{};
        b.words()[-1] = 0x80;
        return b;
    }
    
    template <endian::order o, size_t size>
    sint<o, size> sint<o, size>::max() {
        bounded b{-1};
        b.words()[-1] = 0x7f;
        return b;
    }
    
    template <endian::order o, size_t size>
    bounded<true, o, size> inline &bounded<true, o, size>::operator+=(const bounded<true, o, size>& n) {
        data::arithmetic::plus<byte>(
            this->words().end(), 
            this->words().begin(), 
            const_cast<const bounded*>(this)->words().begin(), 
            n.words().begin());
        return *this;
    }
    
    template <endian::order o, size_t size>
    bounded<false, o, size> inline &bounded<false, o, size>::operator+=(const bounded<false, o, size>& n) {
        data::arithmetic::plus<byte>(
            this->words().end(),
            this->words().begin(), 
            const_cast<const bounded*>(this)->words().begin(),
            n.words().begin());
        return *this;
    }

    template <endian::order o, size_t size>
    bounded<true, o, size> inline &bounded<true, o, size>::operator-=(const bounded<true, o, size>& n) {
        data::arithmetic::minus<byte>(
            this->words().end(), 
            this->words().begin(), 
            const_cast<const bounded*>(this)->words().begin(), 
            n.words().begin());
        return *this;
    }
    
    template <endian::order o, size_t size>
    bounded<false, o, size> inline &bounded<false, o, size>::operator-=(const bounded<false, o, size>& n) {
        data::arithmetic::minus<byte>(
            this->words().end(), 
            this->words().begin(), 
            const_cast<const bounded*>(this)->words().begin(), 
            n.words().begin());
        return *this;
    }

    template <endian::order o, size_t size>
    bounded<true, o, size> inline &bounded<true, o, size>::operator+=(int64 x) {
        return *this += bounded{x};
    }
    
    template <endian::order o, size_t size>
    bounded<false, o, size> inline &bounded<false, o, size>::operator+=(uint64 x) {
        return *this += bounded{x};
    }

    template <endian::order o, size_t size>
    bounded<true, o, size> inline &bounded<true, o, size>::operator-=(int64 x) {
        return *this -= bounded{x};
    }
    
    template <endian::order o, size_t size>
    bounded<false, o, size> inline &bounded<false, o, size>::operator-=(uint64 x) {
        return *this -= bounded{x};
    }
    
    template <endian::order r, size_t size>
    bounded<false, r, size> operator+(const bounded<false, r, size> &a, const bounded<false, r, size> &b) {
        bounded<false, r, size> n{};
        data::arithmetic::plus<byte>(n.words().end(), n.words().begin(), a.words().begin(), b.words().begin());
        return n;
    }
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator+(const bounded<true, r, size> &a, const bounded<true, r, size> &b) {
        bounded<true, r, size> n{};
        data::arithmetic::plus<byte>(n.words().end(), n.words().begin(), a.words().begin(), b.words().begin());
        return n;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator~(const bounded<is_signed, r, size> &n) {
        bounded<is_signed, r, size> x;
        data::arithmetic::bit_negate<byte>(x.end(), x.begin(), n.begin());
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator==(const bounded<is_signed, r, size> &a, const bounded<is_signed, r, size> &b) {
        return data::arithmetic::equal(a.end(), a.begin(), b.begin());
    }
    
    /*
    template <size_t size, endian::order o>
    N_bytes<o> bounded<size, o, false>::modulus() {
        std::string one = std::string{"0x01"} + encoding::hexidecimal::write(bounded{0}).substr(2);
        return N_bytes<o> {one};
    }*/
    
    template <endian::order r, size_t size>
    uint<r, size> inline operator+(const uint<r, size>& a, uint64 b) {
        return a + bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator+(const sint<r, size>& a, int64 b) {
        return a + bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    uint<r, size> inline operator-(const uint<r, size>& a, uint64 b) {
        return a - bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator-(const sint<r, size>& a, int64 b) {
        return a - sint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator==(const uint<r, size> &a, const uint64 &b) {
        return a == uint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator!=(const uint<r, size> &a, const uint64 &b) {
        return a != uint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator==(const sint<r, size> &a, const int64 &b) {
        return a == sint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator!=(const sint<r, size> &a, const int64 &b) {
        return a == sint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator>=(const uint<r, size> &a, const uint64 &b) {
        return a >= uint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator<=(const bounded<false, r, size> &a, const uint64 &b) {
        return a <= bounded<false, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator>=(const sint<r, size> &a, const int64 &b) {
        return a >= sint<r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator<=(const sint<r, size> &a, const int64 &b) {
        return a >= sint<r, size>{b};
    }
    
    template <bool u, endian::order r, size_t x> bool inline is_zero(const bounded<u, r, x> &z) {
        return arithmetic::ones_is_zero(z.words());
    }
    
    template <endian::order r, size_t x> bool inline is_negative(const sint<r, x> &z) {
        return arithmetic::sign_bit_set(z.words());
    }

}

namespace data {

    namespace encoding::hexidecimal {

        template <bool is_signed, endian::order r, size_t size>
        std::string write(const math::number::bounded<is_signed, r, size>& n) {
            std::stringstream ss;
            ss << std::hex << n;
            return ss.str();
        }

    }

    namespace encoding::decimal {

        template <bool is_signed, endian::order r, size_t size>
        std::string write(const math::number::bounded<is_signed, r, size>& n) {
            std::stringstream ss;
            ss << std::dec << n;
            return ss.str();
        }

    }
    
    namespace math {
    
        template <size_t size, endian::order o>
        struct abs<uint<o, size>> {
            uint<o, size> operator()(const uint<o, size>& i) {
                return i;
            }
        };
    
        template <size_t size, endian::order o>
        struct abs<sint<o, size>> {
            uint<o, size> operator()(const sint<o, size>& i) {
                uint<o, size> u;
                sint<o, size> x = number::is_negative(i) ? -i : i;
                std::copy(x.begin(), x.end(), u.begin());
                return u;
            }
        };
        
    }

}

#endif
