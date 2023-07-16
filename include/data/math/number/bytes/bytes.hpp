// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_BYTES
#define DATA_MATH_NUMBER_BYTES_BYTES

#include <data/math/number/gmp/mpz.hpp>
#include <data/encoding/hex.hpp>

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
    bool operator == (const N_bytes<r> &a, const N_bytes<r> &b);
    
    template <endian::order r, complement c> 
    bool operator == (const Z_bytes<r, c> &a, const Z_bytes<r, c> &b);
    
    template <endian::order r> 
    std::weak_ordering operator <=> (const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    std::weak_ordering operator <=> (const Z_bytes<r, complement::ones>&, const Z_bytes<r, complement::ones>&);
    
    template <endian::order r> 
    std::weak_ordering operator <=> (const Z_bytes<r, complement::twos>&, const Z_bytes<r, complement::twos>&);
    
    template <endian::order r, complement c> 
    std::weak_ordering operator <=> (const Z_bytes<r, c>&, const N_bytes<r>&);
    
    template <endian::order r> 
    bool operator == (const N_bytes<r>&, uint64);
    
    template <endian::order r, complement c> 
    bool operator == (const Z_bytes<r, c>&, int64);
    
    template <endian::order r> 
    std::weak_ordering operator <=> (const N_bytes<r>&, uint64);
    
    template <endian::order r, complement c> 
    std::weak_ordering operator <=> (const Z_bytes<r, c>&, int64);
    
    template <endian::order r> 
    bool operator == (const N_bytes<r>&, N);
    
    template <endian::order r, complement c> 
    bool operator == (const Z_bytes<r, c>&, Z);
    
    // bit negate
    template <endian::order r> Z_bytes<r, complement::ones> operator ~ (const N_bytes<r> &);
    template <endian::order r> Z_bytes<r, complement::ones> operator ~ (const Z_bytes<r, complement::ones> &);
    
    // bit xor
    template <endian::order r> Z_bytes<r, complement::ones> operator ^ (const N_bytes<r> &, const N_bytes<r> &);
    
    template <endian::order r> 
    Z_bytes<r, complement::ones> operator ^
        (const Z_bytes<r, complement::ones> &, const Z_bytes<r, complement::ones> &);
    
    template <endian::order r> Z_bytes<r, complement::ones> operator ^ (const Z_bytes<r, complement::ones>&, const N_bytes<r> &);
    template <endian::order r> Z_bytes<r, complement::ones> operator ^ (const N_bytes<r>&, const Z_bytes<r, complement::ones> &);
    
    // bit and 
    template <endian::order r> N_bytes<r> operator & (const N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const Z_bytes<r, c> &, const Z_bytes<r, c> &);
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const N_bytes<r> &, const Z_bytes<r, c> &);
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const Z_bytes<r, c> &, const N_bytes<r> &);
    
    template <endian::order r> N_bytes<r> operator & (const N_bytes<r> &, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const Z_bytes<r, c> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const N_bytes<r>&, int64);
    template <endian::order r, complement c> Z_bytes<r, c> operator & (const Z_bytes<r, c>&, uint64);
    
    // bit or 
    template <endian::order r> N_bytes<r> operator | (const N_bytes<r> &, const N_bytes<r>&);
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const Z_bytes<r, c> &, const Z_bytes<r, c> &);
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const N_bytes<r> &, const Z_bytes<r, c> &);
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const Z_bytes<r, c> &, const N_bytes<r> &);
    
    template <endian::order r> N_bytes<r> operator | (const N_bytes<r> &, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const Z_bytes<r, c> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const N_bytes<r> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> operator | (const Z_bytes<r, c> &, uint64);
    
    // negation
    template <endian::order r> Z_bytes<r, complement::ones> operator - (const N_bytes<r> &);
    template <endian::order r> Z_bytes<r, complement::ones> operator - (const Z_bytes<r, complement::ones> &);
    template <endian::order r> Z_bytes<r, complement::twos> operator - (const Z_bytes<r, complement::twos> &);
    
    template <endian::order r> 
    N_bytes<r> operator + (const N_bytes<r> &, const N_bytes<r> &);
    
    template <endian::order r>
    N_bytes<r> operator - (const N_bytes<r> &, const N_bytes<r> &);
    
    template <endian::order r> 
    N_bytes<r> operator * (const N_bytes<r> &, const N_bytes<r> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator + (const Z_bytes<r, c> &, const Z_bytes<r, c> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator - (const Z_bytes<r, c> &, const Z_bytes<r, c> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator * (const Z_bytes<r, c> &, const Z_bytes<r, c> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator + (const Z_bytes<r, c> &, const N_bytes<r> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator - (const Z_bytes<r, c> &, const N_bytes<r> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator * (const Z_bytes<r, c> &, const N_bytes<r> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator + (const N_bytes<r> &, const Z_bytes<r, c> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator - (const N_bytes<r> &, const Z_bytes<r, c> &);
    
    template <endian::order r, complement c> 
    Z_bytes<r, c> operator * (const N_bytes<r> &, const Z_bytes<r, c> &);
    
    template <endian::order r> N_bytes<r> operator + (const N_bytes<r> &, uint64);
    template <endian::order r> N_bytes<r> operator - (const N_bytes<r> &, uint64);
    template <endian::order r> N_bytes<r> operator * (const N_bytes<r> &, uint64);
    
    template <endian::order r, complement c> Z_bytes<r, c> operator + (const Z_bytes<r, c> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> operator - (const Z_bytes<r, c> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> operator * (const Z_bytes<r, c> &, int64);
    
    template <endian::order r> N_bytes<r> operator << (const N_bytes<r> &, int);
    template <endian::order r> N_bytes<r> operator >> (const N_bytes<r> &, int);
    
    template <endian::order r, complement c> Z_bytes<r, c> operator << (const Z_bytes<r, c> &, int);
    template <endian::order r, complement c> Z_bytes<r, c> operator >> (const Z_bytes<r, c> &, int);
    
    template <endian::order r> bool is_minimal (const N_bytes<r> &);
    template <endian::order r> bool is_minimal (const Z_bytes<r, complement::ones> &);
    template <endian::order r> bool is_minimal (const Z_bytes<r, complement::twos> &);
    
    template <endian::order r> size_t minimal_size (const N_bytes<r> &);
    template <endian::order r> size_t minimal_size (const Z_bytes<r, complement::ones> &);
    template <endian::order r> size_t minimal_size (const Z_bytes<r, complement::twos> &);
    
    template <endian::order r> N_bytes<r> extend (const N_bytes<r> &, size_t);
    template <endian::order r> Z_bytes<r, complement::ones> extend (const Z_bytes<r, complement::ones> &, size_t);
    template <endian::order r> Z_bytes<r, complement::twos> extend (const Z_bytes<r, complement::twos> &, size_t);
    
    template <endian::order r> N_bytes<r> trim (const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> trim (const Z_bytes<r, c> &);
    
}

namespace data::math {
    
    template <endian::order r> using N_bytes = number::N_bytes<r>;
    template <endian::order r> using Z_bytes = number::Z_bytes<r, number::complement::ones>;
    template <endian::order r> using Z_bytes_twos = number::Z_bytes<r, number::complement::twos>;
    
    template <endian::order r> bool is_zero (const N_bytes<r> &);
    template <endian::order r> bool is_zero (const Z_bytes<r> &);
    template <endian::order r> bool is_zero (const Z_bytes_twos<r> &);
    
    template <endian::order r> bool is_negative (const N_bytes<r> &);
    template <endian::order r> bool is_negative (const Z_bytes<r> &x);
    template <endian::order r> bool is_negative (const Z_bytes_twos<r> &x);
    
    template <endian::order r> bool is_positive (const N_bytes<r> &x);
    template <endian::order r> bool is_positive (const Z_bytes<r> &);
    template <endian::order r> bool is_positive (const Z_bytes_twos<r> &);
    
    template <endian::order r> bool is_positive_zero (const Z_bytes<r> &);
    template <endian::order r> bool is_negative_zero (const Z_bytes_twos<r> &);
    
    template <endian::order r> struct abs<N_bytes<r>> {
        N_bytes<r> operator () (const N_bytes<r> &);
    };
    
    template <endian::order r> struct abs<number::Z_bytes<r, number::complement::ones>> {
        N_bytes<r> operator () (const number::Z_bytes<r, number::complement::ones> &);
    };
    
    template <endian::order r> struct abs<number::Z_bytes<r, number::complement::twos>> {
        number::Z_bytes<r, number::complement::twos> operator () (const number::Z_bytes<r, number::complement::twos> &);
    };
    
    template <endian::order r> struct quadrance<N_bytes<r>> { 
        N_bytes<r> operator () (const N_bytes<r> &);
    };
    
    template <endian::order r, number::complement c> struct quadrance<number::Z_bytes<r, c>> { 
        auto operator () (const number::Z_bytes<r, c> &x) -> decltype (data::abs (x));
    };
    
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
        number::Z_bytes<r, c> operator () (const number::Z_bytes<r, c> &a, const number::Z_bytes<r, c> &b) {
            return b - a;
        }
    };
    
}

namespace data {
    
    template <endian::order r> math::sign sign (const math::N_bytes<r> &);
    template <endian::order r, math::number::complement c> math::sign sign (const math::number::Z_bytes<r, c> &);
    
    template <endian::order r> math::number::N_bytes<r> increment (const math::number::N_bytes<r> &);
    template <endian::order r> math::number::N_bytes<r> decrement (const math::number::N_bytes<r> &);
    
    template <endian::order r, math::number::complement c> math::number::Z_bytes<r, c> increment (const math::number::Z_bytes<r, c> &);
    template <endian::order r, math::number::complement c> math::number::Z_bytes<r, c> decrement (const math::number::Z_bytes<r, c> &);
    
}

namespace data::math::number {
    
    template <endian::order r> N_bytes<r> operator / (const N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> operator / (const Z_bytes<r, c>&, const Z_bytes<r, c> &);
    
    template <endian::order r> N_bytes<r> operator / (const N_bytes<r> &, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> operator / (const Z_bytes<r, c> &, int64);
    
    template <endian::order r> N_bytes<r> operator % (const N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> N_bytes<r> operator % (const Z_bytes<r, c> &, const N_bytes<r> &);
    template <endian::order r> 
    Z_bytes<r, complement::twos> operator % (
        const Z_bytes<r, complement::twos> &, const Z_bytes<r, complement::twos> &);
    
    template <endian::order r> uint64 operator % (const N_bytes<r> &, uint64);
    template <endian::order r, complement c> uint64 operator % (const Z_bytes<r, c> &, uint64);
    
    // pre-increment and decreement
    template <endian::order r> N_bytes<r> &operator ++ (N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> &operator ++ (Z_bytes<r, c> &);
    
    template <endian::order r> N_bytes<r> &operator -- (N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> &operator -- (Z_bytes<r, c> &);
    
    // post-increment and decrement
    template <endian::order r> N_bytes<r> operator ++ (N_bytes<r> &, int);
    template <endian::order r, complement c> Z_bytes<r, c> operator++(Z_bytes<r, c> &, int);
    
    template <endian::order r> N_bytes<r> operator -- (N_bytes<r> &, int);
    template <endian::order r, complement c> Z_bytes<r, c> operator -- (Z_bytes<r, c> &, int);
    
    template <endian::order r> N_bytes<r> &operator += (N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> &operator += (Z_bytes<r, c> &, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator -= (N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> &operator -= (Z_bytes<r, c>&, const Z_bytes<r, c>&);
    
    template <endian::order r> N_bytes<r> &operator += (N_bytes<r> &, uint64);
    template <endian::order r> N_bytes<r> &operator -= (N_bytes<r> &, uint64);
    
    template <endian::order r, complement c> Z_bytes<r, c> &operator += (Z_bytes<r, c> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator -= (Z_bytes<r, c> &, int64);
    
    template <endian::order r> N_bytes<r> &operator *= (N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> &operator *= (Z_bytes<r, c> &, const Z_bytes<r, c> &);
    
    template <endian::order r> N_bytes<r> &operator *= (N_bytes<r> &, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator *= (Z_bytes<r, c> &, int64);
    
    template <endian::order r> N_bytes<r> &operator <<= (N_bytes<r> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator <<= (Z_bytes<r, c> &, int64);
    
    template <endian::order r> N_bytes<r> &operator >>= (N_bytes<r> &, int64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator >>= (Z_bytes<r, c> &, int64);
    
    template <endian::order r> N_bytes<r> &operator /= (N_bytes<r> &, const N_bytes<r> &);
    template <endian::order r, complement c> Z_bytes<r, c> &operator /= (Z_bytes<r, c> &, const Z_bytes<r, c> &);
    
    template <endian::order r> N_bytes<r> &operator /= (N_bytes<r> &, uint64);
    template <endian::order r, complement c> Z_bytes<r, c> &operator /= (Z_bytes<r, c> &, int64);
    
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number. 
namespace data::encoding::decimal {
    
    template <endian::order r> 
    maybe<math::number::N_bytes<r>> read (string_view);
    
    struct string;
    
    template <endian::order r> 
    string write (const math::number::N_bytes<r> &);
    
    template <endian::order r> 
    std::ostream inline &write (std::ostream &o, const math::number::N_bytes<r> &n) {
        return o << write (n);
    }
    
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, math::number::complement n> 
    maybe<math::number::Z_bytes<r, n>> read (string_view);
    
    struct string;
    
    template <endian::order r, math::number::complement n> 
    string write (const math::number::Z_bytes<r, n> &);
    
    template <endian::order r, math::number::complement n> 
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, n> &);
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r> 
    maybe<oriented<r, byte>> read (string_view);
    
    template <endian::order r> 
    std::ostream &write (std::ostream &, const oriented<r, byte> &, hex_case q = hex_case::lower);
    
    template <hex_case zz> struct string;
    
    template <hex_case zz, endian::order r> 
    string<zz> write (const oriented<r, byte> &);
    
}

namespace data::encoding::natural {
    
    template <endian::order r> 
    maybe<math::number::N_bytes<r>> read (string_view s);
    
}

namespace data::encoding::integer {
    
    template <endian::order r, math::number::complement c> 
    maybe<math::number::Z_bytes<r, c>> read (string_view s);
    
    template <endian::order r, math::number::complement c> 
    std::ostream &write (std::ostream &, const math::number::Z_bytes<r, c> &);
    
}

#endif
