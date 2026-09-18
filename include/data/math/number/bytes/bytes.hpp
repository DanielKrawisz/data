// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_BYTES
#define DATA_MATH_NUMBER_BYTES_BYTES

#include <data/arithmetic/negativity.hpp>

#include <data/math/group.hpp>

#include <data/complex.hpp>

#include <data/divmod.hpp>

#include <data/bytes.hpp>

#include <data/math/number/types.hpp>

namespace data::math::number {
    
    // arithmetic
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, const N_bytes<r, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator + (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator - (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);
    
    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator * (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);
    
    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator + (const N_bytes<r, word> &, I);

    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator - (const N_bytes<r, word> &, I);

    template <endian r, std::unsigned_integral word, std::unsigned_integral I>
    N_bytes<r, word> operator * (const N_bytes<r, word> &, I);

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> operator + (I, const N_bytes<r, word> &);

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> operator - (I, const N_bytes<r, word> &);

    template <std::unsigned_integral I, endian r, std::unsigned_integral word>
    N_bytes<r, word> operator * (I, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator + (const N_bytes<r, word> &, I);

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator - (const N_bytes<r, word> &, I);

    template <endian r, std::unsigned_integral word, std::signed_integral I>
    Z_bytes<r, negativity::twos, word> operator * (const N_bytes<r, word> &, I);

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator + (I, const N_bytes<r, word> &);

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator - (I, const N_bytes<r, word> &);

    template <std::signed_integral I, endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> operator * (I, const N_bytes<r, word> &);

    template <endian r, negativity neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> operator + (const Z_bytes<r, neg, word> &, I);

    template <endian r, negativity neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> operator - (const Z_bytes<r, neg, word> &, I);

    template <endian r, negativity neg, std::unsigned_integral word, std::integral I>
    Z_bytes<r, neg, word> operator * (const Z_bytes<r, neg, word> &, I);

    template <std::integral I, endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator + (I, const Z_bytes<r, neg, word> &);

    template <std::integral I, endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator - (I, const Z_bytes<r, neg, word> &);

    template <std::integral I, endian r, negativity neg, std::unsigned_integral word>
    Z_bytes<r, neg, word> operator * (I, const Z_bytes<r, neg, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator += (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator += (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator -= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator += (N_bytes<r, word> &, uint64);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator -= (N_bytes<r, word> &, uint64);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator += (Z_bytes<r, c, word> &, int64);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator -= (Z_bytes<r, c, word> &, int64);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator *= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian r, std::unsigned_integral word>
    math::Z_bytes<r, word> &operator *= (math::Z_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator *= (N_bytes<r, word> &, uint64);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator *= (Z_bytes<r, c, word> &, int64);

    // division
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const N_bytes<r, word> &, const Z_bytes<r, c, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator / (const N_bytes<r, word> &, uint64);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> operator / (const Z_bytes<r, c, word> &, int64);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator % (const N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    N_bytes<r, word> operator % (const Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> operator % (
        const Z_bytes<r, negativity::twos, word> &,
        const Z_bytes<r, negativity::twos, word> &);

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> operator % (
        const Z_bytes<r, negativity::BC, word> &,
        const Z_bytes<r, negativity::BC, word> &);
    
    // functions having to do with size.
    template <endian r, std::unsigned_integral word>
    bool is_minimal (const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    bool is_minimal (const Z_bytes<r, negativity::twos, word> &);

    template <endian r, std::unsigned_integral word>
    bool is_minimal (const Z_bytes<r, negativity::BC, word> &);
    
    template <endian r, std::unsigned_integral word>
    size_t minimal_size (const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    size_t minimal_size (const Z_bytes<r, c, word> &);
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> extend (const N_bytes<r, word> &, size_t);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> extend (const Z_bytes<r, c, word> &, size_t);
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> trim (const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> trim (const Z_bytes<r, c, word> &);
    
}

namespace data::math::def {

    template <endian r, negativity c, std::unsigned_integral word = byte>
    using Zb = number::Z_bytes<r, c, word>;
    
    template <endian r, std::unsigned_integral word>
    struct is_zero<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct is_zero<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct is_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };
    
    template <endian r, std::unsigned_integral word>
    struct is_negative<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct is_negative<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct is_negative<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &x);
    };
    
    template <endian r, std::unsigned_integral word>
    struct is_positive<N_bytes<r, word>> {
        bool operator () (const N_bytes<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct is_positive<Z_bytes<r, word>> {
        bool operator () (const Z_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct is_positive<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };
    
    template <endian r, std::unsigned_integral word>
    struct is_positive_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct is_negative_zero<Z_bytes_BC<r, word>> {
        bool operator () (const Z_bytes_BC<r, word> &);
    };
    
    template <endian r, std::unsigned_integral word>
    struct abs<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const N_bytes<r, word> &);
    };
    
    template <endian r, std::unsigned_integral word>
    struct abs<Z_bytes<r, word>> {
        N_bytes<r, word> operator () (const Z_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct abs<Z_bytes_BC<r, word>> {
        Z_bytes_BC<r, word> operator () (const Z_bytes_BC<r, word> &);
    };

    template <endian r, negativity neg, std::unsigned_integral word>
    struct times<Zb<r, neg, word>, Zb<r, neg, word>> {
        Zb<r, neg, word> operator ()
            (const Zb<r, neg, word> &a, const Zb<r, neg, word> &b);

        nonzero<Zb<r, neg, word>> operator ()
            (const nonzero<Zb<r, neg, word>> &a, const nonzero<Zb<r, neg, word>> &b);
    };

    
    template <endian r, negativity neg, std::unsigned_integral word>
    struct inverse<plus<Zb<r, neg, word>>, Zb<r, neg, word>> {
        Zb<r, neg, word> operator () (const Zb<r, neg, word> &a, const Zb<r, neg, word> &b) {
            return b - a;
        }
    };

    template <endian r, std::unsigned_integral word>
    struct divmod<N_bytes<r, word>, N_bytes<r, word>> {
        division<N_bytes<r, word>, N_bytes<r, word>> operator ()
            (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct divmod<Z_bytes<r, word>, N_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
            (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct divmod<Z_bytes<r, word>, Z_bytes<r, word>> {
        division<Z_bytes<r, word>, N_bytes<r, word>> operator ()
            (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct divmod<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> {
        division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> operator ()
            (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct sign<N_bytes<r, word>> {
        math::sign operator () (const N_bytes<r, word> &);
    };

    template <endian r, negativity c, std::unsigned_integral word>
    struct sign<Zb<r, c, word>> {
        math::sign operator () (const Zb<r, c, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_xor<N_bytes<r, word>> {
        N_bytes<r, word> operator () (const N_bytes<r, word> &a, const N_bytes<r, word> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_not<N_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const N_bytes<r, word> &n);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_xor<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &a, const math::Z_bytes<r, word> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_xor<N_bytes<r, word>, math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const N_bytes<r, word> &, const math::Z_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_xor<math::Z_bytes<r, word>, N_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &, const N_bytes<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_xor<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &, const math::Z_bytes_BC<r, word> &);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_not<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &);
    };

    // N_bytes and Z_bytes (two's) will use the default mul_2_pow but
    // Z_bytes_BC will not because it does not satisfy bit_number.
    template <endian r, std::unsigned_integral word>
    struct mul_2_pow<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &, uint32 u);
    };

    template <endian r, std::unsigned_integral word>
    struct div_2<math::N_bytes<r, word>> {
        math::N_bytes<r, word> operator () (const math::N_bytes<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct div_2<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct div_2<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct mod_2<math::N_bytes<r, word>> {
        math::N_bytes<r, word> operator () (const math::N_bytes<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct mod_2<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct mod_2<math::Z_bytes<r, word>> {
        math::Z_bytes<r, word> operator () (const math::Z_bytes<r, word> &x);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_or<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b);
    };

    template <endian r, std::unsigned_integral word>
    struct bit_and<math::Z_bytes_BC<r, word>> {
        math::Z_bytes_BC<r, word> operator () (const math::Z_bytes_BC<r, word> &a, const math::Z_bytes_BC<r, word> &b);
    };

    // convert between any two types of N_bytes
    template <endian ToEndian, std::unsigned_integral ToWord,
        endian FromEndian, std::unsigned_integral FromWord>
    struct convert<number::N_bytes<ToEndian, ToWord>, number::N_bytes<FromEndian, FromWord>> {
        number::N_bytes<ToEndian, ToWord> operator () (const number::N_bytes<FromEndian, FromWord> &from) const;
    };

    // convert between any two types of Z_bytes
    template <endian ToEndian, negativity ToNeg, std::unsigned_integral ToWord,
        endian FromEndian, negativity FromNeg, std::unsigned_integral FromWord>
    struct convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::Z_bytes<FromEndian, FromNeg, FromWord>> {
        number::Z_bytes<ToEndian, ToNeg, ToWord> operator () (const number::Z_bytes<FromEndian, FromNeg, FromWord> &from) const;
    };

    // convert from N_bytes to Z_bytes
    template <endian ToEndian, negativity ToNeg, std::unsigned_integral ToWord,
        endian FromEndian, std::unsigned_integral FromWord>
    struct convert<number::Z_bytes<ToEndian, ToNeg, ToWord>, number::N_bytes<FromEndian, FromWord>> {
        number::Z_bytes<ToEndian, ToNeg, ToWord> operator () (const number::N_bytes<FromEndian, FromWord> &from) const;
    };
    
}

namespace data::math::number {
    
    template <endian r, std::unsigned_integral word>
    uint64 operator % (const N_bytes<r, word> &, uint64);

    template <endian r, negativity c, std::unsigned_integral word>
    uint64 operator % (const Z_bytes<r, c, word> &, uint64);
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator /= (Z_bytes<r, c, word> &, const Z_bytes<r, c, word> &);
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator /= (N_bytes<r, word> &, uint64);

    template <endian r, negativity c, std::unsigned_integral word>
    Z_bytes<r, c, word> &operator /= (Z_bytes<r, c, word> &, int64);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, const N_bytes<r, word> &);

    template <endian r, negativity c, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (Z_bytes<r, c, word> &, const N_bytes<r, word> &);

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> &operator %=
        (Z_bytes<r, negativity::BC, word> &, const Z_bytes<r, negativity::BC, word> &);

    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> &operator %= (N_bytes<r, word> &, uint64);
    
}

namespace data::encoding {
    template <endian::order r, std::unsigned_integral word = byte>
    using N_bytes = math::number::N_bytes<r, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes = math::number::Z_bytes<r, negativity::twos, word>;

    template <endian::order r, std::unsigned_integral word = byte>
    using Z_bytes_BC = math::number::Z_bytes<r, negativity::BC, word>;
}

// functions for reading and wriing these numbers in various formats.
// we also have base58 but that works a priori for any number. 
namespace data::encoding::decimal {
    
    template <endian::order r, std::unsigned_integral word> maybe<N_bytes<r, word>> read (string_view s);
    
    struct string;
    
    template <endian::order r, std::unsigned_integral word> string write (const N_bytes<r, word> &z);
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const N_bytes<r, word> &n) {
        return o << static_cast<std::string> (write (n));
    }
    
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, negativity n, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, n, word>> read (string_view);
    
    struct string;
    
    template <endian::order r, negativity n, std::unsigned_integral word>
    string write (const math::number::Z_bytes<r, n, word> &);
    
    template <endian::order r, negativity n, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, n, word> &);
    
}

namespace data::encoding::hexidecimal {
    
    template <endian::order r, std::unsigned_integral word>
    maybe<oriented<r, word>> read (string_view);
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream &write (std::ostream &, const oriented<r, word> &, hex_case q = hex_case::lower);
    
    template <hex_case zz> struct string;
    
    template <hex_case zz, endian::order r, std::unsigned_integral word>
    string<zz> write (const oriented<r, word> &);
    
}

namespace data::encoding::natural {
    
    template <endian::order r, std::unsigned_integral word>
    maybe<math::N_bytes<r, word>> read (string_view s);
    
}

namespace data::encoding::integer {
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, c, word>> read (string_view s);
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    std::ostream &write (std::ostream &, const math::number::Z_bytes<r, c, word> &);
    
}

#endif
