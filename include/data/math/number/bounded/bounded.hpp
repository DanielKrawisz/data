// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <data/cross.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/encoding/halves.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/encoding/endian/words.hpp>

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
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator-(const bounded<true, r, size>&);
    
    template <endian::order r, size_t size>
    bounded<false, r, size> operator+(const bounded<false, r, size>&, const bounded<false, r, size>&);
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator+(const bounded<true, r, size>&, const bounded<true, r, size>&);
    
    template <endian::order r, size_t size>
    bounded<false, r, size> operator-(const bounded<false, r, size>&, const bounded<false, r, size>&);
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator-(const bounded<true, r, size>&, const bounded<true, r, size>&);
    
    template <endian::order r, size_t size>
    bounded<false, r, size> operator*(const bounded<false, r, size>&, const bounded<false, r, size>&);
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator*(const bounded<true, r, size>&, const bounded<true, r, size>&);

    template <data::endian::order r, size_t size>
    std::ostream& operator<<(std::ostream& s, const data::math::number::bounded<true, r, size>& n);

    template <data::endian::order r, size_t size>
    std::ostream& operator<<(std::ostream& s, const data::math::number::bounded<false, r, size>& n);
    
    template <endian::order r, size_t size>
    bool operator==(const bounded<false, r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator!=(const bounded<false, r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator==(const bounded<true, r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bool operator!=(const bounded<true, r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bool operator>=(const bounded<false, r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator<=(const bounded<false, r, size>&, const uint64&);
    
    template <endian::order r, size_t size>
    bool operator>=(const bounded<true, r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bool operator<=(const bounded<true, r, size>&, const int64&);
    
    template <endian::order r, size_t size>
    bounded<false, r, size> operator+(const bounded<false, r, size>&, uint64);
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator+(const bounded<true, r, size>&, int64);
    
    template <endian::order r, size_t size>
    bounded<false, r, size> operator-(const bounded<false, r, size>&, uint64);
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator-(const bounded<true, r, size>&, int64);
    
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
        
        math::sign sign() const;
        
        bool operator>(const bounded&) const;
        bool operator<(const bounded&) const;
        
        bounded& operator^=(const bounded&);
        
        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
        bounded& operator*=(const bounded&);
        
        bounded& operator+=(const uint32&);
        bounded& operator-=(const uint32&);
        bounded& operator*=(const uint32&);
        
        bounded& operator++();
        bounded& operator--();
        
        bounded operator++(int);
        bounded operator--(int);
        
        bounded& operator<<=(int32);
        bounded& operator>>=(int32);
        
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
        
        data::arithmetic::digits<r> digits() {
            return data::arithmetic::digits<r>{slice<byte>(*this)};
        }
        
        const data::arithmetic::digits<r> digits() const {
            return data::arithmetic::digits<r>{slice<byte>(*const_cast<bounded*>(this))};
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
        
        bounded(const bounded<true, r, size>&) {
            throw method::unimplemented{"bounded<size, o, false>{bounded<size, o, true>}"};
        }
        
        friend struct abs<bounded<false, r, size>, bounded<true, r, size>>;
        
        // TODO use words type eventually. 
        using words_type = data::arithmetic::digits<r>;
        
        words_type words() {
            return digits();
        }
        
        const words_type words() const {
            return digits();
        }
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

        bounded& operator+=(const int32&);
        bounded& operator-=(const int32&);
        bounded& operator*=(const int32&);
        
        static bounded max();
        static bounded min();

        static N_bytes<r> modulus() {
            return bounded<false, r, size>::modulus();
        }

        bounded& operator++();
        bounded& operator--();
        
        bounded operator++(int);
        bounded operator--(int);

        bounded& operator<<=(int32);
        bounded& operator>>=(int32);

        math::division<bounded> divide(const bounded&) const;
        
        bounded operator/(const bounded& n) const {
            return divide(n).Quotient;
        }

        bounded operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
        data::arithmetic::digits<r> digits() {
            return data::arithmetic::digits<r>{slice<byte>(*this)};
        }
        
        const data::arithmetic::digits<r> digits() const {
            return data::arithmetic::digits<r>{slice<byte>(*const_cast<bounded*>(this))};
        }
        
    private:
        explicit bounded(const Z_bytes<r>& z) {
            if (z > Z_bytes<r> {max()} || z < Z_bytes<r> {min()}) throw std::out_of_range{"Z_bytes too big"};
            throw method::unimplemented{"bounded{Z_bytes}"};
        }
        
        // TODO use words type eventually. 
        using words_type = data::arithmetic::digits<r>;
        
        words_type words() {
            return digits();
        }
        
        const words_type words() const {
            return digits();
        }
    };
    
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
    inline std::ostream& operator<<(std::ostream& s, const bounded<true, r, size>& n) {
        return s << Z_bytes<r>{n};
    }

    template <data::endian::order r, size_t size>
    inline std::ostream& operator<<(std::ostream& s, const bounded<false, r, size>& n) {
        return s << N_bytes<r>{n};
    }

    template <endian::order o, size_t size>
    inline math::sign bounded<false, o, size>::sign() const {
        return *this == 0 ? math::zero : math::positive;
    }
        
    template <endian::order o, size_t size>
    inline math::sign bounded<true, o, size>::sign() const {
        if (*this == 0) return math::zero;
        return (*digits().rend()) < 0x80 ? math::positive : math::negative;
    }
    
    template <size_t size, endian::order o>
    struct abs<bounded<size, o, false>, bounded<size, o, false>> {
        bounded<size, o, false> operator()(const bounded<size, o, false>& i) {
            return i;
        }
    };
        
    template <size_t size, endian::order o, bool is_signed>
    struct sign<bounded<size, o, is_signed>> {
        math::sign operator()(const bounded<size, o, is_signed>& i) {
            return i.sign();
        }
    };

    template <size_t size, endian::order o, bool is_signed>
    struct arg<bounded<size, o, is_signed>> {
        bounded<size, o, false> operator()(const bounded<size, o, is_signed>& i) {
            if (i == 0) throw division_by_zero{};
            return i.sign() == math::negative ? -1 : 1;
        }
    };
    
    template <endian::order r, size_t size>
    bounded<false, r, size>::bounded(const uint64 x) : byte_array<size>{byte_array<size>::fill(0)} {
        endian::arithmetic<endian::big, false, 8> n{x};
        std::copy(n.begin(), n.end(), digits().begin());
    }
    
    template <endian::order r, size_t size>
    bounded<true, r, size>::bounded(const int64 x) : byte_array<size>{x < 0 ? byte_array<size>::fill(0xff) : byte_array<size>::fill(0x00)} {
        endian::arithmetic<endian::big, true, 8> n{x};
        std::copy(n.begin(), n.end(), digits().begin());
    }
        
    template <endian::order r, size_t size>
    bounded<false, r, size>::bounded(const byte_array<size>& b) : byte_array<size>{b} {}
        
    template <endian::order r, size_t size>
    bounded<false, r, size>::bounded(slice<byte, size> x) {
        std::copy(x.begin(), x.end(), this->begin());
    }
    
    template <endian::order r, size_t size>
    bounded<false, r, size>::bounded(string_view s) : bounded{} {
        
        ptr<N_bytes_little> dec = encoding::natural::read<endian::little>(s);
        if (dec != nullptr) {
            if (dec->size() <= size) std::copy(dec->begin(), dec->end(), digits().begin());
            else throw std::invalid_argument{"decimal number too big"};
        }
        
        ptr<bytes> hex = encoding::hex::read(s);
        if (hex != nullptr) {
            if (hex->size() == size) std::copy(hex->begin(), hex->end(), this->begin());
            else throw std::invalid_argument{"hex string has the wrong size."};
        }
            
        throw std::invalid_argument{"format is unrecognized."};
        
    }
    
    template <endian::order r, size_t size>
    bounded<true, r, size>::bounded(string_view s) : bounded{} {
        ptr<Z_bytes_little> dec = encoding::integer::read<endian::little>(s);
        if (dec != nullptr) {
            if (dec->size() <= size) {
                std::copy(dec->begin(), dec->end(), digits().begin());
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
    bool bounded<false, r, size>::operator<(const bounded &b) const {
        return arithmetic::less(words().rend(), words().rbegin(), b.words().rbegin());
    }
    
    template <endian::order r, size_t size>
    bool bounded<false, r, size>::operator>(const bounded &b) const {
        return arithmetic::greater(words().rend(), words().rbegin(), b.words().rbegin());
    }
    
    template <endian::order o, size_t size>
    bool bounded<true, o, size>::operator<(const bounded<true, o, size> &b) const {
        math::sign sa = sign();
        math::sign sb = b.sign();
        if (sa == sb) return arithmetic::less(words().rend(), words().rbegin(), b.words().rbegin());
        return sa < sb;
    }
    
    template <endian::order o, size_t size>
    bool bounded<true, o, size>::operator>(const bounded<true, o, size> &b) const {
        math::sign sa = sign();
        math::sign sb = b.sign();
        if (sa == sb) return arithmetic::greater(words().rend(), words().rbegin(), b.words().rbegin());
        return sa > sb;
    }
    
    template <endian::order r, size_t size>
    bounded<false, r, size>& bounded<false, r, size>::operator++() {
        operator+=(1);
        return *this;
    }
        
    template <endian::order r, size_t size>
    bounded<false, r, size>& bounded<false, r, size>::operator--() {
        operator-=(1);
        return *this;
    }
        
    template <endian::order r, size_t size>
    bounded<false, r, size> bounded<false, r, size>::operator++(int) {
        bounded n = *this;
        ++(*this);
        return n;
    }
        
    template <endian::order r, size_t size>
    bounded<false, r, size> bounded<false, r, size>::operator--(int) {
        bounded n = *this;
        ++(*this);
        return n;
    }

    template <endian::order r, size_t size>
    bounded<true, r, size>& bounded<true, r, size>::operator++() {
        operator+=(1);
        return *this;
    }

    template <endian::order r, size_t size>
    bounded<true, r, size>& bounded<true, r, size>::operator--() {
        operator-=(1);
        return *this;
    }
      
    template <endian::order r, size_t size>  
    bounded<true, r, size> bounded<true, r, size>::operator++(int) {
        bounded z = *this;
        ++(*this);
        return z;
    }

    template <endian::order r, size_t size>
    bounded<true, r, size> bounded<true, r, size>::operator--(int) {
        bounded z = *this;
        --(*this);
        return z;
    }
    
/*
    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator<<=(int32 bits) {
        return operator=(array::operator<<(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator>>=(int32 bits) {
        return operator=(array::operator>>(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator<<=(int32 bits) {
        return operator=(array::operator<<(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator>>=(int32 bits) {
        return operator=(array::operator>>(bits));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator+=(const bounded<size, o, false>& n) {
        methods::plus(size, array::words(), n.words(), array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator+=(const bounded<size, o, true>& n) {
        methods::plus(size, array::words(), n.words(), array::words());
        return *this;
    }
    
    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator-=(const bounded<size, o, false>& n) {
        methods::plus(size, array::words(), n.words(), array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator-=(const bounded<size, o, true>& n) {
        methods::plus(array::words_type::Last, array::words(), n.words(), array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator*=(const bounded<size, o, false>& n) {
        return operator=(operator*(n));
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator+=(const bit32& n) {
        return methods::plus(size, array::words(), n, array::words());
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator+=(const bit32& n) {
        return methods::plus(size, array::words(), n, array::words());
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator-=(const bit32& n) {
        methods::minus(size, array::words(), n, array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, true>&
    bounded<size, o, true>::operator-=(const bit32& n) {
        words_type out = array::words();
        methods::minus(size, array::words(), n, out);
        return *this;
    }

    template <size_t size, endian::order o>
    inline bounded<size, o, false>&
    bounded<size, o, false>::operator*=(const bit32& n) {
        methods::times(size, array::words(), n, array::words());
        return *this;
    }

    template <size_t size, endian::order o>
    inline math::division<bounded<size, o, false>>
    bounded<size, o, false>::divide(const bounded& n) const {
        return math::number::natural::divide(*this, n);
    }

    template <size_t size, endian::order o>
    inline math::division<bounded<size, o, true>>
    bounded<size, o, true>::divide(const bounded& n) const {
        return math::number::integer::divide(*this, n);
    }
*/

    template <endian::order r, size_t size>
    inline bounded<true, r, size> operator-(const bounded<true, r, size>& a) {
        return ~a + 1;
    }

    template <endian::order o, size_t size>
    bounded<false, o, size> bounded<false, o, size>::min() {
        return 0;
    }

    template <endian::order o, size_t size>
    bounded<false, o, size> bounded<false, o, size>::max() {
        bounded b{};
        for (int i = 0; i <= size; i++) b[i] = 0xff;
        return b;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> operator~(const bounded<is_signed, r, size> &n) {
        bounded<is_signed, r, size> x;
        arithmetic::bit_negate(x.end(), x.begin(), n.begin());
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bool operator==(const bounded<is_signed, r, size> &a, const bounded<is_signed, r, size> &b) {
        return arithmetic::equal(a.end(), a.begin(), b.begin());
    }
    
    /*
    template <size_t size, endian::order o>
    N_bytes<o> bounded<size, o, false>::modulus() {
        std::string one = std::string{"0x01"} + encoding::hexidecimal::write(bounded{0}).substr(2);
        return N_bytes<o> {one};
    }*/

    template <endian::order o, size_t size>
    bounded<true, o, size> bounded<true, o, size>::min() {
        bounded b{};
        b.digits()[size - 1] = 0x80;
        return b;
    }

    template <endian::order o, size_t size>
    bounded<true, o, size> bounded<true, o, size>::max() {
        bounded b{-1};
        b.digits()[size - 1] = 0x7f;
        return b;
    }
    
    template <endian::order r, size_t size>
    bounded<false, r, size> inline operator+(const bounded<false, r, size>& a, uint64 b) {
        return a + bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bounded<true, r, size> inline operator+(const bounded<true, r, size>& a, int64 b) {
        return a + bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bounded<false, r, size> inline operator-(const bounded<false, r, size>& a, uint64 b) {
        return a - bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bounded<true, r, size> inline operator-(const bounded<true, r, size>& a, int64 b) {
        return a - bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator==(const bounded<false, r, size> &a, const uint64 &b) {
        return a == bounded<false, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator!=(const bounded<false, r, size> &a, const uint64 &b) {
        return a != bounded<false, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator==(const bounded<true, r, size> &a, const int64 &b) {
        return a == bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator!=(const bounded<true, r, size> &a, const int64 &b) {
        return a == bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator>=(const bounded<false, r, size> &a, const uint64 &b) {
        return a >= bounded<false, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator<=(const bounded<false, r, size> &a, const uint64 &b) {
        return a <= bounded<false, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator>=(const bounded<true, r, size> &a, const int64 &b) {
        return a >= bounded<true, r, size>{b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator<=(const bounded<true, r, size> &a, const int64 &b) {
        return a >= bounded<true, r, size>{b};
    }

}

namespace data {
    
    template <size_t size> using uint = math::number::bounded<false, endian::order::little, size>;
    template <size_t size> using integer = math::number::bounded<true, endian::order::little, size>;

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

}

#endif
