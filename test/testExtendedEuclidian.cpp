// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/polynomial.hpp"
#include "data/math/number/rational.hpp"
#include "gtest/gtest.h"

TEST(ExtendedEuclinianTest, TestExtendedEuclidian) {
    using namespace data::exported;
    using extended_i = data::math::number::euclidian::extended<data::uint64, data::int64>;
    using extended_b = data::math::number::euclidian::extended<data::uint<20>, data::integer<20>>;
    using extended_z = data::math::number::euclidian::extended<N, Z>;
    
    EXPECT_EQ(extended_i::algorithm(0, 0).GCD, 0);
    //EXPECT_EQ(extended_b::algorithm(0, 0).GCD, 0);
    EXPECT_EQ(extended_z::algorithm(0, 0).GCD, 0);
    
    EXPECT_EQ(extended_i::algorithm(1, 1).GCD, 1);
    //EXPECT_EQ(extended_b::algorithm(1, 1).GCD, 1);
    EXPECT_EQ(extended_z::algorithm(1, 1).GCD, 1);
    
    EXPECT_EQ(extended_i::algorithm(2, 4).GCD, 4);
    //EXPECT_EQ(extended_b::algorithm(2, 4).GCD, 4);
    EXPECT_EQ(extended_z::algorithm(2, 4).GCD, 4);
    
    EXPECT_EQ(extended_i::algorithm(1145, 916).GCD, 229);
    //EXPECT_EQ(extended_b::algorithm(1145, 916).GCD, 229);
    EXPECT_EQ(extended_z::algorithm(1145, 916).GCD, 229);
    
}

