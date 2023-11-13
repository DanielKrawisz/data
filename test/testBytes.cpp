// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/cross.hpp>
#include <data/string.hpp>
#include <data/encoding/hex.hpp>
#include "gtest/gtest.h"

namespace data {
    
    void from_bits (byte* b, size_t size, string x) {
        if (x.size () != size * 8) throw "bad string";
        
        auto it = x.begin ();
        
        while (size != 0) {
            *b = 0;
            for (int i = 7; i >= 0; i--) {
                if (*it == '1') *b += (1 << i);
                it++;
            }
            
            b++;
            size--;
        }
    }
    
    template <std::unsigned_integral word, size_t size>
    bytes_array<word, size> byte_array_from_bits (string x) {
        bytes_array<word, size> z;
        from_bits ((byte*) z.data (), size * sizeof (word), x);
        return z;
    }
    
    bytes bytes_from_bits (string x) {
        size_t size = x.size () / 8;
        bytes b (size);
        from_bits (b.data (), size, x);
        return b;
    }
    
    template <typename T> void test_bit_negate (T a, T b) {
        EXPECT_EQ (~a, b);
        EXPECT_EQ (a, ~b);
    }
    
    TEST (BytesTest, TestBitNegate) {
        string initial {"00000000111111110101010100110011"};
        string expected {"11111111000000001010101011001100"};
        
        test_bit_negate (
            byte_array_from_bits<byte, 4> (initial),
            byte_array_from_bits<byte, 4> (expected));
        
        test_bit_negate (
            byte_array_from_bits<uint16, 2> (initial),
            byte_array_from_bits<uint16, 2> (expected));
        
        test_bit_negate (
            byte_array_from_bits<uint32, 1> (initial),
            byte_array_from_bits<uint32, 1> (expected));
        
        test_bit_negate (
            bytes_from_bits (initial),
            bytes_from_bits (expected));
        
    }
    
    template <typename T> void test_bit_add (T a, T b, T c) {
        EXPECT_EQ (a & b, c);
    }
    
    TEST (BytesTest, TestByteArrayBitAnd) {
        string a {"11001100010111000101100110101100"};
        string b {"11110000101001011100101010010011"};
        string e {"11000000000001000100100010000000"};
        
        test_bit_add (
            byte_array_from_bits<byte, 4> (a),
            byte_array_from_bits<byte, 4> (b),
            byte_array_from_bits<byte, 4> (e));
        
        test_bit_add (
            byte_array_from_bits<uint16, 2> (a),
            byte_array_from_bits<uint16, 2> (b),
            byte_array_from_bits<uint16, 2> (e));
        
        test_bit_add (
            byte_array_from_bits<uint32, 1> (a),
            byte_array_from_bits<uint32, 1> (b),
            byte_array_from_bits<uint32, 1> (e));
    }
    
    template <typename T> void test_bit_or (T a, T b, T c) {
        EXPECT_EQ (a | b, c);
    }
    
    TEST (BytesTest, TestByteArrayBitOr) {
        string a {"11001100010111000101100110101100"};
        string b {"11110000101001011100101010010011"};
        string e {"11111100111111011101101110111111"};
        
        test_bit_or (
            byte_array_from_bits<byte, 4> (a),
            byte_array_from_bits<byte, 4> (b),
            byte_array_from_bits<byte, 4> (e));
        
        test_bit_or (
            byte_array_from_bits<uint16, 2> (a),
            byte_array_from_bits<uint16, 2> (b),
            byte_array_from_bits<uint16, 2> (e));
        
        test_bit_or (
            byte_array_from_bits<uint32, 1> (a),
            byte_array_from_bits<uint32, 1> (b),
            byte_array_from_bits<uint32, 1> (e));
        
    }
    
    template <typename T> void test_bit_xor (T a, T b, T c) {
        EXPECT_EQ (a ^ b, c);
    }
    
    TEST (BytesTest, TestByteArrayXor) {
        string a {"11001100010111000101100110101100"};
        string b {"11110000101001011100101010010011"};
        string e {"00111100111110011001001100111111"};
        
        test_bit_xor (
            byte_array_from_bits<byte, 4> (a),
            byte_array_from_bits<byte, 4> (b),
            byte_array_from_bits<byte, 4> (e));
        
        test_bit_xor (
            byte_array_from_bits<uint16, 2> (a),
            byte_array_from_bits<uint16, 2> (b),
            byte_array_from_bits<uint16, 2> (e));
        
        test_bit_xor (
            byte_array_from_bits<uint32, 1> (a),
            byte_array_from_bits<uint32, 1> (b),
            byte_array_from_bits<uint32, 1> (e));
        
    }
    
    template <typename T> void test_shift_left (T x, int i, T e) {
        auto n = x << i;
        EXPECT_EQ (n, e) << "expected " << x << " << " << i << " = " << n << " to equal " << e;
    }
    
    template <typename T> void test_shift_right (T x, int i, T e) {
        auto n = x >> i;
        EXPECT_EQ (n, e) << "expected " << x << " >> " << i << " = " << n << " to equal " << e;
    }
    
    template <typename word, size_t x>
    void test_bit_shift (string given, int bits, string expected_left, string expected_right) {
        
        auto g = byte_array_from_bits<word, x> (given);
        auto el = byte_array_from_bits<word, x> (expected_left);
        auto er = byte_array_from_bits<word, x> (expected_right);
        
        test_shift_left (g, bits, el);
        test_shift_right (g, bits, er);
    }
    
    void test_shift (string given, int bits, string expected_left, string expected_right) {
        test_bit_shift<byte, 4> (given, bits, expected_left, expected_right);
        test_bit_shift<uint16, 2> (given, bits, expected_left, expected_right);
        test_bit_shift<uint32, 1> (given, bits, expected_left, expected_right);
    }
    
    TEST (BytesTest, TestBitShift) {
        
        string x    {"11001100010111000101100110101100"};
        string e0l  {"11001100010111000101100110101100"};
        string e0r  {"11001100010111000101100110101100"};
        string e1l  {"10011000101110001011001101011000"};
        string e1r  {"01100110001011100010110011010110"};
        string e2l  {"00110001011100010110011010110000"};
        string e2r  {"00110011000101110001011001101011"};
        string e3l  {"01100010111000101100110101100000"};
        string e3r  {"00011001100010111000101100110101"};
        string e8l  {"01011100010110011010110000000000"};
        string e8r  {"00000000110011000101110001011001"};
        string e9l  {"10111000101100110101100000000000"};
        string e9r  {"00000000011001100010111000101100"};
        string e15l {"00101100110101100000000000000000"};
        string e15r {"00000000000000011001100010111000"};
        
        test_shift (x, 0, e0l, e0r);
        test_shift (x, 1, e1l, e1r);
        test_shift (x, 2, e2l, e2r);
        test_shift (x, 3, e3l, e3r);
        test_shift (x, 8, e8l, e8r);
        test_shift (x, 9, e9l, e9r);
        test_shift (x, 15, e15l, e15r);
        
    } 
    
    TEST (BytesTest, TestSliceBytes) {
        maybe<bytes> x = encoding::hex::read ("00");
        EXPECT_TRUE (bool (x));
        auto a = *x;
        EXPECT_EQ (a.size (), 1);
        EXPECT_EQ (a[0], 0);
        EXPECT_EQ (a[-1], 0);
        EXPECT_EQ (slice<byte> (a).size (), 1);
        EXPECT_EQ (slice<byte> (a)[0], 0);
        EXPECT_EQ (slice<byte> (a)[-1], 0);
        
    }
    
}

