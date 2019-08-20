//
// Created by nekosune on 20/08/2019.
//
#include "data/crypto/digest.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
class DigestTest : public ::testing::Test {
protected:
    void SetUp() override {
        digestA=data::crypto::digest<1>(0);
        digestB=data::crypto::digest<1>(1);
        digestC=data::crypto::digest<1>(0);
    }

    void TearDown() override {

    }

    data::crypto::digest<1> digestA;
    data::crypto::digest<1> digestB;
    data::crypto::digest<1> digestC;
};

TEST_F(DigestTest, DigestEqual) {
    EXPECT_TRUE(digestA==digestC);
    EXPECT_TRUE(digestA==digestA);
    EXPECT_FALSE(digestA==digestB);
}