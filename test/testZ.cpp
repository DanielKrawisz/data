// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"

namespace data {
    
    
    TEST(ZTest, TestZToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{"0"}), std::string{"0x"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-2"}), std::string{"0xfe"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"128"}), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-128"}), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-129"}), std::string{"0xff7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"0x7f33"} << 128), std::string{"0x7f3300000000000000000000000000000000"});
        
    }
    
    TEST(ZTest, TestStringToZ) {
        
        EXPECT_THROW(Z{""}, exception);
        EXPECT_THROW(Z{"-0x01"}, exception);
        
        EXPECT_NO_THROW(Z{"-1"});
        
        EXPECT_EQ(Z{}, Z{"0"});
        EXPECT_EQ(Z{-1}, Z{"-1"});
        EXPECT_EQ(Z{"-1"}, Z{"0xff"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{0}), "0x");
        EXPECT_EQ(encoding::signed_decimal::write(Z{0}), "0");
        EXPECT_EQ(encoding::hexidecimal::write(Z{1}), "0x01");
        EXPECT_EQ(encoding::signed_decimal::write(Z{1}), "1");
        EXPECT_EQ(encoding::hexidecimal::write(Z{-1}), "0xff");
        EXPECT_EQ(encoding::signed_decimal::write(Z{-1}), "-1");
        
        EXPECT_TRUE(Z{"0x80000000000000000000"} < Z{"0x7fffffffffffffffffff"});
        EXPECT_TRUE(Z{"0xff"} < Z{"0x00ff"});
        
    }
    
}
