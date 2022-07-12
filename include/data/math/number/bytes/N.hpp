// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <limits>

#include <data/encoding/digits.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/cross.hpp>

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
    N_bytes<r> &operator<<=(const N_bytes<r>&, int);
    
    template <endian::order r>
    N_bytes<r> &operator>>=(const N_bytes<r>&, int);
    
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
        
        N_bytes(size_t size, byte fill) {
            throw method::unimplemented{"N_bytes(size, fill)"};
        }
        
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
        
        bytes write(endian::order) const; 
        
        N_bytes trim() const;
        
        template <size_t size, endian::order o> 
        explicit N_bytes(const bounded<size, o, false>& b) : N_bytes{bytes_view(b), o} {}
        
        using words_type = encoding::words<r, byte>;
        
        words_type words() {
            return encoding::words<r, byte>{slice<byte>(*this)};
        }
        
        const words_type words() const {
            return encoding::words<r, byte>{slice<byte>(*const_cast<N_bytes*>(this))};
        }
        
        explicit operator uint64() const {
            if (*this > std::numeric_limits<uint64>::max()) throw std::invalid_argument{"value too big"};
            endian::arithmetic<endian::little, false, 8> xx;
            std::copy(words().begin(), words().begin() + 8, xx.begin());
            return uint64(xx);
        } 

    private:
        N_bytes(bytes_view b, endian::order o);
        
        N_bytes(const Z_bytes<r>& z) {
            throw method::unimplemented{"N_bytes{Z_bytes}"};
        }
        
        friend struct abs<N_bytes, Z_bytes<r>>;
    };
    
    template <endian::order r> bool inline is_negative(const N_bytes<r> &) {
        return false;
    }
    
    template <endian::order r> bool inline is_positive(const N_bytes<r> &x) {
        return !is_zero(x);
    }
    
    template <endian::order r> bool inline is_zero(const N_bytes<r> &x) {
        return arithmetic::ones_is_zero(x.words());
    }
    
    template <endian::order r> bool inline is_minimal(const N_bytes<r> &x) {
        return arithmetic::N_is_minimal(x.words());
    }
    
    template <endian::order r> size_t inline minimal_size(const N_bytes<r> &x) {
        return arithmetic::N_minimal_size(x.words());
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
    template <endian::order r> struct commutative<plus<number::N_bytes<r>>, number::N_bytes<r>> {};
    template <endian::order r> struct associative<plus<number::N_bytes<r>>, number::N_bytes<r>> {};
    template <endian::order r> struct commutative<times<number::N_bytes<r>>, number::N_bytes<r>> {};
    template <endian::order r> struct associative<times<number::N_bytes<r>>, number::N_bytes<r>> {};
    
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
        boost::algorithm::unhex(s.begin() + 2, s.end(), n->words().rbegin());
        
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
        auto dn = n.words();
        auto dm = m.words();
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
    
    template <endian::order r> N_bytes<r> N_bytes<r>::trim() const {
        size_t size = minimal_size(*this);
        if (size == this->size()) return *this;
        auto n = N_bytes<r>::zero(size);
        auto w = this->words();
        std::copy(w.begin(), w.begin() + size, n.words().begin());
        return n;
    }
    
    template <endian::order r> N_bytes<r> extend(const N_bytes<r> &x, size_t size) {
        if (size < x.size()) {
            size_t min_size = minimal_size(x); 
            if (size < min_size) throw std::invalid_argument{"cannot extend smaller than minimal size"};
            return extend(x.trim(), size);
        }
        
        if (size == x.size()) return x;
        
        N_bytes<r> z;
        z.resize(size);
        
        auto i = z.words().rbegin();
        for (int n = 0; n < size - x.size(); n++) {
            *i = 0;
            i++;
        }
        
        std::copy(x.words().rbegin(), x.words().rend(), i);
        return z;
    }
    
    template <endian::order r>
    N_bytes<r> inline operator<<(const N_bytes<r> &z, int i) {
        auto n = z;
        return n <<= i;
    }
    
    template <endian::order r>
    N_bytes<r> inline operator>>(const N_bytes<r> &z, int i) {
        auto n = z;
        return n >>= i;
    }
    
    namespace {
        template <endian::order r>
        void inline bit_shift_left(N_bytes<r> &z, uint32 i) {
            z = extend(z, z.size() + (i + 7) / 8);
            return z.bit_shift_left(i);
        }
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator<<=(N_bytes<r> &n, int i) {
        if (i < 0) n.bit_shift_right(-i);
        else bit_shift_left(n, i);
        return n = n.trim();
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator>>=(N_bytes<r> &n, int i) {
        if (i < 0) bit_shift_left(n, -i);
        else n.bit_shift_right(i);
        return n = n.trim();
    }
}

#endif

