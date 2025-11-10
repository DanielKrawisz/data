// Copyright (c) 2024 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/infinite.hpp"
#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename N>
    struct SignedInfinite : ::testing::Test {
        using value = N;
    };

    template <typename Z>
    struct UnsignedInfinite : ::testing::Test {
        using value = Z;
    };

    using unsigned_test_cases = ::testing::Types<
        byte,
        uint32,
        uint32_little,
        uint32_big,
        uint64,
        uint64_little,
        uint64_big,
        N,
        N_bytes_little,
        N_bytes_big,
        dec_uint,
        hex_uint,
        base58_uint>;

    using signed_test_cases = ::testing::Types<
        char,
        int32,
        int32_little,
        int32_big,
        int64,
        int64_little,
        int64_big,
        double,
        Z,
        Z_bytes_little,
        Z_bytes_big,
        Z_bytes_BC_little,
        dec_int,
        hex_int,
        hex_int_BC>;

    TYPED_TEST_SUITE (UnsignedInfinite, unsigned_test_cases);
    TYPED_TEST_SUITE (SignedInfinite, signed_test_cases);

    TYPED_TEST (SignedInfinite, Infinite) {
        using val = typename TestFixture::value;
        using inf = math::signed_limit<val>;

        EXPECT_TRUE (inf::infinity () > inf::negative_infinity ());
        EXPECT_TRUE (inf::infinity () > inf {0});
        EXPECT_TRUE (inf::infinity () > val {0});

        EXPECT_FALSE (inf::infinity () < inf::negative_infinity ());
        EXPECT_FALSE (inf::infinity () < inf {0});
        EXPECT_FALSE (inf::infinity () < val {0});

        EXPECT_TRUE (inf::negative_infinity () < inf::infinity ());
        EXPECT_TRUE (inf {0} < inf::infinity ());
        EXPECT_TRUE (val {0} < inf::infinity ());

        EXPECT_FALSE (inf::negative_infinity () > inf::infinity ());
        EXPECT_FALSE (inf {0} > inf::infinity ());
        EXPECT_FALSE (val {0} > inf::infinity ());

        EXPECT_EQ (inf::infinity (), inf::infinity ());
        EXPECT_EQ (inf::negative_infinity (), inf::negative_infinity ());
        EXPECT_NE (inf::infinity (), inf::negative_infinity ());
        EXPECT_NE (inf::infinity (), inf {0});
        EXPECT_NE (inf::infinity (), val {0});
        EXPECT_EQ (inf {0}, val {0});
    }

    TYPED_TEST (UnsignedInfinite, Infinite) {
        using val = typename TestFixture::value;
        using inf = math::unsigned_limit<val>;

        EXPECT_TRUE (inf::infinity () > inf {0});
        EXPECT_TRUE (inf::infinity () > val {0});

        EXPECT_FALSE (inf::infinity () < inf {0});
        EXPECT_FALSE (inf::infinity () < val {0});

        EXPECT_TRUE (inf {0} < inf::infinity ());
        EXPECT_TRUE (val {0} < inf::infinity ());

        EXPECT_FALSE (inf {0} > inf::infinity ());
        EXPECT_FALSE (val {0} > inf::infinity ());

        EXPECT_EQ (inf::infinity (), inf::infinity ());
        EXPECT_NE (inf::infinity (), inf {0});
        EXPECT_NE (inf::infinity (), val {0});
        EXPECT_EQ (inf {0}, val {0});
    }

}


