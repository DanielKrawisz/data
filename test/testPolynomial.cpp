// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/polynomial.hpp"
#include "data/math/number/rational.hpp"
#include "gtest/gtest.h"

TEST(PolynomialTest, TestPolynomial) {
    using namespace data;
    
    using polynomial = data::math::polynomial<data::math::number::fraction<Z, N>, uint32>;
    using term = polynomial::term;
    
    polynomial p1 = polynomial::make(Z{1}, term{Z{1}, 2});
    polynomial p2 = polynomial::make(Z{2}, term{Z{3}, 1});
    
    polynomial expected_sum = polynomial::make(Z{3}, term{Z{3}, 1}, term{Z{1}, 2});
    polynomial expected_product = polynomial::make(Z{2}, term{Z{3}, 1}, term{Z{2}, 2}, term{Z{3}, 3});
    polynomial expected_composition = polynomial::make(Z{5}, term{Z{12}, 1}, term{Z{9}, 2});
    
    EXPECT_FALSE(p1 == p2);
    EXPECT_TRUE(p1 + p2 == expected_sum);
    EXPECT_TRUE(p1 * p2 == expected_product);
    EXPECT_TRUE(p1(p2) == expected_composition);
    
    // TODO polynomial long division
    
}

