// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

#include <data/math/power.hpp>

// defines float32 and float64
#include <data/float.hpp>
#include <data/math.hpp>

// numbers of either endian of any size up to 64 bits
// using boost. 
#include <data/arithmetic/endian.hpp>

#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/bounded/bounded.hpp>

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bounded.hpp>
#include <data/math/number/bytes.hpp>

#include <data/encoding/base58.hpp>

#include <data/math/fraction.hpp>
#include <data/math/octonion.hpp>

#include <algorithm>

namespace data {
    // we use a wrapper around gmp for natural numbers and integers. 
    
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
    
    using Z_bytes_twos_little = math::number::Z_bytes<endian::little, math::number::complement::twos, byte>;
    
    using Z_bytes_twos_big = math::number::Z_bytes<endian::big, math::number::complement::twos, byte>;
    
    // fixed size numbers of any size, similar to the 
    // built-in types. 
    template <size_t size> 
    using int_little = math::sint<endian::little, size, byte>;
    
    template <size_t size> 
    using int_big = math::sint<endian::big, size, byte>;
    
    template <size_t size> 
    using uint_little = math::uint<endian::little, size, byte>;
    
    template <size_t size> 
    using uint_big = math::uint<endian::big, size, byte>;
    
    using uint128 = uint_little<16>;
    using uint160 = uint_little<20>;
    using uint224 = uint_little<28>;
    using uint256 = uint_little<32>;
    using uint320 = uint_little<40>;
    using uint448 = uint_little<56>;
    using uint512 = uint_little<64>;
    
    using int128 = int_little<16>;
    using int160 = int_little<20>;
    using int224 = int_little<28>;
    using int256 = int_little<32>;
    using int320 = int_little<40>;
    using int448 = int_little<56>;
    using int512 = int_little<64>;
    
    // string numbers. 
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;
    using hex_uint = hex::uint<hex_case::lower>;
    using hex_int = hex::int1<hex_case::lower>;
    using hex_int_ones = hex::int1<hex_case::lower>;
    using hex_int_twos = hex::int2<hex_case::lower>;
    using base58_uint = encoding::base58::string;
    
    // rational numbers. 
    using Q = math::fraction<Z>;
    
    using Q_big = math::fraction<Z_bytes_big>;
    using Q_little = math::fraction<Z_bytes_little>;

    using Q_twos_big = math::fraction<Z_bytes_twos_big>;
    using Q_twos_little = math::fraction<Z_bytes_twos_little>;
    
    // Gaussian integers. 
    using G = math::complex<Z>;
    using GH = math::quaternion<Z>;
    using GO = math::octonion<Z>;
    
    // complex rationals
    using QC = math::complex<Q>;
        
    // rational quaternions
    using QH = math::quaternion<Q>;
        
    // rational octonions
    using QO = math::octonion<Q>;
    
    // Caylay-Dickson floating points. 
    using complex64 = math::complex<float32>;
    using complex128 = math::complex<float64>;
    using quat128 = math::quaternion<float32>;
    using quat256 = math::quaternion<float64>;
    using oct256 = math::octonion<float32>;
    using oct512 = math::octonion<float64>;
    
}

namespace data::math::number {
    
    // explicit instantiations
    template struct N_bytes<endian::big, byte>;
    template struct N_bytes<endian::little, byte>;
    template struct Z_bytes<endian::big, complement::ones, byte>;
    template struct Z_bytes<endian::little, complement::ones, byte>;
    template struct Z_bytes<endian::big, complement::twos, byte>;
    template struct Z_bytes<endian::little, complement::twos, byte>;
    
    template std::weak_ordering operator <=> (const N_bytes<endian::big, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const N_bytes<endian::little, byte> &, const N_bytes<endian::little, byte> &);
    
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, complement::ones, byte> &, const Z_bytes<endian::big, complement::ones, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, complement::ones, byte> &, const Z_bytes<endian::little, complement::ones, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, complement::twos, byte> &, const Z_bytes<endian::big, complement::twos, byte> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, complement::twos, byte> &, const Z_bytes<endian::little, complement::twos, byte> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::ones, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::twos, byte> &, const N_bytes<endian::big, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::ones, byte> &, const N_bytes<endian::little, byte> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::twos, byte> &, const N_bytes<endian::little, byte> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::ones, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::twos, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::ones, byte> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::twos, byte> &, int64);
    
    template bool operator == (const N_bytes<endian::big, byte> &, uint64);
    template bool operator == (const Z_bytes<endian::big, complement::ones, byte> &, int64);
    template bool operator == (const Z_bytes<endian::big, complement::twos, byte> &, int64);
    
    template bool operator == (const N_bytes<endian::little, byte> &, uint64);
    template bool operator == (const Z_bytes<endian::little, complement::ones, byte> &, int64);
    template bool operator == (const Z_bytes<endian::little, complement::twos, byte> &, int64);
    
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
    
    template Z_bytes<endian::big, complement::twos, byte> operator &
        (const Z_bytes<endian::big, complement::twos, byte> &, const Z_bytes<endian::big, complement::twos, byte> &);
    template Z_bytes<endian::big, complement::twos, byte> operator &
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::twos, byte> &);
    template Z_bytes<endian::big, complement::twos, byte> operator &
        (const Z_bytes<endian::big, complement::twos, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::twos, byte> operator |
        (const Z_bytes<endian::big, complement::twos, byte> &, const Z_bytes<endian::big, complement::twos, byte> &);
    template Z_bytes<endian::big, complement::twos, byte> operator |
        (const N_bytes<endian::big, byte> &, const Z_bytes<endian::big, complement::twos, byte> &);
    template Z_bytes<endian::big, complement::twos, byte> operator |
        (const Z_bytes<endian::big, complement::twos, byte> &, const N_bytes<endian::big, byte> &);
    
    template Z_bytes<endian::big, complement::twos, byte> operator - (const Z_bytes<endian::big, complement::twos, byte> &);
    
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
    
    template Z_bytes<endian::little, complement::twos, byte> operator &
        (const Z_bytes<endian::little, complement::twos, byte> &, const Z_bytes<endian::little, complement::twos, byte> &);
    template Z_bytes<endian::little, complement::twos, byte> operator &
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::twos, byte> &);
    template Z_bytes<endian::little, complement::twos, byte> operator &
        (const Z_bytes<endian::little, complement::twos, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::twos, byte> operator |
        (const Z_bytes<endian::little, complement::twos, byte> &, const Z_bytes<endian::little, complement::twos, byte> &);
    template Z_bytes<endian::little, complement::twos, byte> operator |
        (const N_bytes<endian::little, byte> &, const Z_bytes<endian::little, complement::twos, byte> &);
    template Z_bytes<endian::little, complement::twos, byte> operator |
        (const Z_bytes<endian::little, complement::twos, byte> &, const N_bytes<endian::little, byte> &);
    
    template Z_bytes<endian::little, complement::twos, byte> operator - (const Z_bytes<endian::little, complement::twos, byte> &);
    
}

namespace data::encoding::hexidecimal {
    template struct integer<math::number::complement::nones, hex_case::lower>;
    template struct integer<math::number::complement::nones, hex_case::upper>;
    template struct integer<math::number::complement::ones, hex_case::lower>;
    template struct integer<math::number::complement::ones, hex_case::upper>;
    template struct integer<math::number::complement::twos, hex_case::lower>;
    template struct integer<math::number::complement::twos, hex_case::upper>;
}

namespace data {

    static_assert (math::number::natural<N>);
    static_assert (math::number::integer<Z>);

    static_assert (math::number::natural<dec_uint>);
    static_assert (math::number::integer<dec_int>);

    static_assert (math::number::natural<hex_uint>);

    static_assert (math::number::integer<hex_int>);
    static_assert (math::number::integer<hex_int_ones>);
    static_assert (math::number::integer<hex_int_twos>);

    static_assert (math::number::natural<base58_uint>);

    static_assert (math::number::natural<N_bytes_little>);
    static_assert (math::number::natural<N_bytes_big>);
    static_assert (math::number::integer<Z_bytes_little>);
    static_assert (math::number::integer<Z_bytes_big>);
    static_assert (math::number::integer<Z_bytes_twos_little>);
    static_assert (math::number::integer<Z_bytes_twos_big>);

    static_assert (math::number::natural<uint128>); //uint_little<16>;
    static_assert (math::number::natural<uint160>); //uint_little<20>;
    static_assert (math::number::natural<uint224>); //uint_little<28>;
    static_assert (math::number::natural<uint256>); //uint_little<32>;
    static_assert (math::number::natural<uint320>); //uint_little<40>;
    static_assert (math::number::natural<uint448>); //uint_little<56>;
    static_assert (math::number::natural<uint512>); //uint_little<64>;

    static_assert (math::number::integer<int128>); //int_little<16>;
    static_assert (math::number::integer<int160>); //int_little<20>;
    static_assert (math::number::integer<int224>); //int_little<28>;
    static_assert (math::number::integer<int256>); //int_little<32>;
    static_assert (math::number::integer<int320>); //int_little<40>;
    static_assert (math::number::integer<int448>); //int_little<56>;
    static_assert (math::number::integer<int512>); //int_little<64>;
}

#endif
