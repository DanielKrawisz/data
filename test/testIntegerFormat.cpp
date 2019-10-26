// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <data/encoding/integer.hpp>

TEST(IntegerTest, TestIntegerFormat) {
    using namespace data::exported;
    
    EXPECT_FALSE(data::encoding::decimal::valid(""));
    EXPECT_TRUE(data::encoding::decimal::valid("0"));
    EXPECT_FALSE(data::encoding::decimal::valid("01"));
    EXPECT_TRUE(data::encoding::decimal::valid("1"));
    EXPECT_FALSE(data::encoding::hexidecimal::valid(""));
    EXPECT_TRUE(data::encoding::hexidecimal::valid("0x"));
    EXPECT_TRUE(data::encoding::hexidecimal::valid("0xaf"));
    EXPECT_TRUE(data::encoding::hexidecimal::valid("0xAF"));
    EXPECT_FALSE(data::encoding::hexidecimal::valid("0xa"));
    EXPECT_FALSE(data::encoding::hexidecimal::valid("0xaF"));
    EXPECT_FALSE(data::encoding::integer::valid(""));
    
}

