// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data {
    
    TEST(ZTest, TestZToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-2"}), std::string{"0xfd"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"128"}), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-128"}), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-129"}), std::string{"0xff7f"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-2"}), std::string{"0xfd"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"128"}), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-128"}), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-129"}), std::string{"0xff7f"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-2"}), std::string{"0xfd"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"128"}), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-128"}), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-129"}), std::string{"0xff7f"});
        
    }
    
    TEST(ZTest, TestStringToZ) {
        
        EXPECT_FALSE(Z{}.valid());
        EXPECT_FALSE(Z{N{}}.valid());
        EXPECT_FALSE(Z{""}.valid());
        EXPECT_TRUE(Z{"-1"}.valid());
        EXPECT_FALSE(Z{"-0x01"}.valid());
        
        EXPECT_NO_THROW(Z_bytes<endian::big>{});
        EXPECT_THROW(Z_bytes<endian::big>{N{}}, std::invalid_argument);
        EXPECT_THROW(Z_bytes<endian::big>{Z{}}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::big>{""});
        EXPECT_NO_THROW(Z_bytes<endian::big>{"-1"});
        EXPECT_THROW(Z_bytes<endian::big>{"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0x00"});
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0xff"});
        
        EXPECT_NO_THROW(Z_bytes<endian::little>{});
        EXPECT_THROW(Z_bytes<endian::little>{N{}}, std::invalid_argument);
        EXPECT_THROW(Z_bytes<endian::little>{Z{}}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::little>{""});
        EXPECT_NO_THROW(Z_bytes<endian::little>{"-1"});
        EXPECT_THROW(Z_bytes<endian::little>{"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0x00"});
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0xff"});
        
        EXPECT_EQ(N{}, N{""});
        EXPECT_EQ(Z{-1}, Z{"-1"});
        EXPECT_EQ(Z{"-1"}, Z{"0xff"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{0}), "0x00");
        EXPECT_EQ(encoding::integer::write(Z{0}), "0");
        EXPECT_EQ(encoding::hexidecimal::write(Z{1}), "0x01");
        EXPECT_EQ(encoding::integer::write(Z{1}), "1");
        EXPECT_EQ(encoding::hexidecimal::write(Z{-1}), "-0x01");
        EXPECT_EQ(encoding::integer::write(Z{-1}), "-1");
        
        EXPECT_TRUE(Z{"0x80000000000000000000"} < Z{"0x7fffffffffffffffffff"});
        EXPECT_TRUE(Z{"0xff"} < Z{"0x00ff"});
        
    }
    
    TEST(ZTest, TestZeroSize) {
        
        EXPECT_EQ(Z_bytes<endian::big>{"0x"}.size(), 0);
        EXPECT_EQ(Z_bytes<endian::big>{"0x00"}.size(), 1);
        EXPECT_EQ(Z_bytes<endian::big>{"0x000000"}.size(), 3);
        
        EXPECT_EQ(Z_bytes<endian::little>{"0x"}.size(), 0);
        EXPECT_EQ(Z_bytes<endian::little>{"0x00"}.size(), 1);
        EXPECT_EQ(Z_bytes<endian::little>{"0x000000"}.size(), 3);
        
    }
    
    TEST(ZTest, TestZeroAndNegativeZ) {
        
        EXPECT_EQ(Z_bytes<endian::big>{"0x"}, Z_bytes<endian::big>{0});
        EXPECT_EQ(Z_bytes<endian::big>{"0x00"}, Z_bytes<endian::big>{0});
        EXPECT_EQ(Z_bytes<endian::big>{"0x000000"}, Z_bytes<endian::big>{0});
        EXPECT_EQ(Z_bytes<endian::little>{"0x"}, Z_bytes<endian::little>{0});
        EXPECT_EQ(Z_bytes<endian::little>{"0x00"}, Z_bytes<endian::little>{0});
        EXPECT_EQ(Z_bytes<endian::little>{"0x000000"}, Z_bytes<endian::little>{0});
        
        EXPECT_EQ(Z_bytes<endian::big>{"0x01"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(Z_bytes<endian::big>{"0x0001"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(Z_bytes<endian::big>{"0x00000001"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(Z_bytes<endian::little>{"0x01"}, Z_bytes<endian::little>{1});
        EXPECT_EQ(Z_bytes<endian::little>{"0x0001"}, Z_bytes<endian::little>{1});
        EXPECT_EQ(Z_bytes<endian::little>{"0x00000001"}, Z_bytes<endian::little>{1});
        
        EXPECT_EQ(Z_bytes<endian::big>{"0xff"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(Z_bytes<endian::big>{"0xffff"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(Z_bytes<endian::big>{"0xffffff"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(Z_bytes<endian::little>{"0xff"}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(Z_bytes<endian::little>{"0xffff"}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(Z_bytes<endian::little>{"0xffffff"}, Z_bytes<endian::little>{-1});
        
    }
    
    // Problem: string reversal not happening correctly for some reason!!
    TEST(ZTest, TestZToZBytes) {
        
        EXPECT_EQ(Z_bytes<endian::big>{Z{"1"}}, Z_bytes<endian::big>{"1"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"1"}}, Z_bytes<endian::little>{"1"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"23"}}, Z_bytes<endian::big>{"23"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"23"}}, Z_bytes<endian::little>{"23"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"5704566599993321"}}, Z_bytes<endian::big>{"5704566599993321"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"5704566599993321"}}, Z_bytes<endian::little>{"5704566599993321"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"-1"}}, Z_bytes<endian::big>{"-1"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"-1"}}, Z_bytes<endian::little>{"-1"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"-3393939987200333"}}, Z_bytes<endian::big>{"-3393939987200333"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"-3393939987200333"}}, Z_bytes<endian::little>{"-3393939987200333"});
        
    }
    /*
    TEST(ZTest, TestIncrementAndDecrement) {
        
        EXPECT_EQ(++Z_bytes<endian::big>{"0x"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(++Z_bytes<endian::big>{"0x00"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(++Z_bytes<endian::big>{"0x000000"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(++Z_bytes<endian::little>{"0x"}, Z_bytes<endian::little>{1});
        EXPECT_EQ(++Z_bytes<endian::little>{"0x00"}, Z_bytes<endian::little>{1});
        EXPECT_EQ(++Z_bytes<endian::little>{"0x000000"}, Z_bytes<endian::little>{1});
        
        EXPECT_EQ(--Z_bytes<endian::big>{"0x"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(--Z_bytes<endian::big>{"0x00"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(--Z_bytes<endian::big>{"0x000000"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(--Z_bytes<endian::little>{"0x"}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(--Z_bytes<endian::little>{"0x00"}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(--Z_bytes<endian::little>{"0x000000"}, Z_bytes<endian::little>{-1});
        
    }*/
    
    TEST(ZTest, TestZBytesToString) {
        
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"1"}),std::string{"1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"1"}), std::string{"1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"23"}), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"23"}), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"5704566599993321"}), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"5704566599993321"}), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"-1"}), std::string{"-1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"-1"}), std::string{"-1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"-3393939987200333"}), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"-3393939987200333"}), std::string{"-3393939987200333"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"1"}), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"1"}), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"23"}), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"23"}), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"5704566599993321"}), std::string{"144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"5704566599993321"}), std::string{"144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-3393939987200333"}), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-3393939987200333"}), std::string{"-3393939987200333"});
        
    }
    
}
