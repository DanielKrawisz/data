// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/integer.hpp"
#include "data/encoding/base58.hpp"
#include "gtest/gtest.h"

namespace data::encoding {
    using dec_number = decimal::N;
    using hex_number = hexidecimal::N;
    using b58_number = base58::string;
    
    TEST(StringNumbersTest, TestConstructStringNumbers) {
        EXPECT_EQ(dec_number{}, "0");
        EXPECT_EQ(hex_number{}, "0x00");
        EXPECT_EQ(b58_number{}, "1");
        
        EXPECT_EQ(dec_number{}, dec_number{0});
        EXPECT_EQ(hex_number{}, hex_number{0});
        EXPECT_EQ(b58_number{}, b58_number{0});
        
        EXPECT_EQ(dec_number{1}, "1");
        EXPECT_EQ(hex_number{1}, "0x01");
        EXPECT_EQ(b58_number{1}, "2");
        
        EXPECT_EQ(dec_number{27}, "27");
        EXPECT_EQ(hex_number{27}, "0x1b");
        EXPECT_EQ(b58_number{27}, "U");
    }
    
    TEST(StringNumbersTest, TestAddStringNumbers) {
        EXPECT_EQ(dec_number{1} + dec_number{2}, "3");
        EXPECT_EQ(hex_number{1} + hex_number{2}, "0x03");
        EXPECT_EQ(b58_number{1} + b58_number{2}, "4");
    }
    
    TEST(StringNumbersTest, TestSubtractStringNumbers) {
        EXPECT_EQ(dec_number{2} - dec_number{1}, "1");
        EXPECT_EQ(hex_number{2} - hex_number{1}, "0x01");
        EXPECT_EQ(b58_number{2} - b58_number{1}, "2");
        
        EXPECT_EQ(dec_number{1} - dec_number{2}, "0");
        EXPECT_EQ(hex_number{1} - hex_number{2}, "0x00");
        EXPECT_EQ(b58_number{1} - b58_number{2}, "1");
    }
    
    TEST(StringNumbersTest, TestMultiplyStringNumbers) {
        EXPECT_EQ(dec_number{27} * dec_number{25}, "675");
        EXPECT_EQ(hex_number{27} * hex_number{25}, hex_number{675});
        EXPECT_EQ(b58_number{27} * b58_number{25}, b58_number{675});
    }
    
    TEST(StringNumbersTest, TestDivideStringNumbers) {
        uint64 num = 432;
        
        auto hex_div_16 = hex_number{num}.divide(16);
        
        math::division<dec_number, uint64> dec_expected_10{43, 2};
        math::division<hex_number, uint64> hex_expected_16{27, 0};
        math::division<b58_number, uint64> b58_expected_58{7, 26};
        
        math::division<dec_number, uint64> dec_expected_9{48, 0};
        math::division<hex_number, uint64> hex_expected_15{28, 12};
        math::division<b58_number, uint64> b58_expected_57{7, 33};
        
        EXPECT_EQ(dec_number{num}.divide(10), dec_expected_10);
        EXPECT_EQ(hex_div_16,                     hex_expected_16);
        EXPECT_EQ(b58_number{num}.divide(58), b58_expected_58);
        
        EXPECT_EQ(dec_number{num}.divide(9),  dec_expected_9);
        EXPECT_EQ(hex_number{num}.divide(15), hex_expected_15);
        EXPECT_EQ(b58_number{num}.divide(57), b58_expected_57);
    }
}
