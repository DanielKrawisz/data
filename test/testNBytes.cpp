// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/encoding/integer.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data::math::number {
    
    template <endian::order r, typename N = N_bytes<r>> void test_zero_and_one() {
        
        string z0("0x");
        string z1("0x00");
        string z3("0x000000");
        
        string o0("0x01");
        string o1("0x0001");
        string o3("0x00000001");
        
        N Nz0 = N::read(z0);
        N Nz1 = N::read(z1);
        N Nz3 = N::read(z3);
        
        N No0 = N::read(o0);
        N No1 = N::read(o1);
        N No3 = N::read(o3);
        
        EXPECT_EQ(No0.words()[0], 1);
        EXPECT_EQ(No1.words()[0], 1);
        EXPECT_EQ(No3.words()[0], 1);
        
        EXPECT_EQ(*No0.words().begin(), 1);
        EXPECT_EQ(*No1.words().begin(), 1);
        EXPECT_EQ(*No3.words().begin(), 1);
        
        EXPECT_EQ(No0[0], 1);
        EXPECT_EQ(No1[0], r == endian::big ? 0 : 1);
        EXPECT_EQ(No3[0], r == endian::big ? 0 : 1);
        
        EXPECT_EQ(No0[-1], 1);
        EXPECT_EQ(No1[-1], r == endian::little ? 0 : 1);
        EXPECT_EQ(No3[-1], r == endian::little ? 0 : 1);
        
        EXPECT_EQ(Nz0, N{0});
        EXPECT_EQ(Nz1, N{0});
        EXPECT_EQ(Nz3, N{0});
        
        EXPECT_EQ(No0, N{1});
        EXPECT_EQ(No1, N{1});
        EXPECT_EQ(No3, N{1});
        
        EXPECT_EQ(encoding::hexidecimal::write(Nz0), z0);
        EXPECT_EQ(encoding::hexidecimal::write(Nz1), z1);
        EXPECT_EQ(encoding::hexidecimal::write(Nz3), z3);
        
        EXPECT_EQ(encoding::hexidecimal::write(No0), o0);
        EXPECT_EQ(encoding::hexidecimal::write(No1), o1);
        EXPECT_EQ(encoding::hexidecimal::write(No3), o3);
        
        EXPECT_EQ(uint64(Nz0), 0);
        EXPECT_EQ(uint64(Nz1), 0);
        EXPECT_EQ(uint64(Nz3), 0);
        
        EXPECT_EQ(uint64(No0), 1);
        EXPECT_EQ(uint64(No1), 1);
        EXPECT_EQ(uint64(No3), 1);
        
        EXPECT_TRUE(is_minimal(N{1}));
        EXPECT_TRUE(is_minimal(N{0}));
        
    };
    
    TEST(NBytesTest, TestZeroAndOneNBytes) {
        
        test_zero_and_one<endian::little>();
        test_zero_and_one<endian::big>();
        
    }
    
    template <typename N_bytes> void test_N_bytes_to_N() {
        
        EXPECT_EQ(N{N_bytes{}}, N{0});
        
        EXPECT_EQ(N_bytes{N{0}}, N_bytes{0});
        
        EXPECT_EQ(N{N_bytes{1}}, N{1});
        
        EXPECT_EQ(N_bytes{N::read("1")}, N_bytes::read("1"));
        
        EXPECT_EQ(N{N_bytes::read("1")}, N{1});
        
        EXPECT_EQ(N{N_bytes{23}}, N{23});
        
        EXPECT_EQ(N{N_bytes::read("23")}, N{23});
        
        EXPECT_EQ(N_bytes{N::read("23")}, N_bytes::read("23"));
        
        EXPECT_EQ(N{N_bytes::read("5704566599993321")}, N::read("5704566599993321"));
        
        EXPECT_EQ(N_bytes{N::read("5704566599993321")}, N_bytes::read("5704566599993321"));
        
        string big = "2487308784708403643874034353454520837504827042804354352454";
        
        EXPECT_EQ(N{N_bytes::read(big)}, N::read(big));
        
        EXPECT_EQ(N_bytes{N::read(big)}, N_bytes::read(big));
        
        std::string p_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";        
        std::string n_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
        
        auto p_bytes = N_bytes::read(p_hex);
        auto n_bytes = N_bytes::read(n_hex);
        
        N p_from_bytes = N(p_bytes);
        N n_from_bytes = N(n_bytes);
        
        EXPECT_EQ(p_bytes, N_bytes(p_from_bytes));
        EXPECT_EQ(n_bytes, N_bytes(n_from_bytes));
    };
    
    TEST(NBytesTest, TestNBytesToN) {
        test_N_bytes_to_N<N_bytes_little>();
        test_N_bytes_to_N<N_bytes_big>();
    }
    
    template <typename N_bytes> void test_string_to_N_bytes() {
        
        EXPECT_EQ(N_bytes{0}, N_bytes{});
        EXPECT_EQ(N_bytes{0}, N_bytes::read("0"));
        EXPECT_EQ(N_bytes{0}, N_bytes::read("0x"));
        
        EXPECT_FALSE(N_bytes{1} == N_bytes::read("0"));
        EXPECT_FALSE(N_bytes{0} == N_bytes::read("1"));
        
        EXPECT_TRUE(N_bytes{1} == N_bytes::read("1"));
        EXPECT_TRUE(N_bytes{1} == N_bytes::read("0x01"));
        EXPECT_TRUE(N_bytes{1} == N_bytes::read("0x0001"));
        
        EXPECT_EQ(N_bytes::read("23"), N_bytes{23});
    };
    
    TEST(NBytesTest, TestStringToNBytes) {
        
        test_string_to_N_bytes<N_bytes_little>();
        test_string_to_N_bytes<N_bytes_big>();
        
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
        return math::number::N_bytes<r>::read(encoding::hexidecimal::write(n));
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

