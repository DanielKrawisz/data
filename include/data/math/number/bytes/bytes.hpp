// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_BYTES
#define DATA_MATH_NUMBER_BYTES_BYTES

#include <data/math/number/gmp/mpz.hpp>
#include <data/encoding/hex.hpp>
#include <data/cross.hpp>

namespace data::math::number {
    
    // an implementation of the natural numbers that is
    // encoded as a big or little endian sequence of bytes. 
    template <endian::order> struct N_bytes;
    
    // similar implementation of the integers. We have both 
    // one's and two's complement. N_bytes works as the absolute
    // value of only the one's complement types. Two's complement
    // works as its own number system without a corresponding 
    // natural number type. 
    template <endian::order, complement> struct Z_bytes;
    
    template <endian::order r> 
    bool operator==(const N_bytes<r> &a, const N_bytes<r> &b);
    
    template <endian::order r, complement c> 
    bool operator==(const Z_bytes<r, c> &a, const Z_bytes<r, c> &b);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const Z_bytes<r, ones>&, const Z_bytes<r, ones>&);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const Z_bytes<r, twos>&, const Z_bytes<r, twos>&);
    
    template <endian::order r, complement c> 
    std::weak_ordering operator<=>(const Z_bytes<r, c>&, const N_bytes<r>&);
    
    template <endian::order r> 
    bool operator==(const N_bytes<r>&, uint64);
    
    template <endian::order r, complement c> 
    bool operator==(const Z_bytes<r, c>&, int64);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const N_bytes<r>&, uint64);
    
    template <endian::order r, complement c> 
    std::weak_ordering operator<=>(const Z_bytes<r, c>&, int64);
    
    template <endian::order r> 
    bool operator==(const N_bytes<r>&, N);
    
    template <endian::order r> 
    bool operator==(const Z_bytes<r>&, Z);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const N_bytes<r>&, N);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const Z_bytes<r>&, Z);
    
    // bit negate
    template <endian::order r> Z_bytes<r, ones> operator~(const N_bytes<r>&);
    template <endian::order r> Z_bytes<r, ones> operator~(const Z_bytes<r, ones>&);
    
    // bit xor
    template <endian::order r> Z_bytes<r, ones> operator^(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r, ones> operator^(const Z_bytes<r, ones>&, const Z_bytes<r, ones>&);
    template <endian::order r> Z_bytes<r, ones> operator^(const Z_bytes<r, ones>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r, ones> operator^(const N_bytes<r>&, const Z_bytes<r, ones>&);
    
    // bit and 
    template <endian::order r> N_bytes<r> operator&(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator&(const Z_bytes<r, c>&, const Z_bytes<r, c>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator&(const N_bytes<r>&, const Z_bytes<r, c>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator&(const Z_bytes<r, c>&, const N_bytes<r>&);
    
    // bit or 
    template <endian::order r> N_bytes<r> operator|(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator|(const Z_bytes<r, c>&, const Z_bytes<r, c>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator|(const N_bytes<r>&, const Z_bytes<r, c>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator|(const Z_bytes<r, c>&, const N_bytes<r>&);
    
    // negation
    template <endian::order r> Z_bytes<r, ones> operator-(const N_bytes<r>&);
    template <endian::order r> Z_bytes<r, ones> operator-(const Z_bytes<r, ones>&);
    template <endian::order r> Z_bytes<r, twos> operator-(const Z_bytes<r, twos>&);
    
    template <endian::order r> 
    N_bytes<r> operator+(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    N_bytes<r> operator-(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    N_bytes<r> operator*(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator+(const Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator-(const Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator*(const Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator+(const Z_bytes<r, c>&, const N_bytes<r>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator-(const Z_bytes<r, c>&, const N_bytes<r>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator*(const Z_bytes<r, c>&, const N_bytes<r>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator+(const N_bytes<r>&, const Z_bytes<r, c>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator-(const N_bytes<r>&, const Z_bytes<r, c>&);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator*(const N_bytes<r>&, const Z_bytes<r, c>&);
    
    template <endian::order r> 
    N_bytes<r> operator+(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    N_bytes<r> operator-(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    N_bytes<r> operator*(const N_bytes<r>&, uint64);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator+(const Z_bytes<r, c>&, int64);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator-(const Z_bytes<r, c>&, int64);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator*(const Z_bytes<r, c>&, int64);
    
    template <endian::order r> 
    N_bytes<r> operator<<(const N_bytes<r>&, int);
    
    template <endian::order r> 
    N_bytes<r> operator>>(const N_bytes<r>&, int);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator<<(const Z_bytes<r, c>&, int);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator>>(const Z_bytes<r, c>&, int);
    
    template <endian::order r> bool is_zero(const N_bytes<r> &);
    template <endian::order r> bool is_zero(const Z_bytes<r, ones> &);
    template <endian::order r> bool is_zero(const Z_bytes<r, twos> &);
    
    template <endian::order r> bool is_negative(const N_bytes<r> &);
    template <endian::order r> bool is_negative(const Z_bytes<r, ones> &x);
    template <endian::order r> bool is_negative(const Z_bytes<r, twos> &x);
    
    template <endian::order r> bool is_positive(const N_bytes<r> &x);
    template <endian::order r> bool is_positive(const Z_bytes<r, ones> &);
    template <endian::order r> bool is_positive(const Z_bytes<r, twos> &);
    
    template <endian::order r> bool is_positive_zero(const Z_bytes<r, twos> &);
    template <endian::order r> bool is_negative_zero(const Z_bytes<r, twos> &);
    
    template <endian::order r> bool is_minimal(const N_bytes<r> &);
    template <endian::order r> bool is_minimal(const Z_bytes<r, ones> &);
    template <endian::order r> bool is_minimal(const Z_bytes<r, twos> &);
    
    template <endian::order r> size_t minimal_size(const N_bytes<r> &);
    template <endian::order r> size_t minimal_size(const Z_bytes<r, ones> &);
    template <endian::order r> size_t minimal_size(const Z_bytes<r, twos> &);
    
    template <endian::order r> N_bytes<r> extend(const N_bytes<r> &, size_t);
    template <endian::order r> Z_bytes<r, ones> extend(const Z_bytes<r, ones> &, size_t);
    template <endian::order r> Z_bytes<r, twos> extend(const Z_bytes<r, twos> &, size_t);
    
    template <endian::order r> N_bytes<r> trim(const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> trim(const Z_bytes<r, c> &);
    
}

namespace data::math {
    
    template <endian::order r> using N_bytes = number::N_bytes<r>;
    template <endian::order r> using Z_bytes = number::Z_bytes<r, number::ones>;
    
    template <endian::order r> struct abs<N_bytes<r>> {
        N_bytes<r> operator()(const N_bytes<r> &);
    };
    
    template <endian::order r> struct abs<Z_bytes<r>> {
        N_bytes<r> operator()(const Z_bytes<r> &);
    };
    
    template <endian::order r> struct abs<number::Z_bytes<r, number::twos>> {
        number::Z_bytes<r, number::twos> operator()(const number::Z_bytes<r, number::twos> &);
    };
    
    template <endian::order r> struct quadrance<N_bytes<r>> { 
        N_bytes<r> operator()(const N_bytes<r> &);
    };
    
    template <endian::order r, number::complement c> struct quadrance<number::Z_bytes<r, c>> { 
        auto operator()(const number::Z_bytes<r, c> &x) -> decltype(data::abs(x));
    };
    
    template <endian::order r> struct first<N_bytes<r>> {
        N_bytes<r> operator()();
    };
    
    template <endian::order r, number::complement c> struct first<number::Z_bytes<r, c>> {
        number::Z_bytes<r, c> operator()();
    };
    
    template <endian::order r> struct lowest<N_bytes<r>> : first<N_bytes<r>> {};
    
    template <endian::order r> number::N_bytes<r> next(const number::N_bytes<r>&);
    template <endian::order r> number::N_bytes<r> increment(const number::N_bytes<r>&);
    template <endian::order r> number::N_bytes<r> decrement(const number::N_bytes<r>&);
    
    template <endian::order r, number::complement c> number::Z_bytes<r, c> increment(const number::Z_bytes<r, c>&);
    template <endian::order r, number::complement c> number::Z_bytes<r, c> decrement(const number::Z_bytes<r, c>&);
    
    template <endian::order r, number::complement c> number::Z_bytes<r, c> next(const number::Z_bytes<r, c>&);
    
    // Declare that the plus and times operation are commutative and associative. 
    template <endian::order r> 
    struct commutative<plus<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct associative<plus<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct commutative<times<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct associative<times<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r, number::complement c> 
    struct commutative<plus<number::Z_bytes<r, c>>, number::Z_bytes<r, c>> {};
    
    template <endian::order r, number::complement c> 
    struct associative<plus<number::Z_bytes<r, c>>, number::Z_bytes<r, c>> {};
    
    template <endian::order r, number::complement c> 
    struct commutative<times<number::Z_bytes<r, c>>, number::Z_bytes<r, c>> {};
    
    template <endian::order r, number::complement c> 
    struct associative<times<number::Z_bytes<r, c>>, number::Z_bytes<r, c>> {};
    
    template <endian::order r, number::complement c> struct inverse<plus<number::Z_bytes<r, c>>, number::Z_bytes<r, c>> {
        number::Z_bytes<r, c> operator()(const number::Z_bytes<r, c> &a, const number::Z_bytes<r, c> &b) {
            return b - a;
        }
    };
    
}

namespace data {
    
    template <endian::order r> math::sign sign(const math::N_bytes<r> &);
    template <endian::order r, math::number::complement c> math::sign sign(const math::number::Z_bytes<r, c> &);
    
}

namespace data::math::number {
    
    template <endian::order r> N_bytes<r> operator/(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator/(const Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> operator/(const N_bytes<r>&, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> operator/(const Z_bytes<r, c>&, int64);
    
    template <endian::order r> N_bytes<r> operator%(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> N_bytes<r> operator%(const Z_bytes<r, c>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r, twos> operator%(const Z_bytes<r, twos>&, const Z_bytes<r, twos>&);
    
    template <endian::order r> uint64 operator%(const N_bytes<r>&, uint64);
    template <endian::order r, complement c> uint64 operator%(const Z_bytes<r, c>&, uint64);
    
    // pre-increment and decreement
    template <endian::order r> N_bytes<r> &operator++(N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> &operator++(Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator--(N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> &operator--(Z_bytes<r, c>&);
    
    // post-increment and decrement
    template <endian::order r> N_bytes<r> operator++(N_bytes<r>&, int);
    template <endian::order r, complement c> Z_bytes<r, c> operator++(Z_bytes<r, c>&, int);
    
    template <endian::order r> N_bytes<r> operator--(N_bytes<r>&, int);
    template <endian::order r, complement c> Z_bytes<r, c> operator--(Z_bytes<r, c>&, int);
    
    template <endian::order r> N_bytes<r> &operator+=(N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> &operator+=(Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator-=(N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> &operator-=(Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator+=(N_bytes<r>&, uint64);
    template <endian::order r> N_bytes<r> &operator-=(N_bytes<r>&, uint64);
    
    template <endian::order r, complement c> Z_bytes<r, c> &operator+=(Z_bytes<r, c>&, int64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator-=(Z_bytes<r, c>&, int64);
    
    template <endian::order r> N_bytes<r> &operator*=(N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> &operator*=(Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator*=(N_bytes<r>&, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator*=(Z_bytes<r, c>&, int64);
    
    template <endian::order r> N_bytes<r> &operator<<=(N_bytes<r>&, int64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator<<=(Z_bytes<r, c>&, int64);
    
    template <endian::order r> N_bytes<r> &operator>>=(N_bytes<r>&, int64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator>>=(Z_bytes<r, c>&, int64);
    
    template <endian::order r> N_bytes<r> &operator/=(N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> &operator/=(Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator/=(N_bytes<r>&, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator/=(Z_bytes<r, c>&, int64);
    
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number. 
namespace data::encoding::decimal {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> read(string_view s);
    
    struct string;
    
    template <endian::order r> 
    string write(const math::number::N_bytes<r> &z);
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream &o, const math::number::N_bytes<r> &n) {
        return o << write(n);
    }
    
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, math::number::complement n> 
    ptr<math::number::Z_bytes<r, n>> read(string_view s);
    
    struct string;
    
    template <endian::order r, math::number::complement n> 
    string write(const math::number::Z_bytes<r, n> &);
    
    template <endian::order r, math::number::complement n> 
    std::ostream &write(std::ostream &o, const math::number::Z_bytes<r, n> &z);
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r> 
    ptr<oriented<r, byte>> read(string_view s);
    
    template <endian::order r> 
    std::ostream &write(std::ostream &, const oriented<r, byte> &, hex::letter_case q);
    
    template <endian::order r> 
    std::string inline write(const oriented<r, byte> &z) {
        std::stringstream ss;
        write(ss, z);
        return ss.str();
    }
    
    template <endian::order r> 
    std::ostream &write(std::ostream &, const math::number::Z_bytes<r>&);
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream &o, const oriented<r, byte> &d, hex::letter_case q = hex::lower);
    
    template <endian::order r> 
    string write(const oriented<r, byte> &z, hex::letter_case q = hex::lower);
    
}

namespace data::encoding::natural {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> read(string_view s);
    
}

namespace data::encoding::integer {
    
    template <endian::order r, math::number::complement c> 
    ptr<math::number::Z_bytes<r, c>> read(string_view s);
    
    template <endian::order r, math::number::complement c> 
    std::ostream &write(std::ostream &, const math::number::Z_bytes<r, c>&);
    
}

namespace data::math::number {
    
    template <endian::order r> struct N_bytes : oriented<r, byte> {
        
        N_bytes() : oriented<r, byte>{} {}
        
        N_bytes(const uint64 x);
        
        static N_bytes read(string_view x);
        static N_bytes read(bytes_view x);
        
        operator N_bytes<endian::opposite(r)>() const;
        
        template <complement c>
        explicit operator Z_bytes<r, c>() const;
        explicit operator double() const;
        
        static N_bytes zero(size_t size = 0);
        
        explicit operator uint64() const;
        
        N_bytes trim() const;

    private:
        N_bytes(size_t size, byte fill) : oriented<r, byte>(size, fill) {}
    };
    
    template <endian::order r> struct Z_bytes<r, ones> : oriented<r, byte> {
        
        Z_bytes() : oriented<r, byte>{} {}
        
        explicit Z_bytes(int64 x);
        
        static Z_bytes read(string_view x);
        static Z_bytes read(bytes_view x);
        
        operator Z_bytes<endian::opposite(r), ones>() const;
        operator Z_bytes<r, twos>() const;
        
        explicit operator double() const;
        
        static Z_bytes zero(size_t size = 0);
        
        explicit operator int64() const;
        
        Z_bytes trim() const;
        
    private:
        Z_bytes(size_t size, byte fill) : oriented<r, byte>(size, fill) {}
    };
    
    template <endian::order r> struct Z_bytes<r, twos> : oriented<r, byte> {
        Z_bytes() : oriented<r, byte>{} {}
        
        Z_bytes(int64 x);
        
        static Z_bytes read(string_view x);
        static Z_bytes read(bytes_view x);
        
        operator Z_bytes<endian::opposite(r), twos>() const;
        operator Z_bytes<r, ones>() const;
        
        explicit operator double() const;
        
        static Z_bytes zero(size_t size = 0, bool negative = false);
        
        explicit operator int64() const;
        
        Z_bytes trim() const;
        
    private:
        Z_bytes(size_t size, byte fill) : oriented<r, byte>(size, fill) {}
    };
    
    template <endian::order r> N_bytes<r> inline trim(const N_bytes<r> &n) {
        return n.trim();
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline trim(const Z_bytes<r, c> &z) {
        return z.trim();
    }
    
    // some functions that can easily be implemented as other functions using conversions. 
    template <endian::order r, complement c> 
    std::weak_ordering inline operator<=>(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a <=> Z_bytes<r, c>(b);
    }
    
    template <endian::order r, complement c> 
    std::weak_ordering inline operator<=>(const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) <=> b;
    }
    
    template <endian::order r> Z_bytes<r, ones> inline operator~(const N_bytes<r> &b) {
        return ~Z_bytes<r, ones>(b);
    }
    
    template <endian::order r> Z_bytes<r, ones> inline operator^(const Z_bytes<r, ones> &a, const N_bytes<r> &b) {
        return a ^ Z_bytes<r, ones>(b);
    }
    
    template <endian::order r> Z_bytes<r, ones> inline operator^(const N_bytes<r> &a, const Z_bytes<r, ones> &b) {
        return Z_bytes<r, ones>(a) ^ b;
    }
    
    template <endian::order r> Z_bytes<r, ones> inline operator^(const N_bytes<r> &a, const N_bytes<r> &b) {
        return Z_bytes<r, ones>(a) ^ Z_bytes<r, ones>(b);
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator&(const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) & b;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator&(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return Z_bytes<r, c>(b) & a;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator|(const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) | b;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator|(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return Z_bytes<r, c>(b) | a;
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator+(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a + Z_bytes<r, c>(b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator-(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a - Z_bytes<r, c>(b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator-(const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        return a + (-b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator*(const Z_bytes<r, c> &a, const N_bytes<r> &b) {
        return a * Z_bytes<r, c>(b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator+(const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) + b;
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator-(const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) - b;
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator*(const N_bytes<r> &a, const Z_bytes<r, c> &b) {
        return Z_bytes<r, c>(a) * b;
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
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator+(const Z_bytes<r, c> &a, int64 b) {
        return a + Z_bytes<r, c>(b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator-(const Z_bytes<r, c> &a, int64 b) {
        return a - Z_bytes<r, c>(b);
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> inline operator*(const Z_bytes<r, c> &a, int64 b) {
        return a * Z_bytes<r, c>(b);
    }
    
    template <endian::order r> bool inline is_negative(const N_bytes<r> &) {
        return false;
    }
    
    template <endian::order r> bool inline is_positive(const N_bytes<r> &x) {
        return !is_zero(x);
    }
    
    template <endian::order r>
    N_bytes<r> inline operator++(N_bytes<r> &x, int) {
        N_bytes<r> z = x;
        ++x;
        return z;
    }
    
    template <endian::order r>
    N_bytes<r> inline operator--(N_bytes<r> &x, int) {
        N_bytes<r> z = x;
        --x;
        return z;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator+=(N_bytes<r> &x, const N_bytes<r>& n) {
        return x = x + n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator-=(N_bytes<r> &x, const N_bytes<r>& n) {
        return x = x - n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator+=(N_bytes<r> &x, uint64 n) {
        return x = x + n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator-=(N_bytes<r> &x, uint64 n) {
        return x = x - n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator*=(N_bytes<r> &x, const N_bytes<r>& n) {
        return x = x * n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator*=(N_bytes<r> &x, uint64 n) {
        return x = x * n;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator<<=(N_bytes<r> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r>
    N_bytes<r> inline &operator>>=(N_bytes<r> &x, int64 i) {
        return x = x >> i;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline operator++(Z_bytes<r, c> &x, int) {
        Z_bytes<r, c> z = x;
        ++x;
        return z;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline operator--(Z_bytes<r, c> &x, int) {
        Z_bytes<r, c> z = x;
        --x;
        return z;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator+=(Z_bytes<r, c> &x, const Z_bytes<r, c>& n) {
        return x = x + n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator-=(Z_bytes<r, c> &x, const Z_bytes<r, c>& n) {
        return x = x - n;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline &operator+=(Z_bytes<r, c> &x, int64 n) {
        return x = x + n;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline &operator-=(Z_bytes<r, c> &x, int64 n) {
        return x = x - n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator*=(Z_bytes<r, c> &x, const Z_bytes<r, c>& n) {
        return x = x * n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator*=(Z_bytes<r, c> &x, const int64 n) {
        return x = x * n;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator<<=(Z_bytes<r, c> &x, int64 i) {
        return x = x << i;
    }
    
    template <endian::order r, complement c>
    Z_bytes<r, c> inline &operator>>=(Z_bytes<r, c> &x, int64 i) {
        return x = x >> i;
    }
    
    template <endian::order r> inline Z_bytes<r, twos>::Z_bytes(int64 x) : Z_bytes{Z_bytes(Z_bytes<r, ones>{x})} {}
    
    template <endian::order r> 
    Z_bytes<r, twos> inline operator<<(const Z_bytes<r, twos> &x, int i) {
        return Z_bytes<r, twos>(Z_bytes<r, ones>(x) << i);
    }
    
    template <endian::order r> 
    Z_bytes<r, twos> inline operator>>(const Z_bytes<r, twos> &x, int i) {
        return Z_bytes<r, twos>(Z_bytes<r, ones>(x) >> i);
    }
    
    template <endian::order r> Z_bytes<r, ones> inline operator-(const Z_bytes<r, ones> &x) {
        auto z = ~x;
        return ++z;
    }
    
}

namespace data::math {
    
    template <endian::order r> 
    N_bytes<r> inline first<N_bytes<r>>::operator()() {
        return N_bytes<r>{};
    }
    
    template <endian::order r, number::complement c> 
    number::Z_bytes<r, c> inline first<number::Z_bytes<r, c>>::operator()() {
        return number::Z_bytes<r, c>{};
    }
    
    template <endian::order r> number::N_bytes<r> inline increment(const N_bytes<r> &n) {
        auto x = n;
        return ++x;
    }
    
    template <endian::order r, number::complement c> number::Z_bytes<r, c> inline increment(const number::Z_bytes<r, c> &n) {
        auto x = n;
        return ++x; 
    }
    
    template <endian::order r> number::N_bytes<r> inline decrement(const number::N_bytes<r> &n) {
        auto x = n;
        return --x;
    }
    
    template <endian::order r, number::complement c> number::Z_bytes<r, c> inline decrement(const number::Z_bytes<r, c> &n) {
        auto x = n;
        return --x;
    }
    
    template <endian::order r> N_bytes<r> inline abs<N_bytes<r>>::operator()(const N_bytes<r> &x) {
        return x;
    }
    
    template <endian::order r> number::Z_bytes<r, number::twos> inline 
    abs<number::Z_bytes<r, number::twos>>::operator()(const number::Z_bytes<r, number::twos> &x) {
        return number::is_negative(x) ? -x : x;
    }
    
    template <endian::order r> N_bytes<r> 
    abs<Z_bytes<r>>::operator()(const Z_bytes<r> &x) {
        if (number::is_zero(x)) return number::N_bytes<r>{};
        Z_bytes<r> z = number::is_negative(x) ? -x : x;
        auto n = number::N_bytes<r>::zero(z.size());
        std::copy(z.begin(), z.end(), n.begin());
        return n;
    }
    
    template <endian::order r> N_bytes<r> inline quadrance<N_bytes<r>>::operator()(const N_bytes<r> &x) {
        return x * x;
    }
    
    template <endian::order r, number::complement c> auto inline 
    quadrance<number::Z_bytes<r, c>>::operator()(const number::Z_bytes<r, c> &x) -> decltype(data::abs(x)) {
        return data::abs(x) * data::abs(x);
    }
    
    template <endian::order r> N_bytes<r> inline next(const N_bytes<r> &n) {
        return increment(n);
    }
    
    template <endian::order r, number::complement c> number::Z_bytes<r, c> inline next(const number::Z_bytes<r, c> &z) {
        return number::is_positive(z) ? -z : -z - 1;
    }
    
}

// finally come functions that can be implemented in terms of the low
// level functions in arithmetic. 
namespace data::math::number {
    
    template <endian::order r> bool inline is_negative(const Z_bytes<r, ones> &x) {
        return arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_negative(const Z_bytes<r, twos> &x) {
        return !is_zero(x) && arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_positive(const Z_bytes<r, ones> &x) {
        return !is_zero(x) && !arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_positive(const Z_bytes<r, twos> &x) {
        return !is_zero(x) && !arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_zero(const N_bytes<r> &x) {
        return arithmetic::ones_is_zero(x.words());
    }
    
    template <endian::order r> bool inline is_zero(const Z_bytes<r, ones> &x) {
        return arithmetic::ones_is_zero(x.words());
    }
    
    template <endian::order r> bool inline is_zero(const Z_bytes<r, twos> &x) {
        return arithmetic::twos_is_zero(x.words());
    }
    
    template <endian::order r> bool inline is_positive_zero(const Z_bytes<r, twos> &x) {
        return is_zero(x) && !arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_negative_zero(const Z_bytes<r, twos> &x) {
        return is_zero(x) && arithmetic::sign_bit_set(x.words());
    }
    
    template <endian::order r> bool inline is_minimal(const N_bytes<r> &x) {
        return arithmetic::N_is_minimal(x.words());
    }
    
    template <endian::order r> bool inline is_minimal(const Z_bytes<r, ones> &x) {
        return arithmetic::ones_is_minimal(x.words());
    }
    
    template <endian::order r> bool inline is_minimal(const Z_bytes<r, twos> &x) {
        return arithmetic::twos_is_minimal(x.words());
    }
    
    template <endian::order r> size_t inline minimal_size(const N_bytes<r> &x) {
        return arithmetic::N_minimal_size(x.words());
    }
    
    template <endian::order r> size_t inline minimal_size(const Z_bytes<r, ones> &x) {
        return arithmetic::ones_minimal_size(x.words());
    }
    
    template <endian::order r> size_t inline minimal_size(const Z_bytes<r, twos> &x) {
        return arithmetic::twos_minimal_size(x.words());
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
    
    template <endian::order r> Z_bytes<r, ones> extend(const Z_bytes<r, ones> &x, size_t size) {
        if (size < x.size()) {
            size_t min_size = minimal_size(x); 
            if (size < min_size) throw std::invalid_argument{"cannot extend smaller than minimal size"};
            return extend(trim(x), size);
        }
        
        if (size == x.size()) return x;
        
        Z_bytes<r, ones> z;
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
    
    template <endian::order r> Z_bytes<r, twos> extend(const Z_bytes<r, twos> &x, size_t size) {
        if (size < x.size()) {
            size_t min_size = minimal_size(x); 
            if (size < min_size) throw std::invalid_argument{"cannot extend smaller than minimal size"};
            return extend(trim(x), size);
        }
        
        if (size == x.size()) return x;
        
        auto z = Z_bytes<r, twos>::zero(size);
        
        if (x.size() == 0) return z;
        
        auto xw = x.words();
        auto zw = z.words();
        std::copy(xw.begin(), xw.end() - 1, zw.begin());
        byte last = xw[-1];
        
        zw[-1] = last & 0x80 ? 0x80 : 0x00;
        zw[x.size() - 1] = last & 0x7f;
        return z;
    }
    
    template <endian::order r> 
    bool inline operator==(const N_bytes<r> &a, const N_bytes<r> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, complement c> 
    bool inline operator==(const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const N_bytes<r> &a, const N_bytes<r> &b) {
        return arithmetic::N_compare(a.words(), b.words());
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const Z_bytes<r, ones> &a, const Z_bytes<r, ones> &b) {
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
    std::weak_ordering inline operator<=>(const Z_bytes<r, twos> &a, const Z_bytes<r, twos> &b) {
        
        sign na = data::sign(a);
        sign nb = data::sign(b);
        
        if (na == zero) switch (nb) {
            case positive: return std::weak_ordering::less;
            case negative: return std::weak_ordering::greater;
            default: return std::weak_ordering::equivalent;
        }
        
        if (nb == zero) return na == positive ? std::weak_ordering::greater : std::weak_ordering::less;
        
        if (na == positive && nb == positive) return arithmetic::N_compare(a.words(), b.words());
        
        if (na == negative && nb == negative) {
            auto ya = -b;
            auto yb = -a;
            return arithmetic::N_compare(ya.words(), yb.words());
        }
        
        return na == negative ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <endian::order r> 
    std::weak_ordering inline operator<=>(const N_bytes<r> &a, int64 i) {
        if (i < 0) return std::weak_ordering::greater;
        return arithmetic::N_compare(a.words(), endian::arithmetic<r, true, 8>{i}.words());
    }
    
    template <endian::order r, complement c> 
    std::weak_ordering inline operator<=>(const Z_bytes<r, c> &a, int64 i) {
        return a <=> Z_bytes<r, c>{i};
    }
    
    template <endian::order r> 
    bool operator==(const N_bytes<r> &x, int64 i) {
        if (i < 0) return false;
        return arithmetic::N_compare(x.words(), endian::arithmetic<r, true, 8>{i}.words()) == 0;
    }
    
    template <endian::order r, complement c> 
    bool operator==(const Z_bytes<r, c> &x, int64 i) {
        return (x <=> Z_bytes<r, c>{i}) == 0;
    }
    
    template <endian::order r> Z_bytes<r, ones> inline operator-(const N_bytes<r> &x) {
        auto z = Z_bytes<r, ones>(x);
        arithmetic::negate_ones(z.words());
        return z;
    }
    
    template <endian::order r> N_bytes<r> N_bytes<r>::read(bytes_view b) {
        auto x = N_bytes<r>{};
        x.resize(b.size());
        std::copy(b.begin(), b.end(), x.begin());
        return x;
    }
    
    template <endian::order r> Z_bytes<r, ones> Z_bytes<r, ones>::read(bytes_view b) {
        auto x = Z_bytes<r, ones>{};
        x.resize(b.size());
        std::copy(b.begin(), b.end(), x.begin());
        return x;
    }
    
    template <endian::order r> Z_bytes<r, twos> Z_bytes<r, twos>::read(bytes_view b) {
        auto x = Z_bytes<r, ones>{};
        x.resize(b.size());
        std::copy(b.begin(), b.end(), x.begin());
        return x;
    }
    
    template <endian::order r> 
    N_bytes<r> inline N_bytes<r>::zero(size_t size) {
        return N_bytes{size, 0x00};
    }
    
    template <endian::order r> 
    Z_bytes<r, ones> inline Z_bytes<r, ones>::zero(size_t size) {
        return Z_bytes{size, 0x00};
    }
    
    template <endian::order r> 
    Z_bytes<r, twos> Z_bytes<r, twos>::zero(size_t size, bool negative) {
        if (size == 0 && negative) throw std::invalid_argument{"invalid size for negative zero"};
        auto z = Z_bytes(size, 0x00);
        if (negative) z.words()[-1] = 0x80;
        return z;
    }
    
    template <endian::order r> Z_bytes<r, twos> inline operator-(const Z_bytes<r, twos> &x) {
        if (x.size() == 0) return x;
        auto z = x;
        arithmetic::flip_sign_bit(z.words());
        return z;
    }
    
    template <endian::order r> Z_bytes<r, ones> operator~(const Z_bytes<r, ones> &x) {
        auto z = extend(x, x.size() + 1);
        arithmetic::bit_negate<byte>(z.words().end(), z.words().begin(), z.words().begin());
        return trim(z);
    }
    
    template <endian::order r> N_bytes<r> operator&(const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a.size() < b.size()) return b & a;
        auto bt = extend(b, a.size());
        auto x = N_bytes<r>::zero(a.size());
        arithmetic::bit_and<byte>(x.end(), x.begin(), a.begin(), const_cast<const N_bytes<r>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> operator&(const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        if (a.size() < b.size()) return b & a;
        auto bt = extend(b, a.size());
        auto x = Z_bytes<r, c>::zero(a.size());
        arithmetic::bit_and<byte>(x.end(), x.begin(), a.begin(), const_cast<const Z_bytes<r, c>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r> N_bytes<r> operator|(const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a.size() < b.size()) return b | a;
        auto bt = extend(b, a.size());
        auto x = N_bytes<r>::zero(a.size());
        arithmetic::bit_or<byte>(x.end(), x.begin(), a.begin(), const_cast<const N_bytes<r>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> operator|(const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        if (a.size() < b.size()) return b | a;
        auto bt = extend(b, a.size());
        auto x = Z_bytes<r, c>::zero(a.size());
        arithmetic::bit_or<byte>(x.end(), x.begin(), a.begin(), const_cast<const Z_bytes<r, c>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r> Z_bytes<r, ones> operator^(const Z_bytes<r, ones> &a, const Z_bytes<r, ones> &b) {
        if (a.size() < b.size()) return b ^ a;
        auto bt = extend(b, a.size());
        auto x = Z_bytes<r, ones>::zero(a.size());
        arithmetic::bit_xor<byte>(x.end(), x.begin(), a.begin(), const_cast<const Z_bytes<r, ones>&>(bt).begin());
        return x.trim();
    }
    
    template <endian::order r> 
    Z_bytes<r, ones> operator+(const Z_bytes<r, ones> &a, const Z_bytes<r, ones> &b) {
        bool an = is_negative(a);
        bool bn = is_negative(b);
        auto ax = data::abs(a);
        auto bx = data::abs(b);
        if (!an && !bn) return Z_bytes<r, ones>(ax + bx);
        if (an && bn) return -(ax + bx);
        bool gr = ax > bx;
        return an ? (gr ? -(ax - bx) : Z_bytes<r, ones>(bx - ax)) : gr ? Z_bytes<r, ones>(ax - bx) : -(bx - ax);
    }
    
    template <endian::order r> 
    Z_bytes<r, twos> operator+(const Z_bytes<r, twos> &a, const Z_bytes<r, twos> &b) {
        throw method::unimplemented{"Z_bytes + Z_bytes"};
    }
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator*(const Z_bytes<r, c> &a, const Z_bytes<r, c> &b) {
        bool an = is_negative(a);
        bool bn = is_negative(b);
        if ((an && bn) || (!an && !bn)) return Z_bytes<r, c>(data::abs(a) * data::abs(b));
        return -(data::abs(a) * data::abs(b));
    }
    
    template <endian::order r> template<complement c> N_bytes<r>::operator Z_bytes<r, c>() const {
        Z_bytes<r, c> n = Z_bytes<r, c>::zero(this->size() + 1);
        std::copy(this->words().begin(), this->words().end(), n.words().begin());
        return n.trim();
    }
    
    template <endian::order r> inline N_bytes<r>::N_bytes(const uint64 x) : oriented<r, byte>{} {
        this->resize(8);
        endian::arithmetic<r, false, 8> xx{x};
        std::copy(xx.begin(), xx.end(), this->begin());
        *this = this->trim();
    }
    
    template <endian::order r> inline Z_bytes<r, ones>::Z_bytes(int64 x) : oriented<r, byte>{} {
        this->resize(8);
        endian::arithmetic<r, true, 8> n{x};
        std::copy(n.begin(), n.end(), this->begin());
        *this = this->trim();
    }
    
    template <endian::order r> N_bytes<r> N_bytes<r>::trim() const {
        size_t size = minimal_size(*this);
        if (size == this->size()) return *this;
        auto n = N_bytes<r>::zero(size);
        auto w = this->words();
        std::copy(w.begin(), w.begin() + size, n.words().begin());
        return n;
    }
    
    template <endian::order r> Z_bytes<r, ones> Z_bytes<r, ones>::trim() const {
        size_t size = minimal_size(*this);
        if (size == this->size()) return *this;
        auto n = Z_bytes<r, ones>::zero(size);
        auto w = this->words();
        std::copy(w.begin(), w.begin() + size, n.words().begin());
        return n;
    }
    
    template <endian::order r> Z_bytes<r, twos> Z_bytes<r, twos>::trim() const {
        size_t size = minimal_size(*this);
        if (size == this->size()) return *this;
        auto n = Z_bytes<r, twos>::zero(size);
        auto w = this->words();
        std::copy(w.begin(), w.begin() + size, n.words().begin());
        if (size != 0) n.words()[-1] |= (this->words()[-1] & 0x80);
        return n;
    }
    
    template <endian::order r> N_bytes<r>::operator uint64() const {
        if (*this > N_bytes{std::numeric_limits<uint64>::max()}) throw std::invalid_argument{"value too big"};
        endian::arithmetic<endian::little, false, 8> xx{0};
        std::copy(this->words().begin(), this->words().begin() + std::min(static_cast<size_t>(8), this->size()), xx.begin());
        return uint64(xx);
    } 
    
    template <endian::order r> Z_bytes<r, ones>::operator int64() const {
        if (*this > Z_bytes{std::numeric_limits<int64>::max()}) throw std::invalid_argument{"value too big"}; 
        if (*this < Z_bytes{std::numeric_limits<int64>::min()}) throw std::invalid_argument{"value too small"};
        endian::arithmetic<endian::little, false, 8> xx{0};
        std::copy(this->words().begin(), this->words().begin() + std::min(static_cast<size_t>(8), this->size()), xx.begin());
        return int64(xx);
    } 
    
    template <endian::order r> inline Z_bytes<r, twos>::operator int64() const {
        return int64(Z_bytes<r, ones>(*this));
    } 
    
    template <endian::order r> 
    Z_bytes<r, ones>::operator Z_bytes<endian::opposite(r), ones>() const {
        Z_bytes<endian::opposite(r), ones> n;
        n.resize(this->size());
        std::copy(this->words().begin(), this->words().end(), n.begin());
        return n;
    }
    
    template <endian::order r> 
    inline Z_bytes<r, ones>::operator Z_bytes<r, twos>() const {
        if (is_negative(*this)) return -Z_bytes<r, twos>(data::abs(*this));
        return Z_bytes<r, twos>(data::abs(*this));
    }
    
    template <endian::order r> 
    Z_bytes<r, twos>::operator Z_bytes<endian::opposite(r), twos>() const {
        Z_bytes<endian::opposite(r), twos> n;
        n.resize(this->size());
        std::copy(this->words().begin(), this->words().end(), n.begin());
        return n;
    }
    
    template <endian::order r> 
    inline Z_bytes<r, twos>::operator Z_bytes<r, ones>() const {
        if (is_negative(*this)) return -Z_bytes<r, ones>(data::abs(*this));
        auto z = Z_bytes<r, ones>::zero(this->size());
        std::copy(this->begin(), this->end(), z.begin());
        return z;
    }
    
    template <endian::order r> N_bytes<r> &operator++(N_bytes<r> &x) {
        x = trim(x);
        auto remainder = arithmetic::plus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        
        if (remainder != 0) {
            x = extend(x, x.size() + 1);
            x.words()[-1] = remainder;
        }
        
        return x;
    }
    
    template <endian::order r> N_bytes<r> inline &operator--(N_bytes<r> &x) {
        if (!is_zero(x)) arithmetic::minus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        return x = trim(x);
    }
    
    template <endian::order r> Z_bytes<r, ones> &operator++(Z_bytes<r, ones> &x) {
        bool neg = is_negative(x);
        
        auto remainder = arithmetic::plus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        
        if (!neg && (remainder != 0 || is_negative(x))) {
            x = extend(x, x.size() + 1);
            x.words()[-1] = remainder;
        }
        
        return x = trim(x);
    }
    
    template <endian::order r> Z_bytes<r, ones> &operator--(Z_bytes<r, ones> &x) {
        auto remainder = arithmetic::minus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        
        if (remainder != 0) {
            x = extend(x, x.size() + 1);
            x.words()[-1] = 0x00 - remainder;
        }
        
        return x = trim(x);
    }
    
    template <endian::order r> Z_bytes<r, twos> &operator++(Z_bytes<r, twos> &x) {
        if (is_zero(x)) return x = Z_bytes<r, twos>{1};
        if (arithmetic::sign_bit_set(x.words())) return x = -decrement(-x);
        
        auto remainder = arithmetic::plus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        if (remainder != 0) {
            x = extend(x, x.size() + 1);
            x.words()[-1] = remainder;
        }
        
        return x = trim(x);
    }
    
    template <endian::order r> Z_bytes<r, twos> &operator--(Z_bytes<r, twos> &x) {
        if (is_zero(x)) return x = Z_bytes<r, twos>{-1};
        if (arithmetic::sign_bit_set(x.words())) return x = -increment(-x);
        
        arithmetic::minus<byte>(x.words().end(), x.words().begin(), 1, x.words().begin());
        return x = trim(x);
    }
    
    namespace {
    
        template <endian::order r> 
        N_bytes<r> shift_left(const N_bytes<r> &x, byte bytes, byte bits) {
            throw 0;
        }
        
        template <endian::order r> 
        N_bytes<r> shift_right(const N_bytes<r> &x, byte bytes, byte bits) {
            throw 0;
        }
    
        template <endian::order r> 
        Z_bytes<r, ones> shift_left(const Z_bytes<r, ones> &x, byte bytes, byte bits) {
            throw 0;
        }
        
        template <endian::order r> 
        Z_bytes<r, ones> shift_right(const Z_bytes<r, ones> &x, byte bytes, byte bits) {
            throw 0;
        }
        
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator<<(const N_bytes<r> &x, int i) {
        return trim(i < 0 ? shift_right(trim(x), -i / 8, -i % 8) : shift_left(trim(x), (i + 7) / 8, i % 8));
    }
    
    template <endian::order r> 
    N_bytes<r> inline operator>>(const N_bytes<r> &x, int i) {
        return trim(i < 0 ? shift_left(trim(x), (-i + 7) / 8, -i % 8) : shift_right(trim(x), i / 8, i % 8));
    }
    
    template <endian::order r> 
    Z_bytes<r, ones> inline operator<<(const Z_bytes<r, ones> &x, int i) {
        return trim(i < 0 ? shift_right(trim(x), -i / 8, -i % 8) : shift_left(trim(x), (i + 7) / 8, i % 8));
    }
    
    template <endian::order r> 
    Z_bytes<r, ones> inline operator>>(const Z_bytes<r, ones> &x, int i) {
        return trim(i < 0 ? shift_left(trim(x), (-i + 7) / 8, -i % 8) : shift_right(trim(x), i / 8, i % 8));
    }
    
    template <endian::order r> N_bytes<r>::operator N_bytes<endian::opposite(r)>() const {
        N_bytes<endian::opposite(r)> z;
        z.resize(this->size());
        std::copy(this->rbegin(), this->rend(), z.begin());
        return z;
    }
    
    template <endian::order r> 
    N_bytes<r> operator+(const N_bytes<r> &a, const N_bytes<r> &b) {
        throw method::unimplemented{"N_bytes +"};
    }
    
    template <endian::order r> 
    N_bytes<r> operator-(const N_bytes<r> &a, const N_bytes<r> &b) {
        if (a > b) return N_bytes<r>{0};
        throw method::unimplemented{"N_bytes -"};
    }
    
    template <endian::order r> 
    N_bytes<r> operator*(const N_bytes<r> &a, const N_bytes<r> &b) {
        throw method::unimplemented{"N_bytes *"};
    }
}

namespace data {
    
    template <endian::order r> math::sign inline sign(const math::N_bytes<r> &x) {
        return math::arithmetic::N_sign(x.words());
    }
    
    template <endian::order r, math::number::complement c> math::sign inline sign(const math::number::Z_bytes<r, c> &x) {
        return math::number::is_zero(x) ? math::zero : math::arithmetic::sign_bit_set(x.words()) ? math::negative : math::positive;
    }
    
}

#endif
