// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include <stdexcept>

namespace data {
    
    // TODO we don't even use this.
    template<bool is_signed, data::endian::order o, size_t size> 
    using bounded = data::math::number::bounded<is_signed, o, size, byte>;
    
    template <typename Z>
    void test_bit_negate () {
        EXPECT_EQ (~Z (0), Z (-1)) << "expected ~" << Z (0) << " to equal " << Z (-1);
        EXPECT_EQ (~Z (-1), Z (0)) << "expected ~" << Z (-1) << " to equal " << Z (0);
    }
    
    TEST (BitShiftTest, BitNegate) {
        
        test_bit_negate<Z_bytes_big> ();
        test_bit_negate<Z_bytes_little> ();
        test_bit_negate<int_big<9>> ();
        test_bit_negate<int_big<10>> ();
        test_bit_negate<int_big<11>> ();
        test_bit_negate<int_big<12>> ();
        test_bit_negate<int_little<9>> ();
        test_bit_negate<int_little<10>> ();
        test_bit_negate<int_little<11>> ();
        test_bit_negate<int_little<12>> ();
        test_bit_negate<int_big<5, short unsigned int>> ();
        test_bit_negate<int_big<6, short unsigned int>> ();
        test_bit_negate<int_big<7, short unsigned int>> ();
        test_bit_negate<int_big<8, short unsigned int>> ();
        test_bit_negate<int_little<5, short unsigned int>> ();
        test_bit_negate<int_little<6, short unsigned int>> ();
        test_bit_negate<int_little<7, short unsigned int>> ();
        test_bit_negate<int_little<8, short unsigned int>> ();
        test_bit_negate<int_big<3, unsigned int>> ();
        test_bit_negate<int_big<4, unsigned int>> ();
        test_bit_negate<int_big<5, unsigned int>> ();
        test_bit_negate<int_big<6, unsigned int>> ();
        test_bit_negate<int_little<3, unsigned int>> ();
        test_bit_negate<int_little<4, unsigned int>> ();
        test_bit_negate<int_little<5, unsigned int>> ();
        test_bit_negate<int_little<6, unsigned int>> ();

        // TODO add bigger words here
        /*
        test_bit_negate<int_big<2, long unsigned int>> ();
        test_bit_negate<int_big<3, long unsigned int>> ();
        test_bit_negate<int_big<4, long unsigned int>> ();
        test_bit_negate<int_big<5, long unsigned int>> ();
        test_bit_negate<int_little<2, long unsigned int>> ();
        test_bit_negate<int_little<3, long unsigned int>> ();
        test_bit_negate<int_little<4, long unsigned int>> ();
        test_bit_negate<int_little<5, long unsigned int>> ();
        test_bit_negate<int_big<1, long long unsigned int>> ();
        test_bit_negate<int_big<2, long long unsigned int>> ();
        test_bit_negate<int_big<3, long long unsigned int>> ();
        test_bit_negate<int_big<4, long long unsigned int>> ();
        test_bit_negate<int_little<1, long long unsigned int>> ();
        test_bit_negate<int_little<2, long long unsigned int>> ();
        test_bit_negate<int_little<3, long long unsigned int>> ();
        test_bit_negate<int_little<4, long long unsigned int>> ();*/
        
    }
    
    template <typename X> void test_bit_shift_bounded () {
        
        std::string     base_value {"0x100000000001000000000001"};
        std::string  shift_1_right {"0x080000000000800000000000"};
        std::string   shift_1_left {"0x200000000002000000000002"};
        std::string  shift_2_right {"0x040000000000400000000000"};
        std::string   shift_2_left {"0x400000000004000000000004"};
        std::string  shift_4_right {"0x010000000000100000000000"};
        std::string   shift_4_left {"0x000000000010000000000010"};
        std::string shift_33_right {"0x000000000800000000008000"};
        std::string  shift_33_left {"0x000200000000000200000000"};
        std::string shift_66_right {"0x000000000000000004000000"};
        std::string  shift_66_left {"0x000000040000000000000000"};
        
        auto result_1_left = X::read (base_value) << 1;
        EXPECT_EQ (result_1_left,  X::read (shift_1_left))
            << "expected " << base_value << " << " << 1 << " to equal " << shift_1_left 
            << " but instead it equals " << std::hex << result_1_left << std::endl;
        /*
        auto result_1_right = X::read (base_value) >> 1;
        EXPECT_EQ (result_1_right,  X::read (shift_1_right))
            << "expected " << base_value << " >> " << 1 << " to equal " << shift_1_right 
            << " but instead it equals " << std::hex << result_1_right << std::endl;
        
        auto result_2_left = X::read (base_value) << 2;
        EXPECT_EQ (result_2_left,  X::read (shift_2_left))
            << "expected " << base_value << " << " << 2 << " to equal " << shift_2_left 
            << " but instead it equals " << std::hex << result_2_left << std::endl;
        
        auto result_2_right = X::read (base_value) >> 2;
        EXPECT_EQ (result_2_right,  X::read (shift_2_right))
            << "expected " << base_value << " >> " << 2 << " to equal " << shift_2_right 
            << " but instead it equals " << std::hex << result_2_right << std::endl;
        
        auto result_4_left = X::read (base_value) << 4;
        EXPECT_EQ (result_4_left,  X::read (shift_4_left))
            << "expected " << base_value << " << " << 4 << " to equal " << shift_4_left 
            << " but instead it equals " << std::hex << result_4_left << std::endl;
        
        auto result_4_right = X::read (base_value) >> 4;
        EXPECT_EQ (result_4_right,  X::read (shift_4_right))
            << "expected " << base_value << " >> " << 4 << " to equal " << shift_4_right 
            << " but instead it equals " << std::hex << result_4_right << std::endl;
        
        auto result_33_left = X::read (base_value) << 33;
        EXPECT_EQ (result_33_left, X::read (shift_33_left))
            << "expected " << base_value << " << " << 33 << " to equal " << shift_33_left 
            << " but instead it equals " << std::hex << result_33_left << std::endl;
        
        auto result_33_right = X::read (base_value) >> 33;
        EXPECT_EQ (result_33_right, X::read (shift_33_right))
            << "expected " << base_value << " >> " << 33 << " to equal " << shift_33_right 
            << " but instead it equals " << std::hex << result_33_right << std::endl;
        
        auto result_66_left = X::read (base_value) << 66;
        EXPECT_EQ (result_66_left, X::read (shift_66_left))
            << "expected " << base_value << " << " << 66 << " to equal " << shift_66_left 
            << " but instead it equals " << std::hex << result_66_left << std::endl;
        
        auto result_66_right = X::read (base_value) >> 66;
        EXPECT_EQ (result_66_right, X::read (shift_66_right))
            << "expected " << base_value << " >> " << 66 << " to equal " << shift_66_right 
            << " but instead it equals " << std::hex << result_66_right << std::endl;*/
        
    }
    
    TEST (BitShiftTest, BitShiftBounded) {

        test_bit_shift_bounded<int_little<12>> ();
        test_bit_shift_bounded<int_big<12>> ();
        test_bit_shift_bounded<uint_little<12>> ();
        test_bit_shift_bounded<uint_big<12>> ();
        test_bit_shift_bounded<uint_little<6, short unsigned int>> ();
        test_bit_shift_bounded<uint_big<6, short unsigned int>> ();
        test_bit_shift_bounded<int_little<6, short unsigned int>> ();
        test_bit_shift_bounded<int_big<6, short unsigned int>> ();
        test_bit_shift_bounded<uint_little<3, unsigned int>> ();
        test_bit_shift_bounded<uint_big<3, unsigned int>> ();
        test_bit_shift_bounded<int_little<3, unsigned int>> ();
        test_bit_shift_bounded<int_big<3, unsigned int>> ();

        // TODO bigger word sizes; need to adjust the test in order to accomodate them.
        
    }

    // TODO test that << and >> are equal to the mathematical functions
    // x << 1 -> x ^ 2
    // x >> 1 -> (x + 1) / 2
    template <typename X> void test_bit_shift () {}

    TEST (BitShiftTest, BitShiftN) {
        test_bit_shift<N> ();
        test_bit_shift<hex_uint> ();
        test_bit_shift<dec_uint> ();
        test_bit_shift<base58_uint> ();
        test_bit_shift<math::N_bytes<endian::little>> ();
        test_bit_shift<math::N_bytes<endian::big>> ();
        test_bit_shift<math::N_bytes<endian::little, short unsigned int>> ();
        test_bit_shift<math::N_bytes<endian::big, short unsigned int>> ();
        test_bit_shift<math::N_bytes<endian::little, unsigned int>> ();
        test_bit_shift<math::N_bytes<endian::big, unsigned int>> ();

        // TODO bigger word sizes.
    }
    
}

