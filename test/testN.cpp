// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"

TEST(NTest, TestN) {
    using namespace data::exported;
    
    // parameters from https://en.bitcoin.it/wiki/Secp256k1
    
    N p = (N{2} << 256) - (N{2} << 32) - (N{2} << 9) - (N{2} << 8) - (N{2} << 7) - (N{2} << 6) - (N{2} << 4) - 1;

    std::string p_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
    std::string p_dec = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
    
    std::string n_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    std::string n_dec = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
    
    N n = N{n_hex};
    
    EXPECT_TRUE(p == p);
    EXPECT_FALSE(p == n);
    
    EXPECT_TRUE(n == n);
    EXPECT_FALSE(n == p);
    
    EXPECT_TRUE(p == N{p_hex});
    EXPECT_FALSE(p == N{n_hex});
    EXPECT_FALSE(n == N{p_hex});
    
    EXPECT_TRUE(p == N{p_dec});
    EXPECT_TRUE(n == N{n_dec});
    
    EXPECT_FALSE(p == N{n_dec});
    EXPECT_FALSE(n == N{p_dec});
    
    
}

