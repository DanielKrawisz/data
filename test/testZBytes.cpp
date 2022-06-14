// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/encoding/integer.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include "gtest/gtest.h"

namespace data::math {
    
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
        EXPECT_THROW(Z_bytes_big{"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes_big{"0x00"});
        EXPECT_NO_THROW(Z_bytes_big{"0xff"});
        
        EXPECT_NO_THROW(Z_bytes_little{});
        EXPECT_NO_THROW(Z_bytes_little{N{}});
        EXPECT_NO_THROW(Z_bytes_little{Z{}});
        EXPECT_NO_THROW(Z_bytes_little{""});
        EXPECT_NO_THROW(Z_bytes_little{"-1"});
        EXPECT_THROW(Z_bytes_little{"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes_big{"0x00"});
        EXPECT_NO_THROW(Z_bytes_big{"0xff"});
    }
    
    template <endian::order r> using Z_bytes_twos = number::Z_bytes<r, number::twos>;
    
    template <typename Z> void test_zero_and_one() {
        
        string z0("0x");
        string z1("0x00");
        string z3("0x000000");
        
        string o0("0x01");
        string o1("0x0001");
        string o3("0x00000001");
        
        Z Zz0 = Z::read(z0);
        Z Zz1 = Z::read(z1);
        Z Zz3 = Z::read(z3);
        
        Z Zo0 = Z::read(o0);
        Z Zo1 = Z::read(o1);
        Z Zo3 = Z::read(o3);
        
        EXPECT_EQ(Zz0, Z{0});
        EXPECT_EQ(Zz1, Z{0});
        EXPECT_EQ(Zz3, Z{0});
        
        EXPECT_EQ(Zo0, Z{1});
        EXPECT_EQ(Zo1, Z{1});
        EXPECT_EQ(Zo3, Z{1});
        
        EXPECT_EQ(int64(Zz0), 0);
        EXPECT_EQ(int64(Zz1), 0);
        EXPECT_EQ(int64(Zz3), 0);
        
        EXPECT_EQ(int64(Zo0), 1);
        EXPECT_EQ(int64(Zo1), 1);
        EXPECT_EQ(int64(Zo3), 1);
        
        EXPECT_EQ(encoding::hexidecimal::write(Zz0), z0);
        EXPECT_EQ(encoding::hexidecimal::write(Zz1), z1);
        EXPECT_EQ(encoding::hexidecimal::write(Zz3), z3);
        
        EXPECT_EQ(encoding::hexidecimal::write(Zo0), o0);
        EXPECT_EQ(encoding::hexidecimal::write(Zo1), o1);
        EXPECT_EQ(encoding::hexidecimal::write(Zo3), o3);
        
        EXPECT_TRUE(number::is_minimal(Z{1}));
        EXPECT_TRUE(number::is_minimal(Z{0}));
        EXPECT_TRUE(number::is_minimal(Z{-1}));
    
    }
    
    template <typename Z> void test_negative_one_ones() {
        
        string n1("0xff");
        string n2("0xffff");
        string n3("0xffffff");
        
        Z Zn1 = Z::read(n1);
        Z Zn2 = Z::read(n2);
        Z Zn3 = Z::read(n3);
        
        EXPECT_EQ(Zn1, Z{-1});
        EXPECT_EQ(Zn2, Z{-1});
        EXPECT_EQ(Zn3, Z{-1});
        
        EXPECT_EQ(int64(Zn1), -1);
        EXPECT_EQ(int64(Zn2), -1);
        EXPECT_EQ(int64(Zn3), -1);
        
        EXPECT_EQ(encoding::hexidecimal::write(Zn1), n1);
        EXPECT_EQ(encoding::hexidecimal::write(Zn2), n2);
        EXPECT_EQ(encoding::hexidecimal::write(Zn3), n3);
        
    }
    
    template <typename Z> void test_negative_one_twos() {
        
        string n1("0x80");
        string n2("0x8001");
        string n3("0x800001");
        
        Z Zn1 = Z::read(n1);
        Z Zn2 = Z::read(n2);
        Z Zn3 = Z::read(n3);
        
        EXPECT_EQ(Zn1, Z{-1});
        EXPECT_EQ(Zn2, Z{-1});
        EXPECT_EQ(Zn3, Z{-1});
        
        EXPECT_EQ(int64(Zn1), -1);
        EXPECT_EQ(int64(Zn2), -1);
        EXPECT_EQ(int64(Zn3), -1);
    }
    
    TEST(ZBytesTest, TestZeroAndNegativeZBytes) {
        test_zero_and_one<Z_bytes<endian::big>>();
        test_zero_and_one<Z_bytes<endian::little>>();
        test_zero_and_one<Z_bytes_twos<endian::big>>();
        test_zero_and_one<Z_bytes_twos<endian::little>>();
        
        test_negative_one_ones<Z_bytes<endian::big>>();
        test_negative_one_ones<Z_bytes<endian::little>>();
        test_negative_one_twos<Z_bytes_twos<endian::big>>();
        test_negative_one_twos<Z_bytes_twos<endian::little>>();
        
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
    
    template <typename Zb> void test_Z_to_Z_bytes() {
        
        EXPECT_EQ(Zb{Z::read("1")}, Zb::read("1"));
        EXPECT_EQ(Zb{Z::read("23")}, Zb::read("23"));
        EXPECT_EQ(Zb{Z::read("5704566599993321")}, Zb::read("5704566599993321"));
        EXPECT_EQ(Zb{Z::read("-1")}, Zb::read("-1"));
        EXPECT_EQ(Zb{Z::read("-3393939987200333")}, Zb::read("-3393939987200333"));

    }
    
    // Problem: string reversal not happening correctly for some reason!!
    TEST(ZBytesTest, TestZToZBytes) {
        
        test_Z_to_Z_bytes<Z_bytes<endian::big>>();
        test_Z_to_Z_bytes<Z_bytes<endian::little>>();
        test_Z_to_Z_bytes<Z_bytes_twos<endian::big>>();
        test_Z_to_Z_bytes<Z_bytes_twos<endian::little>>();
        
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
        
    template <typename Zb> void test_Z_bytes_to_Z() {
        EXPECT_EQ(Z{Zb{}}, Z{0});
        EXPECT_EQ(Z{Zb::read("0")}, Z{0});
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

