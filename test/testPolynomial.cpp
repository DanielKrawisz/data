// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/polynomial.hpp"
#include "gtest/gtest.h"

TEST(PolynomialTest, TestPolynomial) {
    using namespace data::exported;
    
    using polynomial = data::math::polynomial<int, data::uint32>;
    using term = polynomial::term;
    
    polynomial p1 = polynomial::make(1, term{1, 2});
    polynomial p2 = polynomial::make(2, term{3, 1});
    
    polynomial expected_sum = polynomial::make(3, term{3, 1}, term{1, 2});
    polynomial expected_product = polynomial::make(2, term{3, 1}, term{2, 2}, term{3, 3});
    polynomial expected_composition = polynomial::make(5, term{12, 1}, term{9, 2});
    
    EXPECT_FALSE(p1 == p2);
    EXPECT_TRUE(p1 + p2 == expected_sum);
    EXPECT_TRUE(p1 * p2 == expected_product);
    EXPECT_TRUE(p1(p2) == expected_composition);
    
}

