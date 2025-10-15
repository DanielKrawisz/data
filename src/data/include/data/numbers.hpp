// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

// defines float32 and float64
#include <data/float.hpp>

#include <data/power.hpp>
#include <data/math/number/extended_euclidian.hpp>

// numbers of either endian of any size up to 64 bits
// using boost. 
#include <data/encoding/endian.hpp>

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bounded.hpp>
#include <data/math/number/bytes.hpp>

#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/aks.hpp>

// we have to include this so that everybody
// has a definition of quadrance.
#include <data/complex.hpp>

#include <algorithm>

namespace data {
    // we use a wrapper around GMP for natural numbers and integers.
    // naturals are N, integers are Z.
    
    // N_bytes and Z_bytes are big numbers with a specific
    // representation. They can be big or little endian. 
    // Z_bytes are two's complement.

    using N_bytes_little = math::number::N_bytes<endian::little, byte>;
    
    using N_bytes_big = math::number::N_bytes<endian::big, byte>;
    
    using Z_bytes_little = math::number::Z_bytes<endian::little, neg::twos, byte>;
    
    using Z_bytes_big = math::number::Z_bytes<endian::big, neg::twos, byte>;
    
    using Z_bytes_BC_little = math::number::Z_bytes<endian::little, neg::BC, byte>;
    
    using Z_bytes_BC_big = math::number::Z_bytes<endian::big, neg::BC, byte>;
    
    // fixed size numbers of any size, similar to the 
    // built-in types. 
    template <size_t size, std::unsigned_integral word = byte>
    using int_little = math::sint<endian::little, size, word>;
    
    template <size_t size, std::unsigned_integral word = byte>
    using int_big = math::sint<endian::big, size, word>;
    
    template <size_t size, std::unsigned_integral word = byte>
    using uint_little = math::uint<endian::little, size, word>;
    
    template <size_t size, std::unsigned_integral word = byte>
    using uint_big = math::uint<endian::big, size, word>;

    using  uint80 = math::uint<endian::little, 5, uint16>;
    using   int80 = math::sint<endian::little, 5, uint16>;

    using uint128 = math::uint<endian::little, 2, uint64>;
    using  int128 = math::sint<endian::little, 2, uint64>;

    using uint160 = math::uint<endian::little, 5, uint32>;
    using  int160 = math::sint<endian::little, 5, uint32>;

    using uint224 = math::uint<endian::little, 7, uint32>;
    using  int224 = math::sint<endian::little, 7, uint32>;

    using uint256 = math::uint<endian::little, 4, uint64>;
    using  int256 = math::sint<endian::little, 4, uint64>;

    using uint320 = math::uint<endian::little, 5, uint64>;
    using  int320 = math::sint<endian::little, 5, uint64>;

    using uint448 = math::uint<endian::little, 7, uint64>;
    using  int448 = math::sint<endian::little, 7, uint64>;

    using uint512 = math::uint<endian::little, 8, uint64>;
    using  int512 = math::sint<endian::little, 8, uint64>;

    using  uint80_little = uint_little<10>;
    using uint128_little = uint_little<16>;
    using uint160_little = uint_little<20>;
    using uint224_little = uint_little<28>;
    using uint256_little = uint_little<32>;
    using uint320_little = uint_little<40>;
    using uint448_little = uint_little<56>;
    using uint512_little = uint_little<64>;

    using   int80_little = int_little<10>;
    using  int128_little = int_little<16>;
    using  int160_little = int_little<20>;
    using  int224_little = int_little<28>;
    using  int256_little = int_little<32>;
    using  int320_little = int_little<40>;
    using  int448_little = int_little<56>;
    using  int512_little = int_little<64>;

    using  uint80_big = uint_big<10>;
    using uint128_big = uint_big<16>;
    using uint160_big = uint_big<20>;
    using uint224_big = uint_big<28>;
    using uint256_big = uint_big<32>;
    using uint320_big = uint_big<40>;
    using uint448_big = uint_big<56>;
    using uint512_big = uint_big<64>;

    using   int80_big = int_big<10>;
    using  int128_big = int_big<16>;
    using  int160_big = int_big<20>;
    using  int224_big = int_big<28>;
    using  int256_big = int_big<32>;
    using  int320_big = int_big<40>;
    using  int448_big = int_big<56>;
    using  int512_big = int_big<64>;
    
    // string numbers. 
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;
    using hex_uint = hex::uint<hex_case::lower>;
    using hex_int = hex::int2<hex_case::lower>;
    using hex_int_BC = hex::intBC<hex_case::lower>;
    using base58_uint = encoding::base58::string;

    static_assert (proto_bit_unsigned<hex_uint>);
    
}

namespace data {

    static_assert (unsigned_integral<N>);
    static_assert (integral_system<Z, N>);

    static_assert (unsigned_integral<N_bytes_little>);
    static_assert (unsigned_integral<N_bytes_big>);

    static_assert (integral_system<Z_bytes_little, N_bytes_little>);
    static_assert (integral_system<Z_bytes_big, N_bytes_big>);
    static_assert (proto_number_system<Z_bytes_BC_little>);
    static_assert (proto_number_system<Z_bytes_BC_big>);

    static_assert (unsigned_integral<dec_uint>);
    static_assert (integral_system<dec_int, dec_uint>);

    static_assert (unsigned_integral<hex_uint>);
    static_assert (integral_system<hex_int, hex_uint>);

    static_assert (proto_number_system<hex_int_BC>);

    //static_assert (unsigned_integral<base58_uint>);

}

namespace data::math::number {
    
    // explicit instantiations
    template struct N_bytes<endian::big, byte>;
    template struct N_bytes<endian::little, byte>;
    template struct Z_bytes<endian::big, neg::twos, byte>;
    template struct Z_bytes<endian::little, neg::twos, byte>;
    template struct Z_bytes<endian::big, neg::BC, byte>;
    template struct Z_bytes<endian::little, neg::BC, byte>;
    
    template std::weak_ordering operator <=> (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, neg::twos, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, neg::twos, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, neg::BC, byte> &, const Z_bytes<endian::big, neg::BC, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, neg::BC, byte> &, const Z_bytes<endian::little, neg::BC, byte> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, neg::twos, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, neg::BC, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, neg::twos, byte> &, const N_bytes<endian::little, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, neg::BC, byte> &, const N_bytes<endian::little, byte> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, neg::twos, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, neg::BC, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, neg::twos, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, neg::BC, byte> &, int64);
    
    template bool operator == (const N_bytes<endian::big, byte> &, uint64);
    template bool operator == (const Z_bytes<endian::big, neg::twos, byte> &, int64);
    template bool operator == (const Z_bytes<endian::big, neg::BC, byte> &, int64);
    
    template bool operator == (const N_bytes<endian::little, byte> &, uint64);
    template bool operator == (const Z_bytes<endian::little, neg::twos, byte> &, int64);
    template bool operator == (const Z_bytes<endian::little, neg::BC, byte> &, int64);
    
    template Z_bytes<endian::big, neg::twos, byte> operator ~ (const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator ~ (const Z_bytes<endian::big, neg::twos, byte> &);
    
    template N_bytes<endian::big, byte> operator ^ (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator ^
        (const Z_bytes<endian::big, neg::twos, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator ^
        (const Z_bytes<endian::big, neg::twos, byte> &, const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator ^
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    
    template N_bytes<endian::big, byte> operator & (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template N_bytes<endian::big, byte> operator | (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator - (const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, neg::twos, byte> operator &
        (const Z_bytes<endian::big, neg::twos, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator &
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator &
        (const Z_bytes<endian::big, neg::twos, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, neg::twos, byte> operator |
        (const Z_bytes<endian::big, neg::twos, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator |
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, neg::twos, byte> &);
    template Z_bytes<endian::big, neg::twos, byte> operator |
        (const Z_bytes<endian::big, neg::twos, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, neg::twos, byte> operator - (const Z_bytes<endian::big, neg::twos, byte> &);
    
    template Z_bytes<endian::big, neg::BC, byte> operator - (const Z_bytes<endian::big, neg::BC, byte> &);
    
    template Z_bytes<endian::little, neg::twos, byte> operator ~ (const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator ~ (const Z_bytes<endian::little, neg::twos, byte> &);
    
    template N_bytes<endian::little, byte> operator ^
        (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator ^
        (const Z_bytes<endian::little, neg::twos, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator ^
        (const Z_bytes<endian::little, neg::twos, byte> &, const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator ^
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    
    template N_bytes<endian::little, byte> operator & (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    template N_bytes<endian::little, byte> operator | (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator - (const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, neg::twos, byte> operator &
        (const Z_bytes<endian::little, neg::twos, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator &
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator &
        (const Z_bytes<endian::little, neg::twos, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, neg::twos, byte> operator |
        (const Z_bytes<endian::little, neg::twos, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator |
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, neg::twos, byte> &);
    template Z_bytes<endian::little, neg::twos, byte> operator |
        (const Z_bytes<endian::little, neg::twos, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, neg::twos, byte> operator - (const Z_bytes<endian::little, neg::twos, byte> &);
    
    template Z_bytes<endian::little, neg::BC, byte> operator - (const Z_bytes<endian::little, neg::BC, byte> &);
    
}

namespace data::encoding::hexidecimal {
    template struct integer<neg::nones, hex_case::lower>;
    template struct integer<neg::nones, hex_case::upper>;
    template struct integer<neg::twos, hex_case::lower>;
    template struct integer<neg::twos, hex_case::upper>;
    template struct integer<neg::BC, hex_case::lower>;
    template struct integer<neg::BC, hex_case::upper>;
}

#endif
