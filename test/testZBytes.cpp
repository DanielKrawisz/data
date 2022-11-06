// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"

namespace data {
    
    TEST(ZBytesTest, TestZBytesToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("0")), std::string("0x"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("-1")), std::string("0xff"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("-2")), std::string("0xfe"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("127")), std::string("0x7f"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("128")), std::string("0x0080"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("-128")), std::string("0x80"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("-129")), std::string("0xff7f"));
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("0")), std::string("0x"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("-1")), std::string("0xff"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("-2")), std::string("0xfe"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("127")), std::string("0x7f"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("128")), std::string("0x0080"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("-128")), std::string("0x80"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("-129")), std::string("0xff7f"));
        
    }
    
    TEST(ZBytesTest, TestStringToZBytes) {
        
        EXPECT_NO_THROW(Z_bytes_big{});
        EXPECT_NO_THROW(Z_bytes_big{N{}});
        EXPECT_NO_THROW(Z_bytes_big{Z{}});
        EXPECT_NO_THROW(Z_bytes_big{""});
        EXPECT_NO_THROW(Z_bytes_big{"-1"});
        EXPECT_THROW(Z_bytes_big{"-0x01"}, exception);
        EXPECT_NO_THROW(Z_bytes_big{"0x00"});
        EXPECT_NO_THROW(Z_bytes_big{"0xff"});
        
        EXPECT_NO_THROW(Z_bytes_little{});
        EXPECT_NO_THROW(Z_bytes_little{N{}});
        EXPECT_NO_THROW(Z_bytes_little{Z{}});
        EXPECT_NO_THROW(Z_bytes_little{""});
        EXPECT_NO_THROW(Z_bytes_little{"-1"});
        EXPECT_THROW(Z_bytes_little{"-0x01"}, exception);
        EXPECT_NO_THROW(Z_bytes_big{"0x00"});
        EXPECT_NO_THROW(Z_bytes_big{"0xff"});
        
    }
    
    TEST(ZBytesTest, TestZeroSize) {
        
        EXPECT_EQ(Z_bytes_big::read("0x").size(), 0);
        EXPECT_EQ(Z_bytes_big::read("0x00").size(), 1);
        EXPECT_EQ(Z_bytes_big::read("0x000000").size(), 3);
        
        EXPECT_EQ(Z_bytes_little::read("0x").size(), 0);
        EXPECT_EQ(Z_bytes_little::read("0x00").size(), 1);
        EXPECT_EQ(Z_bytes_little::read("0x000000").size(), 3);
        
    }
    
    TEST(ZBytesTest, TestZeroAndNegativeZBytes) {
        
        EXPECT_EQ(Z_bytes_big::read("0x"), Z_bytes_big{0});
        EXPECT_EQ(Z_bytes_big::read("0x00"), Z_bytes_big{0});
        EXPECT_EQ(Z_bytes_big::read("0x000000"), Z_bytes_big{0});
        EXPECT_EQ(Z_bytes_little::read("0x"), Z_bytes_little{0});
        EXPECT_EQ(Z_bytes_little::read("0x00"), Z_bytes_little{0});
        EXPECT_EQ(Z_bytes_little::read("0x000000"), Z_bytes_little{0});
        
        EXPECT_EQ(Z_bytes_big::read("0x01"), Z_bytes_big{1});
        EXPECT_EQ(Z_bytes_big::read("0x0001"), Z_bytes_big{1});
        EXPECT_EQ(Z_bytes_big::read("0x00000001"), Z_bytes_big{1});
        EXPECT_EQ(Z_bytes_little::read("0x01"), Z_bytes_little{1});
        EXPECT_EQ(Z_bytes_little::read("0x0001"), Z_bytes_little{1});
        EXPECT_EQ(Z_bytes_little::read("0x00000001"), Z_bytes_little{1});
        
        EXPECT_EQ(Z_bytes_big::read("0xff"), Z_bytes_big{-1});
        EXPECT_EQ(Z_bytes_big::read("0xffff"), Z_bytes_big{-1});
        EXPECT_EQ(Z_bytes_big::read("0xffffff"), Z_bytes_big{-1});
        EXPECT_EQ(Z_bytes_little::read("0xff"), Z_bytes_little{-1});
        EXPECT_EQ(Z_bytes_little::read("0xffff"), Z_bytes_little{-1});
        EXPECT_EQ(Z_bytes_little::read("0xffffff"), Z_bytes_little{-1});
        
    }
    
    TEST(ZBytesTest, TestZToZBytes) {
        
        EXPECT_EQ(Z_bytes_big{Z::read("1")}, Z_bytes_big::read("1"));
        EXPECT_EQ(Z_bytes_little{Z::read("1")}, Z_bytes_little::read("1"));
        EXPECT_EQ(Z_bytes_big{Z::read("23")}, Z_bytes_big::read("23"));
        EXPECT_EQ(Z_bytes_little{Z::read("23")}, Z_bytes_little::read("23"));
        EXPECT_EQ(Z_bytes_big{Z::read("5704566599993321")}, Z_bytes_big::read("5704566599993321"));
        EXPECT_EQ(Z_bytes_little{Z::read("5704566599993321")}, Z_bytes_little::read("5704566599993321"));
        EXPECT_EQ(Z_bytes_big{Z::read("-1")}, Z_bytes_big::read("-1"));
        EXPECT_EQ(Z_bytes_little{Z::read("-1")}, Z_bytes_little::read("-1"));
        EXPECT_EQ(Z_bytes_big{Z::read("-3393939987200333")}, Z_bytes_big::read("-3393939987200333"));
        EXPECT_EQ(Z_bytes_little{Z::read("-3393939987200333")}, Z_bytes_little::read("-3393939987200333"));
        
    }
    
    TEST(ZBytesTest, TestZBytesIncrement) {
        
        auto zb0 = Z_bytes_big::read("0x");
        auto zb1 = Z_bytes_big::read("0x00");
        auto zb3 = Z_bytes_big::read("0x000000");
        
        auto zl0 = Z_bytes_little::read("0x");
        auto zl1 = Z_bytes_little::read("0x00");
        auto zl3 = Z_bytes_little::read("0x000000");
        
        EXPECT_EQ(++zb0, Z_bytes_big{1});
        EXPECT_EQ(++zb1, Z_bytes_big{1});
        EXPECT_EQ(++zb3, Z_bytes_big{1});
        
        EXPECT_EQ(++zl0, Z_bytes_little{1});
        EXPECT_EQ(++zl1, Z_bytes_little{1});
        EXPECT_EQ(++zl3, Z_bytes_little{1});
        
    }
    
    TEST(ZBytesTest, TestZBytesDecrement) {
        
        auto zb0 = Z_bytes_big::read("0x");
        auto zb1 = Z_bytes_big::read("0x00");
        auto zb3 = Z_bytes_big::read("0x000000");
        
        auto zl0 = Z_bytes_little::read("0x");
        auto zl1 = Z_bytes_little::read("0x00");
        auto zl3 = Z_bytes_little::read("0x000000");
        
        EXPECT_EQ(--zb0, Z_bytes_big{-1});
        EXPECT_EQ(--zb1, Z_bytes_big{-1});
        EXPECT_EQ(--zb3, Z_bytes_big{-1});
        
        EXPECT_EQ(--zl0, Z_bytes_little{-1});
        EXPECT_EQ(--zl1, Z_bytes_little{-1});
        EXPECT_EQ(--zl3, Z_bytes_little{-1});
        
    }
    
    TEST(ZBytesTest, TestZBytesToString) {
        
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_big::read("1")),std::string{"1"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_little::read("1")), std::string{"1"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_big::read("23")), std::string{"23"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_little::read("23")), std::string{"23"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_big::read("5704566599993321")), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_little::read("5704566599993321")), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_big::read("-1")), std::string{"-1"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_little::read("-1")), std::string{"-1"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_big::read("-3393939987200333")), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_little::read("-3393939987200333")), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_big::read("0xf3f13ac4ac86b3")), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::signed_decimal::write(Z_bytes_little::read("0xf3f13ac4ac86b3")), std::string{"-3393939987200333"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("1")), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("1")), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("23")), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("23")), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("5704566599993321")), std::string{"0x144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("5704566599993321")), std::string{"0x144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("-1")), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("-1")), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_big::read("-3393939987200333")), std::string{"0xf3f13ac4ac86b3"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes_little::read("-3393939987200333")), std::string{"0xf3f13ac4ac86b3"});
        
    }
    
    template<endian::order r>
    N N_Bytes_to_N_stupid(const math::number::N_bytes<r>& n) {
        N x{0};
        for (const byte &b : n.words().reverse()) {
            x <<= 8;
            x += b;
        }
        return x;
    }
    
    template<endian::order r>
    Z Z_Bytes_to_Z_stupid(const math::number::Z_bytes<r>& z) {
        if (math::number::is_negative(z)) return -N_Bytes_to_N_stupid(data::abs(z));
        return N_Bytes_to_N_stupid(data::abs(z));
    }
    
    template<endian::order r>
    math::number::Z_bytes<r> Z_to_Z_Bytes_stupid(const math::Z& n) {
        return math::number::Z_bytes<r>::read(encoding::hexidecimal::write(n));
    }
    
    template <typename in> void Z_Bytes_to_Z(in x) {
        
        Z z{x};
        
        Z_bytes_big big{x};
        Z_bytes_little little{x};
        
        Z_bytes_big stupid_big = Z_to_Z_Bytes_stupid<endian::big>(z);
        Z_bytes_little stupid_little = Z_to_Z_Bytes_stupid<endian::little>(z);
        
        EXPECT_EQ(stupid_big, big);
        EXPECT_EQ(stupid_little, little);
        
        Z Z_big = Z(big);
        Z Z_little = Z(little);
        
        Z Z_big_stupid = Z_Bytes_to_Z_stupid(big);
        Z Z_little_stupid = Z_Bytes_to_Z_stupid(little);
        
        EXPECT_EQ(Z_big_stupid, Z_big);
        EXPECT_EQ(Z_little_stupid, Z_little);
        
        EXPECT_EQ(Z_big, z);
        EXPECT_EQ(Z_little, z);
        
    }
    
    TEST(ZBytesTest, TestZBytesToZ) {
        
        Z_Bytes_to_Z(0);
        Z_Bytes_to_Z(1);
        Z_Bytes_to_Z(3);
        Z_Bytes_to_Z(767);
        Z_Bytes_to_Z("0x0f00000a00aabbccddeeffffffffffffffff");
        Z_Bytes_to_Z("0xf000000a00aabbccddeeffffffffffffffff");
        
    }
    
}

