// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/bytes.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"

namespace data::encoding::hex {

    TEST (Hex, ToArray) {
        string hexString ("0063EA172D63808C");
        ASSERT_TRUE (hexString.valid ());
        bytes b = (bytes) (hexString);
        ASSERT_THAT (b, ::testing::ElementsAre (0x00, 0x63, 0xEA, 0x17, 0x2D, 0x63, 0x80, 0x8C));
    }

    TEST (Hex, ValidString) {
        ASSERT_TRUE (valid ("012345abcd"));
        ASSERT_TRUE (valid ("012345ABCD"));
        ASSERT_FALSE (valid ("012345aBcD"));
        ASSERT_FALSE (valid ("012345abc"));
        ASSERT_FALSE (valid ("012345aHcd"));
    }

    TEST (Hex, InvalidExceptionOnError) {
        string malformedHexString ("0063EA172D63808");
        ASSERT_THROW ((bytes) (malformedHexString), invalid);
    }

    TEST (Hex, WriteBytes) {
        std::vector<byte> testVector {0, 99, 234, 23, 45, 99, 128, 140};
        string written = write (data::byte_slice {testVector.data (), testVector.size ()}, hex_case::upper);
        ASSERT_STREQ (written.c_str (), "0063EA172D63808C");
    }

    TEST (Hex, WritePubKey) {
        std::array<byte, 33> a ({
            0x80, 0x0C, 0x28, 0xFC, 0xA3, 0x86, 0xC7, 0xA2, 0x27, 0x60, 0x0B, 0x2F,
            0xE5, 0x0B, 0x7C, 0xAE, 0x11, 0xEC, 0x86, 0xD3, 0xBF, 0x1F, 0xBE, 0x47,
            0x1B, 0xE8, 0x98, 0x27, 0xE1, 0x9D, 0x72, 0xAA, 0x1D});
        string testString = write (data::byte_slice {a.data (), 33}, hex_case::upper);
        ASSERT_STREQ (testString.c_str (), "800C28FCA386C7A227600B2FE50B7CAE11EC86D3BF1FBE471BE89827E19D72AA1D");

    }

}
