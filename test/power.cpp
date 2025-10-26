// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/power.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename NN> void test_power_case (const string &base, const string &exp, const string &expected) {
        EXPECT_EQ ((pow (NN (N {base}), N (N {exp}))), NN {N (expected)});
    }

    template <typename N> void test_power () {
        test_power_case<N> ("5", "20", "95367431640625");
        test_power_case<N> ("2", "100", "1267650600228229401496703205376");
    }

    template <typename X>
    struct Power : ::testing::Test {
        using number = X;
    };

    using power_test_cases = ::testing::Types<
        N, Z, N_bytes_little, N_bytes_big, Z_bytes_little, Z_bytes_big, Z_bytes_BC_little, Z_bytes_BC_big,
        uint128, int128, uint160, int160, uint224, int224, uint256, int256, dec_uint, base58_uint>;

    TYPED_TEST_SUITE (Power, power_test_cases);

    TYPED_TEST (Power, Power) {

        test_power<typename TestFixture::number> ();

    }

}

