// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// note: if the ordering of these next two includes is reversed,
// the program breaks. This is quite fragile and it is due to
// using overloads of data::sign. This is very bad.
#include <data/numbers.hpp>
#include <data/math.hpp>

#include "gtest/gtest.h"

using namespace data;

template <math::ring Q>
struct test_polynomial {
    using poly = polynomial<Q, uint32>;

    poly X;
    poly P1;
    poly P2;

    test_polynomial () :
    X {poly::var ()},
    P1 {(X ^ 2) + 1},
    P2 {X * 3 + 2} {
        /*
        EXPECT_EQ (X.degree (), 1);
        EXPECT_EQ (P1.degree (), 2);
        EXPECT_EQ (P2.degree (), 1);

        EXPECT_FALSE (P1 == P2);

        EXPECT_EQ ((X ^ 3).degree (), 3);

        auto sum = P1 + P2;
        auto product = P1 * P2;
        auto comp_right = P1 (P2);
        auto comp_left = P2 (P1);

        auto expected_sum = (X ^ 2u) + X * 3u + 3u;
        auto expected_product = (X ^ 3u) * 3u + (X ^ 2u) * 2u + X * 3u + 2u;
        auto expected_comp_right = (X ^ 2u) * 9u + X * 12u + 5u;
        auto expected_comp_left = (X ^ 2u) * 3u + 5u;

        EXPECT_TRUE (sum == expected_sum);
        EXPECT_TRUE (product == expected_product) << "expected " << P1 << " * " << P2 << " -> " << expected_product << " but got " << product;
        EXPECT_TRUE (comp_right == expected_comp_right);
        EXPECT_TRUE (comp_left == expected_comp_left);

        poly expected_d_p1 = X * 2;
        poly expected_d_p2 = poly {3};
        auto expected_d_sum = X * 2 + 3;
        auto expected_d_product = (X ^ 2u) * 9u + X * 4u + 3u;
        auto expected_d_comp_right = X * 18u + 12;
        auto expected_d_comp_left = X * 6u;

        EXPECT_TRUE (P1.derivative () == expected_d_p1);
        EXPECT_TRUE (P2.derivative () == expected_d_p2);
        EXPECT_TRUE (sum.derivative () == expected_d_sum);
        EXPECT_TRUE (product.derivative () == expected_d_product);
        EXPECT_TRUE (comp_right.derivative () == expected_d_comp_right);
        EXPECT_TRUE (comp_left.derivative () == expected_d_comp_left);*/

    }

    void test_polynomial_division () {
        auto expected_quotient = X * Q {1, 3} + Q {2, 9};
        auto expected_remainder = Q {13, 9};

        auto d = divmod (P1, math::nonzero {P2});
        EXPECT_EQ (d.Quotient, expected_quotient);
        EXPECT_EQ (d.Remainder, expected_remainder);
    }
};

template <math::ring Z> requires math::field<fraction<Z>>
struct test_polynomial_division : test_polynomial<Z> {
    test_polynomial_division () : test_polynomial<Z> {} {
        test_polynomial<Z>::test_polynomial_division ();
    }
};

using test_cases = ::testing::Types<
    Z,
    Z_bytes_little,
    Z_bytes_big,
    Z_bytes_BC_little,
    Z_bytes_BC_big,/*
    int64,
    int64_little,
    int64_big,
    int128,
    int128_little,
    int128_big,*/
    dec_int,
    hex_int,
    hex_int_BC,

    fraction<Z>,
    fraction<Z_bytes_little>,
    fraction<Z_bytes_big>,
    fraction<Z_bytes_BC_little>,
    fraction<Z_bytes_BC_big>,/*
    fraction<int64>,
    fraction<int64_little>,
    fraction<int64_big>,
    fraction<int128>,
    fraction<int128_little>,
    fraction<int128_big>,*/
    fraction<dec_int>,
    fraction<hex_int>,
    fraction<hex_int_BC>/*,

    math::complex<fraction<Z>>,
    math::complex<fraction<Z_bytes_little>>,
    math::complex<fraction<Z_bytes_big>>,
    math::complex<fraction<Z_bytes_BC_little>>,
    math::complex<fraction<Z_bytes_BC_big>>,
    math::complex<fraction<int64>>,
    math::complex<fraction<int64_little>>,
    math::complex<fraction<int64_big>>,
    math::complex<fraction<int128>>,
    math::complex<fraction<int128_little>>,
    math::complex<fraction<int128_big>>,
    math::complex<fraction<dec_int>>,
    math::complex<fraction<hex_int>>,
    math::complex<fraction<hex_int_BC>>*/>;


template <typename N>
struct Polynomial : ::testing::Test {
    using base = N;
};

TYPED_TEST_SUITE (Polynomial, test_cases);

TYPED_TEST (Polynomial, Ring) {
    using Q = typename TestFixture::base;

    using poly = polynomial<Q, uint32>;

    poly X = poly::var ();
    poly P1 = (X ^ 2) + 1;
    poly P2 = X * 3 + 2;

    EXPECT_EQ (X.degree (), 1);
    EXPECT_EQ (P1.degree (), 2);
    EXPECT_EQ (P2.degree (), 1);

    EXPECT_FALSE (P1 == P2);

    EXPECT_EQ ((X ^ 3).degree (), 3);

    auto sum = P1 + P2;
    auto product = P1 * P2;
    auto comp_right = P1 (P2);
    auto comp_left = P2 (P1);

    auto expected_sum = (X ^ 2u) + X * 3u + 3u;
    auto expected_product = (X ^ 3u) * 3u + (X ^ 2u) * 2u + X * 3u + 2u;
    auto expected_comp_right = (X ^ 2u) * 9u + X * 12u + 5u;
    auto expected_comp_left = (X ^ 2u) * 3u + 5u;

    EXPECT_TRUE (sum == expected_sum);
    EXPECT_TRUE (product == expected_product) << "expected " << P1 << " * " << P2 << " -> " << expected_product << " but got " << product;
    EXPECT_TRUE (comp_right == expected_comp_right);
    EXPECT_TRUE (comp_left == expected_comp_left);

    poly expected_d_p1 = X * 2;
    poly expected_d_p2 = poly {3};
    auto expected_d_sum = X * 2 + 3;
    auto expected_d_product = (X ^ 2u) * 9u + X * 4u + 3u;
    auto expected_d_comp_right = X * 18u + 12;
    auto expected_d_comp_left = X * 6u;

    EXPECT_TRUE (P1.derivative () == expected_d_p1);
    EXPECT_TRUE (P2.derivative () == expected_d_p2);
    EXPECT_TRUE (sum.derivative () == expected_d_sum);
    EXPECT_TRUE (product.derivative () == expected_d_product);
    EXPECT_TRUE (comp_right.derivative () == expected_d_comp_right);
    EXPECT_TRUE (comp_left.derivative () == expected_d_comp_left);

}

// TODO division
