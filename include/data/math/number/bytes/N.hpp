// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <limits>

#include <data/encoding/digits.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/cross.hpp>
#include <data/encoding/endian/words.hpp>

namespace data::math::number {
    
    template <endian::order r> struct N_bytes;
    
    template <endian::order r>
    bool operator==(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator!=(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<=(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>=(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator==(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r>
    bool operator!=(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r>
    bool operator<=(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r>
    bool operator>=(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r>
    bool operator<(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r>
    bool operator>(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r>
    bool operator==(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator!=(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<=(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>=(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator==(const N_bytes<r>&, const int);
    
    template <endian::order r>
    bool operator!=(const N_bytes<r>&, const int);
    
    template <endian::order r>
    bool operator<=(const N_bytes<r>&, const int);
    
    template <endian::order r>
    bool operator>=(const N_bytes<r>&, const int);
    
    template <endian::order r>
    bool operator<(const N_bytes<r>&, const int);
    
    template <endian::order r>
    bool operator>(const N_bytes<r>&, const int);
    
    template <endian::order r>
    bool operator==(const int, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator!=(const int, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<=(const int, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>=(const int, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<(const int, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>(const int, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator==(const N_bytes<r>&, const uint64);
    
    template <endian::order r>
    bool operator!=(const N_bytes<r>&, const uint64);
    
    template <endian::order r>
    bool operator<=(const N_bytes<r>&, const uint64);
    
    template <endian::order r>
    bool operator>=(const N_bytes<r>&, const uint64);
    
    template <endian::order r>
    bool operator<(const N_bytes<r>&, const uint64);
    
    template <endian::order r>
    bool operator>(const N_bytes<r>&, const uint64);
    
    template <endian::order r>
    bool operator==(const uint64, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator!=(const uint64, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<=(const uint64, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>=(const uint64, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator<(const uint64, const N_bytes<r>&);
    
    template <endian::order r>
    bool operator>(const uint64, const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator+(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator-(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator*(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator+(const N_bytes<r>&, uint32);
    
    template <endian::order r>
    N_bytes<r> operator-(const N_bytes<r>&, uint32);
    
    template <endian::order r>
    N_bytes<r> operator*(const N_bytes<r>&, uint32);
    
    template <endian::order r>
    Z_bytes<r> operator~(const N_bytes<r>&);
    
    template <endian::order r>
    Z_bytes<r> operator-(const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator^(const N_bytes<r>&, uint64 pow);
    
    template <endian::order r>
    N_bytes<r> operator<<(const N_bytes<r>&, int);
    
    template <endian::order r>
    N_bytes<r> operator>>(const N_bytes<r>&, int);
    
    template <endian::order r>
    struct N_bytes : bytes {
        
        N_bytes() : bytes{} {}
        
        N_bytes(const uint64 x) {
            resize(8);
            endian::arithmetic<r, false, 8> xx{x};
            std::copy(xx.begin(), xx.end(), begin());
        }
        
        static N_bytes read(string_view x) {
            if (x.size() == 0) return 0;
            ptr<N_bytes<r>> b = encoding::natural::read<r>(x);
            if (b == nullptr) throw std::logic_error{"Not a valid number"};
            return N_bytes<r>{*b};
        }
        
        explicit N_bytes(string_view s) : N_bytes{read(s)} {}
        
        // A bit inefficient. 
        explicit N_bytes(const N& n) : N_bytes(data::encoding::hexidecimal::write(n)) {}
        
        explicit N_bytes(bytes_view b) {
            resize(b.size());
            std::copy(b.begin(), b.end(), begin());
        }
        
        math::sign sign() const {
            return *this == N_bytes(uint64(0)) ? math::zero : math::positive;
        }
        
        operator N_bytes<endian::opposite(r)>() const {
            N_bytes<endian::opposite(r)> z;
            z.resize(size());
            std::copy(this->rbegin(), this->rend(), z.begin());
            return z;
        }
        
    private:
        
        N_bytes(size_t size, byte fill);
        
        using words = ::data::arithmetic::words<N_bytes<r>, r>;
        
    public:
        
        static N_bytes zero(size_t size) {
            return N_bytes(size, 0x00);
        }
        
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
        
        N_bytes& operator+=(const N_bytes& n) {
            return operator=(*this + n);
        }
        
        N_bytes& operator-=(const N_bytes& n) {
            return operator=(*this - n);
        }
        
        N_bytes& operator*=(const N_bytes& n) {
            return operator=(*this * n);
        }
        
        N_bytes& operator*=(const uint64 n) {
            return operator=(*this * n);
        }
        
        N_bytes& operator^=(uint32 n) {
            return operator=(*this ^ n);
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
        
        N_bytes& operator<<=(int64 x) {
            return operator=(*this << x);
        }
        
        N_bytes& operator>>=(int64 x) {
            return operator=(*this >> x);
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
            std::copy(digits().begin(), digits().begin() + 8, xx.begin());
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
        *n = encoding::read_base<math::N_bytes<endian::big>>(s, 10, digit);
        
        return n;
    }
    
    template <typename range> 
    std::ostream &write(std::ostream& o, range r) {
        math::N_bytes<endian::big> n{};
        
        n.resize(r.end() - r.begin());
        
        std::copy(r.begin(), r.end(), n.rbegin());
        return o << write_base(n, characters());
    }
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r> 
    std::ostream &write(std::ostream& o, const math::N_bytes<r>& n, hex::letter_case q = hex::lower) {
        return hex::write(o << "0x", math::N_bytes<endian::big>(n), q);
    }
    
    template <endian::order r>  
    std::string inline write(const math::N_bytes<r>& n, hex::letter_case q = hex::lower) {
        std::stringstream ss;
        hexidecimal::write(ss, n, q);
        return ss.str();
    }
    
    template <endian::order r> ptr<math::N_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        ptr<math::N_bytes<r>> n = std::make_shared<math::N_bytes<r>>();
        n->resize((s.size() - 2) / 2);
        boost::algorithm::unhex(s.begin() + 2, s.end(), n->digits().rbegin());
        
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

namespace data::math::number {
    
    template <endian::order r>
    bool inline operator==(const N_bytes<r> &a, const uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator!=(const N_bytes<r> &a, const uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<=(const N_bytes<r> &a, const uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>=(const N_bytes<r> &a, const uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<(const N_bytes<r> &a, const uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>(const N_bytes<r> &a, const uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator==(const uint64 a, const N_bytes<r> &b) {
        return N_bytes<r>(a) == b;
    }
    
    template <endian::order r>
    bool inline operator!=(const uint64 a, const N_bytes<r> &b) {
        return N_bytes<r>(a) != b;
    }
    
    template <endian::order r>
    bool inline operator<=(const uint64 a, const N_bytes<r> &b) {
        return N_bytes<r>(a) <= b;
    }
    
    template <endian::order r>
    bool inline operator>=(const uint64 a, const N_bytes<r> &b) {
        return N_bytes<r>(a) >= b;
    }
    
    template <endian::order r>
    bool inline operator<(const uint64 a, const N_bytes<r> &b) {
        return N_bytes<r>(a) < b;
    }
    
    template <endian::order r>
    bool inline operator>(const uint64 a, const N_bytes<r> &b) {
        return N_bytes<r>(a) > b;
    }
    
    template <endian::order r>
    bool inline operator==(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) == b;
    }
    
    template <endian::order r>
    bool inline operator!=(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) != b;
    }
    
    template <endian::order r>
    bool inline operator<=(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) <= b;
    }
    
    template <endian::order r>
    bool inline operator>=(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) >= b;
    }
    
    template <endian::order r>
    bool inline operator<(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) < b;
    }
    
    template <endian::order r>
    bool inline operator>(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) > b;
    }
    
    template <endian::order r>
    bool inline operator==(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a == Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool operator!=(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a != Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<=(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a <= Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>=(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a >= Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a < Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a > Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator==(const N_bytes<r> &a, const int b) {
        return Z_bytes<r>(a) == Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator!=(const N_bytes<r> &a, const int b) {
        return Z_bytes<r>(a) != Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<=(const N_bytes<r> &a, const int b) {
        return Z_bytes<r>(a) <= Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>=(const N_bytes<r> &a, const int b) {
        return Z_bytes<r>(a) >= Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<(const N_bytes<r> &a, const int b) {
        return Z_bytes<r>(a) < Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>(const N_bytes<r> &a, const int b) {
        return Z_bytes<r>(a) > Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator==(const int a, const N_bytes<r> &b) {
        return Z_bytes<r>(a) == Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator!=(const int a, const N_bytes<r> &b) {
        return Z_bytes<r>(a) != Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<=(const int a, const N_bytes<r> &b) {
        return Z_bytes<r>(a) <= Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>=(const int a, const N_bytes<r> &b) {
        return Z_bytes<r>(a) >= Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator<(const int a, const N_bytes<r> &b) {
        return Z_bytes<r>(a) < Z_bytes<r>(b);
    }
    
    template <endian::order r>
    bool inline operator>(const int a, const N_bytes<r> &b) {
        return Z_bytes<r>(a) > Z_bytes<r>(b);
    }
    
    // NOTE: these next 3 functions are the FIRST thing 
    // we need to do to make this class more efficient. 
    template <endian::order r>
    N_bytes<r> inline operator+(const N_bytes<r> &n, uint32 u) {
        return N_bytes<r>(N(n) + u);
    }
    
    template <endian::order r>
    N_bytes<r> inline operator-(const N_bytes<r> &n, uint32 u) {
        return N_bytes<r>(N(n) - u);
    }
    
    // TODO this function is not very efficient because
    // it multiplies by bytes. 
    template <endian::order r>
    N_bytes<r> operator*(const N_bytes<r> &n, uint32 u) {
        N_bytes<r> m{};
        m.resize(n.size() + 4);
        uint64_little x{u};
        auto dn = n.digits();
        auto dm = m.digits();
        auto in = dn.begin();
        auto im = dm.begin();
        auto en = dn.end();
        auto em = dm.end();
        while (in != en) {
            x *= *in;
            *im = x[0];
            x <<= 8;
            im++;
            in++;
        }
        for (int i = 0; i < 4; i++) { 
            *im = x[i];
            im++;
        }
        return N_bytes<r>(N(n) * u);
    }
    
    template <endian::order r>
    bool inline operator==(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N(a) == N(b);
    }
    
    template <endian::order r>
    bool inline operator!=(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N(a) != N(b);
    }
    
    template <endian::order r>
    bool inline operator<=(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N(a) <= N(b);
    }
    
    template <endian::order r>
    bool inline operator>=(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N(a) >= N(b);
    }
    
    template <endian::order r>
    bool inline operator<(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N(a) < N(b);
    }
    
    template <endian::order r>
    bool inline operator>(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N(a) > N(b);
    }
    
    template <endian::order r>
    N_bytes<r> inline operator+(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N_bytes<r>(N(a) + N(b));
    }
    
    template <endian::order r>
    N_bytes<r> inline operator-(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N_bytes<r>(N(a) * N(b));
    }
    
    template <endian::order r>
    N_bytes<r> inline operator*(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N_bytes<r>(N(a) * N(b));
    }
    
    template <endian::order r>
    N_bytes<r> inline operator^(const N_bytes<r> &n, uint64 pow) {
        return N_bytes<r>(N(n) ^ pow);
    }
    
    template <endian::order r> struct bit_shift;
    
    template <> struct bit_shift<endian::big> {
        
        template <typename it1b, typename it2b, typename it2e>
        static void shift_right(it1b ai, it2b bi, it2e be, byte i) {
            uint16 x = combine(byte(0), *ai);
            while (true) {
                *bi = static_cast<byte>(x >> i);
                if (bi != be) return;
                x = combine(*ai, *++ai);
                ++bi;
            }
        }
        
        template <typename it1b, typename it1e, typename it2b>
        static void shift_left(it1b ai, it1e ae, it2b bi, byte i) {
            uint16 x;
            byte last;
            while (true) {
                last = *ai;
                ai++;
                if (ai != ae) break;
                x = combine(last, *ai) >> (8 - i);
                *bi = static_cast<byte>(x);
                bi++;
            }
            x = combine(last, byte(0)) >> (8 - i);
            *bi = static_cast<byte>(x);
        }
        
        N_bytes<endian::big> operator()(const N_bytes<endian::big> &n, int i) {
            if (-i > 8 * n.size()) return {};
            if (i == 0) return n;
            N_bytes<endian::big> x;
            if (i < 0) {
                x = N_bytes<endian::big>::zero(n.size() + i / 8);
                shift_right(n.begin(), x.begin(), x.end(), static_cast<byte>(-i % 8));
            } else {
                x = N_bytes<endian::big>::zero(n.size() + i / 8 + 1);
                shift_left(n.begin(), n.end(), x.begin(), static_cast<byte>(i % 8));
            }
            return x.trim();
        }
    };
    
    template <> struct bit_shift<endian::little> {
        N_bytes<endian::little> operator()(const N_bytes<endian::little> &n, int i) {
            return bit_shift<endian::big>{}(N_bytes<endian::big>(n), i);
        }
    };
    
    template <endian::order r>
    N_bytes<r> inline operator<<(const N_bytes<r> &n, int i) {
        return N_bytes<r>(r == endian::big ? N(n) << i : N(n) >> i);
    }
    
    template <endian::order r>
    N_bytes<r> inline operator>>(const N_bytes<r> &n, int i) {
        return N_bytes<r>(r == endian::big ? N(n) >> i : N(n) << i);
    }
    
    template <endian::order r> 
    N_bytes<r> N_bytes<r>::trim() const {
        uint32 s = size();
        if (s == 0) return N_bytes{bytes{}};
        auto d = digits();
        auto b = d.rbegin();
        while (*b == 0) {
            s--;
            b++;
        }
        N_bytes re{};
        re.resize(s);
        auto q = re.digits().rbegin();
        while (b != d.rend()) {
            *q = *b;
            q++;
            b++;
        }
        return re;
    }
}

#endif

