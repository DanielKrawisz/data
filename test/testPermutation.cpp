// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"

namespace data {
    
    TEST(PermutationTest, TestCycle) {
        using ucycle = cycle<uint32>;
        
        ucycle c{};
        ucycle c1{1};
        ucycle c0{0};
        ucycle c11{1, 1};
        
        EXPECT_EQ(c, c);
        EXPECT_EQ(c1, c1);
        EXPECT_EQ(c11, c11);
        EXPECT_NE(c1, c11);
        EXPECT_NE(c0, c1);
        
        ucycle c12{1, 2};
        ucycle c21{2, 1};
        
        EXPECT_EQ(c12.head(), 1);
        EXPECT_EQ(c21.head(), 2);
        
        EXPECT_EQ(c12, c12);
        EXPECT_EQ(c12, c21);
        
        EXPECT_EQ(c12.reverse(), c12);
        EXPECT_EQ(c12.reverse(), c21);
        EXPECT_EQ(c21.reverse(), c12);
        EXPECT_EQ(c21.reverse(), c21);
        
        ucycle c123{1, 2, 3};
        ucycle c231{2, 3, 1};
        ucycle c312{3, 1, 2};
        ucycle c321{3, 2, 1};
        ucycle c213{2, 1, 3};
        ucycle c132{1, 3, 2};
        
        EXPECT_EQ(c123, c123);
        EXPECT_EQ(c123, c231);
        EXPECT_EQ(c123, c312);
        EXPECT_EQ(c321, c132);
        EXPECT_NE(c123, c132);
        EXPECT_NE(c321, c231);
        
        EXPECT_EQ(c123.reverse(), c321);
        EXPECT_EQ(c321.reverse(), c123);
        
    }
    /*
    TEST(PermutationTest, TestPermutation) {
        using perm = permutation<uint32>;
        using cycle = perm::cycle;
        
        EXPECT_FALSE(valid(perm{cycle{1, 2, 1}}));
        EXPECT_FALSE(valid(perm{cycle{}, cycle{1}}));
        EXPECT_FALSE(valid(perm{cycle{1, 2}, cycle{3, 2}}));
        
        perm p0{};
        perm p0_1{cycle{}};
        perm p0_2{cycle{}, cycle{}};
        perm p0_3{cycle{1}};
        perm p0_4{cycle{1, 1}};
        
        EXPECT_TRUE(valid(p0));
        EXPECT_TRUE(valid(p0_1));
        EXPECT_TRUE(valid(p0_2));
        EXPECT_TRUE(valid(p0_3));
        EXPECT_TRUE(valid(p0_4));
        
        EXPECT_EQ(p0, p0_1);
        EXPECT_EQ(p0, p0_2);
        
        EXPECT_EQ(p0, p0.inverse());
        EXPECT_EQ(p0, p0 * p0);
        
        perm p12 = perm{cycle{1, 2}};
        perm p21 = perm{cycle{2, 1}};
        perm p23 = perm{cycle{2, 3}};
        perm p13 = perm{cycle{1, 3}};
        
        EXPECT_TRUE(valid(p21));
        EXPECT_TRUE(valid(p12));
        EXPECT_TRUE(valid(p13));
        EXPECT_TRUE(valid(p23));
        
        EXPECT_EQ(p0, p12 * p21);
        EXPECT_EQ(p0, p12 * p12);
        EXPECT_EQ(p12, p21);
        EXPECT_EQ(p13, p23 * p12);
        
        perm p123 = perm{cycle{1, 2, 3}};
        perm p321 = perm{cycle{3, 2, 1}};
        
        EXPECT_TRUE(valid(p123));
        EXPECT_TRUE(valid(p321));
        
        EXPECT_EQ(p0, p123 * p321);
        EXPECT_EQ(p321, p123 * p123);
        EXPECT_EQ(p0, p123 * p123 * p123);
        
        perm p1234 = perm{cycle{1, 2}, {3, 4}};
        
        EXPECT_TRUE(valid(p1234));
    }*/
    /*
    TEST(PermutationTest, TestSymmetric) {
        
    }
    
    TEST(PermutationTest, TestAlternating) {
        
    }*/
    
}
