// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/list.hpp>
#include "gtest/gtest.h"

namespace data {

    TEST (ZBytes, ZBytesToHexString) {
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("0")), std::string ("0x"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-1")), std::string ("0xff"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-2")), std::string ("0xfe"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("127")), std::string ("0x7f"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("128")), std::string ("0x0080"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-128")), std::string ("0x80"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big::read ("-129")), std::string ("0xff7f"));
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("0")), std::string ("0x"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-1")), std::string ("0xff"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-2")), std::string ("0xfe"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("127")), std::string ("0x7f"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("128")), std::string ("0x0080"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-128")), std::string ("0x80"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little::read ("-129")), std::string ("0xff7f"));

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("0")), std::string ("0x"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("-1")), std::string ("0xff"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("-2")), std::string ("0xfe"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("127")), std::string ("0x7f"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("128")), std::string ("0x0080"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("-128")), std::string ("0x80"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_big ("-129")), std::string ("0xff7f"));

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("0")), std::string ("0x"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("-1")), std::string ("0xff"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("-2")), std::string ("0xfe"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("127")), std::string ("0x7f"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("128")), std::string ("0x0080"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("-128")), std::string ("0x80"));
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (Z_bytes_little ("-129")), std::string ("0xff7f"));

        EXPECT_EQ (byte_slice (Z_bytes_big ("0")), *encoding::hex::read (""));
        EXPECT_EQ (byte_slice (Z_bytes_big ("-1")), *encoding::hex::read ("ff"));
        EXPECT_EQ (byte_slice (Z_bytes_big ("-2")), *encoding::hex::read ("fe"));
        EXPECT_EQ (byte_slice (Z_bytes_big ("127")), *encoding::hex::read ("7f"));
        EXPECT_EQ (byte_slice (Z_bytes_big ("128")), *encoding::hex::read ("0080"));
        EXPECT_EQ (byte_slice (Z_bytes_big ("-128")), *encoding::hex::read ("80"));
        EXPECT_EQ (byte_slice (Z_bytes_big ("-129")), *encoding::hex::read ("ff7f"));

        EXPECT_EQ (byte_slice (Z_bytes_little ("0")), *encoding::hex::read (""));
        EXPECT_EQ (byte_slice (Z_bytes_little ("-1")), *encoding::hex::read ("ff"));
        EXPECT_EQ (byte_slice (Z_bytes_little ("-2")), *encoding::hex::read ("fe"));
        EXPECT_EQ (byte_slice (Z_bytes_little ("127")), *encoding::hex::read ("7f"));
        EXPECT_EQ (byte_slice (Z_bytes_little ("128")), *encoding::hex::read ("8000"));
        EXPECT_EQ (byte_slice (Z_bytes_little ("-128")), *encoding::hex::read ("80"));
        EXPECT_EQ (byte_slice (Z_bytes_little ("-129")), *encoding::hex::read ("7fff"));
        
    }

    TEST (ZBytes, StringToZBytes) {
        
        EXPECT_NO_THROW (Z_bytes_big {});
        EXPECT_NO_THROW (Z_bytes_big {N {}});
        EXPECT_NO_THROW (Z_bytes_big {Z {}});
        EXPECT_THROW (Z_bytes_big {""}, std::invalid_argument);
        EXPECT_NO_THROW (Z_bytes_big {"-1"});
        EXPECT_THROW (Z_bytes_big {"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW (Z_bytes_big {"0x00"});
        EXPECT_NO_THROW (Z_bytes_big {"0xff"});
        
        EXPECT_NO_THROW (Z_bytes_little {});
        EXPECT_NO_THROW (Z_bytes_little {N {}});
        EXPECT_NO_THROW (Z_bytes_little {Z {}});
        EXPECT_THROW (Z_bytes_little {""}, std::invalid_argument);
        EXPECT_NO_THROW (Z_bytes_little {"-1"});
        EXPECT_THROW (Z_bytes_little {"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW (Z_bytes_big {"0x00"});
        EXPECT_NO_THROW (Z_bytes_big {"0xff"});
        
    }
    
    TEST (ZBytes, ZeroSize) {
        
        EXPECT_EQ (Z_bytes_big::read ("0x").size (), 0);
        EXPECT_EQ (Z_bytes_big::read ("0x00").size (), 1);
        EXPECT_EQ (Z_bytes_big::read ("0x000000").size (), 3);
        
        EXPECT_EQ (Z_bytes_little::read ("0x").size (), 0);
        EXPECT_EQ (Z_bytes_little::read ("0x00").size (), 1);
        EXPECT_EQ (Z_bytes_little::read ("0x000000").size (), 3);
        
    }

    TEST (ZBytes, ZeroAndNegativeZBytes) {
        
        EXPECT_EQ (Z_bytes_big::read ("0x"), Z_bytes_big {0});
        EXPECT_EQ (Z_bytes_big::read ("0x00"), Z_bytes_big {0});
        EXPECT_EQ (Z_bytes_big::read ("0x000000"), Z_bytes_big {0});
        EXPECT_EQ (Z_bytes_little::read ("0x"), Z_bytes_little {0});
        EXPECT_EQ (Z_bytes_little::read ("0x00"), Z_bytes_little {0});
        EXPECT_EQ (Z_bytes_little::read ("0x000000"), Z_bytes_little {0});
        
        EXPECT_EQ (Z_bytes_big::read ("0x01"), Z_bytes_big {1});
        EXPECT_EQ (Z_bytes_big::read ("0x0001"), Z_bytes_big {1});
        EXPECT_EQ (Z_bytes_big::read ("0x00000001"), Z_bytes_big {1});
        EXPECT_EQ (Z_bytes_little::read ("0x01"), Z_bytes_little {1});
        EXPECT_EQ (Z_bytes_little::read ("0x0001"), Z_bytes_little {1});
        EXPECT_EQ (Z_bytes_little::read ("0x00000001"), Z_bytes_little {1});
        
        EXPECT_EQ (Z_bytes_big::read ("0xff"), Z_bytes_big {-1});
        EXPECT_EQ (Z_bytes_big::read ("0xffff"), Z_bytes_big {-1});
        EXPECT_EQ (Z_bytes_big::read ("0xffffff"), Z_bytes_big {-1});
        EXPECT_EQ (Z_bytes_little::read ("0xff"), Z_bytes_little {-1});
        EXPECT_EQ (Z_bytes_little::read ("0xffff"), Z_bytes_little {-1});
        EXPECT_EQ (Z_bytes_little::read ("0xffffff"), Z_bytes_little {-1});
        
    }

    TEST (ZBytes, ZToZBytes) {
        
        EXPECT_EQ (Z_bytes_big {Z::read ("1")}, Z_bytes_big::read ("1"));
        EXPECT_EQ (Z_bytes_little {Z::read ("1")}, Z_bytes_little::read ("1"));
        EXPECT_EQ (Z_bytes_big {Z::read ("23")}, Z_bytes_big::read ("23"));
        EXPECT_EQ (Z_bytes_little {Z::read ("23")}, Z_bytes_little::read ("23"));
        EXPECT_EQ (Z_bytes_big {Z::read ("5704566599993321")}, Z_bytes_big::read ("5704566599993321"));
        EXPECT_EQ (Z_bytes_little {Z::read ("5704566599993321")}, Z_bytes_little::read ("5704566599993321"));
        EXPECT_EQ (Z_bytes_big {Z::read ("-1")}, Z_bytes_big::read ("-1"));
        EXPECT_EQ (Z_bytes_little {Z::read ("-1")}, Z_bytes_little::read ("-1"));
        EXPECT_EQ (Z_bytes_big {Z::read ("-3393939987200333")}, Z_bytes_big::read ("-3393939987200333"));
        EXPECT_EQ (Z_bytes_little {Z::read ("-3393939987200333")}, Z_bytes_little::read ("-3393939987200333"));
        
    }

    TEST (ZBytes, ZBytesIncrement) {
        
        auto zb0 = Z_bytes_big::read ("0x");
        auto zb1 = Z_bytes_big::read ("0x00");
        auto zb3 = Z_bytes_big::read ("0x000000");
        
        auto zl0 = Z_bytes_little::read ("0x");
        auto zl1 = Z_bytes_little::read ("0x00");
        auto zl3 = Z_bytes_little::read ("0x000000");
        
        EXPECT_EQ (++zb0, Z_bytes_big {1});
        EXPECT_EQ (++zb1, Z_bytes_big {1});
        EXPECT_EQ (++zb3, Z_bytes_big {1});
        
        EXPECT_EQ (++zl0, Z_bytes_little {1});
        EXPECT_EQ (++zl1, Z_bytes_little {1});
        EXPECT_EQ (++zl3, Z_bytes_little {1});
        
    }
    
    TEST (ZBytes, ZBytesDecrement) {
        
        auto zb0 = Z_bytes_big::read ("0x");
        auto zb1 = Z_bytes_big::read ("0x00");
        auto zb3 = Z_bytes_big::read ("0x000000");
        
        auto zl0 = Z_bytes_little::read ("0x");
        auto zl1 = Z_bytes_little::read ("0x00");
        auto zl3 = Z_bytes_little::read ("0x000000");
        
        EXPECT_EQ (--zb0, Z_bytes_big {-1});
        EXPECT_EQ (--zb1, Z_bytes_big {-1});
        EXPECT_EQ (--zb3, Z_bytes_big {-1});
        
        EXPECT_EQ (--zl0, Z_bytes_little {-1});
        EXPECT_EQ (--zl1, Z_bytes_little {-1});
        EXPECT_EQ (--zl3, Z_bytes_little {-1});
        
    }

    TEST (ZBytes, ZBytesToString) {
        
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("1")), std::string {"1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("1")), std::string {"1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("23")), std::string {"23"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("23")), std::string{"23"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("5704566599993321")), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("5704566599993321")), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("-1")), std::string {"-1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("-1")), std::string {"-1"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("-3393939987200333")), std::string {"-3393939987200333"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("-3393939987200333")), std::string {"-3393939987200333"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_big::read ("0xf3f13ac4ac86b3")), std::string {"-3393939987200333"});
        EXPECT_EQ (encoding::signed_decimal::write (Z_bytes_little::read ("0xf3f13ac4ac86b3")), std::string {"-3393939987200333"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_big::read ("1")), std::string {"0x01"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_little::read ("1")), std::string {"0x01"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_big::read ("23")), std::string {"0x17"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_little::read ("23")), std::string {"0x17"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_big::read ("5704566599993321")), std::string {"0x144445e9ca47e9"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_little::read ("5704566599993321")), std::string {"0x144445e9ca47e9"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_big::read ("-1")), std::string {"0xff"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_little::read ("-1")), std::string {"0xff"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_big::read ("-3393939987200333")), std::string {"0xf3f13ac4ac86b3"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower>
            (Z_bytes_little::read ("-3393939987200333")), std::string {"0xf3f13ac4ac86b3"});
        
    }
    
    template<endian::order r>
    N N_Bytes_to_N_stupid (const math::number::N_bytes<r, byte> &n) {
        N x {0};
        for (const byte &b : n.words ().reverse ()) {
            x <<= 8;
            x += b;
        }
        return x;
    }
    
    template<endian::order r, math::negativity zz>
    Z Z_Bytes_to_Z_stupid (const math::number::Z_bytes<r, zz, byte> &z) {
        if (is_negative (z)) return -N_Bytes_to_N_stupid (data::abs (z));
        return N_Bytes_to_N_stupid (abs (z));
    }
    
    template<endian::order r, math::negativity zz>
    math::number::Z_bytes<r, zz, byte> Z_to_Z_Bytes_stupid (const math::Z &n) {
        return math::number::Z_bytes<r, zz, byte>::read
            (encoding::hexidecimal::write<math::negativity::twos, hex_case::lower> (n));
    }
    
    template <typename in> void Z_Bytes_to_Z (in x) {
        
        Z z {x};
        
        Z_bytes_big big {x};
        Z_bytes_little little {x};
        
        Z_bytes_big stupid_big = Z_to_Z_Bytes_stupid<endian::big, math::negativity::twos> (z);
        Z_bytes_little stupid_little = Z_to_Z_Bytes_stupid<endian::little, math::negativity::twos> (z);
        
        EXPECT_EQ (stupid_big, big);
        EXPECT_EQ (stupid_little, little);

        Z Z_big = Z (big);
        Z Z_little = Z (little);
        
        Z Z_big_stupid = Z_Bytes_to_Z_stupid (big);
        Z Z_little_stupid = Z_Bytes_to_Z_stupid (little);
        
        EXPECT_EQ (Z_big_stupid, Z_big);
        EXPECT_EQ (Z_little_stupid, Z_little);
        
        EXPECT_EQ (Z_big, z);
        EXPECT_EQ (Z_little, z);
        
    }

    TEST (ZBytesTest, ZBytesToZ) {

        Z_Bytes_to_Z (0);
        Z_Bytes_to_Z (-1);
        Z_Bytes_to_Z (1);
        Z_Bytes_to_Z (3);
        Z_Bytes_to_Z (-3);
        Z_Bytes_to_Z (229);
        Z_Bytes_to_Z (767);
        Z_Bytes_to_Z (916);
        Z_Bytes_to_Z (1145);
        Z_Bytes_to_Z (-1145);
        Z_Bytes_to_Z ("0x0f00000a00aabbccddeeffffffffffffffff");
        Z_Bytes_to_Z ("0xf000000a00aabbccddeeffffffffffffffff");
        
    }

    TEST (ZBytes, ZBytesOnesAndTwos) {

        EXPECT_EQ (Z_bytes_big (Z_bytes_BC_big (Z_bytes_big {1145})), Z_bytes_big {1145});
        EXPECT_EQ (Z_bytes_big (Z_bytes_BC_big (Z_bytes_big {916})), Z_bytes_big {916});
        EXPECT_EQ (Z_bytes_big (Z_bytes_BC_big (Z_bytes_big {229})), Z_bytes_big {229});

        EXPECT_EQ (Z_bytes_little (Z_bytes_BC_little (Z_bytes_little {1145})), Z_bytes_little {1145});
        EXPECT_EQ (Z_bytes_little (Z_bytes_BC_little (Z_bytes_little {916})), Z_bytes_little {916});
        EXPECT_EQ (Z_bytes_little (Z_bytes_BC_little (Z_bytes_little {229})), Z_bytes_little {229});

        EXPECT_EQ (Z_bytes_BC_big (Z_bytes_big (Z_bytes_BC_big {1145})), Z_bytes_BC_big {1145});
        EXPECT_EQ (Z_bytes_BC_big (Z_bytes_big (Z_bytes_BC_big {916})), Z_bytes_BC_big {916});
        EXPECT_EQ (Z_bytes_BC_big (Z_bytes_big (Z_bytes_BC_big {229})), Z_bytes_BC_big {229});

        EXPECT_EQ (Z_bytes_BC_little (Z_bytes_little (Z_bytes_BC_little {1145})), Z_bytes_BC_little {1145});
        EXPECT_EQ (Z_bytes_BC_little (Z_bytes_little (Z_bytes_BC_little {916})), Z_bytes_BC_little {916});
        EXPECT_EQ (Z_bytes_BC_little (Z_bytes_little (Z_bytes_BC_little {229})), Z_bytes_BC_little {229});
    }

    TEST (ZBytes, ZBytesZero) {
        list<string> ones_zeros {"0", "0x", "0x00", "0x0000"};
        list<string> twos_zeros {"0", "0x", "0x00", "0x0000", "0x80", "0x8000"};

        for (const string &str : ones_zeros) {
            auto zb = Z_bytes_big::read (str);
            auto zl = Z_bytes_little::read (str);

            EXPECT_EQ (zb, 0);
            EXPECT_EQ (data::sign (zb), math::zero);

            EXPECT_EQ (zl, 0);
            EXPECT_EQ (data::sign (zl), math::zero);

            EXPECT_EQ (data::abs (zb), 0);
            EXPECT_EQ (data::abs (zl), 0);

            EXPECT_EQ (encoding::signed_decimal::write (zb), "0");
            EXPECT_EQ (encoding::signed_decimal::write (zl), "0");
        }

        for (const string &str : twos_zeros) {
            auto zb = Z_bytes_BC_big (str);
            auto zl = Z_bytes_BC_little (str);

            EXPECT_EQ (zb, 0) << "expected " << std::hex << zb << " to be zero.";
            EXPECT_EQ (data::sign (zb), math::zero);

            EXPECT_EQ (zl, 0);
            EXPECT_EQ (data::sign (zl), math::zero);

            EXPECT_EQ (data::abs (zb), 0);
            EXPECT_EQ (data::abs (zl), 0);

            auto write_zb = encoding::signed_decimal::write (zb);
            auto write_zl = encoding::signed_decimal::write (zl);
            EXPECT_EQ (write_zb, "0") << "expected " << std::hex << zb << " to write as 0 but got " << write_zb;
            EXPECT_EQ (write_zl, "0") << "expected " << std::hex << zl << " to write as 0 but got " << write_zl;
        }
    }
    
}

