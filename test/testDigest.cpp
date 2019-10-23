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

TEST_F(DigestTest, DISABLED_DigestOrdering) {
    data::crypto::digest<20> max{"0xffffffffffffffffffffffffffffffffffffffff"};
    data::crypto::digest<20> min{"0x0"};
    data::crypto::digest<20> mid{"0x88888888888888888888"};
    
    EXPECT_TRUE(min < max);
    EXPECT_TRUE(mid < max);
    EXPECT_TRUE(min < mid);
    EXPECT_FALSE(max < min);
    EXPECT_FALSE(max < mid);
    EXPECT_FALSE(mid < min);
}

TEST_F(DigestTest, DISABLED_SHA256) {
    auto hash = data::sha256::hash(std::string{"Jackdaws love my big sphynx of quartz."});
    auto val = data::crypto::digest<32>{"0xa6db66ca9b32ea762f89c16f7cddca8a8d8c606fe0649ccda57178d8b7184282"};
    EXPECT_EQ(hash, val);
    
}
