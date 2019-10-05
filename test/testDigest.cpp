// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/crypto/sha256.hpp"
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

TEST_F(DigestTest, DigestOrdering) {
    data::crypto::digest<20> max{"0xffffffffffffffffffffffffffffffffffffffff"};
    data::crypto::digest<20> min{"0x0"};
    data::crypto::digest<20> mid{"0x88888888888888888888"};
    
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(mid < max);
    EXPECT_FALSE(max < min);
    EXPECT_FALSE(max < mid);
}

TEST_F(DigestTest, SHA256) {
    EXPECT_TRUE(data::sha256::hash(std::string{}) == data::crypto::digest<32>{"0xa4debfde93df7b31f720884571a6752ac3b89cbf8cfe25ab72e89ea1d9835857"});
    
}
