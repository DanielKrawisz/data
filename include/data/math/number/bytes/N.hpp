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
#include <iostream>

namespace data::math::number {
    
    template <endian::order r> struct Z_bytes;
    
    template <endian::order r>
    struct N_bytes : bytes {
        
        N_bytes() : bytes{} {}
        
        N_bytes(uint64 x) : bytes{8} {
            *(uint64*)(bytes::data()) = endian::native<uint64, r>{}.from(x);
        }
        
        explicit N_bytes(const N& n) : bytes{} {
            n.write_bytes(static_cast<bytes&>(*this), r);
        }
        
        static N_bytes read(string_view x) {
            return x == "" ? 0 : N_bytes<r>{encoding::natural::read(x, r)};
        }
        
        explicit N_bytes(string_view s) : N_bytes{read(s)} {}
        
        N_bytes(bytes_view b) : bytes{b} {}
        
        math::sign sign() const {
            return operator==(0) ? math::zero : math::positive;
        }
        
        constexpr static endian::order opposite = endian::opposite(r);
        
        explicit N_bytes(const N_bytes<opposite>&);
        
    private: 
        N_bytes(size_t size, byte fill) : bytes{size, fill} {}
        
    public:
        static N_bytes zero(size_t size) {
            return N_bytes{0, size};
        }
        
        N_bytes& operator=(const N_bytes<opposite>& n) {
            return operator=(n.reverse());
        }
        
        N_bytes& operator=(const N& n) {
            return operator=(N_bytes{n});
        }
        
        bool operator==(const N_bytes& n) const {
            return N{*this} == N{n}; // Inefficient
        }
        
        bool operator!=(const N_bytes& n) const {
            return !operator==(n);
        }
        
    private:
        using word = boost::endian::endian_arithmetic<r, uint32, 32>;
        using words_type = arithmetic::unfixed_words<uint32, r>;
        using methods = arithmetic::unoriented<words_type, word>;
            
        words_type words() {
            return words_type{static_cast<slice<byte>>(*this)};
        }
        
        const words_type words() const {
            return words_type::make(static_cast<slice<byte>>(*this));
        }
        
    public:
        bool operator<(uint64 n) const {
            return operator<(N_bytes<r>{n});
        }
        
        bool operator<(const N_bytes& n) const {
            return methods::less(std::max(size(), n.size()), words(), n.words());
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
            return methods::less_equal(std::max(size(), n.size()), words(), n.words());
        }
        
        bool operator<=(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<=(N_bytes{z});
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
        
        N_bytes operator+(const N_bytes& n) const {
            throw method::unimplemented{"N_bytes::+"};
        }
        
        N_bytes& operator+=(const N_bytes& n) {
            return operator=(operator+(n));
        }
        
        N_bytes operator-(const N_bytes& n) const {
            throw method::unimplemented{"N_bytes::-"};
        }
        
        N_bytes& operator-=(const N_bytes& n) {
            return operator=(operator-(n));
        }
        
        N_bytes operator*(const N_bytes&) const;
        
        N_bytes& operator*=(const N_bytes& n) {
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
        
        template <size_t size, endian::order o> 
        explicit N_bytes(const bounded<size, o, false>& b) : N_bytes{bytes_view(b), o} {}
        
        constexpr static math::number::natural::interface<N_bytes> is_natural{};

    private:
        N_bytes(bytes_view b, endian::order o) : bytes{b.size()} {
            std::copy(b.begin(), b.end(), begin());
            if (o != r) std::reverse(begin(), end());
        }
        
        N_bytes(const Z_bytes<r>& z) {
            throw method::unimplemented{"N_bytes{Z_bytes}"};
        }
        
        friend struct abs<N_bytes, Z_bytes<r>>;
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
        uint32 s = size();
        if (s == 0) return N_bytes{bytes{}};
        N_bytes re{};
        if (r == endian::big) {
            auto b = begin();
            while (*b == 0) {
                s--;
                b++;
            }
            re.Value = bytes(s);
            std::copy(b, end(), re.begin());
        } else {
            auto b = rbegin();
            while (*b == 0) {
                s--;
                b++;
            }
            re.Value = bytes(size);
            std::copy(b, rend(), re.rend());
        }
        return re;
    }
}

namespace data::encoding::hexidecimal { 
    
    template <endian::order r>
    std::ostream& write(std::ostream& o, const math::number::N_bytes<r>& n) {
        return encoding::hexidecimal::write(o, n, r);
    }
    
}

namespace data::encoding::decimal {
    
    template <endian::order r>
    std::ostream& write(std::ostream& o, const math::number::N_bytes<r>& n) {
        throw method::unimplemented{"decimal::write(N_bytes)"};
    }
    
}

template <data::endian::order r>
std::ostream& operator<<(std::ostream& o, const data::math::number::N_bytes<r>& n) {
    if (o.flags() & std::ios::hex) return data::encoding::decimal::write(o, n);
    if (o.flags() & std::ios::dec) return data::encoding::hexidecimal::write(o, n);
    return o;
}

#endif

