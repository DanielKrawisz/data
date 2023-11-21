// Copyright (c) 2023 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/crypto/hash/hash.hpp"
#include "gtest/gtest.h"

namespace data::crypto {

    TEST (HashTest, TestSHA) {

    }

    TEST (HashTest, TestRIPEMD) {

    }

    TEST (HashTest, TestBitcoinHash) {
        bytes data = *encoding::hex::read ("00010203fdfeff");
        digest256 digest = Bitcoin_256 (data);
        EXPECT_TRUE (true);
    }

}

