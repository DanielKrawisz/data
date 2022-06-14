// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/encoding/integer.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include "gtest/gtest.h"

namespace data {
    template <typename Z> void test_Z_to_string() {
        
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("0")), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("-1")), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("-2")), std::string{"0xfe"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("127")), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("128")), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("-128")), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("-129")), std::string{"0xff7f"});
        
        EXPECT_EQ(encoding::integer::write(Z::read("1")),std::string{"1"});
        EXPECT_EQ(encoding::integer::write(Z::read("23")), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(Z::read("5704566599993321")), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::integer::write(Z::read("-1")), std::string{"-1"});
        EXPECT_EQ(encoding::integer::write(Z::read("-3393939987200333")), std::string{"-3393939987200333"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("1")), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("23")), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("5704566599993321")), std::string{"144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("-1")), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z::read("-3393939987200333")), std::string{"-3393939987200333"});
        
    };
    
    TEST(ZTest, TestZToString) {
        test_Z_to_string<Z>();
        test_Z_to_string<Z_bytes_big>();
        test_Z_to_string<Z_bytes_little>();
        // TODO need something different for two's complement. 
    }
    
    template <typename Z> void test_string_to_Z() {
        
        EXPECT_NO_THROW(Z{});
        EXPECT_NO_THROW(Z::read(""));
        EXPECT_NO_THROW(Z::read("-1"));
        EXPECT_THROW(Z::read("-0x01"), std::invalid_argument);
        EXPECT_NO_THROW(Z::read("0x00"));
        EXPECT_NO_THROW(Z::read("0xff"));
        EXPECT_EQ(Z{-1}, Z::read("-1"));
        EXPECT_EQ(Z::read("-1"), Z::read("0xff"));
        
        EXPECT_EQ(encoding::hexidecimal::write(Z{"0"}), std::string{"0x"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-2"}), std::string{"0xfe"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"128"}), std::string{"0x0080"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-128"}), std::string{"0x80"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"-129"}), std::string{"0xff7f"});
        EXPECT_EQ(encoding::hexidecimal::write(Z{"0x7f33"} << 128), std::string{"0x7f3300000000000000000000000000000000"});
        
        EXPECT_TRUE(Z::read("0x80000000000000000000") < Z::read("0x7fffffffffffffffffff"));
        EXPECT_TRUE(Z::read("0xff") < Z::read("0x00ff"));
        
        EXPECT_EQ(encoding::signed_decimal::write(Z{0}), "0");
        EXPECT_EQ(encoding::signed_decimal::write(Z{1}), "1");
        EXPECT_EQ(encoding::signed_decimal::write(Z{-1}), "-1");
        
    }
    
    TEST(ZTest, TestStringToZ) {
        
        test_string_to_Z<Z>();
        test_string_to_Z<Z_bytes_big>();
        test_string_to_Z<Z_bytes_little>();
        
    }
    
}
