// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/crypto/digest.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
class DigestTest : public ::testing::Test {
protected:
    void SetUp() override {
        digestA=data::crypto::digest<20>(0);
        digestB=data::crypto::digest<20>(1);
        digestC=data::crypto::digest<20>(0);
    }

    void TearDown() override {

    }

    data::crypto::digest<20> digestA;
    data::crypto::digest<20> digestB;
    data::crypto::digest<20> digestC;
};

TEST_F(DigestTest, DigestEqual) {
    EXPECT_TRUE(digestA==digestC);
    EXPECT_TRUE(digestA==digestA);
    EXPECT_FALSE(digestA==digestB);
}

TEST_F(DigestTest, DigestNotEqual) {
    EXPECT_TRUE(digestA!=digestB);
    EXPECT_FALSE(digestA!=digestA);
    EXPECT_FALSE(digestA!=digestC);
}