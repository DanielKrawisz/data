// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"

namespace data {
    
    TEST(ZBytesTest, TestZBytesToHexString) {
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("0")), std::string("0x00"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("-1")), std::string("0xff"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("-2")), std::string("0xfd"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("127")), std::string("0x7f"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("128")), std::string("0x0080"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("-128")), std::string("0x80"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>::read("-129")), std::string("0xff7f"));
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("0")), std::string("0x00"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("-1")), std::string("0xff"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("-2")), std::string("0xfd"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("127")), std::string("0x7f"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("128")), std::string("0x0080"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("-128")), std::string("0x80"));
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>::read("-129")), std::string("0xff7f"));
        
    }
    
    TEST(ZBytesTest, TestStringToZBytes) {
        
        EXPECT_NO_THROW(Z_bytes<endian::big>{});
        EXPECT_THROW(Z_bytes<endian::big>{N{}}, std::invalid_argument);
        EXPECT_THROW(Z_bytes<endian::big>{Z{}}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::big>{""});
        EXPECT_NO_THROW(Z_bytes<endian::big>{"-1"});
        EXPECT_THROW(Z_bytes<endian::big>{"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0x00"});
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0xff"});
        
        EXPECT_NO_THROW(Z_bytes<endian::little>{});
        EXPECT_THROW(Z_bytes<endian::little>{N{}}, std::invalid_argument);
        EXPECT_THROW(Z_bytes<endian::little>{Z{}}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::little>{""});
        EXPECT_NO_THROW(Z_bytes<endian::little>{"-1"});
        EXPECT_THROW(Z_bytes<endian::little>{"-0x01"}, std::invalid_argument);
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0x00"});
        EXPECT_NO_THROW(Z_bytes<endian::big>{"0xff"});
        
    }
    
    TEST(ZBytesTest, TestZeroSize) {
        
        EXPECT_EQ(Z_bytes<endian::big>::read("0x").size(), 0);
        EXPECT_EQ(Z_bytes<endian::big>::read("0x00").size(), 1);
        EXPECT_EQ(Z_bytes<endian::big>::read("0x000000").size(), 3);
        
        EXPECT_EQ(Z_bytes<endian::little>::read("0x").size(), 0);
        EXPECT_EQ(Z_bytes<endian::little>::read("0x00").size(), 1);
        EXPECT_EQ(Z_bytes<endian::little>::read("0x000000").size(), 3);
        
    }
    
    TEST(ZBytesTest, TestZeroAndNegativeZBytes) {
        
        EXPECT_EQ(Z_bytes<endian::big>::read("0x"), Z_bytes<endian::big>{0});
        EXPECT_EQ(Z_bytes<endian::big>::read("0x00"), Z_bytes<endian::big>{0});
        EXPECT_EQ(Z_bytes<endian::big>::read("0x000000"), Z_bytes<endian::big>{0});
        EXPECT_EQ(Z_bytes<endian::little>::read("0x"), Z_bytes<endian::little>{0});
        EXPECT_EQ(Z_bytes<endian::little>::read("0x00"), Z_bytes<endian::little>{0});
        EXPECT_EQ(Z_bytes<endian::little>::read("0x000000"), Z_bytes<endian::little>{0});
        
        EXPECT_EQ(Z_bytes<endian::big>::read("0x01"), Z_bytes<endian::big>{1});
        EXPECT_EQ(Z_bytes<endian::big>::read("0x0001"), Z_bytes<endian::big>{1});
        EXPECT_EQ(Z_bytes<endian::big>::read("0x00000001"), Z_bytes<endian::big>{1});
        EXPECT_EQ(Z_bytes<endian::little>::read("0x01"), Z_bytes<endian::little>{1});
        EXPECT_EQ(Z_bytes<endian::little>::read("0x0001"), Z_bytes<endian::little>{1});
        EXPECT_EQ(Z_bytes<endian::little>::read("0x00000001"), Z_bytes<endian::little>{1});
        
        EXPECT_EQ(Z_bytes<endian::big>::read("0xff"), Z_bytes<endian::big>{-1});
        EXPECT_EQ(Z_bytes<endian::big>::read("0xffff"), Z_bytes<endian::big>{-1});
        EXPECT_EQ(Z_bytes<endian::big>::read("0xffffff"), Z_bytes<endian::big>{-1});
        EXPECT_EQ(Z_bytes<endian::little>::read("0xff"), Z_bytes<endian::little>{-1});
        EXPECT_EQ(Z_bytes<endian::little>::read("0xffff"), Z_bytes<endian::little>{-1});
        EXPECT_EQ(Z_bytes<endian::little>::read("0xffffff"), Z_bytes<endian::little>{-1});
        
    }
    
    // Problem: string reversal not happening correctly for some reason!!
    TEST(ZBytesTest, TestZToZBytes) {
        
        EXPECT_EQ(Z_bytes<endian::big>{Z{"1"}}, Z_bytes<endian::big>{"1"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"1"}}, Z_bytes<endian::little>{"1"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"23"}}, Z_bytes<endian::big>{"23"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"23"}}, Z_bytes<endian::little>{"23"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"5704566599993321"}}, Z_bytes<endian::big>{"5704566599993321"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"5704566599993321"}}, Z_bytes<endian::little>{"5704566599993321"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"-1"}}, Z_bytes<endian::big>{"-1"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"-1"}}, Z_bytes<endian::little>{"-1"});
        EXPECT_EQ(Z_bytes<endian::big>{Z{"-3393939987200333"}}, Z_bytes<endian::big>{"-3393939987200333"});
        EXPECT_EQ(Z_bytes<endian::little>{Z{"-3393939987200333"}}, Z_bytes<endian::little>{"-3393939987200333"});
        
    }
    
    TEST(ZBytesTest, TestIncrementAndDecrement) {
        
        EXPECT_EQ(++Z_bytes<endian::big>{"0x"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(++Z_bytes<endian::big>{"0x00"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(++Z_bytes<endian::big>{"0x000000"}, Z_bytes<endian::big>{1});
        EXPECT_EQ(++Z_bytes<endian::little>{"0x"}, Z_bytes<endian::little>{1});
        EXPECT_EQ(++Z_bytes<endian::little>{"0x00"}, Z_bytes<endian::little>{1});
        EXPECT_EQ(++Z_bytes<endian::little>{"0x000000"}, Z_bytes<endian::little>{1});
        
        EXPECT_EQ(--Z_bytes<endian::big>{"0x"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(--Z_bytes<endian::big>{"0x00"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(--Z_bytes<endian::big>{"0x000000"}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(--Z_bytes<endian::little>{"0x"}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(--Z_bytes<endian::little>{"0x00"}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(--Z_bytes<endian::little>{"0x000000"}, Z_bytes<endian::little>{-1});
        
    }
    
    TEST(ZBytesTest, TestZBytesToString) {
        
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"1"}),std::string{"1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"1"}), std::string{"1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"23"}), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"23"}), std::string{"23"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"5704566599993321"}), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"5704566599993321"}), std::string{"5704566599993321"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"-1"}), std::string{"-1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"-1"}), std::string{"-1"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::big>{"-3393939987200333"}), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::integer::write(Z_bytes<endian::little>{"-3393939987200333"}), std::string{"-3393939987200333"});
        
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"1"}), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"1"}), std::string{"0x01"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"23"}), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"23"}), std::string{"0x17"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"5704566599993321"}), std::string{"144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"5704566599993321"}), std::string{"144445e9ca47e9"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-1"}), std::string{"0xff"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::big>{"-3393939987200333"}), std::string{"-3393939987200333"});
        EXPECT_EQ(encoding::hexidecimal::write(Z_bytes<endian::little>{"-3393939987200333"}), std::string{"-3393939987200333"});
        
    }
    
    TEST(NBytesTest, TestZBytesToZ) {
        
        EXPECT_EQ(Z{Z_bytes<endian::big>{}}, Z{0});
        EXPECT_EQ(Z{Z_bytes<endian::big>{""}}, Z{0});
        EXPECT_EQ(Z{Z_bytes<endian::little>{}}, Z{0});
        EXPECT_EQ(Z{Z_bytes<endian::little>{""}}, Z{0});
        
    }
    
}
