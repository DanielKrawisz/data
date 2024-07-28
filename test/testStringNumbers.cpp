// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/encoding/base58.hpp"
#include "gtest/gtest.h"

namespace data::math {
    template <typename N> void test_invalid (N &invalid, N &valid) {
        
        EXPECT_THROW (data::sign (invalid), exception);
        
        EXPECT_THROW (abs<N>{} (invalid), exception);
        
        EXPECT_THROW (invalid++, exception);
        
        EXPECT_THROW (invalid--, exception);
        
        EXPECT_THROW (++invalid, exception);
        
        EXPECT_THROW (--invalid, exception);
        
        EXPECT_THROW (invalid << 0, exception);
        
        EXPECT_THROW (invalid >> 0, exception);
        
        EXPECT_THROW (invalid <=> valid, exception);
        
        EXPECT_THROW (invalid | valid, exception);
        
        EXPECT_THROW (invalid & valid, exception);
        
        EXPECT_THROW (invalid + valid, exception);
        
        EXPECT_THROW (invalid - valid, exception);
        
        EXPECT_THROW (invalid * valid, exception);
        
        EXPECT_THROW (valid <=> invalid, exception);
        
        EXPECT_THROW (valid | invalid, exception);
        
        EXPECT_THROW (valid & invalid, exception);
        
        EXPECT_THROW (valid + invalid, exception);
        
        EXPECT_THROW (valid - invalid, exception);
        
        EXPECT_THROW (valid * invalid, exception);
        
    }
    
    TEST (StringNumbersTest, TestInvalidStringNumbers) {
        
        dec_uint d {""};
        dec_int s {""};
        base58_uint b {""};
        hex_uint h0 {""};
        hex_int_ones h1 {""};
        hex_int_twos h2 {""};
        
        dec_uint dv {"1"};
        dec_int sv {"1"};
        base58_uint bv {"1"};
        hex_uint h0v {"1"};
        hex_int_ones h1v {"1"};
        hex_int_twos h2v {"1"};
        
        test_invalid (d, dv);
        test_invalid (s, sv);
        test_invalid (b, bv);
        test_invalid (h0, h0v);
        test_invalid (h1, h1v);
        test_invalid (h2, h2v);
        
        EXPECT_THROW (number::trim (h0), std::exception);
        EXPECT_THROW (number::trim (h1), std::exception);
        EXPECT_THROW (number::trim (h2), std::exception);
        
        EXPECT_THROW (number::extend (h0, 50), std::exception);
        EXPECT_THROW (number::extend (h1, 50), std::exception);
        EXPECT_THROW (number::extend (h2, 50), std::exception);
        
        EXPECT_THROW (-h1, std::exception);
        EXPECT_THROW (-h2, std::exception);
        
        EXPECT_THROW (~h1, std::exception);
        
    }
}

namespace data::encoding {
    
    TEST (StringNumbersTest, TestConstructStringNumbers) {
        
        EXPECT_EQ (dec_uint {}, "0");
        EXPECT_EQ (dec_int {}, "0");
        EXPECT_EQ (hex_uint {}, "0x");
        EXPECT_EQ (hex_int {}, "0x");
        EXPECT_EQ (hex_int_twos {}, "0x");
        EXPECT_EQ (base58_uint {}, "1");
        
        EXPECT_EQ (dec_uint {}, dec_uint {0u});
        EXPECT_EQ (dec_int {}, dec_int {0});
        EXPECT_EQ (hex_uint {}, hex_uint {0});
        EXPECT_EQ (base58_uint {}, base58_uint {0});
        
        EXPECT_EQ (dec_uint {1u}, "1");
        EXPECT_EQ (hex_uint {1u}, "0x01");
        EXPECT_EQ (base58_uint {1u}, "2");
        
        EXPECT_EQ (dec_uint {27u}, "27");
        EXPECT_EQ (hex_uint {27u}, "0x1b");
        EXPECT_EQ (base58_uint {27u}, "U");
        
    }
    
    TEST (StringNumbersTest, TestAddStringNumbers) {
        
        EXPECT_EQ (dec_uint {1u} + dec_uint {2}, "3");
        EXPECT_EQ (dec_int {1} + dec_int {2}, "3");
        
        auto hex_uint_1 = hex_uint {1u};
        auto hex_uint_2 = hex_uint {2u};
        EXPECT_EQ (hex_uint_1 + hex_uint_2, "0x03");
        
        EXPECT_EQ (hex_int {1} + hex_int {2}, "0x03");
        auto hex_int2_1 = hex_int_twos {1};
        auto hex_int2_2 = hex_int_twos {2};
        EXPECT_EQ (hex_int2_1 + hex_int2_2, "0x03");
        
        EXPECT_EQ (base58_uint {1} + base58_uint {2}, "4");
        
    }
    
    TEST (StringNumbersTest, TestSubtractStringNumbers) {

        auto dec_minus_result = dec_uint {2} - dec_uint {1};
        EXPECT_EQ (dec_minus_result, "1");
        EXPECT_EQ (hex_uint {2} - hex_uint {1}, "0x01");
        EXPECT_EQ (base58_uint {2} - base58_uint {1}, "2");
        
        EXPECT_EQ (dec_uint {1} - dec_uint {2}, "0");
        EXPECT_EQ (dec_int {1} - dec_int {2}, "-1");
        EXPECT_EQ (hex_uint {1} - hex_uint {2}, "0x");
        
        auto hex_int_1 = hex_int {1};
        auto hex_int_2 = hex_int {2};
        auto hex_minus = hex_int_1 - hex_int_2;

        EXPECT_EQ (hex_minus, "0xff");
        EXPECT_EQ (hex_int_twos {1} - hex_int_twos {2}, "0x81");
        EXPECT_EQ (base58_uint {1} - base58_uint {2}, "1");
        
    }
    
    TEST (StringNumbersTest, TestMultiplyStringNumbers) {
        
        EXPECT_EQ (dec_uint {27} * dec_uint {25}, "675");
        EXPECT_EQ (dec_int {27} * dec_int {25}, "675");
        EXPECT_EQ (dec_int {27} * dec_int {-25}, dec_int {"-675"});
        EXPECT_EQ (hex_uint {27} * hex_uint {25}, hex_uint {675});
        EXPECT_EQ (hex_int {27} * hex_int {25}, hex_int {675});
        EXPECT_EQ (hex_int_twos{27} * hex_int_twos {25}, hex_int_twos {675});
        EXPECT_EQ (hex_int {27} * hex_int {-25}, hex_int {-675});
        EXPECT_EQ (hex_int_twos {27} * hex_int_twos {-25}, hex_int_twos {-675});
        EXPECT_EQ (base58_uint {27} * base58_uint {25}, base58_uint {675});
        
    }

    TEST (StringNumbersTest, TestDivideStringNumbers) {
        
        uint64 num = 432;
        
        auto hex_div_16 = hex_uint {num}.divide (16);
        
        math::division<dec_uint, uint64>    dec_expected_10 {43, 2};
        math::division<hex_uint>            hex_expected_16 {27, 0};
        math::division<base58_uint, uint64> b58_expected_58 {7, 26};
        
        math::division<dec_uint, uint64>     dec_expected_9 {48, 0};
        math::division<hex_uint>           hex_expected_15 {28, 12};
        math::division<base58_uint, uint64> b58_expected_57 {7, 33};
        
        EXPECT_EQ (dec_uint {num}.divide (10),     dec_expected_10);
        EXPECT_EQ (hex_div_16,                     hex_expected_16);
        EXPECT_EQ (base58_uint {num}.divide (58),  b58_expected_58);
        
        EXPECT_EQ (dec_uint {num}.divide (9),       dec_expected_9);
        EXPECT_EQ (hex_uint {num}.divide (15),     hex_expected_15);
        EXPECT_EQ (base58_uint {num}.divide (57),  b58_expected_57);
        
    }

    TEST (StringNumbersTest, TestConvertStringNumbers) {
        EXPECT_EQ (N (dec_uint {1145}), N (1145));
        EXPECT_EQ (N (dec_uint {916}), N (916));
        EXPECT_EQ (N (dec_uint {229}), N (229));

        EXPECT_EQ (Z (dec_int {1145}), Z (1145));
        EXPECT_EQ (Z (dec_int {916}), Z (916));
        EXPECT_EQ (Z (dec_int {229}), Z (229));

        EXPECT_EQ (N (base58_uint {1145}), N (1145));
        EXPECT_EQ (N (base58_uint {916}), N (916));
        EXPECT_EQ (N (base58_uint {229}), N (229));

        EXPECT_EQ (N (hex_uint {1145}), N (1145));
        EXPECT_EQ (N (hex_uint {916}), N (916));
        EXPECT_EQ (N (hex_uint {229}), N (229));

        EXPECT_EQ (Z (hex_int {1145}), Z (1145));
        EXPECT_EQ (Z (hex_int {916}), Z (916));
        EXPECT_EQ (Z (hex_int {229}), Z (229));

        EXPECT_EQ (Z (hex_int_twos {1145}), Z (1145));
        EXPECT_EQ (Z (hex_int_twos {916}), Z (916));
        EXPECT_EQ (Z (hex_int_twos {229}), Z (229));
    }
}
