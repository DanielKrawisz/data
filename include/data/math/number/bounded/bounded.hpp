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
#include <data/math/number/bytes/N.hpp>
#include <data/encoding/words.hpp>
#include <data/io/wait_for_enter.hpp>

namespace data::math::number {
    
    // satisfies range<byte>
    template <bool u, endian::order, size_t size> requires (size >= 8)
    struct bounded;
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator~(const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator&(const bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator|(const bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator^(const bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator&=(bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator|=(bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator^=(bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator<<(const bounded<u, r, size>&, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator>>(const bounded<u, r, size>&, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator<<=(bounded<u, r, size>&, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator>>=(bounded<u, r, size>&, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator++(bounded<u, r, size>&, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator++(bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator--(bounded<u, r, size>&, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator--(bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator+(const bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator+=(bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator-(const bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator-=(bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator*(const bounded<u, r, size>&, const bounded<u, r, size>&);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator*=(bounded<u, r, size>&, const bounded<u, r, size>&);
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
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool operator==(const bounded<x, r, n>&, const bounded<y, o, z>&);
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const sint<r, size>&, const sint<r, size>&);
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const uint<r, size>&, const uint<r, size>&);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::weak_ordering operator<=>(const bounded<x, r, n>&, const bounded<y, o, z>&);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool operator==(const bounded<x, r, n>&, const endian::arithmetic<y, o, z>&);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::weak_ordering operator<=>(const bounded<x, r, n>&, const endian::arithmetic<y, o, z>&);
    
    template <endian::order r, size_t size, endian::order o>
    bool operator==(const sint<r, size>&, const Z_bytes<o>&);
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering operator<=>(const sint<r, size>&, const Z_bytes<o>&);
    
    template <endian::order r, size_t size, endian::order o>
    bool operator==(const uint<r, size>&, const N_bytes<o>&);
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering operator<=>(const uint<r, size>&, const N_bytes<o>&);
    
    template <endian::order r, size_t size>
    bool operator==(const sint<r, size>&, const Z&);
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const sint<r, size>&, const Z&);
    
    template <endian::order r, size_t size>
    bool operator==(const uint<r, size>&, const N&);
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const uint<r, size>&, const N&);
    
    template <bool u, endian::order r, size_t size>
    sint<r, size> operator-(const bounded<u, r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator|(const sint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator&(const sint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator^(const sint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator+(const sint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator-(const sint<r, size>&, const uint<r, size>&);
    
    template <endian::order r, size_t size>
    sint<r, size> operator*(const sint<r, size>&, const uint<r, size>&);
    
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
    struct bounded<false, r, size> : oriented<r, byte, size> {
        
        bounded() : oriented<r, byte, size>() {
            this->fill(0x00);
        }
        
        bounded(const uint64 x);
        
        bounded(const byte_array<size>& x) : oriented<r, byte, size>{x} {}
        
        explicit bounded(slice<byte, size> x);
        
        // The string can be a hex string or a representation of a number. 
        explicit bounded(string_view s);
        static bounded read(string_view s) {
            return bounded{s};
        }
        
        math::sign sign() const;
        
        bounded& operator+=(uint64);
        bounded& operator-=(uint64);
        bounded& operator*=(uint64);
        
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
        
        operator N_bytes<r>() const {
            N_bytes<r> n;
            n.resize(size);
            std::copy(this->begin(), this->end(), n.begin());
            return n;
        }
        
        explicit operator N() const {
            return N(N_bytes<r>(*this));
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
    struct bounded<true, r, size> : oriented<r, byte, size> {
        
        bounded() : oriented<r, byte, size>{} {
            this->fill(0);
        }
        
        bounded(const int64 x);
        
        bounded(const byte_array<size>& x) : oriented<r, byte, size>{x} {}
        
        bounded(const bounded<false, r, size>&);
        
        explicit bounded(string_view s);
        static bounded read(string_view s) {
            return bounded{s};
        }
        
        explicit bounded(slice<byte, size>);
        
        math::sign sign() const;

        bounded& operator+=(int64);
        bounded& operator-=(int64);
        bounded& operator*=(int64);
        
        static bounded max();
        static bounded min();

        static N_bytes<r> modulus() {
            return uint<r, size>::modulus();
        }

        math::division<bounded> divide(const bounded&) const;
        
        bounded operator/(const bounded& n) const {
            return divide(n).Quotient;
        }

        bounded operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
        operator Z_bytes<r>() const {
            Z_bytes<r> n;
            n.resize(size);
            std::copy(this->begin(), this->end(), n.begin());
            return n;
        }
        
        explicit operator Z() const {
            return Z(Z_bytes<r>(*this));
        }
        
    private:
        explicit bounded(const Z_bytes<r>& z) {
            if (z > Z_bytes<r> {max()} || z < Z_bytes<r> {min()}) throw std::out_of_range{"Z_bytes too big"};
            throw method::unimplemented{"bounded{Z_bytes}"};
        }
    };
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool inline operator==(const bounded<x, r, n> &a, const bounded<y, o, z> &b) {
        return (a <=> b) == 0;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool operator==(const bounded<x, r, n> &a, const endian::arithmetic<y, o, z> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o>
    bool operator==(const sint<r, size> &a, const Z_bytes<o> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o>
    bool operator==(const uint<r, size> &a, const N_bytes<o> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size>
    bool operator==(const sint<r, size> &a, const Z &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size>
    bool operator==(const uint<r, size> &a, const N &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering operator<=>(const sint<r, size> &a, const Z_bytes<o> &b) {
        return Z_bytes<r>(a) <=> b;
    }
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering operator<=>(const uint<r, size> &a, const N_bytes<o> &b) {
        return Z_bytes<r>(a) <=> b;
    }
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const sint<r, size> &a, const Z &b) {
        return Z(a) <=> b;
    }
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const uint<r, size> &a, const N &b) {
        return N(a) <=> b;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::weak_ordering operator<=>(const bounded<x, r, n> &a, const endian::arithmetic<y, o, z> &b) {
        return a <=> bounded<y, o, z>(b);
    }
    
    template <endian::order r, size_t x> bool inline is_positive(const uint<r, x> &n) {
        return !is_zero(n);
    }
    
    template <endian::order r, size_t x> bool inline is_positive(const sint<r, x> &n) {
        return !is_negative(n) && !is_zero(n);
    }
    
    template <endian::order r, size_t x> bool inline is_negative(const uint<r, x> &n) {
        return false;
    }
    
    template <data::endian::order r, size_t size>
    std::ostream inline &operator<<(std::ostream& s, const sint<r, size>& n) {
        return s << Z_bytes<r>(n);
    }

    template <data::endian::order r, size_t size>
    std::ostream inline &operator<<(std::ostream& s, const uint<r, size>& n) {
        return s << N_bytes<r>(n);
    }

    template <endian::order o, size_t size>
    inline math::sign uint<o, size>::sign() const {
        return *this == 0 ? math::zero : math::positive;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator++(bounded<u, r, size> &x, int) {
        bounded n = x;
        ++x;
        return n;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator--(bounded<u, r, size> &x, int) {
        bounded n = x;
        ++x;
        return n;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> operator|(const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy(b.begin(), b.end(), x.begin());
        return a | x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> operator&(const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy(b.begin(), b.end(), x.begin());
        return a & x;        
    }
    
    template <endian::order r, size_t size>
    sint<r, size> operator^(const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy(b.begin(), b.end(), x.begin());
        return a ^ x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> operator+(const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy(b.begin(), b.end(), x.begin());
        return a + x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> operator-(const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy(b.begin(), b.end(), x.begin());
        return a - x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> operator*(const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy(b.begin(), b.end(), x.begin());
        return a * x;
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

namespace data::math::number {
    
    template <endian::order r, size_t size>
    std::weak_ordering operator<=>(const sint<r, size> &a, const sint<r, size> &b) {
        bool na = is_negative(a);
        bool nb = is_negative(b);
        
        if (na && nb) {
            return arithmetic::N_compare(b.words(), a.words());
        }
        
        if (!na && !nb) return arithmetic::N_compare(a.words(), b.words());
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <endian::order r, size_t size>
    std::weak_ordering inline operator<=>(const uint<r, size> &a, const uint<r, size> &b) {
        return arithmetic::N_compare(a.words(), b.words());
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::weak_ordering inline operator<=>(const bounded<x, r, n>&, const bounded<y, o, z>&) {
        throw 0;
    }
        
    template <endian::order o, size_t size>
    inline math::sign sint<o, size>::sign() const {
        if (*this == 0) return math::zero;
        return (this->words()[-1]) < 0x80 ? math::positive : math::negative;
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded(const uint64 x) : oriented<r, byte, size>{} {
        this->fill(0);
        endian::arithmetic<false, endian::little, 8> n{x};
        std::copy(n.begin(), n.end(), this->words().begin());
    }
    
    template <endian::order r, size_t size>
    sint<r, size>::bounded(const int64 x) : oriented<r, byte, size>{} {
        this->fill(x < 0 ? 0xff : 0x00); 
        endian::arithmetic<true, endian::little, 8> n{x};
        std::copy(n.begin(), n.end(), this->words().begin());
    }
        
    template <endian::order r, size_t size>
    uint<r, size>::bounded(slice<byte, size> x) {
        std::copy(x.begin(), x.end(), this->begin());
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded(string_view s) : bounded{} {
        
        ptr<N_bytes<endian::little>> dec = encoding::natural::read<endian::little>(s);
        if (dec != nullptr) {
            if (dec->size() <= size) {
                std::copy(dec->begin(), dec->end(), this->words().begin());
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
                if (is_negative(*dec)) this->fill(0xff);
                std::copy(dec->begin(), dec->end(), this->words().begin());
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
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator++(bounded<u, r, size> &x) {
        data::arithmetic::plus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        return x;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator--(bounded<u, r, size> &x) {
        data::arithmetic::minus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        return x;
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
    
    template <bool u, endian::order r, size_t size>
    sint<r, size> operator-(const bounded<u, r, size>& a) {
        sint<r, size> x;
        std::copy(a.begin(), a.end(), x.begin());
        x.bit_negate();
        return ++x;
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
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator&(const bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        bounded<u, r, size> x;
        data::arithmetic::bit_and<byte>(x.words().end(), x.words().begin(), a.words().begin(), b.words().begin());
        return x;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator|(const bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        bounded<u, r, size> x;
        data::arithmetic::bit_or<byte>(x.words().end(), x.words().begin(), a.words().begin(), b.words().begin());
        return x;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator^(const bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        bounded<u, r, size> x;
        data::arithmetic::bit_xor<byte>(x.words().end(), x.words().begin(), a.words().begin(), b.words().begin());
        return x;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator&=(bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        data::arithmetic::bit_and<byte>(a.words().end(), a.words().begin(), 
            const_cast<const bounded<u, r, size>&>(a).words().begin(), b.words().begin());
        return a;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator|=(bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        data::arithmetic::bit_or<byte>(a.words().end(), a.words().begin(), 
            const_cast<const bounded<u, r, size>&>(a).words().begin(), b.words().begin());
        return a;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator^=(bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        data::arithmetic::bit_xor<byte>(a.words().end(), a.words().begin(), 
            const_cast<const bounded<u, r, size>&>(a).words().begin(), b.words().begin());
        return a;
    }
    
    template <bool u, endian::order o, size_t size>
    bounded<u, o, size> inline operator+(const bounded<u, o, size> &a, const bounded<u, o, size> &n) {
        bounded<u, o, size> x;
        data::arithmetic::plus<byte>(x.words().end(), x.words().begin(), a.words().begin(), n.words().begin());
        return x;
    }
    
    template <bool u, endian::order o, size_t size>
    bounded<u, o, size> inline &operator+=(bounded<u, o, size> &a, const bounded<u, o, size> &n) {
        data::arithmetic::plus<byte>(a.words().end(), a.words().begin(), 
            const_cast<const bounded<u, o, size>&>(a).words().begin(), n.words().begin());
        return a;
    }
    
    template <bool u, endian::order o, size_t size>
    bounded<u, o, size> operator-(const bounded<u, o, size> &a, const bounded<u, o, size> &n) {
        bounded<u, o, size> z;
        std::copy(n.begin(), n.end(), z.begin());
        z.bit_negate();
        ++z;
        bounded<u, o, size> x;
        data::arithmetic::minus<byte>(x.words().end(), x.words().begin(), a.words().begin(), z.words().begin());
        return x;
    }
    
    template <bool u, endian::order o, size_t size>
    bounded<u, o, size> &operator-=(bounded<u, o, size> &a, const bounded<u, o, size> &n) {
        bounded<u, o, size> z;
        std::copy(n.begin(), n.end(), z.begin());
        z.bit_negate();
        ++z;
        data::arithmetic::plus<byte>(a.words().end(), a.words().begin(), 
            const_cast<const bounded<u, o, size>&>(a).words().begin(), z.words().begin());
        return a;
    }
    
    template <bool u, endian::order o, size_t size>
    bounded<u, o, size> inline operator*(const bounded<u, o, size> &a, const bounded<u, o, size> &n) {
        bounded<u, o, size> x;
        data::arithmetic::times<byte>(x.words().end(), x.words().begin(), a.words().begin(), n.words().begin());
        return x;
    }
    
    template <bool u, endian::order o, size_t size>
    bounded<u, o, size> inline &operator*=(bounded<u, o, size> &a, const bounded<u, o, size> &n) {
        data::arithmetic::times<byte>(a.words().end(), a.words().begin(), 
            const_cast<const bounded<u, o, size>&>(a).words().begin(), n.words().begin());
        return a;
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
    
    template <bool u, endian::order r, size_t x> bool inline is_zero(const bounded<u, r, x> &z) {
        return arithmetic::ones_is_zero(z.words());
    }
    
    template <endian::order r, size_t x> bool inline is_negative(const sint<r, x> &z) {
        return arithmetic::sign_bit(z.words());
    }

}

#endif
