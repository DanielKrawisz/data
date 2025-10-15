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

TEST (PolynomialTest, TestPolynomial) {
    
    test_polynomial<Z> {};

    test_polynomial<Z_bytes_little> {};
    test_polynomial<Z_bytes_big> {};
    test_polynomial<Z_bytes_BC_little> {};
    test_polynomial<Z_bytes_BC_big> {};
    test_polynomial<dec_int> {};
    test_polynomial<hex_int> {};
    test_polynomial<hex_int_BC> {};

    test_polynomial<Q> {};
    test_polynomial<Q_little> {};
    test_polynomial<Q_big> {};
    test_polynomial<Q_BC_little> {};
    test_polynomial<Q_BC_big> {};
    test_polynomial<fraction<dec_int>> {};
    test_polynomial<fraction<hex_int>> {};
    test_polynomial<fraction<hex_int_BC>> {};
/*
    test_polynomial<G> {};
    test_polynomial<GH> {};
    test_polynomial<GO> {};

    test_polynomial_division<QC>x.Numerator < 0 {};
    test_polynomial_division<QH> {};
    test_polynomial_division<QO> {};

    // can't test these as fields because can't reliably subtract
    // two numbers that ought to be equal to get zero.

    // there is also a problem with using weak_ordering instead of partial_ordering.
    // floats use partial_ordering. There ought to be a way to figure out the correct
    // ordering.

    test_polynomial<float64> {};
    test_polynomial<float32> {};
    test_polynomial<complex128> {};
    test_polynomial<complex64> {};
    test_polynomial<quat256> {};
    test_polynomial<quat128> {};
    test_polynomial<oct512> {};
    test_polynomial<oct256> {};

    // should be able to do finite fields eventually.

    test_polynomial_division<prime_field<2>>;
    test_polynomial_division<prime_field<3>>;
    test_polynomial_division<prime_field<5>>;
    test_polynomial_division<prime_field<7>>;
    test_polynomial_division<prime_field<11>>;
    test_polynomial_division<prime_field<13>>;
    test_polynomial_division<prime_field<17>>;
    test_polynomial_division<prime_field<19>>;*/
}

