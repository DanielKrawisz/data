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
    
    using unsigned_bounded_8_little = data::math::number::bounded<std::array<data::byte, 4>, 4, data::endian::little, false>;
    using unsigned_bounded_8_big = data::math::number::bounded<std::array<data::byte, 4>, 4, data::endian::big, false>;
    using signed_bounded_8_little = data::math::number::bounded<std::array<data::byte, 4>, 4, data::endian::little, true>;
    using signed_bounded_8_big = data::math::number::bounded<std::array<data::byte, 4>, 4, data::endian::big, true>;
    
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
    
}
