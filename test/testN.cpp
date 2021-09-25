// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include <data/math/number/bytes/N.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data {
    
    TEST(NTest, TestStringToN) {
        
        EXPECT_FALSE(N{}.valid());
        EXPECT_FALSE(N{""}.valid());
        EXPECT_FALSE(N{"a"}.valid());
        EXPECT_FALSE(N{"-"}.valid());
        EXPECT_FALSE(N{"-1"}.valid());
        EXPECT_FALSE(N{"01"}.valid());
        EXPECT_FALSE(N{"0x1"}.valid());
        
        EXPECT_TRUE(N{0}.valid());
        EXPECT_TRUE(N{"0"}.valid());
        EXPECT_TRUE(N{"0x"}.valid());
        
        EXPECT_EQ(N{}, N{""});
        
        EXPECT_EQ(N{0}, N{"0"});
        EXPECT_EQ(N{0}, N{"0x"});
        
        EXPECT_NE(N{0}, N{""});
        EXPECT_NE(N{1}, N{"0"});
        EXPECT_NE(N{0}, N{"1"});
        
        EXPECT_EQ(N{1}, N{"1"});
        EXPECT_EQ(N{1}, N{"0x01"});
        EXPECT_EQ(N{1}, N{"0x0001"});
        EXPECT_EQ(N{255}, N{"0xff"});
        
        EXPECT_EQ(encoding::hexidecimal::write(N{0}), "0x00");
        EXPECT_EQ(encoding::integer::write(N{0}), "0");
        EXPECT_EQ(encoding::hexidecimal::write(N{1}), "0x01");
        EXPECT_EQ(encoding::integer::write(N{1}), "1");
        
    }
    
    TEST(NTest, TestNToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(N{"0"}), std::string{"0x00"});
        EXPECT_EQ(encoding::hexidecimal::write(N{"127"}), std::string{"0x7f"});
        EXPECT_EQ(encoding::hexidecimal::write(N{"128"}), std::string{"0x80"});
        
    }
    
    TEST(NTest, TestNNegativeHex) {
        
        EXPECT_NE(N{"0x8000000000000000000000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x80000000000000000000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x80000000000000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x8000000000000000000000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x8000000000000000000000000000"}, N(0));
        EXPECT_NE(N{"0x8000000000"}, N(0));
        EXPECT_NE(N{"0x8000"}, N(0));
        EXPECT_NE(N{"0x80"}, N(0));
        
    }
    
    TEST(NTest, TestMultiply) {
        
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
    
    TEST(NTest, TestShift) {
        
        EXPECT_TRUE(N{1} >>  1 == N{"0x00000000000000000000"});
        EXPECT_TRUE(N{1} <<  1 == N{"0x00000000000000000002"});
        EXPECT_TRUE(N{1} <<  3 == N{"0x00000000000000000008"});
        EXPECT_TRUE(N{1} <<  7 == N{"0x00000000000000000080"});
        EXPECT_TRUE(N{1} << 79 == N{"0x80000000000000000000"});
        
    }
        
    template<endian::order r> using N_bytes = math::number::N_bytes<r>;
    
    TEST(NTest, TestN) {
        
        // parameters from https://en.bitcoin.it/wiki/Secp256k1
        
        N p = (N{1} << 256) - (N{1} << 32) - (N{1} << 9) - (N{1} << 8) - (N{1} << 7) - (N{1} << 6) - (N{1} << 4) - 1;
        
        std::string p_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
        std::string p_dec = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
        
        std::string n_hex = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
        std::string n_dec = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
        
        N n = N{n_hex};
        
        EXPECT_TRUE(p == p);
        EXPECT_FALSE(p == n);
        
        EXPECT_TRUE(n == n);
        EXPECT_FALSE(n == p);
        
        EXPECT_TRUE(p == N{p_hex});
        EXPECT_FALSE(p == N{n_hex});
        EXPECT_FALSE(n == N{p_hex});
        
        EXPECT_TRUE(p == N{p_dec});
        EXPECT_TRUE(n == N{n_dec});
        
        EXPECT_FALSE(p == N{n_dec});
        EXPECT_FALSE(n == N{p_dec});
        
        N_bytes<endian::big> p_bytes{p_hex};
        N_bytes<endian::big> n_bytes{n_hex};
        
        N p_from_big{p_bytes};
        N n_from_big{n_bytes};
        
        N p_from_little{N_bytes<endian::little>{p_from_big}};
        N n_from_little{N_bytes<endian::little>{n_from_big}};
        
        EXPECT_EQ(p_bytes, N_bytes<endian::big>{p_from_big});
        EXPECT_EQ(n_bytes, N_bytes<endian::big>{n_from_big});
        
        EXPECT_EQ(p_from_big, p_from_little);
        EXPECT_EQ(n_from_big, n_from_little);
        
    }
    
    TEST(NTest, TestNRemainder) {
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") % 1, N(0));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") % 10, N(5));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") % 16, N(15));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") % 58, N(29));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 1,
            N("26959946667150639794667015087019630673637144422540572481103610249215"));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 10, 
            N("2695994666715063979466701508701963067363714442254057248110361024921"));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 16, 
            N("1684996666696914987166688442938726917102321526408785780068975640575"));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 58, 
            N("464826666675011030942534742879648804717881800388630560019027762917"));
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N("26959946667150639794667015087019630673637144422540572481103610249215"), N{1});
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N("2695994666715063979466701508701963067363714442254057248110361024921"), N{10});
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N("1684996666696914987166688442938726917102321526408785780068975640575"), N{16});
        
        EXPECT_EQ(N("26959946667150639794667015087019630673637144422540572481103610249215") / 
            N("464826666675011030942534742879648804717881800388630560019027762917"), N{58});
    }
    
    TEST(NTest, TestRoot) {
        auto root_17_0 = math::number::gmp::root(N("17"), 0);
        auto root_25_0 = math::number::gmp::root(N("25"), 0);
        auto root_422_1 = math::number::gmp::root(N("422"), 1);
        auto root_4_2 = math::number::gmp::root(N("4"), 2);
        auto root_16_2 = math::number::gmp::root(N("16"), 2);
        auto root_25_2 = math::number::gmp::root(N("25"), 2);
        auto root_27_2 = math::number::gmp::root(N("27"), 2);
        auto root_125_3 = math::number::gmp::root(N("125"), 3);
        auto root_128_3 = math::number::gmp::root(N("128"), 3);
        auto root_128_7 = math::number::gmp::root(N("128"), 7);
        auto root_1798307508862833999690304313948111955510002315423096853_19 =
            math::number::gmp::root(N("1798307508862833999690304313948111955510002315423096853"), 19);
        auto root_1798307508862833999690304313948111955510002315423096853_18 =
            math::number::gmp::root(N("1798307508862833999690304313948111955510002315423096853"), 18);
        auto root_1798307508862833999690304313948111955510002315423096851_19 =
            math::number::gmp::root(N("1798307508862833999690304313948111955510002315423096851"), 19);
        
        EXPECT_FALSE(root_17_0.valid());
        EXPECT_FALSE(root_25_0.valid());
        EXPECT_TRUE(root_422_1.valid());
        EXPECT_TRUE(root_4_2.valid());
        EXPECT_TRUE(root_16_2.valid());
        EXPECT_TRUE(root_25_2.valid());
        EXPECT_FALSE(root_27_2.valid());
        EXPECT_TRUE(root_125_3.valid());
        EXPECT_FALSE(root_128_3.valid());
        EXPECT_TRUE(root_128_7.valid());
        EXPECT_TRUE(root_1798307508862833999690304313948111955510002315423096853_19.valid());
        EXPECT_FALSE(root_1798307508862833999690304313948111955510002315423096853_18.valid());
        EXPECT_FALSE(root_1798307508862833999690304313948111955510002315423096851_19.valid());
        
        EXPECT_EQ(root_422_1, N("422"));
        EXPECT_EQ(root_4_2, N("2"));
        EXPECT_EQ(root_16_2, N("4"));
        EXPECT_EQ(root_25_2, N("5"));
        EXPECT_EQ(root_125_3, N("5"));
        EXPECT_EQ(root_128_7, N("2"));
        EXPECT_EQ(root_1798307508862833999690304313948111955510002315423096853_19, N("717"));
    }
    
    TEST(NTest, TestAKS) {
        math::number::AKS<N> aks{};
        
        EXPECT_FALSE(aks.is_prime(N{"0"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"1"}).valid());
        EXPECT_TRUE(aks.is_prime(N{"2"}).valid());
        EXPECT_TRUE(aks.is_prime(N{"3"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"4"}).valid());
        EXPECT_TRUE(aks.is_prime(N{"5"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"6"}).valid());
        EXPECT_TRUE(aks.is_prime(N{"7"}).valid());
        
        EXPECT_TRUE(aks.is_prime(N{"23"}).valid());
        EXPECT_TRUE(aks.is_prime(N{"101"}).valid());
        
        EXPECT_FALSE(aks.is_prime(N{"519"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"3439"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"988320843"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"2904873984723454089"}).valid());
        EXPECT_FALSE(aks.is_prime(N{"4095842309824958234058934985234958304985083"}).valid());
        
        // These tests are commented out because they run too slow for practical use. 
        /*EXPECT_TRUE(aks.is_prime(N{"523"}).valid());
        
        EXPECT_TRUE(aks.is_prime(N{"3449"}).valid());
        
        EXPECT_TRUE(aks.is_prime(N{"988320847"}).valid());
        
        EXPECT_TRUE(aks.is_prime(N{"2904873984723454103"}).valid());
        
        EXPECT_TRUE(aks.is_prime(N{"4095842309824958234058934985234958304985117"}).valid());
        
        EXPECT_TRUE(aks.is_prime(N{"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}).valid());*/
        
    }
    
}
