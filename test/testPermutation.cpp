// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"

template <typename elem>
std::ostream &write_list(std::ostream &o, const std::list<data::entry<elem, elem>> &m) {
    o << "{";
    for (const auto &e : m) o << e << " "; 
    return o << "}";
}

namespace data::math {
    
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
    
    TEST(PermutationTest, TestPermutation) {
        using perm = permutation<uint32>;
        
        EXPECT_FALSE(valid(perm{{1, 2, 1}}));
        EXPECT_TRUE(valid(perm{{}, {1}}));
        EXPECT_FALSE(valid(perm{{1, 2}, {3, 2}}));
        
        perm p0{};
        perm p0_1{{}};
        perm p0_2{{}, {}};
        perm p0_3{{1}};
        perm p0_4{{1, 1}};
        
        EXPECT_TRUE(valid(p0));
        EXPECT_TRUE(valid(p0_1));
        EXPECT_TRUE(valid(p0_2));
        EXPECT_TRUE(valid(p0_3));
        EXPECT_TRUE(valid(p0_4));
        
        EXPECT_EQ(p0, p0_1);
        EXPECT_EQ(p0, p0_2);
        EXPECT_EQ(p0, p0_3);
        EXPECT_EQ(p0, p0_4);
        
        EXPECT_EQ(p0, p0.inverse());
        EXPECT_EQ(p0, p0 * p0);
        
        perm p12 = perm{{1, 2}};
        perm p21 = perm{{2, 1}};
        perm p23 = perm{{2, 3}};
        perm p13 = perm{{1, 3}};
        
        EXPECT_TRUE(valid(p21));
        EXPECT_TRUE(valid(p12));
        EXPECT_TRUE(valid(p13));
        EXPECT_TRUE(valid(p23));
        
        EXPECT_EQ(p0, p12 * p21);
        EXPECT_EQ(p0, p12 * p12);
        EXPECT_EQ(p12, p21);
        
        perm p123 = perm{{1, 2, 3}};
        perm p321 = perm{{3, 2, 1}};
        
        EXPECT_TRUE(valid(p123));
        EXPECT_TRUE(valid(p321));
        
        EXPECT_EQ(p0, p123 * p321);
        EXPECT_EQ(p321, p123 * p123);
        EXPECT_EQ(p0, p123 * p123 * p123);
        EXPECT_EQ(p123, p23 * p12);
        EXPECT_EQ(p321, p12 * p23);
        EXPECT_EQ(p321, p13 * p12);
        EXPECT_EQ(p123, p12 * p13);
        
        perm p1234 = perm{{1, 2}, {3, 4}};
        
        EXPECT_TRUE(valid(p1234));
    }
    /*
    constexpr auto d1 = decimal{"1"};
    constexpr auto d2 = decimal{"2"};
    constexpr auto d3 = decimal{"3"};
    constexpr auto d4 = decimal{"4"};
    constexpr auto d5 = decimal{"5"};
    constexpr auto d6 = decimal{"6"};
    constexpr auto d7 = decimal{"7"};
    constexpr auto d8 = decimal{"8"};
    constexpr auto d9 = decimal{"9"};
    
    template <typename N, auto & natural>
    using symmetric = data::math::algebra::symmetric_group<N, natural>;
    
    template <typename N, auto & natural>
    using alternating = data::math::algebra::alternating_group<N, natural>;
    
    TEST(PermutationTest, TestSymmetric) {
        symmetric<uint32, d1>{};
        symmetric<uint32, d2>{};
        symmetric<uint32, d3>{};
        symmetric<uint32, d4>{};
        symmetric<uint32, d5>{};
        symmetric<uint32, d6>{};
        symmetric<uint32, d7>{};
        symmetric<uint32, d8>{};
        symmetric<uint32, d9>{};
    }
    
    TEST(PermutationTest, TestAlternating) {
        alternating<uint32, d1>{};
        alternating<uint32, d3>{};
        alternating<uint32, d5>{};
        alternating<uint32, d7>{};
        alternating<uint32, d9>{};
    }*/
    
}
