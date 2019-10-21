// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"
#include <iostream>

namespace {
    using N = data::math::number::gmp::N;
    using N = data::math::number::gmp::N;
    using namespace data::exported;
    
    TEST(NTest, TestStringToN) {
        
        EXPECT_FALSE(N{}.valid());
        EXPECT_FALSE(N{""}.valid());
        EXPECT_FALSE(N{"a"}.valid());
        EXPECT_FALSE(N{"-"}.valid());
        EXPECT_FALSE(N{"-1"}.valid());
        EXPECT_FALSE(N{"01"}.valid());
        EXPECT_FALSE(N{"0x1"}.valid());
        
        EXPECT_TRUE(N{0}.valid());
        EXPECT_TRUE(N{"0"}.valid());
        EXPECT_TRUE(N{"0x"}.valid());
        
        EXPECT_TRUE(N{} == N{""});
        
        EXPECT_TRUE(N{0} == N{"0"});
        EXPECT_TRUE(N{0} == N{"0x"});
        
        EXPECT_FALSE(N{0} == N{""});
        EXPECT_FALSE(N{1} == N{"0"});
        EXPECT_FALSE(N{0} == N{"1"});
        
        EXPECT_EQ(data::encoding::hexidecimal::write(N{0}), "0x00");
        EXPECT_EQ(data::encoding::decimal::write(N{0}), "0");
        EXPECT_EQ(data::encoding::hexidecimal::write(N{1}), "0x01");
        EXPECT_EQ(data::encoding::decimal::write(N{1}), "1");
        
        EXPECT_TRUE(N{1} == N{"1"});
        EXPECT_TRUE(N{1} == N{"0x01"});
        EXPECT_TRUE(N{1} == N{"0x0001"});
        
    }
    
    TEST(NTest, TestStringToZ) {
        
        EXPECT_FALSE(Z{}.valid());
        EXPECT_FALSE(Z{""}.valid());
        EXPECT_TRUE(Z{"-1"}.valid());
        EXPECT_TRUE(Z{"-0x01"}.valid());
        
        EXPECT_EQ(Z{"-1"}, Z{"-0x01"});
        EXPECT_EQ(N{}, N{""});
        
        EXPECT_EQ(data::encoding::hexidecimal::write(Z{0}), "0x00");
        EXPECT_EQ(data::encoding::decimal::write(Z{0}), "0");
        EXPECT_EQ(data::encoding::hexidecimal::write(Z{1}), "0x01");
        EXPECT_EQ(data::encoding::decimal::write(Z{1}), "1");
        EXPECT_EQ(data::encoding::hexidecimal::write(Z{-1}), "-0x01");
        EXPECT_EQ(data::encoding::decimal::write(Z{-1}), "-1");
        
    }
    
    TEST(NTest, TestMultiply) {
        
        EXPECT_TRUE(N{0} * N{0} == N{0});
        EXPECT_TRUE(N{1} * N{0} == N{0});
        EXPECT_TRUE(N{0} * N{1} == N{0});
        EXPECT_TRUE(N{1} * N{1} == N{1});
        
        N n{0};
        
        EXPECT_TRUE(n == N{0});
        
        n++;
        
        EXPECT_TRUE(n == N{1});
        
        n += 1;
        
        EXPECT_TRUE(n == N{2});
        
    }
    
    TEST(NTest, TestDivision) {
        
        data::math::division<N> div = N{"0x805AA786A57B3BFC0DFDF2EC86760339F018114A7E30C2D2701CF294DC60829D9B011CD8E391"}.divide(58);
        
        EXPECT_EQ(div.Quotient, N{"281747799128083566676930618873129531941258515171631968206030619132595956076515625422029804"});
        EXPECT_EQ(div.Remainder, N{25});
        
    }
    
    TEST(NTest, TestShift) {
        
        EXPECT_TRUE(N{1} >>  1 == N{"0x00000000000000000000"});
        EXPECT_TRUE(N{1} <<  1 == N{"0x00000000000000000002"});
        EXPECT_TRUE(N{1} <<  3 == N{"0x00000000000000000008"});
        EXPECT_TRUE(N{1} <<  7 == N{"0x00000000000000000080"});
        EXPECT_TRUE(N{1} << 79 == N{"0x80000000000000000000"});
        
    }
    
    TEST(NTest, TestN) {
        
        // parameters from https://en.bitcoin.it/wiki/Secp256k1
        
        N p = (N{1} << 256) - (N{1} << 32) - (N{1} << 9) - (N{1} << 8) - (N{1} << 7) - (N{1} << 6) - (N{1} << 4) - 1;
        
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
    
}
