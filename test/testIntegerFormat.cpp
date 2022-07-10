// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/numbers.hpp>

namespace data::encoding {

    TEST (IntegerFormatTest, TestIntegerFormat) {
        
        EXPECT_FALSE (decimal::valid (""));
        EXPECT_TRUE (decimal::valid ("0"));
        EXPECT_FALSE (decimal::valid ("01"));
        EXPECT_TRUE (decimal::valid ("1"));
        EXPECT_FALSE (hexidecimal::valid (""));
        EXPECT_TRUE (hexidecimal::valid ("0x"));
        EXPECT_TRUE (hexidecimal::valid ("0xaf"));
        EXPECT_TRUE (hexidecimal::valid ("0xAF"));
        EXPECT_FALSE (hexidecimal::valid ("0xa"));
        EXPECT_FALSE (hexidecimal::valid ("0xaF"));
        
        EXPECT_FALSE (integer::valid (""));
        EXPECT_TRUE (integer::valid ("-1"));
        EXPECT_FALSE (integer::valid ("-0xaf"));
        
        EXPECT_TRUE (integer::valid ("0"));
        EXPECT_TRUE (integer::valid ("0x00"));
        EXPECT_TRUE (integer::valid ("-1"));
        EXPECT_TRUE (integer::valid ("0xff"));
        EXPECT_TRUE (integer::valid ("0x8000"));
        EXPECT_TRUE (integer::valid ("0x7fff"));
        
        EXPECT_FALSE (integer::valid ("00"));
        EXPECT_FALSE (integer::valid (""));
        EXPECT_FALSE (integer::valid ("-01"));
        EXPECT_FALSE (integer::valid ("-0xff"));
        EXPECT_FALSE (integer::valid ("-0"));
        
        EXPECT_FALSE (integer::negative ("0"));
        EXPECT_FALSE (integer::negative ("0x00"));
        EXPECT_FALSE (integer::negative ("-0"));
        EXPECT_TRUE (integer::negative ("-1"));
        EXPECT_TRUE (integer::negative ("-01"));
        EXPECT_TRUE (integer::negative ("0xff"));
        EXPECT_TRUE (integer::negative ("0x8000"));
        EXPECT_FALSE (integer::negative ("0x7fff"));
        
    }
    
    TEST (IntegerFormatTest, TestWriteNumbersN) {
        
        EXPECT_EQ (encoding::decimal::write (N_bytes_big {1}), std::string {"1"});
        EXPECT_EQ (encoding::decimal::write (N_bytes_big {11}), std::string {"11"});
        EXPECT_EQ (encoding::decimal::write (N_bytes_big {23}), std::string {"23"});
        EXPECT_EQ (encoding::decimal::write (N_bytes_big::read ("5704566599993321")), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::decimal::write
            (N_bytes_big::read ("57045489000484922397266599993321")),
                std::string {"57045489000484922397266599993321"});

        EXPECT_EQ (encoding::decimal::write (N_bytes_little {1}), std::string {"1"});
        EXPECT_EQ (encoding::decimal::write (N_bytes_little {11}), std::string {"11"});
        EXPECT_EQ (encoding::decimal::write (N_bytes_little {23}), std::string {"23"});
        EXPECT_EQ (encoding::decimal::write (N_bytes_little::read ("5704566599993321")), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::decimal::write
            (N_bytes_little::read ("57045489000484922397266599993321")),
                std::string {"57045489000484922397266599993321"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_big {1}), std::string {"0x01"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_little {1}), std::string {"0x01"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_big {23}), std::string {"0x17"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_little {23}), std::string {"0x17"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (N_bytes_big::read ("5704566599993321")), std::string {"0x144445e9ca47e9"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (N_bytes_little::read ("5704566599993321")), std::string {"0x144445e9ca47e9"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_big::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_big::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_big::read ("128")), std::string {"0x80"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_little::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_little::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes_little::read ("128")), std::string {"0x80"});

    }

    TEST (IntegerFormatTest, TestWriteNumbersZ) {

        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big {1}), std::string {"1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big {11}), std::string {"11"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big {23}), std::string {"23"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("5704566599993321")), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::signed_decimal::write
            (Z_bytes_big::read ("57045489000484922397266599993321")),
                std::string {"57045489000484922397266599993321"});

        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little {1}), std::string {"1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little {11}), std::string {"11"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little {23}), std::string {"23"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("5704566599993321")), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::signed_decimal::write
            (Z_bytes_little::read ("57045489000484922397266599993321")),
                std::string {"57045489000484922397266599993321"});

        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big {-1}), std::string {"-1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big {-11}), std::string {"-11"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big {-23}), std::string {"-23"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("-5704566599993321")), std::string {"-5704566599993321"});
        EXPECT_EQ (encoding::signed_decimal::write
            (Z_bytes_little::read ("-57045489000484922397266599993321")),
                std::string {"-57045489000484922397266599993321"});

        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little {-1}), std::string {"-1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little {-11}), std::string {"-11"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little {-23}), std::string {"-23"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("-5704566599993321")), std::string {"-5704566599993321"});
        EXPECT_EQ (encoding::signed_decimal::write
            (Z_bytes_little::read ("-57045489000484922397266599993321")),
                std::string {"-57045489000484922397266599993321"});

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-1")), std::string {"0xff"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-2")), std::string {"0xfe"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("128")), std::string {"0x0080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-128")), std::string {"0x80"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-129")), std::string {"0xff7f"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-1")), std::string {"0xff"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-2")), std::string {"0xfe"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("128")), std::string {"0x0080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-128")), std::string {"0x80"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-129")), std::string {"0xff7f"});

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-1")), std::string {"0xff"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-2")), std::string {"0xfe"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("128")), std::string {"0x0080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-128")), std::string {"0x80"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-129")), std::string {"0xff7f"});

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("-1")), std::string {"0x81"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("-2")), std::string {"0x82"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("128")), std::string {"0x0080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("-128")), std::string {"0x8080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_big::read ("-129")), std::string {"0x8081"});

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("0")), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("-1")), std::string {"0x81"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("-2")), std::string {"0x82"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("127")), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("128")), std::string {"0x0080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("-128")), std::string {"0x8080"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_twos_little::read ("-129")), std::string {"0x8081"});
        
    }

    TEST (IntegerFormatTest, TestReadHex) {

        std::string hex_string {"0x0001020304"};

        auto ob = *encoding::hexidecimal::read<endian::big> (hex_string);
        auto ol = *encoding::hexidecimal::read<endian::little> (hex_string);

        EXPECT_EQ (ob[0], 0);
        EXPECT_EQ (ol[4], 0);
        EXPECT_EQ (ob[4], 4);
        EXPECT_EQ (ol[0], 4);

        EXPECT_EQ (*ob.begin (), 0);
        EXPECT_EQ (*ob.rbegin (), 4);
        EXPECT_EQ (*ol.begin (), 4);
        EXPECT_EQ (*ol.rbegin (), 0);

        EXPECT_EQ (*ob.words ().begin (), 4);
        EXPECT_EQ (*ob.words ().rbegin (), 0);
        EXPECT_EQ (*ol.words ().begin (), 4);
        EXPECT_EQ (*ol.words ().rbegin (), 0);

    }

}
