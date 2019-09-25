// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/bounded.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"

template<size_t size, data::endian::order o, bool is_signed> 
using bounded = data::math::number::bounded::number<std::array<data::byte, size>, size, o, is_signed>;

class BoundedTest : public ::testing::Test {
protected:
    void SetUp() override {
        buA=bounded<20, data::endian::big, false>(0);
        buB=bounded<20, data::endian::big, false>(1);
        buC=bounded<20, data::endian::big, false>(0);
        
        luA=bounded<20, data::endian::little, false>(0);
        luB=bounded<20, data::endian::little, false>(1);
        luC=bounded<20, data::endian::little, false>(0);
        
        bsA=bounded<20, data::endian::big, true>(0);
        bsB=bounded<20, data::endian::big, true>(1);
        bsC=bounded<20, data::endian::big, true>(0);
        
        lsA=bounded<20, data::endian::little, true>(0);
        lsB=bounded<20, data::endian::little, true>(1);
        lsC=bounded<20, data::endian::little, true>(0);
    }

    void TearDown() override {

    }

    bounded<20, data::endian::big, false> buA;
    bounded<20, data::endian::big, false> buB;
    bounded<20, data::endian::big, false> buC;

    bounded<20, data::endian::little, false> luA;
    bounded<20, data::endian::little, false> luB;
    bounded<20, data::endian::little, false> luC;

    bounded<20, data::endian::big, true> bsA;
    bounded<20, data::endian::big, true> bsB;
    bounded<20, data::endian::big, true> bsC;

    bounded<20, data::endian::little, true> lsA;
    bounded<20, data::endian::little, true> lsB;
    bounded<20, data::endian::little, true> lsC;
};

TEST_F(BoundedTest, BoundedEqual) {
    EXPECT_TRUE(buA==buC);
    EXPECT_TRUE(buA==buA);
    EXPECT_FALSE(buA==buB);
    
    EXPECT_TRUE(bsA==bsC);
    EXPECT_TRUE(bsA==bsA);
    EXPECT_FALSE(bsA==bsB);
    
    EXPECT_TRUE(luA==luC);
    EXPECT_TRUE(luA==luA);
    EXPECT_FALSE(luA==luB);
    
    EXPECT_TRUE(lsA==lsC);
    EXPECT_TRUE(lsA==lsA);
    EXPECT_FALSE(lsA==lsB);
}

TEST_F(BoundedTest, BoundedNotEqual) {
    EXPECT_TRUE(buA!=buB);
    EXPECT_FALSE(buA!=buA);
    EXPECT_FALSE(buA!=buC);
    
    EXPECT_TRUE(bsA!=bsB);
    EXPECT_FALSE(bsA!=bsA);
    EXPECT_FALSE(bsA!=bsC);
    
    EXPECT_TRUE(luA!=luB);
    EXPECT_FALSE(luA!=luA);
    EXPECT_FALSE(luA!=luC);
    
    EXPECT_TRUE(lsA!=lsB);
    EXPECT_FALSE(lsA!=lsA);
    EXPECT_FALSE(lsA!=lsC);
}
