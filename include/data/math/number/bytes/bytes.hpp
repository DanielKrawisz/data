// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_BYTES
#define DATA_MATH_NUMBER_BYTES_BYTES

#include <data/math/division.hpp>
#include <data/math/abs.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>
#include <data/math/arithmetic.hpp>
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
    template <endian::order> struct Z_bytes;
    
    template <endian::order r> 
    bool operator==(const N_bytes<r> &a, const N_bytes<r> &b);
    
    template <endian::order r> 
    bool operator==(const Z_bytes<r> &a, const Z_bytes<r> &b);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const Z_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    bool operator==(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    bool operator==(const Z_bytes<r>&, int64);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    std::weak_ordering operator<=>(const Z_bytes<r>&, int64);
    
    // bit negate
    template <endian::order r> Z_bytes<r> operator~(const N_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator~(const Z_bytes<r>&);
    
    // bit xor
    template <endian::order r> Z_bytes<r> operator^(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator^(const Z_bytes<r>&, const Z_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator^(const Z_bytes<r>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator^(const N_bytes<r>&, const Z_bytes<r>&);
    
    // bit and 
    template <endian::order r> N_bytes<r> operator&(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator&(const Z_bytes<r>&, const Z_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator&(const N_bytes<r>&, const Z_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator&(const Z_bytes<r>&, const N_bytes<r>&);
    
    // bit or 
    template <endian::order r> N_bytes<r> operator|(const N_bytes<r>&, const N_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator|(const Z_bytes<r>&, const Z_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator|(const N_bytes<r>&, const Z_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator|(const Z_bytes<r>&, const N_bytes<r>&);
    
    // negation
    template <endian::order r> Z_bytes<r> operator-(const N_bytes<r>&);
    template <endian::order r> Z_bytes<r> operator-(const Z_bytes<r>&);
    
    template <endian::order r> 
    N_bytes<r> operator+(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    N_bytes<r> operator-(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    N_bytes<r> operator*(const N_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator+(const Z_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator-(const Z_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator*(const Z_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator+(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator-(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator*(const Z_bytes<r>&, const N_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator+(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator-(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    Z_bytes<r> operator*(const N_bytes<r>&, const Z_bytes<r>&);
    
    template <endian::order r> 
    N_bytes<r> operator+(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    N_bytes<r> operator-(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    N_bytes<r> operator*(const N_bytes<r>&, uint64);
    
    template <endian::order r> 
    Z_bytes<r> operator+(const Z_bytes<r>&, int64);
    
    template <endian::order r> 
    Z_bytes<r> operator-(const Z_bytes<r>&, int64);
    
    template <endian::order r> 
    Z_bytes<r> operator*(const Z_bytes<r>&, int64);
    
    template <endian::order r> 
    N_bytes<r> operator<<(const N_bytes<r>&, int);
    
    template <endian::order r> 
    N_bytes<r> operator>>(const N_bytes<r>&, int);
    
    template <endian::order r> 
    Z_bytes<r> operator<<(const Z_bytes<r>&, int);
    
    template <endian::order r> 
    Z_bytes<r> operator>>(const Z_bytes<r>&, int);
    
    template <endian::order r> bool is_zero(const N_bytes<r> &);
    template <endian::order r> bool is_zero(const Z_bytes<r> &);
    
    template <endian::order r> bool is_negative(const N_bytes<r> &);
    template <endian::order r> bool is_negative(const Z_bytes<r> &x);
    
    template <endian::order r> bool is_positive(const N_bytes<r> &x);
    template <endian::order r> bool is_positive(const Z_bytes<r> &);
    
    template <endian::order r> bool is_minimal(const N_bytes<r> &);
    template <endian::order r> bool is_minimal(const Z_bytes<r> &);
    
    template <endian::order r> size_t minimal_size(const N_bytes<r> &);
    template <endian::order r> size_t minimal_size(const Z_bytes<r> &);
    
    template <endian::order r> N_bytes<r> extend(const N_bytes<r> &, size_t);
    template <endian::order r> Z_bytes<r> extend(const Z_bytes<r> &, size_t);
    
    template <endian::order r> N_bytes<r> trim(const N_bytes<r> &);
    template <endian::order r> Z_bytes<r> trim(const Z_bytes<r> &);
    
}

namespace data::math {
    
    template <endian::order r> using N_bytes = number::N_bytes<r>;
    template <endian::order r> using Z_bytes = number::Z_bytes<r>;
    
    template <endian::order r> struct abs<N_bytes<r>> {
        N_bytes<r> operator()(const N_bytes<r> &);
    };
    
    template <endian::order r> struct abs<Z_bytes<r>> {
        N_bytes<r> operator()(const Z_bytes<r> &);
    };
    
    template <endian::order r> struct quadrance<N_bytes<r>> { 
        N_bytes<r> operator()(const N_bytes<r> &);
    };
    
    template <endian::order r> struct quadrance<number::Z_bytes<r>> { 
        auto operator()(const number::Z_bytes<r> &x) -> decltype(data::abs(x));
    };
    
    template <endian::order r> number::N_bytes<r> next(const number::N_bytes<r>&);
    template <endian::order r> number::N_bytes<r> increment(const number::N_bytes<r>&);
    template <endian::order r> number::N_bytes<r> decrement(const number::N_bytes<r>&);
    
    template <endian::order r> number::Z_bytes<r> increment(const number::Z_bytes<r>&);
    template <endian::order r> number::Z_bytes<r> decrement(const number::Z_bytes<r>&);
    
    // Declare that the plus and times operation are commutative and associative. 
    template <endian::order r> 
    struct commutative<plus<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct associative<plus<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct commutative<times<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct associative<times<N_bytes<r>>, N_bytes<r>> {};
    
    template <endian::order r> 
    struct commutative<plus<number::Z_bytes<r>>, number::Z_bytes<r>> {};
    
    template <endian::order r> 
    struct associative<plus<number::Z_bytes<r>>, number::Z_bytes<r>> {};
    
    template <endian::order r> 
    struct commutative<times<number::Z_bytes<r>>, number::Z_bytes<r>> {};
    
    template <endian::order r> 
    struct associative<times<number::Z_bytes<r>>, number::Z_bytes<r>> {};
    
    template <endian::order r> struct inverse<plus<number::Z_bytes<r>>, number::Z_bytes<r>> {
        number::Z_bytes<r> operator()(const number::Z_bytes<r> &a, const number::Z_bytes<r> &b) {
            return b - a;
        }
    };
    
}

namespace data {
    
    template <endian::order r> math::sign sign(const math::N_bytes<r> &);
    template <endian::order r> math::sign sign(const math::Z_bytes<r> &);
    
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number. 
namespace data::encoding::decimal {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> read(string_view s);
    
    template <endian::order r> 
    std::ostream &write(std::ostream &, const math::number::N_bytes<r>&);
    
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r> 
    ptr<math::number::Z_bytes<r>> read(string_view s);
    
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
    
    template <endian::order r> 
    ptr<math::number::Z_bytes<r>> read(string_view s);
    
}

#endif
