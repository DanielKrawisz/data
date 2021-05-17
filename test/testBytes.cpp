// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include "gtest/gtest.h"

namespace data {
    
    void from_bits(byte* b, size_t size, string x) {
        if (x.size() != size * 8) throw "bad string";
        
        auto it = x.begin();
        
        while(size != 0) {
            *b = 0;
            for (int i = 7; i >= 0; i--) {
                if(*it == '1') *b += (1 << i);
                it++;
            }
            
            b++;
            size--;
        }
    }
    
    template <size_t size>
    byte_array<size> byte_array_from_bits(string x) {
        byte_array<size> z;
        from_bits(z.data(), size, x);
        return z;
    }
    
    bytes bytes_from_bits(string x) {
        size_t size = x.size() / 8;
        bytes b(size);
        from_bits(b.data(), size, x);
        return b;
        
    }
    
    TEST(BytestringTest, TestBitNegate) {
        string initial{ "00000000111111110101010100110011"};
        string expected{"11111111000000001010101011001100"};
        
        byte_array<4> ai = byte_array_from_bits<4>(initial);
        byte_array<4> af = byte_array_from_bits<4>(expected);
        
        bytes bi = bytes_from_bits(initial);
        bytes bf = bytes_from_bits(expected);
        
        EXPECT_EQ(~ai, af);
        EXPECT_EQ(ai, ~af);
        
        EXPECT_EQ(~bi, bf);
        EXPECT_EQ(bi, ~bf);
        
    }
    
    TEST(BytestringTest, TestByteArrayBitAnd) {
        string a{"11001100010111000101100110101100"};
        string b{"11110000101001011100101010010011"};
        string e{"11000000000001000100100010000000"};
        
        byte_array<4> ba = byte_array_from_bits<4>(a);
        byte_array<4> bb = byte_array_from_bits<4>(b);
        byte_array<4> be = byte_array_from_bits<4>(e);
        
        EXPECT_EQ(ba & bb, be);
    }
    
    TEST(BytestringTest, TestByteArrayBitOr) {
        string a{"11001100010111000101100110101100"};
        string b{"11110000101001011100101010010011"};
        string e{"11111100111111011101101110111111"};
        
        byte_array<4> ba = byte_array_from_bits<4>(a);
        byte_array<4> bb = byte_array_from_bits<4>(b);
        byte_array<4> be = byte_array_from_bits<4>(e);
        
        EXPECT_EQ(ba | bb, be);
        
    }
    
    TEST(BytestringTest, TestByteArrayXor) {
        string a{"11001100010111000101100110101100"};
        string b{"11110000101001011100101010010011"};
        string e{"00111100111110011001001100111111"};
        
        byte_array<4> ba = byte_array_from_bits<4>(a);
        byte_array<4> bb = byte_array_from_bits<4>(b);
        byte_array<4> be = byte_array_from_bits<4>(e);
        
        EXPECT_EQ(ba ^ bb, be);
        
    }
    
}

