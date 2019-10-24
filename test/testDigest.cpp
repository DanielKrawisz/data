// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/crypto/sha256.hpp"
#include "data/crypto/sha512.hpp"
#include "data/crypto/ripemd160.hpp"
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
    EXPECT_TRUE(min < mid);
    EXPECT_FALSE(max < min);
    EXPECT_FALSE(max < mid);
    EXPECT_FALSE(mid < min);
}

TEST_F(DigestTest, Hash) {
    std::string msg{"Jackdaws love my big sphynx of quartz."};
    
    auto SHA256_hash = data::sha256::hash(msg);
    auto SHA512_hash = data::sha512::hash(msg);
    auto RIPEMD_hash = data::ripemd160::hash(msg);
    
    auto SHA256_expected = data::crypto::digest<32>{"0xa6db66ca9b32ea762f89c16f7cddca8a8d8c606fe0649ccda57178d8b7184282"};
    auto SHA512_expected = data::crypto::digest<64>{"0x140a0784f4e66f9a8c1bfe9c6c2f50b61f1e60b8ada2b3abc2038e403527fd8205c8ea5be360dd05e506de4c9e98b798708b6336dd48e9afa7c397d165dd95c5"};
    auto RIPEMD_expected = data::crypto::digest<20>{"0xcf01cfdea94f36d50081a1f1628d54e22b4fdb8e"};
    
    EXPECT_EQ(SHA256_hash, SHA256_expected);
    EXPECT_EQ(SHA512_hash, SHA512_expected);
    EXPECT_EQ(RIPEMD_hash, RIPEMD_expected);
    
}
