// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/crypto/secp256k1.hpp>
#include "data/encoding/hex.hpp"
#include "data/encoding/invalid.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {

TEST(HexTest, HexHexToArray) {
    const char* inputHex="0063EA172D63808C";
    data::encoding::hex::string hexString(std::string("0063EA172D63808C"));
    ASSERT_TRUE(hexString.valid());
    ASSERT_THAT((data::bytes)(hexString), ::testing::ElementsAre(0,99,234,23,45,99,128,140));
}

TEST(HexTest, HexValidString) {
    ASSERT_TRUE(data::encoding::hex::valid("012345abcd"));
    ASSERT_TRUE(data::encoding::hex::valid("012345ABCD"));
    ASSERT_FALSE(data::encoding::hex::valid("012345aBcD"));
    ASSERT_FALSE(data::encoding::hex::valid("012345abc"));
    ASSERT_FALSE(data::encoding::hex::valid("012345aHcd"));
}

TEST(HexTest,HexInvalidExceptionOnError) {
    data::encoding::hex::string malformedHexString(std::string("0063EA172D63808"));
    ASSERT_THROW((data::bytes)(malformedHexString), data::encoding::invalid);
}

TEST(HexTest,HexWriteBytes) {
        data::bytes testArray{0, 99, 234, 23, 45, 99, 128, 140};
        std::string written = data::encoding::hex::write(data::bytes_view{testArray.data(), testArray.size()});
        ASSERT_STREQ(written.c_str(),"0063EA172D63808C");
}

TEST(HexTest,HexWritePubKey) {
        data::crypto::secp256k1::compressed_pubkey testKey(std::array<data::byte,33>({0x80,0x0C,0x28,0xFC,0xA3,0x86,0xC7,0xA2,0x27,0x60,0x0B,0x2F,0xE5,0x0B,0x7C,0xAE,0x11,0xEC,0x86,0xD3,0xBF,0x1F,0xBE,0x47,0x1B,0xE8,0x98,0x27,0xE1,0x9D,0x72,0xAA,0x1D}));
        std::string testString = data::encoding::hex::write(testKey);
        ASSERT_STREQ(testString.c_str(),"800C28FCA386C7A227600B2FE50B7CAE11EC86D3BF1FBE471BE89827E19D72AA1D");

}


}
