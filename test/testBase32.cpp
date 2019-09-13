// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/base32.hpp"
#include "data/types.hpp"
#include "data/encoding/invalid.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {

    TEST(Base32Test, Base32HexToArray) {
        std::vector<data::byte> out=data::encoding::base32::read("KRSXG5A=");
        EXPECT_THAT(out,::testing::ElementsAre(84,101,115,116));
        //EXPECT_THAT(bytes,::testing::ElementsAre(0x80,0x5A,0xA7,0x86,0xA5,0x7B,0x3B,0xFC,0x0D,0xFD,0xF2,0xEC,0x86,0x76,0x03,0x39,0xF0,0x18,0x11,0x4A,0x7E,0x30,0xC2,0xD2,0x70,0x1C,0xF2,0x94,0xDC,0x60,0x82,0x9D,0x9B,0x01,0x1C,0xD8,0xE3,0x91));
    }

}

