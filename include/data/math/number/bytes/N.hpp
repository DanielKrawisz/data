// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <data/math/number/natural.hpp>
#include <data/encoding/endian.hpp>
#include <data/encoding/integer.hpp>
#include <data/math/number/abs.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/encoding/words.hpp>
#include <limits>

namespace data::math::number {
    
    template <endian::order r> struct Z_bytes;
    
    template <endian::order r>
    struct N_bytes {
        bytes Value;
        
        N_bytes() : Value{} {}
        
        N_bytes(uint64 x) {
            Value.resize(8);
            *(uint64*)(Value.data()) = endian::native<uint64, r>{}.from(x);
        }
        
        explicit N_bytes(const N& n) : Value{n.write(r)} {}
        
        static N_bytes read(string_view x) {
            return N_bytes<r>{encoding::natural::read(x, r)};
        }
        
        explicit N_bytes(string_view s) : N_bytes{read(s)} {}
        
        N_bytes(bytes_view b) : Value{b} {}
        
        operator bytes_view() const {
            return bytes_view{Value};
        }
        
        math::sign sign() const {
            return operator==(0) ? math::zero : math::positive;
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
        
        explicit N_bytes(const N_bytes<opposite>&);
        
        N_bytes<opposite> reverse() const;
        
        N_bytes& operator=(const N_bytes& n) {
            Value = n.Value;
            return *this;
        }
        
        N_bytes& operator=(const N_bytes<opposite>& n) {
            return operator=(n.reverse());
        }
        
        N_bytes& operator=(const N& n) {
            return operator=(N_bytes{n});
        }
        
        bool operator==(uint64 n) const {
            return operator==(N_bytes{n});
        }
        
        bool operator==(const N_bytes& n) const {
            return N{*this} == N{n}; // Inefficient
        }
        
        bool operator==(const Z_bytes<r>& z) const {
            return z < 0 ? false : operator==(z.abs());
        }
        
        bool operator==(const N_bytes<opposite>& n) const {
            return operator==(n.reverse());
        }
        
        bool operator==(const Z_bytes<opposite>& z) const {
            return z < 0 ? false : operator==(z.abs());
        }
        
        bool operator==(const N& n) const {
            return N{*this} == n;
        }
        
        bool operator==(const Z& z) const {
            return z < 0 ? false : operator==(z.abs());
        }
        
        bool operator!=(uint64 n) const {
            return !operator==(n);
        }
        
        bool operator!=(const N_bytes& n) const {
            return !operator==(n);
        }
        
        bool operator!=(const Z_bytes<r>& z) const {
            return !operator==(z);
        };
        
        bool operator!=(const N_bytes<opposite>& n) const {
            return !operator==(n);
        }
        
        bool operator!=(const Z_bytes<opposite>& z) const {
            return !operator==(z);
        }
        
    private:
        using word = endian::ordered<uint32, r>;
        using words_type = arithmetic::unfixed_words<uint32, r>;
        using methods = arithmetic::unoriented<words_type, word>;
            
        words_type words() {
            return words_type{slice<byte>{Value}};
        }
        
        const words_type words() const {
            return words_type::make(slice<byte>::make(Value));
        }
        
    public:
        bool operator<(uint64 n) const {
            return operator<(N_bytes<r>{n});
        }
        
        bool operator<(const N_bytes& n) const {
            return methods::less(std::max(Value.size(), n.Value.size()), words(), n.words());
        }
        
        bool operator<(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<(N_bytes{z.Value});
        }
        
        bool operator<(const N_bytes<opposite>& n) const {
            return operator<(n.reverse());
        }
        
        bool operator<(const Z_bytes<opposite>& n) const {
            return operator<(n.reverse());
        }
        
        bool operator<(const N& n) const {
            return N{*this} < n;
        }
        
        bool operator<(const Z& n) const {
            return N{*this} < n;
        }
        
        bool operator>(uint64 n) const {
            return !operator<=(n);
        }
        
        bool operator>(const N_bytes& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z_bytes<r>& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const N_bytes<opposite>& n) const {
            return operator>(n.reverse());
        }
        
        bool operator>(const Z_bytes<opposite>& n) const {
            return operator>(n.reverse());
        }
        
        bool operator>(const N& n) const {
            return !operator<=(n);
        }
        
        bool operator>(const Z& n) const {
            return !operator<=(n);
        }
        
        bool operator<=(uint64 n) const {
            return operator<=(N_bytes{n});
        }
        
        bool operator<=(const N_bytes& n) const {
            return methods::less_equal(std::max(Value.size(), n.Value.size()), words(), n.words());
        }
        
        bool operator<=(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<=(N_bytes{z.Value});
        }
        
        bool operator<=(const N_bytes<opposite>& n) const {
            return operator<=(n.reverse());
        }
        
        bool operator<=(const Z_bytes<opposite>& n) const {
            return operator<=(n.reverse());
        }
        
        bool operator<=(const N& n) const {
            return N{*this} <= n;
        }
        
        bool operator<=(const Z& n) const {
            return N{*this} <= n;
        }
        
        bool operator>=(uint64 n) const {
            return operator>=(N_bytes{n});
        }
        
        bool operator>=(const N_bytes& n) const;
        
        bool operator>=(const Z_bytes<r>& z) const;
        
        bool operator>=(const N_bytes<opposite>& n) const {
            return operator>=(n.reverse());
        }
        
        bool operator>=(const Z_bytes<opposite>& n) const {
            return operator>=(n.reverse());
        }
        
        bool operator>=(const N& n) const {
            return N{*this} >= n;
        }
        
        bool operator>=(const Z& n) const {
            return N{*this} >= n;
        }
        
        N_bytes& operator++() {
            operator+=(1);
            return *this;
        }
        
        N_bytes& operator--() {
            operator+=(1);
            return *this;
        }
        
        N_bytes operator++(int) {
            N_bytes z = *this;
            ++(*this);
            return z;
        }
        
        N_bytes operator--(int) {
            N_bytes z = *this;
            ++(*this);
            return z;
        }
        
        N_bytes operator+(uint64 n) const;
        
        N_bytes operator+(const N_bytes& n) const;
        
        N_bytes operator+(const Z_bytes<r>&) const;
        
        N_bytes operator+(const N_bytes<opposite>& n) const {
            return operator+(n.reverse());
        }
        
        N_bytes operator+(const Z_bytes<opposite>& n) const {
            return operator+(n.reverse());
        }
        
        N_bytes operator+(const N&) const;
        
        N_bytes operator+(const Z&) const;
        
        N_bytes& operator+=(uint64 n) {
            return operator=(operator+(n));
        } 
        
        N_bytes& operator+=(const N_bytes& n) {
            return operator=(operator+(n));
        }
        
        N_bytes& operator+=(const Z_bytes<r>& n) {
            return operator=(operator+(n));
        }
        
        N_bytes& operator+=(const N_bytes<opposite>& n) {
            return operator=(operator+(n));
        }
        
        N_bytes& operator+=(const Z_bytes<opposite>& n) {
            return operator=(operator+(n));
        }
        
        N_bytes& operator+=(const N& n) {
            return operator=(operator+(n));
        }
        
        N_bytes& operator+=(const Z& n) {
            return operator=(operator+(n));
        }
        
        N_bytes operator-(uint64 n) const {
            return operator-(N_bytes{n});
        }
        
        N_bytes operator-(const N_bytes& n) const;
        
        N_bytes operator-(const Z_bytes<r>& n) const {
            if (operator<(n)) return 0;
            throw method::unimplemented{"N_bytes::operator-"};
        }
        
        N_bytes operator-(const N_bytes<opposite>& n) const {
            return operator-(n.reverse());
        }
        
        N_bytes operator-(const Z_bytes<opposite>& n) const {
            return operator-(n.reverse());
        }
        
        N_bytes operator-(const N& n) const {
            return operator-(N_bytes{n});
        }
        
        N_bytes operator-(const Z& n) const {
            if (operator<(n)) return 0;
            throw method::unimplemented{"N_bytes::operator-"};
        }
        
        N_bytes& operator-=(uint64 n) {
            return operator=(operator-(n));
        }
        
        N_bytes& operator-=(const N_bytes& n) {
            return operator=(operator-(n));
        }
        
        N_bytes& operator-=(const Z_bytes<r>& n) {
            return operator=(operator-(n));
        }
        
        N_bytes& operator-=(const N_bytes<opposite>& n) {
            return operator=(operator-(n));
        }
        
        N_bytes& operator-=(const Z_bytes<opposite>& n) {
            return operator=(operator-(n));
        }
        
        N_bytes& operator-=(const N& n) {
            return operator=(operator-(n));
        }
        
        N_bytes& operator-=(const Z& n) {
            return operator=(operator-(n));
        }
        
        N_bytes operator*(uint64 n) const;
        
        N_bytes operator*(const N_bytes&) const;
        
        N_bytes operator*(const N_bytes<opposite>& n) const {
            return operator*(n.reverse());
        }
        
        N_bytes operator*(const Z_bytes<r>&) const;
        
        N_bytes operator*(const Z_bytes<opposite>& n) const {
            return operator*(n.reverse());
        }
        
        N_bytes operator*(const N&) const;
        
        N_bytes operator*(const Z&) const;
        
        N_bytes& operator*=(uint64 n) {
            return operator=(operator*(n));
        }
        
        N_bytes& operator*=(const N_bytes& n) {
            return operator=(operator*(n));
        }
        
        N_bytes& operator*=(const N_bytes<opposite>& n) {
            return operator=(operator*(n));
        }
        
        N_bytes& operator*=(const Z_bytes<r>& n) const {
            return operator=(operator*(n));
        }
        
        N_bytes& operator*=(const Z_bytes<opposite>& n) const {
            return operator=(operator*(n));
        }
        
        N_bytes& operator*=(const N& n) {
            return operator=(operator*(n));
        }
        
        N_bytes& operator*=(const Z& n) const {
            return operator=(operator*(n));
        }
        
        N_bytes operator^(uint32 n) const;
        
        N_bytes& operator^=(uint32 n) {
            return operator=(operator^(n));
        }
        
        math::division<N_bytes> divide(const N_bytes&) const;
        
        bool operator|(const N_bytes& n) const {
            return divide(n).Remainder == 0;
        }
        
        N_bytes operator/(const N_bytes& n) const {
            return divide(n).Quotient;
        }
        
        N_bytes operator%(const N_bytes& n) const {
            return divide(n).Remainder;
        }
        
        N_bytes& operator/=(const N_bytes& n) {
            return operator=(operator/(n));
        }
        
        N_bytes& operator%=(const N_bytes& n) {
            return operator=(operator%(n));
        }
        
        N_bytes operator/(const N_bytes<opposite>& n) const {
            return operator/(n.reverse());
        }
        
        N_bytes operator%(const N_bytes<opposite>& n) const {
            return operator/(n.reverse());
        }
        
        N_bytes& operator/=(const N_bytes<opposite>& n) {
            return operator=(operator/(n));
        }
        
        N_bytes& operator%=(const N_bytes<opposite>& n) {
            return operator=(operator%(n));
        }
        
        N_bytes operator/(const N&) const;
        
        N_bytes operator%(const N&) const;
        
        N_bytes& operator/=(const N& n) {
            return operator=(operator/(n));
        }
        
        N_bytes& operator%=(const N& n) {
            return operator=(operator%(n));
        }
        
        N_bytes operator<<(int64 x) const {
            throw method::unimplemented{"N_bytes::<<"};
        }
        
        N_bytes operator>>(int64 x) const {
            throw method::unimplemented{"N_bytes::>>"};
        }
        
        N_bytes& operator<<=(int64 x) {
            return operator=(operator<<(x));
        }
        
        N_bytes& operator>>=(int64 x) {
            return operator=(operator>>(x));
        }
        
        bytes write(endian::order) const; 
        
        N_bytes trim() const;
        
        template <typename indexed, size_t size, endian::order o> 
        explicit N_bytes(const bounded<indexed, size, o, false>& b) : N_bytes{bytes_view{b.Array.data(), size}, o} {}
        
        constexpr static math::number::natural::interface<N_bytes> is_natural{};

    private:
        N_bytes(bytes_view b, endian::order o) {
            Value.resize(b.size());
            std::copy(b.begin(), b.end(), Value.begin());
            if (o != r) std::reverse(Value.begin(), Value.end());
        }
    };
    
    template <endian::order o> 
    struct abs<N_bytes<o>, Z_bytes<o>> {
        N_bytes<o> operator()(const Z_bytes<o>& i);
    };
    
    namespace low {
        template <typename B, typename E, typename O>
        void trim(uint32 size, B b, E e, O o) {
            while(*b == 0) {
                size--;
                b++;
            }
            std::copy(b, e, o);
        }
    }
    
    template <endian::order r> 
    N_bytes<r> N_bytes<r>::trim() const {
        uint32 size = Value.size();
        if (size == 0) return N_bytes{bytes{}};
        N_bytes re{};
        if (r == endian::big) {
            auto b = Value.begin();
            while (*b == 0) {
                size--;
                b++;
            }
            re.Value.resize(size);
            std::copy(b, Value.end(), re.Value.begin());
        } else {
            auto b = Value.rbegin();
            while (*b == 0) {
                size--;
                b++;
            }
            re.Value.resize(size);
            std::copy(b, Value.rend(), re.Value.rend());
        }
        return re;
    }
}

namespace data::encoding::hexidecimal { 
    
    template <endian::order o>
    string write(const math::number::N_bytes<o>& n) {
        throw method::unimplemented{"hexidecimal::write(N_bytes)"};
    }
    
}

namespace data::encoding::decimal {
    
    template <endian::order o>
    string write(const math::number::N_bytes<o>& n) {
        throw method::unimplemented{"decimal::write(N_bytes)"};
    }
    
}

template <data::endian::order r>
std::ostream& operator<<(std::ostream& o, const data::math::number::N_bytes<r>& n) {
    if (o.flags() & std::ios::hex) o << data::encoding::decimal::write(n);
    if (o.flags() & std::ios::dec) o << data::encoding::hexidecimal::write(n);
    return o;
}

#endif

