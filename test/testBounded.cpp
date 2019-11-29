// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include <stdexcept>

namespace {
    using namespace data::exported;
    
    template<size_t size, data::endian::order o, bool is_signed> 
    using bounded = data::math::number::bounded<std::array<data::byte, size>, size, o, is_signed>;
    
    using u8l = bounded<8, data::endian::little, false>;
    using u8b = bounded<8, data::endian::big, false>;
    using s8l = bounded<8, data::endian::little, true>;
    using s8b  = bounded<8, data::endian::big, true>;
    
    TEST(BoundedTest, Bounded01) {
        
        EXPECT_EQ(N{u8b{0}}, N{0});
        EXPECT_EQ(N{u8b{1}}, N{1});
        EXPECT_NE(N{u8b{1}}, N{0});
        EXPECT_NE(N{u8b{0}}, N{1});
        EXPECT_GT(N{u8b{1}}, N{0});
        EXPECT_LT(N{u8b{0}}, N{1});
        
        EXPECT_EQ(N{u8l{0}}, N{0});
        EXPECT_EQ(N{u8l{1}}, N{1});
        EXPECT_NE(N{u8l{1}}, N{0});
        EXPECT_NE(N{u8l{0}}, N{1});
        EXPECT_GT(N{u8l{1}}, N{0});
        EXPECT_LT(N{u8l{0}}, N{1});
        
        EXPECT_EQ(Z{s8b{0}}, Z{0});
        EXPECT_EQ(Z{s8b{1}}, Z{1});
        EXPECT_NE(Z{s8b{1}}, Z{0});
        EXPECT_NE(Z{s8b{0}}, Z{1});
        EXPECT_GT(Z{s8b{1}}, Z{0});
        EXPECT_LT(Z{s8b{0}}, Z{1});
        
        EXPECT_EQ(Z{s8l{0}}, Z{0});
        EXPECT_EQ(Z{s8l{1}}, Z{1});
        EXPECT_NE(Z{s8l{1}}, Z{0});
        EXPECT_NE(Z{s8l{0}}, Z{1});
        EXPECT_GT(Z{s8l{1}}, Z{0});
        EXPECT_LT(Z{s8l{0}}, Z{1});
        
    }
    
    TEST(BoundedTest, BoundedMinMax) {
        EXPECT_TRUE(u8b::max() > u8b::min());
        EXPECT_TRUE(u8l::max() > u8l::min());
        
        EXPECT_FALSE(u8b::max() < u8b::min());
        EXPECT_FALSE(u8l::max() < u8l::min());
        
        EXPECT_TRUE(s8b::max() > s8b::min());
        EXPECT_TRUE(s8l::max() > s8l::min());
        
        EXPECT_FALSE(s8b::max() < s8b::min());
        EXPECT_FALSE(s8l::max() < s8l::min());
        
        EXPECT_EQ(u8b::min(), u8b{0});
        EXPECT_EQ(u8l::min(), u8l{0});
        
        EXPECT_EQ(u8b::max(), u8b{"0xffffffffffffffff"});
        EXPECT_EQ(u8l::max(), u8l{"0xffffffffffffffff"});
        
        EXPECT_EQ(s8b::min(), s8b{"0x8000000000000000"});
        EXPECT_EQ(s8l::min(), s8l{"0x8000000000000000"});
        
        EXPECT_EQ(s8b::max(), s8b{"0x7FFFFFFFFFFFFFFF"});
        EXPECT_EQ(s8l::max(), s8l{"0x7FFFFFFFFFFFFFFF"});
        
    }
    
    TEST(BoundedTest, BoundedSign) {
        
        EXPECT_FALSE(s8b{0} < 0);
        EXPECT_FALSE(s8b{1} < 0);
        EXPECT_TRUE(s8b{-1} < 0);
        
        EXPECT_FALSE(s8l{0} < 0);
        EXPECT_FALSE(s8l{1} < 0);
        EXPECT_TRUE(s8l{-1} < 0);
        
    }
    
    TEST(BoundedTest, BoundedReadString) {
        
        EXPECT_THROW(u8b{"-1"}, std::invalid_argument);
        EXPECT_THROW(u8l{"-1"}, std::invalid_argument);
        
        EXPECT_EQ(s8b{"-0x01"}, s8b{"-1"});
        EXPECT_EQ(s8l{"-0x01"}, s8l{"-1"});
        
        EXPECT_THROW(u8b{"-0x01"}, std::invalid_argument);
        EXPECT_THROW(u8l{"-0x01"}, std::invalid_argument);
        
        EXPECT_EQ(s8b{"0xffffffffffffffff"}, s8b{"-1"});
        EXPECT_EQ(s8l{"0xffffffffffffffff"}, s8l{"-1"});
        
        EXPECT_THROW(s8b{"-0xffffffffffffffff"}, std::invalid_argument);
        EXPECT_THROW(s8l{"-0xffffffffffffffff"}, std::invalid_argument);
        
        EXPECT_THROW(u8b{"0x000000000000000001"}, std::invalid_argument);
        EXPECT_THROW(u8l{"0x000000000000000001"}, std::invalid_argument);
        
        EXPECT_THROW(s8b{"0x000000000000000001"}, std::invalid_argument);
        EXPECT_THROW(s8l{"0x000000000000000001"}, std::invalid_argument);
        
    }
    
}
