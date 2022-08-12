// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include <data/math/number/bytes/N.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data::math::number {
    
    TEST(NBytesTest, TestStringToNBytes) {
        
        EXPECT_THROW(N_bytes<endian::big>::read("a"), exception);
        EXPECT_THROW(N_bytes<endian::big>::read("-"), exception);
        EXPECT_THROW(N_bytes<endian::big>::read("-1"), exception);
        EXPECT_THROW(N_bytes<endian::big>::read("01"), exception);
        EXPECT_THROW(N_bytes<endian::big>::read("0x1"), exception);
        
        EXPECT_THROW(N_bytes<endian::little>::read("a"), exception);
        EXPECT_THROW(N_bytes<endian::little>::read("-"), exception);
        EXPECT_THROW(N_bytes<endian::little>::read("-1"), exception);
        EXPECT_THROW(N_bytes<endian::little>::read("01"), exception);
        EXPECT_THROW(N_bytes<endian::little>::read("0x1"), exception);
        
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>{});
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>::read(""));
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>::read("0"));
        EXPECT_EQ(N_bytes<endian::big>{0}, N_bytes<endian::big>::read("0x"));
        
        EXPECT_FALSE(N_bytes<endian::big>{1} == N_bytes<endian::big>::read("0"));
        EXPECT_FALSE(N_bytes<endian::big>{0} == N_bytes<endian::big>::read("1"));
        
        EXPECT_TRUE(N_bytes<endian::big>{1} == N_bytes<endian::big>::read("1"));
        EXPECT_TRUE(N_bytes<endian::big>{1} == N_bytes<endian::big>::read("0x01"));
        EXPECT_TRUE(N_bytes<endian::big>{1} == N_bytes<endian::big>::read("0x0001"));
        
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>{});
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>::read(""));
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>::read("0"));
        EXPECT_EQ(N_bytes<endian::little>{0}, N_bytes<endian::little>::read("0x"));
        
        EXPECT_FALSE(N_bytes<endian::little>{1} == N_bytes<endian::little>::read("0"));
        EXPECT_FALSE(N_bytes<endian::little>{0} == N_bytes<endian::little>::read("1"));
        
        EXPECT_TRUE(N_bytes<endian::little>{1} == N_bytes<endian::little>::read("1"));
        EXPECT_TRUE(N_bytes<endian::little>{1} == N_bytes<endian::little>::read("0x01"));
        EXPECT_TRUE(N_bytes<endian::little>{1} == N_bytes<endian::little>::read("0x0001"));
        
        EXPECT_EQ(N_bytes<endian::big>::read("23"), N_bytes<endian::big>{23});
        EXPECT_EQ(N_bytes<endian::little>::read("23"), N_bytes<endian::little>{23});
        
    }
    
    TEST(NBytesTest, TestNBytesToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"0"}), std::string{"0x"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"128"}), std::string{"0x80"});
        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"0"}), std::string{"0x"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"128"}), std::string{"0x80"});
        
    }
    
    TEST(NBytesTest, TestZeroSize) {
        
        EXPECT_EQ(N_bytes<endian::big>{"0x"}.size(), 0);
        EXPECT_EQ(N_bytes<endian::big>{"0x00"}.size(), 1);
        EXPECT_EQ(N_bytes<endian::big>{"0x000000"}.size(), 3);
        
        EXPECT_EQ(N_bytes<endian::little>{"0x"}.size(), 0);
        EXPECT_EQ(N_bytes<endian::little>{"0x00"}.size(), 1);
        EXPECT_EQ(N_bytes<endian::little>{"0x000000"}.size(), 3);
        
    }
    
    TEST(NBytesTest, TestZeroAndNegativeNBytes) {
        
        EXPECT_EQ(N_bytes<endian::big>::read("0x"), N_bytes<endian::big>{0});
        EXPECT_EQ(N_bytes<endian::big>::read("0x00"), N_bytes<endian::big>{0});
        EXPECT_EQ(N_bytes<endian::big>::read("0x000000"), N_bytes<endian::big>{0});
        EXPECT_EQ(N_bytes<endian::little>::read("0x"), N_bytes<endian::little>{0});
        EXPECT_EQ(N_bytes<endian::little>::read("0x00"), N_bytes<endian::little>{0});
        EXPECT_EQ(N_bytes<endian::little>::read("0x000000"), N_bytes<endian::little>{0});
        
        EXPECT_EQ(N_bytes<endian::big>::read("0x01"), N_bytes<endian::big>{1});
        EXPECT_EQ(N_bytes<endian::big>::read("0x0001"), N_bytes<endian::big>{1});
        EXPECT_EQ(N_bytes<endian::big>::read("0x00000001"), N_bytes<endian::big>{1});
        EXPECT_EQ(N_bytes<endian::little>::read("0x01"), N_bytes<endian::little>{1});
        EXPECT_EQ(N_bytes<endian::little>::read("0x0001"), N_bytes<endian::little>{1});
        EXPECT_EQ(N_bytes<endian::little>::read("0x00000001"), N_bytes<endian::little>{1});
        
    }
    
    TEST(NBytesTest, TestNToNBytes) {
        
        EXPECT_EQ(N_bytes<endian::big>{N::read("1")}, N_bytes<endian::big>::read("1"));
        EXPECT_EQ(N_bytes<endian::little>{N::read("1")}, N_bytes<endian::little>::read("1"));
        EXPECT_EQ(N_bytes<endian::big>{N::read("23")}, N_bytes<endian::big>::read("23"));
        EXPECT_EQ(N_bytes<endian::little>{N::read("23")}, N_bytes<endian::little>::read("23"));
        EXPECT_EQ(N_bytes<endian::big>{N::read("5704566599993321")}, N_bytes<endian::big>::read("5704566599993321"));
        EXPECT_EQ(N_bytes<endian::little>{N::read("5704566599993321")}, N_bytes<endian::little>::read("5704566599993321"));
        
    }
    
    template <endian::order r> 
    struct test_bit_shift {
        test_bit_shift(string num, int shift) {
            EXPECT_EQ(N_bytes<r>(num) >> shift, N_bytes<r>(num) << -shift);
            EXPECT_EQ(N_bytes<r>(num) << shift, N_bytes<r>(num) >> -shift);
            EXPECT_EQ(N(num) >> shift, N(N_bytes<r>(num) >> shift));
            EXPECT_EQ(N(num) << shift, N(N_bytes<r>(num) << shift));
        }
    };
    
    TEST(NBytesTest, TestNBitShift) {
        for (const string &num : list<string>{"0", "1", "23", "5704566599993321"}) 
            for (const int &shift : list<int>{0, 1, 5, 8, 15, 16}) {
                test_bit_shift<endian::big>{num, shift};
                test_bit_shift<endian::little>{num, shift};
            };
        
    }
    
    TEST(NBytesTest, TestNBytesToString) {
        
        EXPECT_EQ(encoding::decimal::write(N_bytes<endian::big>{1}), std::string{"1"});
        EXPECT_EQ(encoding::decimal::write(N_bytes<endian::little>{1}), std::string{"1"});
        EXPECT_EQ(encoding::decimal::write(N_bytes<endian::big>{23}), std::string{"23"});
        EXPECT_EQ(encoding::decimal::write(N_bytes<endian::little>{23}), std::string{"23"});
        EXPECT_EQ(encoding::decimal::write(N_bytes<endian::big>{"5704566599993321"}), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::decimal::write(N_bytes<endian::little>{"5704566599993321"}), std::string{"5704566599993321"});
        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{1}.trim()), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{1}.trim()), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{23}.trim()), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{23}.trim()), std::string{"0x17"});        
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::big>{"5704566599993321"}), std::string{"0x144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(N_bytes<endian::little>{"5704566599993321"}), std::string{"0x144445e9ca47e9"});
        
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
    math::number::N_bytes<r> N_to_N_Bytes_stupid(const math::N& n) {
        return math::number::N_bytes<r>::read(encoding::hexidecimal::write<encoding::hex::lower>(n));
    }
    
    template <typename in> void N_Bytes_to_N(in x) {
        
        N n{x};
        
        N_bytes_big big{x};
        N_bytes_little little{x};
        
        N_bytes_big stupid_big = N_to_N_Bytes_stupid<endian::big>(n);
        N_bytes_little stupid_little = N_to_N_Bytes_stupid<endian::little>(n);
        
        EXPECT_EQ(stupid_big, big);
        EXPECT_EQ(stupid_little, little);
        
        N N_big = N(big);
        N N_little = N(little);
        
        N N_big_stupid = N_Bytes_to_N_stupid(big);
        N N_little_stupid = N_Bytes_to_N_stupid(little);
        
        EXPECT_EQ(N_big_stupid, N_big);
        EXPECT_EQ(N_little_stupid, N_little);
        
        EXPECT_EQ(N_big, n);
        EXPECT_EQ(N_little, n);
        
    }
    
    TEST(NBytesTest, TestNBytesToN) {
        
        N_Bytes_to_N<uint64>(0);
        N_Bytes_to_N<uint64>(1);
        N_Bytes_to_N<uint64>(3);
        N_Bytes_to_N<uint64>(767);
        N_Bytes_to_N<string>("0x0f00000a00aabbccddeeffffffffffffffff");
        N_Bytes_to_N<string>("0xf000000a00aabbccddeeffffffffffffffff");
        
    }
    
    TEST(NBytesTest, TestIncrement) {
        
        auto nl0 = N_bytes<endian::big>::read("0x");
        auto nl1 = N_bytes<endian::big>::read("0x00");
        auto nl3 = N_bytes<endian::big>::read("0x000000");
        auto nb0 = N_bytes<endian::little>::read("0x");
        auto nb1 = N_bytes<endian::little>::read("0x00");
        auto nb3 = N_bytes<endian::little>::read("0x000000");
        
        EXPECT_EQ(++nl0, N_bytes<endian::big>{1});
        EXPECT_EQ(++nl1, N_bytes<endian::big>{1});
        EXPECT_EQ(++nl3, N_bytes<endian::big>{1});
        EXPECT_EQ(++nb0, N_bytes<endian::little>{1});
        EXPECT_EQ(++nb1, N_bytes<endian::little>{1});
        EXPECT_EQ(++nb3, N_bytes<endian::little>{1});
        
    }
    
}

