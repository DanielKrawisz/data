// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

#include <data/math/power.hpp>

// defines float32 and float64
#include <data/float.hpp>

// numbers of either endian of any size up to 64 bits
// using boost. 
#include <data/encoding/endian/arithmetic.hpp>

#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/bounded/bounded.hpp>

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bounded.hpp>

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
    
    using N_bytes_little = math::N_bytes<endian::little>;
    
    using N_bytes_big = math::N_bytes<endian::big>;
    
    using Z_bytes_little = math::Z_bytes<endian::little>;
    
    using Z_bytes_big = math::Z_bytes<endian::big>;
    
    using Z_bytes_twos_little = math::number::Z_bytes<endian::little, math::number::complement::twos>;
    
    using Z_bytes_twos_big = math::number::Z_bytes<endian::big, math::number::complement::twos>;
    
    // fixed size numbers of any size, similar to the 
    // built-in types. 
    template <size_t size> 
    using int_little = math::sint<endian::little, size>;
    
    template <size_t size> 
    using int_big = math::sint<endian::big, size>;
    
    template <size_t size> 
    using uint_little = math::uint<endian::little, size>;
    
    template <size_t size> 
    using uint_big = math::uint<endian::big, size>;
    
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

namespace data::math {
    template <> struct divide<uint64, uint64> {
        division<uint64> operator () (uint64 a, uint64 b) {
            return number::natural::divide (a, b);
        }
    };
    
    template <> struct divide<int64, int64> {
        division<int64> operator () (int64 a, int64 b) {
            return number::natural::divide (a, b);
        }
    };
}

namespace data::math::number {
    
    // explicit instantiations
    template struct N_bytes<endian::big>;
    template struct N_bytes<endian::little>;
    template struct Z_bytes<endian::big, complement::ones>;
    template struct Z_bytes<endian::little, complement::ones>;
    template struct Z_bytes<endian::big, complement::twos>;
    template struct Z_bytes<endian::little, complement::twos>;
    
    template std::weak_ordering operator <=> (const N_bytes<endian::big> &, const N_bytes<endian::big> &);
    template std::weak_ordering operator <=> (const N_bytes<endian::little> &, const N_bytes<endian::little> &);
    
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, complement::ones> &, const Z_bytes<endian::big, complement::ones> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, complement::ones> &, const Z_bytes<endian::little, complement::ones> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::big, complement::twos> &, const Z_bytes<endian::big, complement::twos> &);
    template std::weak_ordering operator <=>
        (const Z_bytes<endian::little, complement::twos> &, const Z_bytes<endian::little, complement::twos> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::ones>&, const N_bytes<endian::big> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::twos>&, const N_bytes<endian::big> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::ones>&, const N_bytes<endian::little> &);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::twos>&, const N_bytes<endian::little> &);
    
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::ones> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::big, complement::twos> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::ones> &, int64);
    template std::weak_ordering operator <=> (const Z_bytes<endian::little, complement::twos> &, int64);
    
    template bool operator == (const N_bytes<endian::big> &, uint64);
    template bool operator == (const Z_bytes<endian::big, complement::ones> &, int64);
    template bool operator == (const Z_bytes<endian::big, complement::twos> &, int64);
    
    template bool operator == (const N_bytes<endian::little> &, uint64);
    template bool operator == (const Z_bytes<endian::little, complement::ones> &, int64);
    template bool operator == (const Z_bytes<endian::little, complement::twos> &, int64);
    
    template Z_bytes<endian::big, complement::ones> operator ~ (const N_bytes<endian::big> &);
    template Z_bytes<endian::big, complement::ones> operator ~ (const Z_bytes<endian::big, complement::ones> &);
    
    template Z_bytes<endian::big, complement::ones> operator ^ (const N_bytes<endian::big> &, const N_bytes<endian::big> &);
    template Z_bytes<endian::big, complement::ones> operator ^
        (const Z_bytes<endian::big, complement::ones> &, const Z_bytes<endian::big, complement::ones> &);
    template Z_bytes<endian::big, complement::ones> operator ^
        (const Z_bytes<endian::big, complement::ones> &, const N_bytes<endian::big> &);
    template Z_bytes<endian::big, complement::ones> operator ^
        (const N_bytes<endian::big> &, const Z_bytes<endian::big, complement::ones> &);
    
    template N_bytes<endian::big> operator & (const N_bytes<endian::big> &, const N_bytes<endian::big> &);
    template N_bytes<endian::big> operator | (const N_bytes<endian::big> &, const N_bytes<endian::big> &);
    template Z_bytes<endian::big, complement::ones> operator - (const N_bytes<endian::big> &);
    
    template Z_bytes<endian::big, complement::ones> operator &
        (const Z_bytes<endian::big, complement::ones> &, const Z_bytes<endian::big, complement::ones> &);
    template Z_bytes<endian::big, complement::ones> operator &
        (const N_bytes<endian::big> &, const Z_bytes<endian::big, complement::ones> &);
    template Z_bytes<endian::big, complement::ones> operator &
        (const Z_bytes<endian::big, complement::ones> &, const N_bytes<endian::big> &);
    
    template Z_bytes<endian::big, complement::ones> operator |
        (const Z_bytes<endian::big, complement::ones> &, const Z_bytes<endian::big, complement::ones> &);
    template Z_bytes<endian::big, complement::ones> operator |
        (const N_bytes<endian::big> &, const Z_bytes<endian::big, complement::ones> &);
    template Z_bytes<endian::big, complement::ones> operator |
        (const Z_bytes<endian::big, complement::ones> &, const N_bytes<endian::big> &);
    
    template Z_bytes<endian::big, complement::ones> operator - (const Z_bytes<endian::big, complement::ones> &);
    
    template Z_bytes<endian::big, complement::twos> operator &
        (const Z_bytes<endian::big, complement::twos>&, const Z_bytes<endian::big, complement::twos> &);
    template Z_bytes<endian::big, complement::twos> operator &
        (const N_bytes<endian::big> &, const Z_bytes<endian::big, complement::twos> &);
    template Z_bytes<endian::big, complement::twos> operator &
        (const Z_bytes<endian::big, complement::twos>&, const N_bytes<endian::big> &);
    
    template Z_bytes<endian::big, complement::twos> operator |
        (const Z_bytes<endian::big, complement::twos> &, const Z_bytes<endian::big, complement::twos> &);
    template Z_bytes<endian::big, complement::twos> operator |
        (const N_bytes<endian::big> &, const Z_bytes<endian::big, complement::twos> &);
    template Z_bytes<endian::big, complement::twos> operator |
        (const Z_bytes<endian::big, complement::twos> &, const N_bytes<endian::big> &);
    
    template Z_bytes<endian::big, complement::twos> operator - (const Z_bytes<endian::big, complement::twos> &);
    
    template Z_bytes<endian::little, complement::ones> operator ~ (const N_bytes<endian::little> &);
    template Z_bytes<endian::little, complement::ones> operator ~ (const Z_bytes<endian::little, complement::ones> &);
    
    template Z_bytes<endian::little, complement::ones> operator ^ (const N_bytes<endian::little> &, const N_bytes<endian::little> &);
    template Z_bytes<endian::little, complement::ones> operator ^
        (const Z_bytes<endian::little, complement::ones> &, const Z_bytes<endian::little, complement::ones> &);
    template Z_bytes<endian::little, complement::ones> operator ^
        (const Z_bytes<endian::little, complement::ones> &, const N_bytes<endian::little> &);
    template Z_bytes<endian::little, complement::ones> operator ^
        (const N_bytes<endian::little> &, const Z_bytes<endian::little, complement::ones> &);
    
    template N_bytes<endian::little> operator & (const N_bytes<endian::little> &, const N_bytes<endian::little> &);
    template N_bytes<endian::little> operator | (const N_bytes<endian::little> &, const N_bytes<endian::little> &);
    template Z_bytes<endian::little, complement::ones> operator - (const N_bytes<endian::little> &);
    
    template Z_bytes<endian::little, complement::ones> operator &
        (const Z_bytes<endian::little, complement::ones>&, const Z_bytes<endian::little, complement::ones> &);
    template Z_bytes<endian::little, complement::ones> operator &
        (const N_bytes<endian::little> &, const Z_bytes<endian::little, complement::ones> &);
    template Z_bytes<endian::little, complement::ones> operator &
        (const Z_bytes<endian::little, complement::ones>&, const N_bytes<endian::little> &);
    
    template Z_bytes<endian::little, complement::ones> operator |
        (const Z_bytes<endian::little, complement::ones> &, const Z_bytes<endian::little, complement::ones> &);
    template Z_bytes<endian::little, complement::ones> operator |
        (const N_bytes<endian::little> &, const Z_bytes<endian::little, complement::ones> &);
    template Z_bytes<endian::little, complement::ones> operator |
        (const Z_bytes<endian::little, complement::ones> &, const N_bytes<endian::little> &);
    
    template Z_bytes<endian::little, complement::ones> operator - (const Z_bytes<endian::little, complement::ones> &);
    
    template Z_bytes<endian::little, complement::twos> operator &
        (const Z_bytes<endian::little, complement::twos> &, const Z_bytes<endian::little, complement::twos> &);
    template Z_bytes<endian::little, complement::twos> operator &
        (const N_bytes<endian::little> &, const Z_bytes<endian::little, complement::twos> &);
    template Z_bytes<endian::little, complement::twos> operator &
        (const Z_bytes<endian::little, complement::twos> &, const N_bytes<endian::little> &);
    
    template Z_bytes<endian::little, complement::twos> operator |
        (const Z_bytes<endian::little, complement::twos> &, const Z_bytes<endian::little, complement::twos> &);
    template Z_bytes<endian::little, complement::twos> operator |
        (const N_bytes<endian::little> &, const Z_bytes<endian::little, complement::twos> &);
    template Z_bytes<endian::little, complement::twos> operator |
        (const Z_bytes<endian::little, complement::twos> &, const N_bytes<endian::little> &);
    
    template Z_bytes<endian::little, complement::twos> operator - (const Z_bytes<endian::little, complement::twos> &);
    
}

#endif
