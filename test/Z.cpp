// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>

#include <gtest/gtest.h>

namespace data {
    
    TEST (Z, ZToHexString) {
        
        string Zh_0 = encoding::hexidecimal::write<hex_case::lower> (Z::read ("0"));
        EXPECT_EQ (Zh_0, std::string {"0x"});
        
        string Zh_ff = encoding::hexidecimal::write<hex_case::lower> (Z::read ("-1"));
        EXPECT_EQ (Zh_ff, std::string {"0xff"});
        
        string Zh_n2 = encoding::hexidecimal::write<hex_case::lower> (Z::read ("-2"));
        EXPECT_EQ (Zh_n2, std::string {"0xfe"});
        
        string Zh_127 = encoding::hexidecimal::write<hex_case::lower> (Z::read ("127"));
        EXPECT_EQ (Zh_127, std::string {"0x7f"});
        
        string Zh_128 = encoding::hexidecimal::write<hex_case::lower> (Z::read ("128"));
        EXPECT_EQ (Zh_128, std::string {"0x0080"});
        
        string Zh_80 = encoding::hexidecimal::write<hex_case::lower> (Z::read ("-128"));
        EXPECT_EQ (Zh_80, std::string {"0x80"});
        
        string Zh_ff7f = encoding::hexidecimal::write<hex_case::lower> (Z::read ("-129"));
        EXPECT_EQ (Zh_ff7f, std::string {"0xff7f"});
        
        string Zh_qqqq = encoding::hexidecimal::write<hex_case::lower> (Z::read ("0x7f33") << 128);
        EXPECT_EQ (Zh_qqqq, std::string {"0x7f3300000000000000000000000000000000"});
        
    }
    
    TEST (Z, StringToZ) {
        
        EXPECT_THROW (Z::read (""), exception);
        EXPECT_THROW (Z::read ("-0x01"), exception);
        
        EXPECT_NO_THROW (Z::read ("-1"));
        
        EXPECT_EQ (Z {}, Z::read ("0"));
        EXPECT_EQ (Z {-1}, Z::read ("-1"));
        EXPECT_EQ (Z::read ("-1"), Z::read ("0xff"));
        
        EXPECT_EQ (encoding::signed_decimal::write (Z {0}), "0");
        EXPECT_EQ (encoding::signed_decimal::write (Z {1}), "1");
        EXPECT_EQ (encoding::signed_decimal::write (Z {-1}), "-1");
        
        string Zh_0 = encoding::hexidecimal::write<hex_case::lower> (Z {0});
        EXPECT_EQ (Zh_0, "0x");
        
        string Zh_1 = encoding::hexidecimal::write<hex_case::lower> (Z {1});
        EXPECT_EQ (Zh_1, "0x01");
        
        string Zh_n1 = encoding::hexidecimal::write<hex_case::lower> (Z {-1});
        EXPECT_EQ (Zh_n1, "0xff");
        
        EXPECT_TRUE (Z::read ("0x80000000000000000000") < Z::read ("0x7fffffffffffffffffff"));
        EXPECT_TRUE (Z::read ("0xff") < Z::read ("0x00ff"));
        
    }

    template<endian r>
    N N_Bytes_to_N_stupid (const math::number::N_bytes<r, byte> &n) {
        N x {0};
        for (const byte &b : n.words ().reverse ()) {
            x <<= 8;
            x += b;
        }
        return x;
    }

    template<endian r, negativity zz>
    Z Z_Bytes_to_Z_stupid (const math::number::Z_bytes<r, zz, byte> &z) {
        if (is_negative (z)) return -N_Bytes_to_N_stupid (data::abs (z));
        return N_Bytes_to_N_stupid (abs (z));
    }

    template<endian r, negativity zz>
    math::number::Z_bytes<r, zz, byte> Z_to_Z_Bytes_stupid (const Z &n) {
        return math::number::Z_bytes<r, zz, byte>::read
            (encoding::hexidecimal::write<hex_case::lower> (n));
    }

    template <typename in> void Z_Bytes_to_Z (in x) {

        Z z = Z::read (x);

        Z_bytes_big big {x};
        Z_bytes_little little {x};

        Z_bytes_big stupid_big = Z_to_Z_Bytes_stupid<endian::big, negativity::twos> (z);
        Z_bytes_little stupid_little = Z_to_Z_Bytes_stupid<endian::little, negativity::twos> (z);

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

    TEST (Z, ZBytesToZ) {

        Z_Bytes_to_Z ("0");
        Z_Bytes_to_Z ("-1");
        Z_Bytes_to_Z ("1");
        Z_Bytes_to_Z ("3");
        Z_Bytes_to_Z ("-3");
        Z_Bytes_to_Z ("229");
        Z_Bytes_to_Z ("767");
        Z_Bytes_to_Z ("916");
        Z_Bytes_to_Z ("1145");
        Z_Bytes_to_Z ("-1145");
        Z_Bytes_to_Z ("0x0f00000a00aabbccddeeffffffffffffffff");
        Z_Bytes_to_Z ("0xf000000a00aabbccddeeffffffffffffffff");

    }

    TEST (Z, ZToZBytes) {

        EXPECT_EQ (Z_bytes_big {Z::read ("1")}, Z_bytes_big {1});
        EXPECT_EQ (Z_bytes_little {Z::read ("1")}, Z_bytes_little {1});
        EXPECT_EQ (Z_bytes_big {Z::read ("23")}, Z_bytes_big {23});
        EXPECT_EQ (Z_bytes_little {Z::read ("23")}, Z_bytes_little {23});
        EXPECT_EQ (Z_bytes_big {Z::read ("5704566599993321")}, Z_bytes_big {5704566599993321});
        EXPECT_EQ (Z_bytes_little {Z::read ("5704566599993321")}, Z_bytes_little {5704566599993321});
        EXPECT_EQ (Z_bytes_big {Z::read ("-1")}, Z_bytes_big {-1});
        EXPECT_EQ (Z_bytes_little {Z::read ("-1")}, Z_bytes_little {-1});
        EXPECT_EQ (Z_bytes_big {Z::read ("-3393939987200333")}, Z_bytes_big {-3393939987200333});
        EXPECT_EQ (Z_bytes_little {Z::read ("-3393939987200333")}, Z_bytes_little {-3393939987200333});

        EXPECT_EQ (Z_bytes_BC_big {Z::read ("1")}, Z_bytes_big {1});
        EXPECT_EQ (Z_bytes_BC_little {Z::read ("1")}, Z_bytes_little {1});
        EXPECT_EQ (Z_bytes_BC_big {Z::read ("23")}, Z_bytes_big {23});
        EXPECT_EQ (Z_bytes_BC_little {Z::read ("23")}, Z_bytes_little {23});
        EXPECT_EQ (Z_bytes_BC_big {Z::read ("5704566599993321")}, Z_bytes_big::read ("5704566599993321"));
        EXPECT_EQ (Z_bytes_BC_little {Z::read ("5704566599993321")}, Z_bytes_little::read ("5704566599993321"));
        EXPECT_EQ (Z_bytes_BC_big {Z::read ("-1")}, Z_bytes_big::read ("-1"));
        EXPECT_EQ (Z_bytes_BC_little {Z::read ("-1")}, Z_bytes_little::read ("-1"));
        EXPECT_EQ (Z_bytes_BC_big {Z::read ("-3393939987200333")}, Z_bytes_big::read ("-3393939987200333"));
        EXPECT_EQ (Z_bytes_BC_little {Z::read ("-3393939987200333")}, Z_bytes_little::read ("-3393939987200333"));

        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("-3393939987200333")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("-3393939987200333")));

        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("-3393939987200333")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("-3393939987200333")));

        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16> {1}));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::twos, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::big, negativity::twos, uint16>::read ("-3393939987200333")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::twos, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::little, negativity::twos, uint16>::read ("-3393939987200333")));

        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16> {1}));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("1")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("23")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("23")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("5704566599993321")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("5704566599993321")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("-1")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("-1")));
        EXPECT_EQ ((math::number::Z_bytes<endian::big, negativity::BC, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::big, negativity::BC, uint16>::read ("-3393939987200333")));
        EXPECT_EQ ((math::number::Z_bytes<endian::little, negativity::BC, uint16> {Z::read ("-3393939987200333")}),
                   (math::number::Z_bytes<endian::little, negativity::BC, uint16>::read ("-3393939987200333")));

    }
    
}
