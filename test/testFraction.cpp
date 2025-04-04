// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename Q> void test_divide_by_zero () {

    }

    template <typename Q> void test_lowest_terms () {}
    template <typename Q> void test_compare () {}
    template <typename Q> void test_arithmetic () {}

    template <math::ring Z>
    struct test_fraction {
        void operator () () {
            using Q = math::fraction<Z>;

            EXPECT_TRUE (Q {0}.valid ());
            EXPECT_TRUE (Q {1}.valid ());
            EXPECT_TRUE (Q {-1}.valid ());

            test_divide_by_zero<Q> ();
            test_lowest_terms<Q> ();
            test_compare<Q> ();
            test_arithmetic<Q> ();
        }
    };

    TEST (FractionTest, TestFraction) {

        //test_fraction<int64> {} ();
        test_fraction<Z> {} ();
        test_fraction<Z_bytes_little> {} ();
        test_fraction<Z_bytes_big> {} ();
        test_fraction<Z_bytes_twos_big> {} ();
        test_fraction<Z_bytes_twos_little> {} ();
        test_fraction<int_big<9>> {} ();
        test_fraction<int_little<9>> {} ();
        test_fraction<int_big<10>> {} ();
        test_fraction<int_little<10>> {} ();
        test_fraction<int_big<11>> {} ();
        test_fraction<int_little<11>> {} ();
        test_fraction<int_big<12>> {} ();
        test_fraction<int_little<12>> {} ();
        test_fraction<dec_int> {} ();
        test_fraction<hex_int> {} ();
        test_fraction<hex_int_twos> {} ();

    }

}
