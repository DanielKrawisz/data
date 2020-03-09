// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data {
    
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
        
        EXPECT_EQ(N{}, N{""});
        
        EXPECT_EQ(N{0}, N{"0"});
        EXPECT_EQ(N{0}, N{"0x"});
        
        EXPECT_NE(N{0}, N{""});
        EXPECT_NE(N{1}, N{"0"});
        EXPECT_NE(N{0}, N{"1"});
        
        EXPECT_EQ(N{1}, N{"1"});
        EXPECT_EQ(N{1}, N{"0x01"});
        EXPECT_EQ(N{1}, N{"0x0001"});
        EXPECT_EQ(N{255}, N{"0xff"});
        
        EXPECT_EQ(encoding::hexidecimal::write(N{0}), "0x00");
        EXPECT_EQ(encoding::integer::write(N{0}), "0");
        EXPECT_EQ(encoding::hexidecimal::write(N{1}), "0x01");
        EXPECT_EQ(encoding::integer::write(N{1}), "1");
        
    }
    
    TEST(NTest, TestNToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(N{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(N{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N{"128"}), std::string{"0x80"});
        
    }
    
    TEST(NTest, TestNNegativeHex) {
        
        EXPECT_NE(N{"0x8000000000000000000000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x80000000000000000000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x80000000000000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x8000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x8000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x8000000000"}, N(0));
        EXPECT_NE(N{"0x8000"}, N(0));
        EXPECT_NE(N{"0x80"}, N(0));
        
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
        /*
        N_bytes<endian::big> p_bytes{p_hex};
        N_bytes<endian::big> n_bytes{n_hex};
        
        N p_from_big{p_bytes};
        N n_from_big{n_bytes};
        
        N p_from_little{N_bytes<endian::little>{p_from_big}};
        N n_from_little{N_bytes<endian::little>{n_from_big}};
        
        EXPECT_EQ(p_bytes, N_bytes<endian::big>{p_from_big});
        EXPECT_EQ(n_bytes, N_bytes<endian::big>{n_from_big});
        
        EXPECT_EQ(p_from_big, p_from_little);
        EXPECT_EQ(n_from_big, n_from_little);*/
        
    }
    
    TEST(NTest, TestNDivision) {
        // 26959946667150639794667015087019630673637144422540572481103610249215 / 1
    }
    
}
