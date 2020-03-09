// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data {
    
    TEST(NBytesTest, TestStringToNBytes) {
        
        EXPECT_FALSE(N_bytes<endian::big>{"a"}.valid());
        EXPECT_FALSE(N_bytes<endian::big>{"-"}.valid());
        EXPECT_FALSE(N_bytes<endian::big>{"-1"}.valid());
        EXPECT_FALSE(N_bytes<endian::big>{"01"}.valid());
        EXPECT_FALSE(N_bytes<endian::big>{"0x1"}.valid());
        
        EXPECT_FALSE(N_bytes<endian::little>{"a"}.valid());
        EXPECT_FALSE(N_bytes<endian::little>{"-"}.valid());
        EXPECT_FALSE(N_bytes<endian::little>{"-1"}.valid());
        EXPECT_FALSE(N_bytes<endian::little>{"01"}.valid());
        EXPECT_FALSE(N_bytes<endian::little>{"0x1"}.valid());
        
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>{});
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>{""});
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>{"0"});
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>{"0x"});
        
        EXPECT_FALSE(N_bytes<endian::big>{1} == N_bytes<endian::big>{"0"});
        EXPECT_FALSE(N_bytes<endian::big>{0} == N_bytes<endian::big>{"1"});
        
        EXPECT_TRUE(N_bytes<endian::big>{1} == N_bytes<endian::big>{"1"});
        EXPECT_TRUE(N_bytes<endian::big>{1} == N_bytes<endian::big>{"0x01"});
        EXPECT_TRUE(N_bytes<endian::big>{1} == N_bytes<endian::big>{"0x0001"});
        
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>{});
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>{""});
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>{"0"});
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>{"0x"});
        
        EXPECT_FALSE(N_bytes<endian::little>{1} == N_bytes<endian::little>{"0"});
        EXPECT_FALSE(N_bytes<endian::little>{0} == N_bytes<endian::little>{"1"});
        
        EXPECT_TRUE(N_bytes<endian::little>{1} == N_bytes<endian::little>{"1"});
        EXPECT_TRUE(N_bytes<endian::little>{1} == N_bytes<endian::little>{"0x01"});
        EXPECT_TRUE(N_bytes<endian::little>{1} == N_bytes<endian::little>{"0x0001"});
        
    }
    
    TEST(NBytesTest, TestNBytesToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"128"}), std::string{"0x80"});
        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"128"}), std::string{"0x80"});
        
    }
    
    TEST(NBytesTest, TestZeroSize) {
        
        EXPECT_EQ(N_bytes<endian::big>{"0x"}.size(), 0);
        EXPECT_EQ(N_bytes<endian::big>{"0x00"}.size(), 1);
        EXPECT_EQ(N_bytes<endian::big>{"0x000000"}.size(), 3);
        
        EXPECT_EQ(N_bytes<endian::little>{"0x"}.size(), 0);
        EXPECT_EQ(N_bytes<endian::little>{"0x00"}.size(), 1);
        EXPECT_EQ(N_bytes<endian::little>{"0x000000"}.size(), 3);
        
    }
    
    TEST(NBytesTest, TestZeroAndNegativeNBytes) {
        
        EXPECT_EQ(N_bytes<endian::big>{"0x"}, N_bytes<endian::big>{0});
        EXPECT_EQ(N_bytes<endian::big>{"0x00"}, N_bytes<endian::big>{0});
        EXPECT_EQ(N_bytes<endian::big>{"0x000000"}, N_bytes<endian::big>{0});
        EXPECT_EQ(N_bytes<endian::little>{"0x"}, N_bytes<endian::little>{0});
        EXPECT_EQ(N_bytes<endian::little>{"0x00"}, N_bytes<endian::little>{0});
        EXPECT_EQ(N_bytes<endian::little>{"0x000000"}, N_bytes<endian::little>{0});
        
        EXPECT_EQ(N_bytes<endian::big>{"0x01"}, N_bytes<endian::big>{1});
        EXPECT_EQ(N_bytes<endian::big>{"0x0001"}, N_bytes<endian::big>{1});
        EXPECT_EQ(N_bytes<endian::big>{"0x00000001"}, N_bytes<endian::big>{1});
        EXPECT_EQ(N_bytes<endian::little>{"0x01"}, N_bytes<endian::little>{1});
        EXPECT_EQ(N_bytes<endian::little>{"0x0001"}, N_bytes<endian::little>{1});
        EXPECT_EQ(N_bytes<endian::little>{"0x00000001"}, N_bytes<endian::little>{1});
        
    }
    
    // Problem: string reversal not happening correctly for some reason!!
    TEST(NBytesTest, TestNToNBytes) {
        
        EXPECT_EQ(N_bytes<endian::big>{N{"1"}}, N_bytes<endian::big>{"1"});
        EXPECT_EQ(N_bytes<endian::little>{N{"1"}}, N_bytes<endian::little>{"1"});
        EXPECT_EQ(N_bytes<endian::big>{N{"23"}}, N_bytes<endian::big>{"23"});
        EXPECT_EQ(N_bytes<endian::little>{N{"23"}}, N_bytes<endian::little>{"23"});
        EXPECT_EQ(N_bytes<endian::big>{N{"5704566599993321"}}, N_bytes<endian::big>{"5704566599993321"});
        EXPECT_EQ(N_bytes<endian::little>{N{"5704566599993321"}}, N_bytes<endian::little>{"5704566599993321"});
        
    }
    
    TEST(NBytesTest, TestNBytesToString) {
        
        EXPECT_EQ(encoding::integer::write(N_bytes<endian::big>{"1"}), std::string{"1"});
        EXPECT_EQ(encoding::integer::write(N_bytes<endian::little>{"1"}), std::string{"1"});
        EXPECT_EQ(encoding::integer::write(N_bytes<endian::big>{"23"}), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(N_bytes<endian::little>{"23"}), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(N_bytes<endian::big>{"5704566599993321"}), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::integer::write(N_bytes<endian::little>{"5704566599993321"}), std::string{"5704566599993321"});
        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"1"}), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"1"}), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"23"}), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"23"}), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"5704566599993321"}), std::string{"144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"5704566599993321"}), std::string{"144445e9ca47e9"});
        
    }
    
    TEST(NBytesTest, TestIncrement) {
        
        EXPECT_EQ(++N_bytes<endian::big>{"0x"}, N_bytes<endian::big>{1});
        EXPECT_EQ(++N_bytes<endian::big>{"0x00"}, N_bytes<endian::big>{1});
        EXPECT_EQ(++N_bytes<endian::big>{"0x000000"}, N_bytes<endian::big>{1});
        EXPECT_EQ(++N_bytes<endian::little>{"0x"}, N_bytes<endian::little>{1});
        EXPECT_EQ(++N_bytes<endian::little>{"0x00"}, N_bytes<endian::little>{1});
        EXPECT_EQ(++N_bytes<endian::little>{"0x000000"}, N_bytes<endian::little>{1});
        
    }
    
    TEST(NBytesTest, TestNBytesToN) {
        
        EXPECT_EQ(N{N_bytes<endian::big>{}}, N{0});
        EXPECT_EQ(N{N_bytes<endian::big>{""}}, N{0});
        EXPECT_EQ(N{N_bytes<endian::little>{}}, N{0});
        EXPECT_EQ(N{N_bytes<endian::little>{""}}, N{0});
        
        EXPECT_EQ(Z{Z_bytes<endian::big>{}}, Z{0});
        EXPECT_EQ(Z{Z_bytes<endian::big>{""}}, Z{0});
        EXPECT_EQ(Z{Z_bytes<endian::little>{}}, Z{0});
        EXPECT_EQ(Z{Z_bytes<endian::little>{""}}, Z{0});
        
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
    
    // TODO needs to be changed because << and >> need to be removed from N. 
    TEST(NTest, TestShift) {
        
        EXPECT_TRUE(N{1} >>  1 == N{"0x00000000000000000000"});
        EXPECT_TRUE(N{1} <<  1 == N{"0x00000000000000000002"});
        EXPECT_TRUE(N{1} <<  3 == N{"0x00000000000000000008"});
        EXPECT_TRUE(N{1} <<  7 == N{"0x00000000000000000080"});
        EXPECT_TRUE(N{1} << 79 == N{"0x80000000000000000000"});
        
    }
    
    // TODO same.
    TEST(NTest, TestN) {
        /*
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
        
        N_bytes<endian::big> p_bytes{p_hex};
        N_bytes<endian::big> n_bytes{n_hex};
        
        N p_from_big{p_bytes};
        N n_from_big{n_bytes};
        
        N p_from_little{N_bytes<endian::little>{p_from_big}};
        N n_from_little{N_bytes<endian::little>{n_from_big}};
        
        EXPECT_EQ(p_bytes, N_bytes<endian::big>{p_from_big});
        EXPECT_EQ(n_bytes, N_bytes<endian::big>{n_from_big});
        
        EXPECT_EQ(p_from_big, p_from_little);
        EXPECT_EQ(n_from_big, n_from_little);
        */
    }
    
}

