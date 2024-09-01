
// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename X> void test_complex (X zero, X one, X i) {
        EXPECT_NE (zero, one);
        EXPECT_NE (zero, i);
        EXPECT_NE (one, i);

        EXPECT_EQ (zero * zero, zero);
        EXPECT_EQ (zero * one, zero);
        EXPECT_EQ (one * zero, zero);
        EXPECT_EQ (zero * i, zero);
        EXPECT_EQ (i * zero, zero);
        EXPECT_EQ (one * one, one);
        EXPECT_EQ (i * one, i);
        EXPECT_EQ (one * i, i);
        EXPECT_EQ (i * i, -one);

        EXPECT_EQ (~zero, zero);
        EXPECT_EQ (~one, one);
        EXPECT_EQ (~i, -i);
    };

    template <typename X> void test_quaternion (X zero, X one, X i, X j) {
        test_complex<X> (zero, one, i);
        test_complex<X> (zero, one, j);
        EXPECT_NE (i, j);
        EXPECT_NE (-i, j);

        auto k = j * i;
        EXPECT_NE (zero, k);
        EXPECT_NE (one, k);
        EXPECT_NE (i, k);
        EXPECT_NE (j, k);

        EXPECT_EQ (k * k, -one);
        EXPECT_EQ (i * j, -k);
        EXPECT_EQ (j * k, -i);
        EXPECT_EQ (k * i, -j);
        EXPECT_EQ (k * j * i, -one);
    };

    template <typename X> void test_octonion (X zero, X one, X i, X j, X k) {
        test_quaternion<X> (zero, one, i, j);
        test_quaternion<X> (zero, one, j, k);
        test_quaternion<X> (zero, one, k, i);

        auto e3 = i * j;
        auto e5 = j * k;
        auto e6 = k * i;
        auto e7 = i * (j * k);

        EXPECT_NE (i, e3);
        EXPECT_NE (-i, e3);
        EXPECT_NE (j, e3);
        EXPECT_NE (-j, e3);
        EXPECT_NE (k, e3);
        EXPECT_NE (-k, e3);

        EXPECT_NE (i, e5);
        EXPECT_NE (-i, e5);
        EXPECT_NE (j, e5);
        EXPECT_NE (-j, e5);
        EXPECT_NE (k, e5);
        EXPECT_NE (-k, e5);

        EXPECT_NE (i, e6);
        EXPECT_NE (-i, e6);
        EXPECT_NE (j, e6);
        EXPECT_NE (-j, e6);
        EXPECT_NE (k, e6);
        EXPECT_NE (-k, e6);

        EXPECT_NE (i, e7);
        EXPECT_NE (-i, e7);
        EXPECT_NE (j, e7);
        EXPECT_NE (-j, e7);
        EXPECT_NE (k, e7);
        EXPECT_NE (-k, e7);

        EXPECT_NE (e3, e5);
        EXPECT_NE (e3, e6);
        EXPECT_NE (e3, e7);

        EXPECT_NE (-e3, e5);
        EXPECT_NE (-e3, e6);
        EXPECT_NE (-e3, e7);

        EXPECT_NE (e5, e6);
        EXPECT_NE (e5, e7);

        EXPECT_NE (-e5, e6);
        EXPECT_NE (-e5, e7);

        EXPECT_NE (e6, e7);
        EXPECT_NE (-e6, e7);

        auto x = (i * j) * k;

        EXPECT_EQ (e7, -x);
    };

    TEST (CaylayDicksonTest, TestComplex) {

        // rational complexes
        test_complex<QC> (QC {0}, QC {1}, QC::I ());

        // rational quaternions
        test_quaternion<QH> (QH {0}, QH {1}, QH::I (), QH::J ());

        // rational octonions
        test_octonion<QO> (QO {0}, QO {1}, QO::E1 (), QO::E2 (), QO::E4 ());

        // Caylay-Dickson floating points.
        test_complex<complex64> (complex64 {0}, complex64 {1}, complex64::I ());
        test_complex<complex128> (complex128 {0}, complex128 {1}, complex128::I ());
        test_quaternion<quat128> (quat128 {0}, quat128 {1}, quat128::I (), quat128::J ());
        test_quaternion<quat256> (quat256 {0}, quat256 {1}, quat256::I (), quat256::J ());
        test_octonion<oct256> (oct256 {0}, oct256 {1}, oct256::E1 (), oct256::E2 (), oct256::E4 ());
        test_octonion<oct512> (oct512 {0}, oct512 {1}, oct512::E1 (), oct512::E2 (), oct512::E4 ());

    }

}
