// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include <stdexcept>

namespace {
    using N = data::math::number::gmp::N;
    using Z = data::math::number::gmp::Z;
    using namespace data::exported;
    
    template<size_t size, data::endian::order o, bool is_signed> 
    using bounded = data::math::number::bounded<std::array<data::byte, size>, size, o, is_signed>;
    
    using unsigned_bounded_8_little = bounded<8, data::endian::little, false>;
    using unsigned_bounded_8_big    = bounded<8, data::endian::big, false>;
    using signed_bounded_8_little   = bounded<8, data::endian::little, true>;
    using signed_bounded_8_big      = bounded<8, data::endian::big, true>;
    
    TEST(BoundedTest, BoundedMinMax) {
        
        EXPECT_EQ(unsigned_bounded_8_big::min(),    unsigned_bounded_8_big{0});
        EXPECT_EQ(unsigned_bounded_8_little::min(), unsigned_bounded_8_little{0});
        
        EXPECT_EQ(unsigned_bounded_8_big::max(),    unsigned_bounded_8_big{   "0xffffffffffffffff"});
        EXPECT_EQ(unsigned_bounded_8_little::max(), unsigned_bounded_8_little{"0xffffffffffffffff"});
        
        EXPECT_EQ(signed_bounded_8_big::min(),    signed_bounded_8_big{   "0x8000000000000000"});
        EXPECT_EQ(signed_bounded_8_little::min(), signed_bounded_8_little{"0x8000000000000000"});
        
        EXPECT_EQ(signed_bounded_8_big::max(),    signed_bounded_8_big{   "0x7FFFFFFFFFFFFFFF"});
        EXPECT_EQ(signed_bounded_8_little::max(), signed_bounded_8_little{"0x7FFFFFFFFFFFFFFF"});
        
    }
    
    TEST(BoundedTest, BoundedReadString) {
        
        EXPECT_THROW(unsigned_bounded_8_big{   "-1"}, std::invalid_argument);
        EXPECT_THROW(unsigned_bounded_8_little{"-1"}, std::invalid_argument);
        
        EXPECT_EQ(signed_bounded_8_big{   "-0x01"}, signed_bounded_8_big{   "-1"});
        EXPECT_EQ(signed_bounded_8_little{"-0x01"}, signed_bounded_8_little{"-1"});
        
        EXPECT_THROW(unsigned_bounded_8_big{   "-0x01"}, std::invalid_argument);
        EXPECT_THROW(unsigned_bounded_8_little{"-0x01"}, std::invalid_argument);
        
        EXPECT_EQ(signed_bounded_8_big{   "0xffffffffffffffff"}, signed_bounded_8_big{   "-1"});
        EXPECT_EQ(signed_bounded_8_little{"0xffffffffffffffff"}, signed_bounded_8_little{"-1"});
        
        EXPECT_THROW(signed_bounded_8_big{   "-0xffffffffffffffff"}, std::invalid_argument);
        EXPECT_THROW(signed_bounded_8_little{"-0xffffffffffffffff"}, std::invalid_argument);
        
        EXPECT_THROW(unsigned_bounded_8_big{   "0x000000000000000001"}, std::invalid_argument);
        EXPECT_THROW(unsigned_bounded_8_little{"0x000000000000000001"}, std::invalid_argument);
        
        EXPECT_THROW(signed_bounded_8_big{   "0x000000000000000001"}, std::invalid_argument);
        EXPECT_THROW(signed_bounded_8_little{"0x000000000000000001"}, std::invalid_argument);
        
    }
    
    TEST(BoundedTest, Bounded01) {
        
        EXPECT_EQ(N{unsigned_bounded_8_big{0}}, N{0});
        EXPECT_EQ(N{unsigned_bounded_8_big{1}}, N{1});
        EXPECT_NE(N{unsigned_bounded_8_big{1}}, N{0});
        EXPECT_NE(N{unsigned_bounded_8_big{0}}, N{1});
        EXPECT_GT(N{unsigned_bounded_8_big{1}}, N{0});
        EXPECT_LT(N{unsigned_bounded_8_big{0}}, N{1});
        
        EXPECT_EQ(N{unsigned_bounded_8_little{0}}, N{0});
        EXPECT_EQ(N{unsigned_bounded_8_little{1}}, N{1});
        EXPECT_NE(N{unsigned_bounded_8_little{1}}, N{0});
        EXPECT_NE(N{unsigned_bounded_8_little{0}}, N{1});
        EXPECT_GT(N{unsigned_bounded_8_little{1}}, N{0});
        EXPECT_LT(N{unsigned_bounded_8_little{0}}, N{1});
        
        EXPECT_EQ(Z{signed_bounded_8_big{0}}, Z{0});
        EXPECT_EQ(Z{signed_bounded_8_big{1}}, Z{1});
        EXPECT_NE(Z{signed_bounded_8_big{1}}, Z{0});
        EXPECT_NE(Z{signed_bounded_8_big{0}}, Z{1});
        EXPECT_GT(Z{signed_bounded_8_big{1}}, Z{0});
        EXPECT_LT(Z{signed_bounded_8_big{0}}, Z{1});
        
        EXPECT_EQ(Z{signed_bounded_8_little{0}}, Z{0});
        EXPECT_EQ(Z{signed_bounded_8_little{1}}, Z{1});
        EXPECT_NE(Z{signed_bounded_8_little{1}}, Z{0});
        EXPECT_NE(Z{signed_bounded_8_little{0}}, Z{1});
        EXPECT_GT(Z{signed_bounded_8_little{1}}, Z{0});
        EXPECT_LT(Z{signed_bounded_8_little{0}}, Z{1});
        
    }
    
}
