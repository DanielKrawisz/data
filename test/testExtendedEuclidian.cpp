// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/extended_euclidian.hpp"
#include "gtest/gtest.h"

namespace data::math {
    
    using Nb = number::N_bytes<endian::big, byte>;
    using Nl = number::N_bytes<endian::little, byte>;
    
    using Zb1 = Z_bytes_big;
    using Zl1 = Z_bytes_little;
    using Zb2 = Z_bytes_BC_big;
    using Zl2 = Z_bytes_BC_little;

    template <typename tuple>
    struct extended_euclidian_test : ::testing::Test {
        using natural = typename std::tuple_element<1, tuple>::type;
        using integer = typename std::tuple_element<0, tuple>::type;
    };

    using test_cases = ::testing::Types<
        tuple<uint64, uint64>, 
        tuple<int64, int64>, 
        tuple<uint64_little, uint64_little>,
        tuple<int64_little, int64_little>,
        tuple<uint64_big, uint64_big>,
        tuple<int64_big, int64_big>,
        tuple<uint80, uint80>,
        tuple<uint80_little, uint80_little>,
        tuple<uint80_big, uint80_big>,
        tuple<int80, int80>,
        tuple<int80_little, int80_little>,
        tuple<int80_big, int80_big>,
        tuple<int128, int128>,
        tuple<int128_little, int128_little>,
        tuple<int128_big, int128_big>,
        tuple<uint128, uint128>,
        tuple<uint128_little, uint128_little>,
        tuple<uint128_big, uint128_big>,
        tuple<Z, N>,
        tuple<Zl1, Nl>,
        tuple<Zb1, Nb>,
        tuple<Z_bytes_BC_little, Z_bytes_BC_little>,
        tuple<Z_bytes_BC_big, Z_bytes_BC_big>,
        tuple<dec_int, dec_uint>,
        tuple<hex_int, hex_uint>,
        tuple<hex_int_BC, hex_int_BC>>;

    TYPED_TEST_SUITE (extended_euclidian_test, test_cases);

    TYPED_TEST (extended_euclidian_test, TestType) {
        using N = typename TestFixture::natural;
        using Z = typename TestFixture::integer;

        using X = math::number::euclidian::extended<N, Z>;
        EXPECT_THROW ((void) X::algorithm (N {0}, N {0}).GCD, math::division_by_zero);

        EXPECT_EQ (X::algorithm (N {1}, N {1}).GCD, 1);

        EXPECT_EQ (X::algorithm (N {2}, N {4}).GCD, 2);

        EXPECT_EQ (X::algorithm (N {1145}, N {916}).GCD, 229);
    }
    
}
