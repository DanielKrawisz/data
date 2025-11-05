// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename N> concept has_numeric_limits = requires {
        { []() constexpr { (void) math::numeric_limits<N>::max (); }() };
        { []() constexpr { (void) math::numeric_limits<N>::min (); }() };
    };

    static_assert (has_numeric_limits<uint64>);
    static_assert (has_numeric_limits<int64>);
    static_assert (has_numeric_limits<uint64_big>);
    static_assert (has_numeric_limits<int64_big>);
    static_assert (has_numeric_limits<uint64_little>);
    static_assert (has_numeric_limits<int64_little>);

    static_assert (has_numeric_limits<uint128>);
    static_assert (has_numeric_limits<int128>);
    static_assert (has_numeric_limits<uint128_big>);
    static_assert (has_numeric_limits<int128_big>);
    static_assert (has_numeric_limits<uint128_little>);
    static_assert (has_numeric_limits<int128_little>);

    static_assert (has_numeric_limits<uint160>);
    static_assert (has_numeric_limits<int160>);
    static_assert (has_numeric_limits<uint160_big>);
    static_assert (has_numeric_limits<int160_big>);
    static_assert (has_numeric_limits<uint160_little>);
    static_assert (has_numeric_limits<int160_little>);

    static_assert (has_numeric_limits<uint256>);
    static_assert (has_numeric_limits<int256>);
    static_assert (has_numeric_limits<uint256_big>);
    static_assert (has_numeric_limits<int256_big>);
    static_assert (has_numeric_limits<uint256_little>);
    static_assert (has_numeric_limits<int256_little>);
    // TODO in order to make these work, we need
    // to ensure that nullptr works like zero for N and Z.
    // This is not normally allowed by GMP.
/*
    static_assert (has_numeric_limits<N>);
    static_assert (has_numeric_limits<Z>);
    static_assert (has_numeric_limits<N_bytes_big>);
    static_assert (has_numeric_limits<Z_bytes_big>);
    static_assert (has_numeric_limits<N_bytes_little>);
    static_assert (has_numeric_limits<Z_bytes_little>);
    static_assert (has_numeric_limits<Z_bytes_BC_big>);
    static_assert (has_numeric_limits<Z_bytes_BC_little>);

    static_assert (has_numeric_limits<base58_uint>);
    static_assert (has_numeric_limits<dec_uint>);
    static_assert (has_numeric_limits<hex_uint>);
    static_assert (has_numeric_limits<dec_int>);
    static_assert (has_numeric_limits<hex_int>);
    static_assert (has_numeric_limits<hex_int_BC>);*/

    static_assert (has_numeric_limits<modular<uint64 {2}>>);
    static_assert (has_numeric_limits<modular<int64 {3}>>);
    static_assert (has_numeric_limits<modular<uint64_big {5}>>);
    static_assert (has_numeric_limits<modular<int64_big {7}>>);
    static_assert (has_numeric_limits<modular<uint64_little {11}>>);
    static_assert (has_numeric_limits<modular<int64_little {13}>>);

    static_assert (has_numeric_limits<modular<uint128 {17}>>);
    static_assert (has_numeric_limits<modular<int128 {19}>>);
    static_assert (has_numeric_limits<modular<uint128_big {23}>>);
    static_assert (has_numeric_limits<modular<int128_big {29}>>);
    static_assert (has_numeric_limits<modular<uint128_little {31}>>);
    static_assert (has_numeric_limits<modular<int128_little {37}>>);

    static_assert (has_numeric_limits<modular<uint160 {41}>>);
    static_assert (has_numeric_limits<modular<int160 {43}>>);
    static_assert (has_numeric_limits<modular<uint160_big {47}>>);
    static_assert (has_numeric_limits<modular<int160_big {53}>>);
    static_assert (has_numeric_limits<modular<uint160_little {59}>>);
    static_assert (has_numeric_limits<modular<int160_little {61}>>);

    static_assert (has_numeric_limits<modular<uint256 {67}>>);
    static_assert (has_numeric_limits<modular<int256 {71}>>);
    static_assert (has_numeric_limits<modular<uint256_big {73}>>);
    static_assert (has_numeric_limits<modular<int256_big {79}>>);
    static_assert (has_numeric_limits<modular<uint256_little {83}>>);
    static_assert (has_numeric_limits<modular<int256_little {89}>>);

}
