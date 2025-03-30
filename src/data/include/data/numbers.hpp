// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

// defines float32 and float64
#include <data/float.hpp>
#include <data/power.hpp>

// numbers of either endian of any size up to 64 bits
// using boost. 
#include <data/arithmetic/endian.hpp>

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bounded.hpp>
#include <data/math/number/bytes.hpp>

#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/aks.hpp>

#include <algorithm>

namespace data {
    // we use a wrapper around GMP for natural numbers and integers.
    
    // Natural numbers
    using N = math::N;
    
    // Integers
    using Z = math::Z;
    
    // N_bytes and Z_bytes are big numbers with a specific
    // representation. They can be big or little endian. 
    // Z_bytes are one's complement. 

    using N_bytes_little = math::number::N_bytes<endian::little, byte>;
    
    using N_bytes_big = math::number::N_bytes<endian::big, byte>;
    
    using Z_bytes_little = math::number::Z_bytes<endian::little, math::number::complement::ones, byte>;
    
    using Z_bytes_big = math::number::Z_bytes<endian::big, math::number::complement::ones, byte>;
    
    using Z_bytes_BC_little = math::number::Z_bytes<endian::little, math::number::complement::BC, byte>;
    
    using Z_bytes_BC_big = math::number::Z_bytes<endian::big, math::number::complement::BC, byte>;
    
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
    using uint128 = math::uint<endian::little, 4, uint32>;
    using uint160 = math::uint<endian::little, 5, uint32>;
    using uint224 = math::uint<endian::little, 7, uint32>;
    using uint256 = math::uint<endian::little, 8, uint32>;
    using uint320 = math::uint<endian::little, 10, uint32>;
    using uint448 = math::uint<endian::little, 14, uint32>;
    using uint512 = math::uint<endian::little, 16, uint32>;
    
    using   int80 = math::sint<endian::little, 5, uint16>;
    using  int128 = math::sint<endian::little, 4, uint32>;
    using  int160 = math::sint<endian::little, 5, uint32>;
    using  int224 = math::sint<endian::little, 7, uint32>;
    using  int256 = math::sint<endian::little, 8, uint32>;
    using  int320 = math::sint<endian::little, 10, uint32>;
    using  int448 = math::sint<endian::little, 14, uint32>;
    using  int512 = math::sint<endian::little, 16, uint32>;

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
    using hex_int = hex::int1<hex_case::lower>;
    using hex_int_ones = hex::int1<hex_case::lower>;
    using hex_int_BC = hex::intBC<hex_case::lower>;
    using base58_uint = encoding::base58::string;
    
}

namespace data {

    static_assert (signed_type<float>);
    static_assert (signed_type<double>);

    static_assert (unsigned_integral<byte>);
    static_assert (unsigned_integral<uint16>);
    static_assert (unsigned_integral<uint32>);
    static_assert (unsigned_integral<uint64>);
    static_assert (unsigned_integral<uint80>);
    static_assert (unsigned_integral<uint128>);
    static_assert (unsigned_integral<uint160>);
    static_assert (unsigned_integral<uint224>);
    static_assert (unsigned_integral<uint256>);
    static_assert (unsigned_integral<uint320>);
    static_assert (unsigned_integral<uint448>);
    static_assert (unsigned_integral<uint512>);

    static_assert (integral_system<int8, byte>);
    static_assert (integral_system<int16, uint16>);
    static_assert (integral_system<int32, uint32>);
    static_assert (integral_system<int64, uint64>);
    static_assert (integral_system<int80, uint80>);
    static_assert (integral_system<int128, uint128>);
    static_assert (integral_system<int160, uint160>);
    static_assert (integral_system<int224, uint224>);
    static_assert (integral_system<int256, uint256>);
    static_assert (integral_system<int320, uint320>);
    static_assert (integral_system<int448, uint448>);
    static_assert (integral_system<int512, uint512>);

    static_assert (unsigned_integral<uint8_big>);
    static_assert (unsigned_integral<uint16_big>);
    static_assert (unsigned_integral<uint24_big>);
    static_assert (unsigned_integral<uint32_big>);
    static_assert (unsigned_integral<uint40_big>);
    static_assert (unsigned_integral<uint48_big>);
    static_assert (unsigned_integral<uint56_big>);
    static_assert (unsigned_integral<uint64_big>);
    static_assert (unsigned_integral<uint80_big>);
    static_assert (unsigned_integral<uint128_big>);
    static_assert (unsigned_integral<uint160_big>);
    static_assert (unsigned_integral<uint224_big>);
    static_assert (unsigned_integral<uint256_big>);
    static_assert (unsigned_integral<uint320_big>);
    static_assert (unsigned_integral<uint448_big>);
    static_assert (unsigned_integral<uint512_big>);

    static_assert (integral_system<int8_big, uint8_big>);
    static_assert (integral_system<int16_big, uint16_big>);
    static_assert (integral_system<int24_big, uint24_big>);
    static_assert (integral_system<int32_big, uint32_big>);
    static_assert (integral_system<int40_big, uint40_big>);
    static_assert (integral_system<int48_big, uint48_big>);
    static_assert (integral_system<int56_big, uint56_big>);
    static_assert (integral_system<int64_big, uint64_big>);

    static_assert (unsigned_integral<uint8_little>);
    static_assert (unsigned_integral<uint16_little>);
    static_assert (unsigned_integral<uint24_little>);
    static_assert (unsigned_integral<uint32_little>);
    static_assert (unsigned_integral<uint40_little>);
    static_assert (unsigned_integral<uint48_little>);
    static_assert (unsigned_integral<uint56_little>);
    static_assert (unsigned_integral<uint64_little>);
    static_assert (unsigned_integral<uint80_little>);
    static_assert (unsigned_integral<uint128_little>);
    static_assert (unsigned_integral<uint160_little>);
    static_assert (unsigned_integral<uint224_little>);
    static_assert (unsigned_integral<uint256_little>);
    static_assert (unsigned_integral<uint320_little>);
    static_assert (unsigned_integral<uint448_little>);
    static_assert (unsigned_integral<uint512_little>);

    static_assert (integral_system<int8_little, uint8_little>);
    static_assert (integral_system<int16_little, uint16_little>);
    static_assert (integral_system<int24_little, uint24_little>);
    static_assert (integral_system<int32_little, uint32_little>);
    static_assert (integral_system<int40_little, uint40_little>);
    static_assert (integral_system<int48_little, uint48_little>);
    static_assert (integral_system<int56_little, uint56_little>);
    static_assert (integral_system<int64_little, uint64_little>);

    static_assert (unsigned_integral<N>);
    static_assert (integral_system<Z, N>);

    static_assert (unsigned_integral<N_bytes_little>);
    static_assert (unsigned_integral<N_bytes_big>);

    static_assert (integral_system<Z_bytes_little, N_bytes_little>);
    static_assert (integral_system<Z_bytes_big, N_bytes_big>);
    static_assert (proto_integral_system<Z_bytes_BC_little>);
    static_assert (proto_integral_system<Z_bytes_BC_big>);

    static_assert (unsigned_integral<dec_uint>);
    static_assert (integral_system<dec_int, dec_uint>);

    static_assert (unsigned_integral<hex_uint>);
    static_assert (integral_system<hex_int, hex_uint>);

    static_assert (integral_system<hex_int_BC>);

    //static_assert (unsigned_integral<base58_uint>);

}

namespace data::math::number {
    
    // explicit instantiations
    template struct N_bytes<endian::big, byte>;
    template struct N_bytes<endian::little, byte>;
    template struct Z_bytes<endian::big, complement::ones, byte>;
    template struct Z_bytes<endian::little, complement::ones, byte>;
    template struct Z_bytes<endian::big, complement::BC, byte>;
    template struct Z_bytes<endian::little, complement::BC, byte>;
    
    template std::weak_ordering operator <=> (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, complement::ones, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, complement::ones, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, complement::BC, byte> &, const Z_bytes<endian::big, complement::BC, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, complement::BC, byte> &, const Z_bytes<endian::little, complement::BC, byte> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::ones, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::BC, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::ones, byte> &, const N_bytes<endian::little, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::BC, byte> &, const N_bytes<endian::little, byte> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::ones, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::BC, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::ones, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::BC, byte> &, int64);
    
    template bool operator == (const N_bytes<endian::big, byte> &, uint64);
    template bool operator == (const Z_bytes<endian::big, complement::ones, byte> &, int64);
    template bool operator == (const Z_bytes<endian::big, complement::BC, byte> &, int64);
    
    template bool operator == (const N_bytes<endian::little, byte> &, uint64);
    template bool operator == (const Z_bytes<endian::little, complement::ones, byte> &, int64);
    template bool operator == (const Z_bytes<endian::little, complement::BC, byte> &, int64);
    
    template Z_bytes<endian::big, complement::ones, byte> operator ~ (const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator ~ (const Z_bytes<endian::big, complement::ones, byte> &);
    
    template Z_bytes<endian::big, complement::ones, byte> operator ^ (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator ^
        (const Z_bytes<endian::big, complement::ones, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator ^
        (const Z_bytes<endian::big, complement::ones, byte> &, const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator ^
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    
    template N_bytes<endian::big, byte> operator & (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template N_bytes<endian::big, byte> operator | (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator - (const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::ones, byte> operator &
        (const Z_bytes<endian::big, complement::ones, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator &
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator &
        (const Z_bytes<endian::big, complement::ones, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::ones, byte> operator |
        (const Z_bytes<endian::big, complement::ones, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator |
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template Z_bytes<endian::big, complement::ones, byte> operator |
        (const Z_bytes<endian::big, complement::ones, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::ones, byte> operator - (const Z_bytes<endian::big, complement::ones, byte> &);
    
    template Z_bytes<endian::big, complement::BC, byte> operator &
        (const Z_bytes<endian::big, complement::BC, byte> &, const Z_bytes<endian::big, complement::BC, byte> &);
    template Z_bytes<endian::big, complement::BC, byte> operator &
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::BC, byte> &);
    template Z_bytes<endian::big, complement::BC, byte> operator &
        (const Z_bytes<endian::big, complement::BC, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::BC, byte> operator |
        (const Z_bytes<endian::big, complement::BC, byte> &, const Z_bytes<endian::big, complement::BC, byte> &);
    template Z_bytes<endian::big, complement::BC, byte> operator |
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::BC, byte> &);
    template Z_bytes<endian::big, complement::BC, byte> operator |
        (const Z_bytes<endian::big, complement::BC, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::BC, byte> operator - (const Z_bytes<endian::big, complement::BC, byte> &);
    
    template Z_bytes<endian::little, complement::ones, byte> operator ~ (const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator ~ (const Z_bytes<endian::little, complement::ones, byte> &);
    
    template Z_bytes<endian::little, complement::ones, byte> operator ^
        (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator ^
        (const Z_bytes<endian::little, complement::ones, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator ^
        (const Z_bytes<endian::little, complement::ones, byte> &, const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator ^
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    
    template N_bytes<endian::little, byte> operator & (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    template N_bytes<endian::little, byte> operator | (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator - (const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::ones, byte> operator &
        (const Z_bytes<endian::little, complement::ones, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator &
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator &
        (const Z_bytes<endian::little, complement::ones, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::ones, byte> operator |
        (const Z_bytes<endian::little, complement::ones, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator |
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template Z_bytes<endian::little, complement::ones, byte> operator |
        (const Z_bytes<endian::little, complement::ones, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::ones, byte> operator - (const Z_bytes<endian::little, complement::ones, byte> &);
    
    template Z_bytes<endian::little, complement::BC, byte> operator &
        (const Z_bytes<endian::little, complement::BC, byte> &, const Z_bytes<endian::little, complement::BC, byte> &);
    template Z_bytes<endian::little, complement::BC, byte> operator &
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::BC, byte> &);
    template Z_bytes<endian::little, complement::BC, byte> operator &
        (const Z_bytes<endian::little, complement::BC, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::BC, byte> operator |
        (const Z_bytes<endian::little, complement::BC, byte> &, const Z_bytes<endian::little, complement::BC, byte> &);
    template Z_bytes<endian::little, complement::BC, byte> operator |
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::BC, byte> &);
    template Z_bytes<endian::little, complement::BC, byte> operator |
        (const Z_bytes<endian::little, complement::BC, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::BC, byte> operator - (const Z_bytes<endian::little, complement::BC, byte> &);
    
}

namespace data::encoding::hexidecimal {
    template struct integer<math::number::complement::nones, hex_case::lower>;
    template struct integer<math::number::complement::nones, hex_case::upper>;
    template struct integer<math::number::complement::ones, hex_case::lower>;
    template struct integer<math::number::complement::ones, hex_case::upper>;
    template struct integer<math::number::complement::BC, hex_case::lower>;
    template struct integer<math::number::complement::BC, hex_case::upper>;
}

#endif
