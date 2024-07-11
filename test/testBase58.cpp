// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/base58.hpp"
#include "data/encoding/hex.hpp"
#include "data/math/number/gmp/N.hpp"
#include "data/encoding/invalid.hpp"
#include <data/data.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"

namespace data::encoding {
    
    TEST (Base58Test, Base58ValidString) {
        EXPECT_TRUE (base58::valid ("1"));
        EXPECT_TRUE (base58::valid ("KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS"));
        EXPECT_TRUE (base58::valid ("3m8npvpNDU6k8zcAH8RBcUZeDLWx"));
        EXPECT_FALSE (base58::valid ("KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMIHPj74cgBWRmRvBS"));
    }
    
    TEST (Base58Test, Base58Invert) {
        for (int i = 0; i < base58::characters ().size (); i++) {
            EXPECT_EQ (base58::digit (base58::characters ()[i]), i);
        }
    }
    
    TEST (Base58Test, Base58NToString) {

        EXPECT_EQ (*base58::decode<N> ("1"), N {0});
        EXPECT_EQ (*base58::decode<N> ("2"), N {1});
        EXPECT_EQ (*base58::decode<N> ("1"), N {"0"});
        EXPECT_EQ (*base58::decode<N> ("2"), N {"1"});
        EXPECT_EQ (*base58::decode<N> ("1"), N {"0x"});
        EXPECT_EQ (*base58::decode<N> ("2"), N {"0x01"});
        EXPECT_EQ (*base58::decode<N> ("56"), N {"0xED"});
        EXPECT_EQ (*base58::decode<N> ("KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS"),
            N {"0x805AA786A57B3BFC0DFDF2EC86760339F018114A7E30C2D2701CF294DC60829D9B011CD8E391"});
        
        EXPECT_EQ (base58::encode<N> (*base58::decode<N> ("KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS")),
            "KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS");
        
        EXPECT_EQ (base58::write (*base58::read ("KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS")),
            "KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS");
    }
    
    TEST (Base58Test, Base58WriteBytes) {
        bytes testArray {0x80,0x5A,0xA7,0x86,0xA5,0x7B,0x3B,0xFC,0x0D,0xFD,0xF2,0xEC,0x86,0x76,0x03,0x39,0xF0,0x18,0x11,
            0x4A,0x7E,0x30,0xC2,0xD2,0x70,0x1C,0xF2,0x94,0xDC,0x60,0x82,0x9D,0x9B,0x01,0x1C,0xD8,0xE3,0x91};
        
        EXPECT_EQ (base58::write (testArray), "KzFvxm6N9qW11MbVoZM8c3tp6UHqf1qrh9EMcHPj74cgBWRmRvBS");
        EXPECT_EQ (*base58::read (base58::write (testArray)), testArray);
    }
    
}
