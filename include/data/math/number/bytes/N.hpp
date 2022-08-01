// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <limits>

#include <data/encoding/digits.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/cross.hpp>

namespace data::math::number {
    
    template <endian::order r>
    N_bytes<r> operator+(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator-(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r>
    N_bytes<r> operator*(const N_bytes<r>&, const N_bytes<r>&);
    
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
    struct N_bytes : oriented<r, byte> {
        
        N_bytes() : oriented<r, byte>{} {}
        
        N_bytes(const uint64 x) {
            this->resize(8);
            endian::arithmetic<false, r, 8> xx{x};
            std::copy(xx.begin(), xx.end(), this->begin());
        }
        
        static N_bytes read(string_view x) {
            if (x.size() == 0) return 0;
            ptr<N_bytes<r>> b = encoding::natural::read<r>(x);
            if (b == nullptr) throw std::logic_error{"Not a valid number"};
            return N_bytes<r>{*b};
        }
        
        explicit N_bytes(string_view s) : N_bytes{read(s)} {}
        
        // inefficient but works. 
        explicit N_bytes(const N& n) : N_bytes() {
            this->resize(n.Value.size() * sizeof(mp_limb_t));
            /*
            auto it = this->words().rbegin();
            for (const mp_limb_t &x : n.Value) {
                endian::arithmetic<endian::big, false, sizeof(mp_limb_t)> xx{x};
                std::copy(xx.begin(), xx.end(), it);
            }
            this->trim();
            */
            N x = n;
            auto w = this->words();
            auto it = w.begin();
            auto e = w.end();
            int step = 0;
            while (x > 0) {
                auto d = x % 256;
                *it = d;
                it++;
                x >>= 8;
                step++;
            }
            while (it != e) {
                *it = 0;
                it++;
                step++;
            }
            this->trim();
        }
        
        explicit N_bytes(bytes_view b) {
            this->resize(b.size());
            std::copy(b.begin(), b.end(), this->begin());
        }
        
        math::sign sign() const {
            return *this == N_bytes(uint64(0)) ? math::zero : positive;
        }
        
        operator N_bytes<endian::opposite(r)>() const {
            N_bytes<endian::opposite(r)> z;
            z.resize(this->size());
            std::copy(this->rbegin(), this->rend(), z.begin());
            return z;
        }
        
    private:
        
        N_bytes(size_t size, byte fill) {
            this->resize(size);
            this->fill(fill);
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
        
        N_bytes &trim();
        
        explicit operator N() const {
            N z; 
            for (const byte &b : this->words()) {
                z << 8;
                z += b;
            }
            return z;
        }
        /*
        template <size_t size, endian::order o> 
        explicit N_bytes(const bounded<size, o, false>& b) : N_bytes{bytes_view(b), o} {}*/
        
        explicit operator uint64() const {
            if (*this > std::numeric_limits<uint64>::max()) throw std::invalid_argument{"value too big"};
            endian::arithmetic<false, endian::little, 8> xx;
            std::copy(this->words().begin(), this->words().begin() + 8, xx.begin());
            return uint64(xx);
        } 

    private:
        N_bytes(bytes_view b, endian::order o);
        
        N_bytes(const Z_bytes<r>& z) {
            if (math::number::is_negative(z)) throw std::logic_error{"negative Z_bytes to N_bytes"};
            this->resize(z.size());
            std::copy(z.begin(), z.end(), this->begin());
        }
        
        friend struct abs<Z_bytes<r>>;
    };
    
    template <endian::order r> N_bytes<r> inline trim(const N_bytes<r> &x) {
        auto n = x;
        return n.trim();
    }
    
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
    
    template <endian::order r> 
    N_bytes<r> inline operator+(const N_bytes<r> &a, uint64 b) {
        return a + N_bytes<r>(b);
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator-(const N_bytes<r> &a, uint64 b) {
        return a - N_bytes<r>(b);
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator*(const N_bytes<r> &a, uint64 b) {
        return a * N_bytes<r>(b);
    }
    
}

namespace data::math {
    template <endian::order r>
    N_bytes<r> inline abs<N_bytes<r>>::operator()(const N_bytes<r>& i) {
        return i;
    }
    
}

namespace data::math::number {
    template <endian::order r>
    std::ostream& operator<<(std::ostream& o, const N_bytes<r>& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        // TODO for dec, we convert N_bytes to N. This is inefficient but it works for now. 
        if (o.flags() & std::ios::dec) return encoding::decimal::write(o, n);
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
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream& o, const math::number::N_bytes<r> &n) {
        return write(o, math::N{n});
    }
    
    template <endian::order r> 
    string inline write(const math::number::N_bytes<r> &n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream& o, const math::N_bytes<r>& n, hex::letter_case q = hex::lower) {
        return hex::write(o << "0x", n.words().reverse(), q);
    }
    
    template <endian::order r>  
    std::string inline write(const math::N_bytes<r>& n, hex::letter_case q = hex::lower) {
        std::stringstream ss;
        hexidecimal::write(ss, n, q);
        return ss.str();
    }
    
}

namespace data::encoding::natural {
    
    template <endian::order r> ptr<math::N_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        if (decimal::valid(s)) return decimal::read<r>(s);
        auto p = hexidecimal::read<r>(s);
        auto n = std::make_shared<math::N_bytes<r>>();
        n->resize(p->size());
        std::copy(p->begin(), p->end(), n->begin());
        return n;
    }
}

namespace data::math::number {
    
    template <endian::order r> 
    bool inline operator==(const N_bytes<r> &a, const N_bytes<r> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return a <=> Z_bytes<r>(b);
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const N_bytes<r> &a, const N_bytes<r> &b) {
        return arithmetic::N_compare(a.words(), b.words());
    }
    
    template <endian::order r>
    bool inline operator==(const N_bytes<r> &a, uint64 b) {
        return a == N_bytes<r>(b);
    }
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const N_bytes<r> &a, uint64 b) {
        return a <=> N_bytes<r>(b);
    }
    
    template <endian::order r> Z_bytes<r> inline operator&(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) & b;
    }
    
    template <endian::order r> Z_bytes<r> inline operator&(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return Z_bytes<r>(b) & a;
    }
    
    template <endian::order r> Z_bytes<r> inline operator|(const N_bytes<r> &a, const Z_bytes<r> &b) {
        return Z_bytes<r>(a) | b;
    }
    
    template <endian::order r> Z_bytes<r> inline operator|(const Z_bytes<r> &a, const N_bytes<r> &b) {
        return Z_bytes<r>(b) | a;
    }
    
    template <endian::order r>
    N_bytes<r> inline operator+(const N_bytes<r> &a, const N_bytes<r> &b) {
        return N_bytes<r>(N(a) + N(b));
    }
    
    template <endian::order r>
    Z_bytes<r> inline operator-(const N_bytes<r> &a) {
        return -Z_bytes<r>(a);
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
    
    template <endian::order r> N_bytes<r> &N_bytes<r>::trim() {
        size_t size = minimal_size(*this);
        if (size == this->size()) return *this;
        auto n = N_bytes<r>::zero(size);
        auto w = this->words();
        std::copy(w.begin(), w.begin() + size, n.words().begin());
        return *this = n;
    }
    
    template <endian::order r> N_bytes<r> extend(const N_bytes<r> &x, size_t size) {
        if (size < x.size()) {
            size_t min_size = minimal_size(x); 
            if (size < min_size) throw std::invalid_argument{"cannot extend smaller than minimal size"};
            return extend(trim(x), size);
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
            return z.words().bit_shift_left(i);
        }
        
        template <endian::order r>
        void inline bit_shift_right(N_bytes<r> &z, uint32 i) {
            return z.words().bit_shift_right(i);
        }
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator<<=(N_bytes<r> &n, int i) {
        if (i < 0) bit_shift_right(n, -i);
        else bit_shift_left(n, i);
        return n = n.trim();
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator>>=(N_bytes<r> &n, int i) {
        if (i < 0) bit_shift_left(n, -i);
        else bit_shift_right(n, i);
        return n = n.trim();
    }
    
    template <endian::order r> N_bytes<r> operator&(const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a.size() < b.size()) return b & a;
        auto bt = extend(b, a.size());
        auto x = N_bytes<r>::zero(a.size());
        data::arithmetic::bit_and<byte>(x.end(), x.begin(), a.begin(), const_cast<const N_bytes<r>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r> N_bytes<r> operator|(const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a.size() < b.size()) return b | a;
        auto bt = extend(b, a.size());
        auto x = N_bytes<r>::zero(a.size());
        data::arithmetic::bit_or<byte>(x.end(), x.begin(), a.begin(), const_cast<const N_bytes<r>&>(bt).begin());
        return x.trim();
    }
}

namespace data {
    template <endian::order r> 
    math::sign inline sign(const math::N_bytes<r> &n) {
        return math::arithmetic::N_sign(n.words());
    }
}

#endif

