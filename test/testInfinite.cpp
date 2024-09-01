// Copyright (c) 2024 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/infinite.hpp"
#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename X> void test_signed_infinite () {
        using inf = math::signed_limit<X>;

        EXPECT_TRUE (inf::infinity () > inf::negative_infinity ());
        EXPECT_TRUE (inf::infinity () > inf {0});
        EXPECT_TRUE (inf::infinity () > X {0});

        EXPECT_FALSE (inf::infinity () < inf::negative_infinity ());
        EXPECT_FALSE (inf::infinity () < inf {0});
        EXPECT_FALSE (inf::infinity () < X {0});

        EXPECT_TRUE (inf::negative_infinity () < inf::infinity ());
        EXPECT_TRUE (inf {0} < inf::infinity ());
        EXPECT_TRUE (X {0} < inf::infinity ());

        EXPECT_FALSE (inf::negative_infinity () > inf::infinity ());
        EXPECT_FALSE (inf {0} > inf::infinity ());
        EXPECT_FALSE (X {0} > inf::infinity ());

        EXPECT_EQ (inf::infinity (), inf::infinity ());
        EXPECT_EQ (inf::negative_infinity (), inf::negative_infinity ());
        EXPECT_NE (inf::infinity (), inf::negative_infinity ());
        EXPECT_NE (inf::infinity (), inf {0});
        EXPECT_NE (inf::infinity (), X {0});
        EXPECT_EQ (inf {0}, X {0});
    }

    template <typename X> void test_unsigned_infinite () {
        using inf = math::unsigned_limit<X>;

        EXPECT_TRUE (inf::infinity () > inf {0});
        EXPECT_TRUE (inf::infinity () > X {0});

        EXPECT_FALSE (inf::infinity () < inf {0});
        EXPECT_FALSE (inf::infinity () < X {0});

        EXPECT_TRUE (inf {0} < inf::infinity ());
        EXPECT_TRUE (X {0} < inf::infinity ());

        EXPECT_FALSE (inf {0} > inf::infinity ());
        EXPECT_FALSE (X {0} > inf::infinity ());

        EXPECT_EQ (inf::infinity (), inf::infinity ());
        EXPECT_NE (inf::infinity (), inf {0});
        EXPECT_NE (inf::infinity (), X {0});
        EXPECT_EQ (inf {0}, X {0});
    }

    TEST (InfiniteTest, TestInfinite) {

        test_unsigned_infinite<byte> ();
        test_signed_infinite<char> ();
        test_unsigned_infinite<uint32> ();
        test_unsigned_infinite<uint32_little> ();
        test_unsigned_infinite<uint32_big> ();
        test_unsigned_infinite<uint64> ();
        test_unsigned_infinite<uint64_little> ();
        test_unsigned_infinite<uint64_big> ();
        test_unsigned_infinite<int32> ();
        test_signed_infinite<int32_little> ();
        test_signed_infinite<int32_big> ();
        test_signed_infinite<int64> ();
        test_signed_infinite<int64_little> ();
        test_signed_infinite<int64_big> ();
        test_signed_infinite<double> ();
        test_unsigned_infinite<N> ();
        test_signed_infinite<Z> ();
        test_signed_infinite<Z_bytes_little> ();
        test_unsigned_infinite<N_bytes_little> ();
        test_signed_infinite<Z_bytes_big> ();
        test_unsigned_infinite<N_bytes_big> ();
        test_signed_infinite<Z_bytes_twos_little> ();
        test_unsigned_infinite<dec_uint> ();
        test_unsigned_infinite<hex_uint> ();
        test_signed_infinite<dec_int> ();
        test_unsigned_infinite<base58_uint> ();
        test_signed_infinite<hex_int> ();
        test_signed_infinite<hex_int_twos> ();

    }

}


