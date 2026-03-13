// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

// defines float32 and float64
#include <data/float.hpp>


#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/integral.hpp>

#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/aks.hpp>

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

    static_assert (Integer<long int>);

    static_assert (Natural<N>);
    static_assert (number_system<Z, N>);

    static_assert (Integer<Z>);

    static_assert (Natural<N_bytes_little>);
    static_assert (Natural<N_bytes_big>);

    static_assert (number_system<Z_bytes_little, N_bytes_little>);
    static_assert (number_system<Z_bytes_big, N_bytes_big>);

    static_assert (Integer<Z_bytes_BC_little>);
    static_assert (Integer<Z_bytes_BC_big>);
    static_assert (number_system<Z_bytes_BC_little>);
    static_assert (number_system<Z_bytes_BC_big>);

    static_assert (Natural<dec_uint>);
    static_assert (number_system<dec_int, dec_uint>);

    static_assert (Natural<hex_uint>);
    static_assert (number_system<hex_int, hex_uint>);

    static_assert (Integer<hex_int_BC>);
    static_assert (number_system<hex_int_BC>);

    static_assert (Natural<base58_uint>);

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
