// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <data/encoding/integer.hpp>

TEST(IntegerTest, TestIntegerFormat) {
    using namespace data::encoding;
    
    EXPECT_FALSE(decimal::valid(""));
    EXPECT_TRUE(decimal::valid("0"));
    EXPECT_FALSE(decimal::valid("01"));
    EXPECT_TRUE(decimal::valid("1"));
    EXPECT_FALSE(hexidecimal::valid(""));
    EXPECT_TRUE(hexidecimal::valid("0x"));
    EXPECT_TRUE(hexidecimal::valid("0xaf"));
    EXPECT_TRUE(hexidecimal::valid("0xAF"));
    EXPECT_FALSE(hexidecimal::valid("0xa"));
    EXPECT_FALSE(hexidecimal::valid("0xaF"));
    EXPECT_FALSE(integer::valid(""));
    EXPECT_TRUE(integer::valid("-1"));
    EXPECT_FALSE(integer::valid("-0xaf"));
    
    EXPECT_FALSE(integer::negative("0"));
    EXPECT_FALSE(integer::negative("0x00"));
    EXPECT_FALSE(integer::negative("-0"));
    EXPECT_TRUE(integer::negative("-1"));
    EXPECT_TRUE(integer::negative("-01"));
    EXPECT_TRUE(integer::negative("0xff"));
    EXPECT_TRUE(integer::negative("0x8000"));
    EXPECT_FALSE(integer::negative("0x7fff"));
    
}

