// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/list.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include <stdexcept>

#include <data/io/wait_for_enter.hpp>

namespace data {
    
    template <typename Z> void test_bit_negate () {
        EXPECT_EQ (~Z (0), Z (-1)) << "expected ~" << Z (0) << " to equal " << Z (-1);
        EXPECT_EQ (~Z (-1), Z (0)) << "expected ~" << Z (-1) << " to equal " << Z (0);
    }

    template <typename X>
    struct BitNegate : ::testing::Test {
        using number = X;
    };

    using bit_negate_test_cases = ::testing::Types<
        int64, int64_little, int64_big,
        int80, int80_little, int80_big,
        int128, int128_little, int128_big,
        int160, int160_little, int160_big,
        int256, int256_little, int256_big,
        int512, int512_little, int512_big,
        Z_bytes_little, Z_bytes_big,
        math::Z_bytes<endian::little, unsigned short>,
        math::Z_bytes<endian::little, unsigned int>,
        math::Z_bytes<endian::little, unsigned long>,
        math::Z_bytes<endian::little, unsigned long long>,
        hex_int>;

    TYPED_TEST_SUITE (BitNegate, bit_negate_test_cases);
    
    TYPED_TEST (BitNegate, BitNegate) {
        using X = typename TestFixture::number;
        test_bit_negate<X> ();
    }

    template <typename Z> void test_bit_xor_unsigned () {
        EXPECT_EQ ((bit_xor (Z (0), Z (0))), Z (0));
        EXPECT_EQ ((bit_xor (Z (1), Z (1))), Z (0));
        EXPECT_EQ ((bit_xor (Z (1), Z (2))), Z (3));
    }

    template <typename Z> void test_bit_and_unsigned () {
        EXPECT_EQ ((bit_and (Z (0), Z (0))), Z (0));
        EXPECT_EQ ((bit_and (Z (1), Z (1))), Z (1));
        EXPECT_EQ ((bit_and (Z (1), Z (2))), Z (0));
    }

    template <typename Z> void test_bit_or_unsigned () {
        EXPECT_EQ ((bit_or (Z (0), Z (0))), Z (0));
        EXPECT_EQ ((bit_or (Z (1), Z (1))), Z (1));
        EXPECT_EQ ((bit_or (Z (1), Z (2))), Z (3));
    }

    template <typename X>
    struct BitArithmetic : ::testing::Test {
        using number = X;
    };

    // number types required to have bit xor:
    //   * natural numbers
    //   * numbers with complement 2
    using bit_arith_test_cases = ::testing::Types<
        N, dec_uint,
        hex_uint,
        base58_uint,
        int64, int64_little, int64_big,
        int80, int80_little, int80_big,
        int128, int128_little, int128_big,
        int160, int160_little, int160_big,
        int256, int256_little, int256_big,
        int512, int512_little, int512_big,
        uint64, uint64_little, uint64_big,
        uint80, uint80_little, uint80_big,
        uint128, uint128_little, uint128_big,
        uint160, uint160_little, uint160_big,
        uint256, uint256_little, uint256_big,
        uint512, uint512_little, uint512_big,
        Z_bytes_little, Z_bytes_big,
        N_bytes_little, N_bytes_big,
        math::Z_bytes<endian::little, unsigned short>,
        math::Z_bytes<endian::little, unsigned int>,
        math::Z_bytes<endian::little, unsigned long>,
        math::Z_bytes<endian::little, unsigned long long>,
        math::N_bytes<endian::little, unsigned short>,
        math::N_bytes<endian::little, unsigned int>,
        math::N_bytes<endian::little, unsigned long>,
        math::N_bytes<endian::little, unsigned long long>>;

    TYPED_TEST_SUITE (BitArithmetic, bit_arith_test_cases);

    TYPED_TEST (BitArithmetic, BitXor) {
        test_bit_xor_unsigned<typename TestFixture::number> ();
    }

    TYPED_TEST (BitArithmetic, BitOr) {
        test_bit_xor_unsigned<typename TestFixture::number> ();
    }

    TYPED_TEST (BitArithmetic, BitAnd) {
        test_bit_xor_unsigned<typename TestFixture::number> ();
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
            << " but instead it equals " << std::hex << result_66_right << std::endl;
        
    }
    
    TEST (BitOps, BitShiftBounded) {

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
    template <typename N> void test_bit_shift_unbounded (list<string> numbers, list<int32> shifts) {

        for (const string &number_string: numbers) {

            N number = N {number_string};
            std::cout << "  shift number " << number << std::endl;
            for (int32 shift : shifts) {
                std::cout << "    by " << shift << std::endl;
                N expected_left = nest ([] (const N &n) {
                    return n * 2;
                }, number, shift);

                N expected_right = nest ([] (const N &n) {
                    return n / 2;
                }, number, shift);
                std::cout << "    expected shift right: " << expected_right << std::endl;
                wait_for_enter ();

                //auto computed_left = number << shift;
                auto computed_right = number >> shift;

                //EXPECT_EQ (expected_left, computed_left);
                EXPECT_EQ (expected_right, computed_right) <<
                    "expected " << number << " >> " << shift << " -> " << expected_right << "; but got " << computed_right;
            }
        }

    }

    list<int32> shifts {0, 1, 2, 3, 5, 7, 11, 13, 17, 257};

    list<string> positive_numbers {
        "0", "1", "2", "3", "5", "7", "11", "13", "17", "257", "131071",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFEE37",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF",
        "0x000000010000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7",
        "0x00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFE56D",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973",
        "0x0000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF",
        "0x00000000000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
        "0x00000000000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA51868783BF2F966B7FCC0148F709A5D0"
            "3BB5C9B8899C47AEBB6FB71E91386409"};

    list<string> negative_numbers {"-1", "-2", "-3", "-5", "-7", "-11", "-13", "-17", "-257", "-131071"};

    template <typename N> void test_bit_shift_signed () {
        test_bit_shift_unbounded<N> (positive_numbers + negative_numbers, shifts);
    }

    template <typename N> void test_bit_shift_unsigned () {
        test_bit_shift_unbounded<N> (positive_numbers, shifts);
    }

    TEST (BitOps, BitShiftUnbounded) {
        // NOTE some of the commented tests don't compile and others are too slow.
        /*
        test_bit_shift_unsigned<N> ();
        test_bit_shift_unsigned<hex_uint> ();
        test_bit_shift_unsigned<dec_uint> ();
        test_bit_shift_unsigned<base58_uint> ();

        test_bit_shift_signed<Z> ();
        test_bit_shift_signed<hex_int> ();
        test_bit_shift_signed<hex_int_twos> ();
        test_bit_shift_signed<dec_int> ();*/

        test_bit_shift_unsigned<math::N_bytes<endian::little>> ();/*
        test_bit_shift_unsigned<math::N_bytes<endian::big>> ();
        test_bit_shift_unsigned<math::N_bytes<endian::little, short unsigned int>> ();
        test_bit_shift_unsigned<math::N_bytes<endian::big, short unsigned int>> ();
        test_bit_shift_unsigned<math::N_bytes<endian::little, unsigned int>> ();
        test_bit_shift_unsigned<math::N_bytes<endian::big, unsigned int>> ();

        test_bit_shift_signed<math::Z_bytes<endian::little>> ();
        test_bit_shift_signed<math::Z_bytes<endian::big>> ();
        test_bit_shift_signed<math::Z_bytes<endian::little, short unsigned int>> ();
        test_bit_shift_signed<math::Z_bytes<endian::big, short unsigned int>> ();
        test_bit_shift_signed<math::Z_bytes<endian::little, unsigned int>> ();
        test_bit_shift_signed<math::Z_bytes<endian::big, unsigned int>> ();

        test_bit_shift_signed<math::Z_bytes_BC<endian::little>> ();
        test_bit_shift_signed<math::Z_bytes_BC<endian::big>> ();
        test_bit_shift_signed<math::Z_bytes_BC<endian::little, short unsigned int>> ();
        test_bit_shift_signed<math::Z_bytes_BC<endian::big, short unsigned int>> ();
        test_bit_shift_signed<math::Z_bytes_BC<endian::little, unsigned int>> ();
        test_bit_shift_signed<math::Z_bytes_BC<endian::big, unsigned int>> ();*/

        // TODO bigger word sizes.
    }

    // Test that numbers get extended to perform bit ops if necessary.
    TEST (BitOpsTest, BitAndOr) {



    }
    
}

