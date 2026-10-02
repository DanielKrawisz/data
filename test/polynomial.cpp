// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// note: if the ordering of these next two includes is reversed,
// the program breaks. This is quite fragile and it is due to
// using overloads of data::sign. This is very bad.
#include <data/numbers.hpp>
#include <data/math.hpp>

#include <gtest/gtest.h>

using namespace data;

using polynomial_test_cases = ::testing::Types<
    Z,
    Z_bytes_little,
    Z_bytes_big,
    Z_bytes_BC_little,
    Z_bytes_BC_big,
    int64,
    int64_little,
    int64_big,
    int128,
    int128_little,
    int128_big,
    dec_int,
    hex_int,
    hex_int_BC>;

template <typename Z>
struct RealPolynomialRing : ::testing::Test {
    using base = Z;
};

template <typename Z>
struct RealPolynomialField : ::testing::Test {
    using Q = math::fraction<Z>;
    using mod_17 = math::number::modular<uint32 {17}>;
    using mod_19 = math::number::modular<uint32 {19}>;
};

template <typename Z>
struct ComplexPolynomialRing : ::testing::Test {
    using base = math::complex<Z>;
};

template <typename Z>
struct ComplexPolynomialField : ::testing::Test {
    using base = math::fraction<math::complex<Z>>;
};

TYPED_TEST_SUITE (RealPolynomialRing, polynomial_test_cases);

TYPED_TEST_SUITE (RealPolynomialField, polynomial_test_cases);

TYPED_TEST_SUITE (ComplexPolynomialRing, polynomial_test_cases);

TYPED_TEST_SUITE (ComplexPolynomialField, polynomial_test_cases);

template <typename Z> void test_polynomial_basic_algebra () {
    using poly = polynomial<Z, int32>;

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

    auto expected_sum = (X ^ 2) + X * 3u + 3u;
    auto expected_product = (X ^ 3) * 3u + (X ^ 2u) * 2u + X * 3u + 2u;
    auto expected_comp_right = (X ^ 2) * 9u + X * 12u + 5u;
    auto expected_comp_left = (X ^ 2) * 3u + 5u;

    EXPECT_TRUE (sum == expected_sum);
    EXPECT_TRUE (product == expected_product) << "expected " << P1 << " * " << P2 << " -> " << expected_product << " but got " << product;
    EXPECT_TRUE (comp_right == expected_comp_right);
    EXPECT_TRUE (comp_left == expected_comp_left);

    poly expected_d_p1 = X * 2;
    poly expected_d_p2 = poly {3};
    auto expected_d_sum = X * 2 + 3;
    auto expected_d_product = (X ^ 2) * 9u + X * 4u + 3u;
    auto expected_d_comp_right = X * 18u + 12;
    auto expected_d_comp_left = X * 6u;

    EXPECT_TRUE (P1.derivative () == expected_d_p1);
    EXPECT_TRUE (P2.derivative () == expected_d_p2);
    EXPECT_TRUE (sum.derivative () == expected_d_sum);
    EXPECT_TRUE (product.derivative () == expected_d_product);
    EXPECT_TRUE (comp_right.derivative () == expected_d_comp_right);
    EXPECT_TRUE (comp_left.derivative () == expected_d_comp_left);
}

template <typename Z> void test_polynomial_division () {
    using poly = polynomial<Z, int32>;

    poly X = poly::var ();
    poly P1 = (X ^ 3) + (X ^ 2) * 2 - X + 7;
    poly P2 = X ^ 2 + 1;

    auto [Q, R] = divmod (P1, math::nonzero {P2});

    EXPECT_EQ (Q, X + 2);
    EXPECT_EQ (R, X * -2 + 5);
    EXPECT_EQ (P1, P2 * Q + R);
}

TYPED_TEST (RealPolynomialRing, Algebra) {
    test_polynomial_basic_algebra<typename TestFixture::base> ();
}

TYPED_TEST (RealPolynomialField, Algbera) {
    test_polynomial_basic_algebra<typename TestFixture::Q> ();
    test_polynomial_basic_algebra<typename TestFixture::mod_17> ();
    test_polynomial_basic_algebra<typename TestFixture::mod_19> ();
}

TYPED_TEST (RealPolynomialField, Division) {
    test_polynomial_basic_algebra<typename TestFixture::Q> ();
    test_polynomial_basic_algebra<typename TestFixture::mod_17> ();
    test_polynomial_basic_algebra<typename TestFixture::mod_19> ();
}
/*

TYPED_TEST (RealPolynomialField, Division) {
    using Q = typename TestFixture::base;
}

TYPED_TEST (ComplexPolynomialRing, Algebra) {
    test_polynomial_basic_algebra<typename TestFixture::base> ();
}

TYPED_TEST (ComplexPolynomialRing, Division) {
    using G = typename TestFixture::base;
}

TYPED_TEST (ComplexPolynomialField, Algebra) {
    test_polynomial_basic_algebra<typename TestFixture::base> ();
}

TYPED_TEST (ComplexPolynomialField, Division) {
    using C = typename TestFixture::base;
}*/

// TODO division
