// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/hex.hpp"
#include "data/encoding/invalid.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {

TEST(HexTest, HexHexToArray) {
    const char* inputHex="0063EA172D63808C";
    data::encoding::hex::string hexString(std::string("0063EA172D63808C"));
    ASSERT_THAT(static_cast<data::bytes>(hexString),::testing::ElementsAre(0,99,234,23,45,99,128,140));
}

TEST(HexTest, HexValidHexString) {
    ASSERT_TRUE(data::encoding::hex::valid("012345abcd"));
    ASSERT_TRUE(data::encoding::hex::valid("012345ABCD"));
    ASSERT_TRUE(data::encoding::hex::valid("012345aBcD"));
    ASSERT_FALSE(data::encoding::hex::valid("012345abc"));
    ASSERT_FALSE(data::encoding::hex::valid("012345aHcd"));
}

TEST(HexTest,HexInvalidExceptionOnError) {
    ASSERT_THROW(data::encoding::hex::string malformedHexString(std::string("0063EA172D63808")),data::encoding::invalid );
}

TEST(HexTest,HexWriteBytes) {
        data::bytes testArray{0,99,234,23,45,99,128,140};
        ASSERT_STREQ(data::encoding::hex::write(testArray).c_str(),"0063EA172D63808C");
}


}
