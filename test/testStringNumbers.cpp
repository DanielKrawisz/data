// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data::math {
    template <typename N> void test_invalid(N &invalid, N &valid) {
        
        EXPECT_THROW(sign(invalid), std::invalid_argument);
        
        EXPECT_THROW(abs<N>{}(invalid), std::invalid_argument);
        
        EXPECT_THROW(invalid++, std::invalid_argument);
        
        EXPECT_THROW(invalid--, std::invalid_argument);
        
        EXPECT_THROW(++invalid, std::invalid_argument);
        
        EXPECT_THROW(--invalid, std::invalid_argument);
        
        EXPECT_THROW(invalid << 0, std::invalid_argument);
        
        EXPECT_THROW(invalid >> 0, std::invalid_argument);
        
        EXPECT_THROW(invalid <=> valid, std::invalid_argument);
        
        EXPECT_THROW(invalid | valid, std::invalid_argument);
        
        EXPECT_THROW(invalid & valid, std::invalid_argument);
        
        EXPECT_THROW(invalid + valid, std::invalid_argument);
        
        EXPECT_THROW(invalid - valid, std::invalid_argument);
        
        EXPECT_THROW(invalid * valid, std::invalid_argument);
        
        EXPECT_THROW(valid <=> invalid, std::invalid_argument);
        
        EXPECT_THROW(valid | invalid, std::invalid_argument);
        
        EXPECT_THROW(valid & invalid, std::invalid_argument);
        
        EXPECT_THROW(valid + invalid, std::invalid_argument);
        
        EXPECT_THROW(valid - invalid, std::invalid_argument);
        
        EXPECT_THROW(valid * invalid, std::invalid_argument);
        
    }
    
    TEST(HexidecimalTest, TestHexidecimalInvalid) {
        
        dec_uint d{""};
        base58_uint b{""};
        hex_uint h0{""};
        hex_int_ones h1{""};
        hex_int_twos h2{""};
        
        dec_uint dv{"1"};
        base58_uint bv{"1"};
        hex_uint h0v{"1"};
        hex_int_ones h1v{"1"};
        hex_int_twos h2v{"1"};
        
        test_invalid(d, dv);
        test_invalid(b, bv);
        test_invalid(h0, h0v);
        test_invalid(h1, h1v);
        test_invalid(h2, h2v);
        
        EXPECT_THROW(number::trim(h0), std::invalid_argument);
        EXPECT_THROW(number::trim(h1), std::invalid_argument);
        EXPECT_THROW(number::trim(h2), std::invalid_argument);
        
        EXPECT_THROW(number::extend(h0, 50), std::invalid_argument);
        EXPECT_THROW(number::extend(h1, 50), std::invalid_argument);
        EXPECT_THROW(number::extend(h2, 50), std::invalid_argument);
        
        EXPECT_THROW(-h1, std::invalid_argument);
        EXPECT_THROW(-h2, std::invalid_argument);
        
        EXPECT_THROW(~h1, std::invalid_argument);
        
    }
}

namespace data {
    
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
        uint32 num = 432;
        
        auto hex_div_16 = hex_uint{static_cast<int64>(num)}.divide(16);
        auto hex_div_15 = hex_uint{static_cast<int64>(num)}.divide(15);
        
        math::division<dec_uint, uint64>       dec_expected_10{dec_uint{43}, 2};
        math::division<hex_uint, uint64>       hex_expected_16{hex_uint{27}, 0};
        math::division<base58_uint, uint64>    base58_expected_58{base58_uint{7}, 26};
        
        math::division<dec_uint, uint64>       dec_expected_9{dec_uint{48}, 0};
        math::division<hex_uint, uint64>       hex_expected_15{hex_uint{28}, 12};
        math::division<base58_uint, uint64>    base58_expected_57{base58_uint{7}, 33};
        
        EXPECT_EQ(dec_uint{num}.divide(10),    dec_expected_10);
        EXPECT_EQ(hex_div_16,                  hex_expected_16);
        EXPECT_EQ(base58_uint{num}.divide(58), base58_expected_58);
        
        EXPECT_EQ(dec_uint{num}.divide(9),     dec_expected_9);
        EXPECT_EQ(hex_div_15,                  hex_expected_15);
        EXPECT_EQ(base58_uint{num}.divide(57), base58_expected_57);
    }
}
