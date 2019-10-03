// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/base64.hpp"
#include "data/encoding/invalid.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {



    TEST(Base64Test, Base64HexToArray) {
        data::encoding::base64::string base64String(std::string("VGhpcyBJcyBhIFRlc3QgZnJvbSB0aGUgR3JlYXQgYW5kIFBvd2VyZnVsIEJhc2U2NCEh"));
        data::bytes bytes=static_cast<data::bytes>(base64String);
        EXPECT_THAT(bytes,::testing::ElementsAre(0x54,0x68,0x69,0x73,0x20,0x49,0x73,0x20,0x61,0x20,0x54,0x65,0x73,0x74,0x20,0x66,0x72,0x6f,0x6d,0x20, 0x74, 0x68, 0x65, 0x20, 0x47, 0x72, 0x65, 0x61, 0x74, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x66, 0x75, 0x6c, 0x20, 0x42, 0x61, 0x73, 0x65, 0x36, 0x34, 0x21, 0x21));
    }

    TEST(Base64Test, Base64ValidHexString) {
        EXPECT_TRUE(data::encoding::base64::valid("VGhpcyBJcyBhIFRlc3QgZnJvbSB0aGUgR3JlYXQgYW5kIFBvd2VyZnVsIEJhc2U2NCEh"));
        EXPECT_TRUE(data::encoding::base64::valid("VGhpcyBpcyBteSBib29rLCBhbmQgSSdtIGdvaW5nIHRvIFJFQUQgSVQh"));
        EXPECT_FALSE(data::encoding::base64::valid("VGhpcyBJcyBhIFGUgR3JlYW5~kIFBvd+2hc2UfgNCEh"));
    }

    TEST(Base64Test,Base64InvalidExceptionOnError) {
        EXPECT_THROW(data::encoding::base64::string malformedHexString(std::string("VGhpcyBJcyBhIFGUgR3JlYW5~kIFBvd+2hc2UfgNCEh")),data::encoding::invalid );
    }

    TEST(Base64Test,Base64WriteBytes) {
        data::bytes testArray{0x54,0x68,0x69,0x73,0x20,0x49,0x73,0x20,0x61,0x20,0x54,0x65,0x73,0x74,0x20,0x66,0x72,0x6f,0x6d,0x20, 0x74, 0x68, 0x65, 0x20, 0x47, 0x72, 0x65, 0x61, 0x74, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x66, 0x75, 0x6c, 0x20, 0x42, 0x61, 0x73, 0x65, 0x36, 0x34, 0x21, 0x21};
        ASSERT_STREQ(data::encoding::base64::write(testArray).c_str(),"VGhpcyBJcyBhIFRlc3QgZnJvbSB0aGUgR3JlYXQgYW5kIFBvd2VyZnVsIEJhc2U2NCEh");
    }


}
