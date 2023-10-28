// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

// We test that number types work like Bitcoin numbers. 
namespace data::math::number {
    template <typename Z> requires requires (const Z &z) {
        {is_minimal (z)} -> std::same_as<bool>;
    } struct test_minimal {
        test_minimal () {
            EXPECT_TRUE (is_minimal (Z {string {"0x"}}));
            EXPECT_FALSE (is_minimal (Z {string {"0x00"}}));
            EXPECT_TRUE (is_minimal (Z {string {"0x01"}}));
            EXPECT_FALSE (is_minimal (Z {string {"0x0001"}}));
            EXPECT_TRUE (is_minimal (Z {string {"0x81"}}));
            EXPECT_FALSE (is_minimal (Z {string {"0x8001"}}));
        }
    };
    
    template <typename Z> requires requires (const Z &z) {
        { is_zero (z) } -> std::same_as<bool>;
        { is_positive_zero (z) } -> std::same_as<bool>;
        { is_negative_zero (z) } -> std::same_as<bool>;
    } && requires (const Z &a, const Z &b) {
        { data::identical (a, b) } -> std::same_as<bool>;
    } && requires (size_t size, bool negative) {
        { Z::zero (size, negative) } -> std::same_as<Z>;
    } struct test_zero {
        test_zero () {

            EXPECT_TRUE (data::identical (Z::zero (0, false), Z::read ("0x")));
            EXPECT_TRUE (data::identical (Z::zero (1, false), Z::read ("0x00")));
            EXPECT_TRUE (data::identical (Z::zero (1, true), Z::read ("0x80")));
            EXPECT_TRUE (data::identical (Z::zero (2, false), Z::read ("0x0000")));
            EXPECT_TRUE (data::identical (Z::zero (2, true), Z::read ("0x8000")));

            EXPECT_FALSE (data::identical (Z::zero (0), Z::zero (1)));
            EXPECT_FALSE (data::identical (Z::zero (1), Z::zero (1, true)));
            EXPECT_FALSE (data::identical (Z::zero (1), Z::zero (2)));
            EXPECT_FALSE (data::identical (Z::zero (2), Z::zero (2, true)));

        }
    };
    
    template <typename Z> requires requires (const Z &a, const Z &b) {
        {a == b} -> std::same_as<bool>;
        {a != b} -> std::same_as<bool>;
    } struct test_equal {
        test_equal () {

        }
    };
    
    template <typename Z> requires requires (const Z &a, const Z &b) {
        {a < b} -> std::same_as<bool>;
        {a > b} -> std::same_as<bool>;
        {a <= b} -> std::same_as<bool>;
        {a >= b} -> std::same_as<bool>;
    } struct test_compare {
        test_compare () {

        }
    };
    
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { std::min (a, b) } -> std::same_as<Z>;
        { std::max (a, b) } -> std::same_as<Z>;
    } struct test_min_max {
        test_min_max () {

        }
    };
    
    // TODO need xor and not zero.
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { a && b } -> std::same_as<Z>;
        { a || b } -> std::same_as<Z>;
    } && requires (const Z &a) {
        { !a } -> std::same_as<Z>;
    } struct test_logic {
        test_logic () {

        }
    };
    
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { a & b } -> std::same_as<Z>;
        { a | b } -> std::same_as<Z>;
    } && requires (const Z &a) {
        { ~a } -> std::same_as<Z>;
    } struct test_bit_logic {
        test_bit_logic () {

        }
    };
    
    template <typename Z> requires requires (const Z &z) {
        { increment (z) } -> std::same_as<Z>;
        { decrement (z) } -> std::same_as<Z>;
    } struct test_increment_and_decrement {
        test_increment_and_decrement () {
            // what does 0x81 increment to? 0x or 0x00 or 0x80?
        }
    };
    
    template <typename Z> requires requires (const Z &z) {
        { -z } -> std::same_as<Z>;
    } struct test_negate : virtual test_zero<Z> {
        test_negate () {
            // required that -0 => negative zero and -(negative zero) -> 0.
            EXPECT_TRUE (data::identical (-Z::zero (1, true), Z::zero (1, false)));
            EXPECT_TRUE (data::identical (-Z::zero (2, true), Z::zero (2, false)));
            EXPECT_TRUE (data::identical (-Z::zero (3, true), Z::zero (3, false)));

            EXPECT_TRUE (data::identical (-Z::zero (1, false), Z::zero (1, true)));
            EXPECT_TRUE (data::identical (-Z::zero (2, false), Z::zero (2, true)));
            EXPECT_TRUE (data::identical (-Z::zero (3, false), Z::zero (3, true)));

            // negative numbers should be the same size. (not minimal)
        }
    };
    
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { a + b } -> std::same_as<Z>;
        { a - b } -> std::same_as<Z>;
        { a * b } -> std::same_as<Z>;
        { a / b } -> std::same_as<Z>;
        { a % b } -> std::same_as<Z>;
    } struct test_arithmetic {
        test_arithmetic () {

        }
    };

    template <typename Z> requires requires (const Z &z) {
        { data::abs (z) } -> std::convertible_to<Z>;
    } struct test_abs : virtual test_zero<Z> {
        test_abs () {

            {
                // abs must convert negative zero to positive zero.
                EXPECT_TRUE (identical (data::abs (Z::zero (1, true)), Z::zero (1, false)));
                EXPECT_TRUE (identical (data::abs (Z::zero (2, true)), Z::zero (2, false)));
                EXPECT_TRUE (identical (data::abs (Z::zero (3, true)), Z::zero (3, false)));

            }
        }
    };

    template <typename Z>
    struct test_twos_complement :
        test_abs<Z>, test_arithmetic<Z>, test_negate<Z>,
        test_increment_and_decrement<Z>, //test_bit_logic<Z>,
        //test_logic<Z>, test_min_max<Z>,
        test_compare<Z>, test_equal<Z>, test_minimal<Z> {
        test_twos_complement () {

        }
    };
    
    TEST (TwosComplementTest, TestTwosComplement) {
        test_twos_complement<hex_int_twos> {};
        test_twos_complement<Z_bytes_twos_big> {};
        test_twos_complement<Z_bytes_twos_little> {};
    }
    
}

