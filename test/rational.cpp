// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/fraction.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename Q> void test_divide_by_zero () {

        EXPECT_THROW (Q {1} / Q {0}, math::division_by_zero);

    }

    template <typename Q> void test_lowest_terms () {

        EXPECT_EQ (Q {14} / Q {6}, Q {7} / Q {3});

    }

    template <typename Q> void test_compare () {}

    template <typename Q> void test_arithmetic () {}

    template <Integer num, typename denum>
    requires requires (const num &z, const num &n) {
        { over<num, denum> (z, n) } -> Same<math::fraction<num, denum>>;
    } //&& NumberComparable<math::fraction<num, denum>>
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

    template <typename tuple>
    struct RationalFraction : ::testing::Test {
        using numerator = typename std::tuple_element<0, tuple>::type;
        using denominator = typename std::tuple_element<1, tuple>::type;
    };

    using test_cases = ::testing::Types<
        tuple<int64, int64>,
        tuple<int64_little, int64_little>,
        tuple<int64_big, int64_big>,
        tuple<int80, int80>,
        tuple<int80_little, int80_little>,
        tuple<int80_big, int80_big>,
        tuple<int128, int128>,
        tuple<int128_little, int128_little>,
        tuple<int128_big, int128_big>,
        tuple<Z, N>,
        tuple<Z_bytes_little, N_bytes_big>,
        tuple<Z_bytes_big, N_bytes_big>,
        tuple<Z_bytes_BC_little, Z_bytes_BC_little>,
        tuple<Z_bytes_BC_big, Z_bytes_BC_big>,
        tuple<math::Z_bytes<endian::little, unsigned short>, math::N_bytes<endian::little, unsigned short>>,
        tuple<math::Z_bytes<endian::big, unsigned int>, math::N_bytes<endian::big, unsigned int>>,
        tuple<math::Z_bytes<endian::little, unsigned long>, math::N_bytes<endian::little, unsigned long>>,
        tuple<math::Z_bytes<endian::big, unsigned long long>, math::N_bytes<endian::big, unsigned long long>>,
        tuple<math::Z_bytes_BC<endian::big, unsigned short>, math::Z_bytes_BC<endian::big, unsigned short>>,
        tuple<math::Z_bytes_BC<endian::little, unsigned int>, math::Z_bytes_BC<endian::little, unsigned int>>,
        tuple<math::Z_bytes_BC<endian::big, unsigned long>, math::Z_bytes_BC<endian::big, unsigned long>>,
        tuple<math::Z_bytes_BC<endian::little, unsigned long long>, math::Z_bytes_BC<endian::little, unsigned long long>>,
        tuple<dec_int, dec_uint>,
        tuple<hex_int, hex_uint>,
        tuple<hex_int_BC, hex_int_BC>>;

    TYPED_TEST_SUITE (RationalFraction, test_cases);

    TYPED_TEST (RationalFraction, TestType) {
        using denominator = typename TestFixture::denominator;
        using numerator = typename TestFixture::numerator;

        test_fraction<numerator, denominator> {} ();
    }

}
