// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <limits>

#include <data/encoding/digits.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/cross.hpp>
#include <data/encoding/words.hpp>

namespace data::math::number {
    
    template <endian::order r>
    struct N_bytes : bytes {
        
        N_bytes() : bytes{} {}
        
        N_bytes(const uint64 x) {
            resize(8);
            endian::arithmetic<endian::big, false, 8> xx{x};
            std::copy(xx.begin(), xx.end(), begin());
        }
        
        static N_bytes read(string_view x) {
            if (x.size() == 0) return 0;
            ptr<bytes> b = encoding::natural::read<r>(x);
            if (b == nullptr) return {};
            return N_bytes<r>{bytes_view(*b)};
        }
        
        explicit N_bytes(string_view s) : N_bytes{read(s)} {}
        
        // A bit inefficient. 
        explicit N_bytes(const N& n) : N_bytes(data::encoding::hexidecimal::write(n)) {}
        
        explicit N_bytes(bytes_view b) {
            resize(b.size());
            std::copy(b.begin(), b.end(), begin());
        }
        
        math::sign sign() const {
            return operator==(0) ? math::zero : math::positive;
        }
        
        bool operator==(const N_bytes& n) const {
            return N{*this} == N{n}; // Inefficient
        }
        
        bool operator!=(const N_bytes& n) const {
            return !operator==(n);
        }
        
    private:
        
        N_bytes(size_t size, byte fill);
        
        using words = ::data::arithmetic::words<N_bytes<r>, r>;
        
    public:
        
        static N_bytes zero(size_t size) {
            return N_bytes(size, 0x00);
        }
        
        bool operator<(uint64 n) const {
            return operator<(N_bytes<r>{n});
        }
        
        bool operator<(const N_bytes& n) const;
        
        bool operator<(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<(N_bytes{z.Value});
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
            return arithmetic::less_equal(digits().rend(), digits().rbegin(), n.digits().rend());
        }
        
        bool operator<=(const Z_bytes<r>& z) const {
            return z.is_negative() ? false : operator<=(N_bytes{z});
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
        
        bool operator>=(const N_bytes& n) const {
            return arithmetic::greater_equal(words{}.rend(*this), words{}.rbegin(*this), words{}.rend(n));
        }
        
        bool operator>=(const Z_bytes<r>& z) const;
        
        bool operator>=(const N& n) const {
            return N{*this} >= n;
        }
        
        bool operator>=(const Z& n) const {
            return N{*this} >= n;
        }
        
        N_bytes operator~() const;
        
        N_bytes& operator++() {
            operator+=(1);
            return *this;
        }
        
        N_bytes& operator--() {
            operator+=(1);
            return *this;
        }
        
        N_bytes operator++(int) const {
            N_bytes z = *this;
            ++(*this);
            return z;
        }
        
        N_bytes operator--(int) const {
            N_bytes z = *this;
            ++(*this);
            return z;
        }
        
        N_bytes operator+(const N_bytes& n) const {
            // TODO very inefficient. 
            return N_bytes{N{*this} + N{n}};
        }
        
        N_bytes& operator+=(const N_bytes& n) {
            return (N_bytes&)operator=(operator+(n));
        }
        
        N_bytes operator-(const N_bytes& n) const {
            throw method::unimplemented{"N_bytes::-"};
        }
        
        N_bytes& operator-=(const N_bytes& n) {
            return operator=(operator-(n));
        }
        
        N_bytes operator*(const N_bytes& n) const {
            // TODO inefficient
            return N_bytes{N{*this} * N{n}};
        }
        
        N_bytes& operator*=(const N_bytes& n) {
            return (N_bytes&)operator=(operator*(n));
        }
        
        N_bytes operator^(uint32 n) const;
        
        N_bytes& operator^=(uint32 n) {
            return operator=(operator^(n));
        }
        
        math::division<N_bytes> divide(const N_bytes& n) const {
            return natural::divide<N_bytes>(*this, n);
        }
        
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
        
        data::arithmetic::digits<r> digits() {
            return data::arithmetic::digits<r>{slice<byte>(*this)};
        }
        
        const data::arithmetic::digits<r> digits() const {
            return data::arithmetic::digits<r>{slice<byte>(*const_cast<N_bytes*>(this))};
        }
        
        explicit operator uint64() const {
            if (*this > std::numeric_limits<uint64>::max()) throw std::invalid_argument{"value too big"};
            endian::arithmetic<endian::little, false, 8> xx;
            std::copy(digits().begin(), digits().begin() + 4, xx.begin());
            return uint64(xx);
        } 

    private:
        N_bytes(bytes_view b, endian::order o);
        
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

namespace data::math::number {
    template <endian::order r> 
    struct abs<N_bytes<r>, N_bytes<r>> {
        N_bytes<r> operator()(const N_bytes<r>& i) {
            return i;
        }
    };
}

namespace data::math {
    
    // Declare that the plus and times operation on N are commutative. 
    template <endian::order r> struct commutative<data::plus<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    template <endian::order r> struct associative<data::plus<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    template <endian::order r> struct commutative<data::times<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    template <endian::order r> struct associative<data::times<math::number::N_bytes<r>>, math::number::N_bytes<r>> {};
    
}

namespace data::math::number {
    template <endian::order r>
    std::ostream& operator<<(std::ostream& o, const N_bytes<r>& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        // TODO for dec, we convert N_bytes to N. This is inefficient but it works for now. 
        if (o.flags() & std::ios::dec) return encoding::integer::write(o, gmp::N{n});
        return o;
    }
}

namespace data::encoding::decimal {
    
    template <endian::order r> ptr<math::N_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        ptr<math::N_bytes<r>> n = std::make_shared<math::N_bytes<r>>();
        
        for (char x : s) {
            
            *n *= 10;
            *n += digit(x);
        }
        
        return n;
    }
    
    template <typename range> 
    std::ostream &write(std::ostream& o, range r) {
        math::N_bytes<endian::little> n{};
        
        n.resize(r.end() - r.begin());
        
        std::copy(r.begin(), r.end(), n.begin());
        
        return o << write_base(n, characters());
    }
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r> ptr<math::N_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        ptr<math::N_bytes<r>> n = std::make_shared<math::N_bytes<r>>();
        n->resize((s.size() - 2) / 2);
        auto it = s.begin() + 2;
        while (it != s.end()) {
            *n = 16 * static_cast<byte>(digit(*it)) + digit(*(it + 1));
            it += 2;
        }
        
        return n;
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r> 
    std::ostream &write(std::ostream& o, const math::number::N_bytes<r> &n) {
        return decimal::write(o, n);
    }
    
    template <endian::order r> 
    string write(const math::number::N_bytes<r> &n) {
        std::stringstream ss;
        decimal::write(ss, n);
        return ss.str();
    }
}

#endif

