// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/number/extended_euclidian.hpp"
#include "data/math/number/bytes/N.hpp"
#include "data/math/number/bytes/Z.hpp"
#include "gtest/gtest.h"

namespace data {
    
    TEST(ExtendedEuclidianTest, TestDivision) {
        
        math::division<N> div = N{"0x805AA786A57B3BFC0DFDF2EC86760339F018114A7E30C2D2701CF294DC60829D9B011CD8E391"}.divide(58);
        
        EXPECT_EQ(div.Quotient, N{"281747799128083566676930618873129531941258515171631968206030619132595956076515625422029804"});
        EXPECT_EQ(div.Remainder, N{25});
        
    }
    
    TEST(ExtendedEuclinianTest, TestExtendedEuclidian) {
        // TODO restore these
        using extended_i = math::number::euclidian::extended<int64, uint64>;
        using extended_l = math::number::euclidian::extended<int_little<20>, uint_little<20>>;
        using extended_b = math::number::euclidian::extended<int_big<20>, uint_big<20>>;
        using extended_z = math::number::euclidian::extended<Z, N>;
        using extended_zbl = math::number::euclidian::extended<Z_bytes_little, N_bytes_little>;
        using extended_zbb = math::number::euclidian::extended<Z_bytes_big, N_bytes_big>;
        
        EXPECT_THROW(extended_i::algorithm(0, 0).GCD, data::math::division_by_zero);
        EXPECT_THROW(extended_l::algorithm(0, 0).GCD, data::math::division_by_zero);
        EXPECT_THROW(extended_b::algorithm(0, 0).GCD, data::math::division_by_zero);
        EXPECT_THROW(extended_z::algorithm(0, 0).GCD, data::math::division_by_zero);
        EXPECT_THROW(extended_zbl::algorithm(0, 0).GCD, data::math::division_by_zero);
        EXPECT_THROW(extended_zbb::algorithm(0, 0).GCD, data::math::division_by_zero);
        
        EXPECT_EQ(extended_i::algorithm(1, 1).GCD, 1);
        //EXPECT_EQ(extended_l::algorithm(1, 1).GCD, 1);
        //EXPECT_EQ(extended_b::algorithm(1, 1).GCD, 1);
        EXPECT_EQ(extended_z::algorithm(1, 1).GCD, N{1});
        EXPECT_EQ(extended_zbl::algorithm(1, 1).GCD, 1);
        EXPECT_EQ(extended_zbb::algorithm(1, 1).GCD, 1);
        
        EXPECT_EQ(extended_i::algorithm(2, 4).GCD, 2);
        //EXPECT_EQ(extended_l::algorithm(2, 4).GCD, 2);
        //EXPECT_EQ(extended_b::algorithm(2, 4).GCD, 2);
        EXPECT_EQ(extended_z::algorithm(2, 4).GCD, N{2});
        //EXPECT_EQ(extended_zbl::algorithm(2, 4).GCD, 2);
        //EXPECT_EQ(extended_zbb::algorithm(2, 4).GCD, 2);
        
        EXPECT_EQ(extended_i::algorithm(1145, 916).GCD, 229);
        //EXPECT_EQ(extended_l::algorithm(1145, 916).GCD, 229);
        //EXPECT_EQ(extended_b::algorithm(1145, 916).GCD, 229);
        EXPECT_EQ(extended_z::algorithm(1145, 916).GCD, N{229});
        //EXPECT_EQ(extended_zbl::algorithm(1145, 916).GCD, 229);
        //EXPECT_EQ(extended_zbb::algorithm(1145, 916).GCD, 229);
        
    }
    
}
