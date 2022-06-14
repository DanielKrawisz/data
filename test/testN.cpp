// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/encoding/integer.hpp>
#include "gtest/gtest.h"

namespace data {
    
    template <typename N> void test_string_to_N() {
        
        EXPECT_THROW(N::read(""), std::invalid_argument);
        EXPECT_THROW(N::read("a"), std::invalid_argument);
        EXPECT_THROW(N::read("-"), std::invalid_argument);
        EXPECT_THROW(N::read("-1"), std::invalid_argument);
        EXPECT_THROW(N::read("01"), std::invalid_argument);
        EXPECT_THROW(N::read("0x1"), std::invalid_argument);
        
        EXPECT_EQ(N{}, N::read("0"));
        
        EXPECT_EQ(N{0}, N::read("0"));
        EXPECT_EQ(N{0}, N::read("0x"));
        
        EXPECT_NE(N{1}, N::read("0"));
        EXPECT_NE(N{0}, N::read("1"));
        
        EXPECT_EQ(N{1}, N::read("1"));
        EXPECT_EQ(N{1}, N::read("0x01"));
        EXPECT_EQ(N{1}, N::read("0x0001"));
        EXPECT_EQ(N{255}, N::read("0xff"));
        
        EXPECT_EQ(encoding::hexidecimal::write(N{0}), "0x");
        EXPECT_EQ(encoding::decimal::write(N{0}), "0");
        EXPECT_EQ(encoding::hexidecimal::write(N{1}), "0x01");
        EXPECT_EQ(encoding::decimal::write(N{1}), "1");

    }
    
    TEST(NTest, TestStringToN) {
        test_string_to_N<N>();
        test_string_to_N<N_bytes_big>();
        test_string_to_N<N_bytes_little>();
        
    }
    
    template <typename N> void test_N_to_hex_string() {
        EXPECT_EQ(encoding::hexidecimal::write(N::read("0")), std::string{"0x"});
        EXPECT_EQ(encoding::hexidecimal::write(N::read("127")), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N::read("128")), std::string{"0x80"});
    }
    
    TEST(NTest, TestNToHexString) {
        
        test_N_to_hex_string<N>();
        test_N_to_hex_string<N_bytes_big>();
        test_N_to_hex_string<N_bytes_little>();
        
    }
    
    template <typename N> void test_N_negative_hex() {
        EXPECT_NE(N::read("0x8000000000000000000000000000000000000000000000000000000000000000"), N(0));
        EXPECT_NE(N::read("0x80000000000000000000000000000000000000000000000000000000000000"), N(0));
        EXPECT_NE(N::read("0x80000000000000000000000000000000000000000000000000000000"), N(0));
        EXPECT_NE(N::read("0x8000000000000000000000000000000000000000000000"), N(0));
        EXPECT_NE(N::read("0x8000000000000000000000000000"), N(0));
        EXPECT_NE(N::read("0x8000000000"), N(0));
        EXPECT_NE(N::read("0x8000"), N(0));
        EXPECT_NE(N::read("0x80"), N(0));
    }
    
    TEST(NTest, TestNNegativeHex) {
        
        test_N_negative_hex<N>();
        test_N_negative_hex<N_bytes_big>();
        test_N_negative_hex<N_bytes_little>();
        
    }
    
    template <typename N> void test_multiply() {
        EXPECT_TRUE(N{0} * N{0} == N{0});
        EXPECT_TRUE(N{1} * N{0} == N{0});
        EXPECT_TRUE(N{0} * N{1} == N{0});
        EXPECT_TRUE(N{1} * N{1} == N{1});
        
        N n{0};
        
        EXPECT_TRUE(n == N{0});
        
        n++;
        
        EXPECT_TRUE(n == N{1});
        
        n += 1;
        
        EXPECT_TRUE(n == N{2});
    }
    
    TEST(NTest, TestMultiply) {
        test_multiply<N>();
        test_multiply<N_bytes_big>();
        test_multiply<N_bytes_little>();
        test_multiply<dec_uint>();
        test_multiply<base58_uint>();
    }
    
    template <typename N> void test_shift() {
        
        EXPECT_TRUE(N{1} >>  1 == N::read("0x00000000000000000000"));
        EXPECT_TRUE(N{1} <<  1 == N::read("0x00000000000000000002"));
        EXPECT_TRUE(N{1} <<  3 == N::read("0x00000000000000000008"));
        EXPECT_TRUE(N{1} <<  7 == N::read("0x00000000000000000080"));
        EXPECT_TRUE(N{1} << 79 == N::read("0x80000000000000000000"));
    }
    
    TEST(NTest, TestShift) {
        
        test_shift<N>();
        test_shift<N_bytes_big>();
        test_shift<N_bytes_little>();
        
    }
    
    template <typename N> void test_secp256k1_primes() {
        
        // parameters from https://en.bitcoin.it/wiki/Secp256k1
        
        N p = (N{1} << 256) - (N{1} << 32) - (N{1} << 9) - (N{1} << 8) - (N{1} << 7) - (N{1} << 6) - (N{1} << 4) - 1;
        
        std::string p_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
        std::string p_dec = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
        
        std::string n_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
        std::string n_dec = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
        
        N n = N::read(n_hex);
        
        EXPECT_TRUE(p == p);
        EXPECT_FALSE(p == n);
        
        EXPECT_TRUE(n == n);
        EXPECT_FALSE(n == p);
        
        EXPECT_TRUE(p == N::read(p_hex));
        EXPECT_FALSE(p == N::read(n_hex));
        EXPECT_FALSE(n == N::read(p_hex));
        
        EXPECT_TRUE(p == N::read(p_dec));
        EXPECT_TRUE(n == N::read(n_dec));
        
        EXPECT_FALSE(p == N::read(n_dec));
        EXPECT_FALSE(n == N::read(p_dec));
        
    }
    
    TEST(NTest, TestN) {
        test_secp256k1_primes<N>();
        test_secp256k1_primes<N_bytes_big>();
        test_secp256k1_primes<N_bytes_little>();
    }
    
    template <typename N> void test_remainder() {
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") % 1, N(0));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") % 10, N(5));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") % 16, N(15));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") % 58, N(29));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 1,
            N::read("26959946667150639794667015087019630673637144422540572481103610249215"));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 10, 
            N::read("2695994666715063979466701508701963067363714442254057248110361024921"));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 16, 
            N::read("1684996666696914987166688442938726917102321526408785780068975640575"));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 58, 
            N::read("464826666675011030942534742879648804717881800388630560019027762917"));
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N::read("26959946667150639794667015087019630673637144422540572481103610249215"), N{1});
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N::read("2695994666715063979466701508701963067363714442254057248110361024921"), N{10});
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N::read("1684996666696914987166688442938726917102321526408785780068975640575"), N{16});
        
        EXPECT_EQ(N::read("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N::read("464826666675011030942534742879648804717881800388630560019027762917"), N{58});
    }
    
    TEST(NTest, TestNRemainder) {
        
        test_remainder<N>();
        test_remainder<N_bytes_big>();
        test_remainder<N_bytes_little>();
    }
    
    TEST(NTest, TestAKS) {
        math::number::AKS<N> aks{};
        
        EXPECT_FALSE(aks.is_prime(N::read("0")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("1")).valid());
        EXPECT_TRUE(aks.is_prime(N::read("2")).valid());
        EXPECT_TRUE(aks.is_prime(N::read("3")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("4")).valid());
        EXPECT_TRUE(aks.is_prime(N::read("5")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("6")).valid());
        EXPECT_TRUE(aks.is_prime(N::read("7")).valid());
        
        EXPECT_TRUE(aks.is_prime(N::read("23")).valid());
        EXPECT_TRUE(aks.is_prime(N::read("101")).valid());
        
        EXPECT_FALSE(aks.is_prime(N::read("519")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("3439")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("988320843")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("2904873984723454089")).valid());
        EXPECT_FALSE(aks.is_prime(N::read("4095842309824958234058934985234958304985083")).valid());
        
        // These tests are commented out because they run too slow for practical use. 
        //EXPECT_TRUE(aks.is_prime(N::read("523")).valid());
        
        //EXPECT_TRUE(aks.is_prime(N::read("3449")).valid());
        
        //EXPECT_TRUE(aks.is_prime(N::read("988320847")).valid());
        
        //EXPECT_TRUE(aks.is_prime(N::read("2904873984723454103")).valid());
        
        //EXPECT_TRUE(aks.is_prime(N::read("4095842309824958234058934985234958304985117")).valid());
        
        //EXPECT_TRUE(aks.is_prime(N::read("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F")).valid());
        
    }
    
}
