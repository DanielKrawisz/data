// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/encoding/base58.hpp"
#include "gtest/gtest.h"

namespace data::encoding {
    
    TEST(StringNumbersTest, TestConstructStringNumbers) {
        
        EXPECT_EQ(dec_uint{}, "0");
        EXPECT_EQ(hex_uint{}, "0x");
        EXPECT_EQ(base58_uint{}, "1");
        
        EXPECT_EQ(dec_uint{}, dec_uint{0});
        EXPECT_EQ(hex_uint{}, hex_uint{0});
        EXPECT_EQ(base58_uint{}, base58_uint{0});
        
        EXPECT_EQ(dec_uint{1}, "1");
        EXPECT_EQ(hex_uint{1}, "0x01");
        EXPECT_EQ(base58_uint{1}, "2");
        
        EXPECT_EQ(dec_uint{27}, "27");
        EXPECT_EQ(hex_uint{27}, "0x1b");
        EXPECT_EQ(base58_uint{27}, "U");
        
    }
    
    TEST(StringNumbersTest, TestAddStringNumbers) {
        
        EXPECT_EQ(dec_uint{1} + dec_uint{2}, "3");
        EXPECT_EQ(hex_uint{1} + hex_uint{2}, "0x03");
        EXPECT_EQ(base58_uint{1} + base58_uint{2}, "4");
        
    }
    
    TEST(StringNumbersTest, TestSubtractStringNumbers) {
        
        EXPECT_EQ(dec_uint{2} - dec_uint{1}, "1");
        EXPECT_EQ(hex_uint{2} - hex_uint{1}, "0x01");
        EXPECT_EQ(base58_uint{2} - base58_uint{1}, "2");
        
        EXPECT_EQ(dec_uint{1} - dec_uint{2}, "0");
        EXPECT_EQ(hex_uint{1} - hex_uint{2}, "0x");
        EXPECT_EQ(base58_uint{1} - base58_uint{2}, "1");
        
    }
    
    TEST(StringNumbersTest, TestMultiplyStringNumbers) {
        
        EXPECT_EQ(dec_uint{27} * dec_uint{25}, "675");
        EXPECT_EQ(hex_uint{27} * hex_uint{25}, hex_uint{675});
        EXPECT_EQ(base58_uint{27} * base58_uint{25}, base58_uint{675});
        
    }
    
    TEST(StringNumbersTest, TestDivideStringNumbers) {
        
        uint64 num = 432;
        
        auto hex_div_16 = hex_uint{num}.divide(16);
        
        math::division<dec_uint, uint64>    dec_expected_10{43, 2};
        math::division<hex_uint, uint64>    hex_expected_16{27, 0};
        math::division<base58_uint, uint64> b58_expected_58{7, 26};
        
        math::division<dec_uint, uint64>    dec_expected_9{48, 0};
        math::division<hex_uint, uint64>    hex_expected_15{28, 12};
        math::division<base58_uint, uint64> b58_expected_57{7, 33};
        
        EXPECT_EQ(dec_uint{num}.divide(10),    dec_expected_10);
        EXPECT_EQ(hex_div_16,                    hex_expected_16);
        EXPECT_EQ(base58_uint{num}.divide(58), b58_expected_58);
        
        EXPECT_EQ(dec_uint{num}.divide(9),     dec_expected_9);
        EXPECT_EQ(hex_uint{num}.divide(15),    hex_expected_15);
        EXPECT_EQ(base58_uint{num}.divide(57), b58_expected_57);
        
    }
}
