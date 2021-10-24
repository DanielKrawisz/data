// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"

namespace data {
    
    
    TEST(ZTest, TestZToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-2"}), std::string{"0xfe"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"128"}), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-128"}), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-129"}), std::string{"0xff7f"});
        
    }
    
    TEST(ZTest, TestStringToZ) {
        
        EXPECT_FALSE(Z{}.valid());
        EXPECT_FALSE(Z{N{}}.valid());
        EXPECT_FALSE(Z{""}.valid());
        EXPECT_TRUE(Z{"-1"}.valid());
        EXPECT_FALSE(Z{"-0x01"}.valid());
        
        EXPECT_EQ(N{}, N{""});
        EXPECT_EQ(Z{-1}, Z{"-1"});
        EXPECT_EQ(Z{"-1"}, Z{"0xff"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{0}), "0x00");
        EXPECT_EQ(encoding::integer::write(Z{0}), "0");
        EXPECT_EQ(encoding::hexidecimal::write(Z{1}), "0x01");
        EXPECT_EQ(encoding::integer::write(Z{1}), "1");
        EXPECT_EQ(encoding::hexidecimal::write(Z{-1}), "0xff");
        EXPECT_EQ(encoding::integer::write(Z{-1}), "-1");
        
        EXPECT_TRUE(Z{"0x80000000000000000000"} < Z{"0x7fffffffffffffffffff"});
        EXPECT_TRUE(Z{"0xff"} < Z{"0x00ff"});
        
    }
    
}
