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

    template <typename Z> requires requires (const Z &z) {
        { -z } -> std::same_as<Z>;
    } struct test_negate : virtual test_zero<Z> {
        test_negate () {

            // negate always converts to the minimal representation.
            auto zero0 = Z::zero (0, false);
            auto neg_zero0 = -zero0;

            EXPECT_TRUE (data::identical (neg_zero0, zero0)) << "expected " << neg_zero0 << " === " << zero0;

            EXPECT_TRUE (data::identical (-Z::zero (1, false), zero0));
            EXPECT_TRUE (data::identical (-Z::zero (2, false), zero0));
            EXPECT_TRUE (data::identical (-Z::zero (3, false), zero0));

            EXPECT_TRUE (data::identical (-Z::zero (1, true), zero0));
            EXPECT_TRUE (data::identical (-Z::zero (2, true), zero0));
            EXPECT_TRUE (data::identical (-Z::zero (3, true), zero0));

            EXPECT_TRUE (data::identical (-Z::read ("0x01"), Z::read ("0x81")));
            EXPECT_TRUE (data::identical (-Z::read ("0x81"), Z::read ("0x01")));
            EXPECT_TRUE (data::identical (-Z::read ("0x0001"), Z::read ("0x81")));
            EXPECT_TRUE (data::identical (-Z::read ("0x8001"), Z::read ("0x01")));
            EXPECT_TRUE (data::identical (-Z::read ("0x000001"), Z::read ("0x81")));
            EXPECT_TRUE (data::identical (-Z::read ("0x800001"), Z::read ("0x01")));
            EXPECT_TRUE (data::identical (-Z::read ("0x00000001"), Z::read ("0x81")));
            EXPECT_TRUE (data::identical (-Z::read ("0x80000001"), Z::read ("0x01")));

        }
    };

    template <typename Z> requires requires (const Z &z) {
        { data::abs (z) } -> std::convertible_to<Z>;
    } struct test_abs : virtual test_zero<Z> {
        test_abs () {

            // abs will not change the number if it is not negative.
            // otherwise, it will return the minimal representation.

            EXPECT_TRUE (identical (data::abs (Z::zero (1, true)), Z::zero (1, true)));
            EXPECT_TRUE (identical (data::abs (Z::zero (2, true)), Z::zero (2, true)));
            EXPECT_TRUE (identical (data::abs (Z::zero (3, true)), Z::zero (3, true)));

            EXPECT_TRUE (identical (data::abs (Z::read ("0x01")), Z::read ("0x01")));
            EXPECT_TRUE (identical (data::abs (Z::read ("0x0001")), Z::read ("0x0001")));
            EXPECT_TRUE (identical (data::abs (Z::read ("0x000001")), Z::read ("0x000001")));

            EXPECT_TRUE (data::identical (data::abs (Z::read ("0x81")), -Z::read ("0x81")));
            EXPECT_TRUE (data::identical (data::abs (Z::read ("0x8001")), -Z::read ("0x81")));
            EXPECT_TRUE (data::identical (data::abs (Z::read ("0x800001")), -Z::read ("0x81")));

        }
    };

    template <typename Z> requires requires (const Z &z) {
        { increment (z) } -> std::same_as<Z>;
        { decrement (z) } -> std::same_as<Z>;
    } struct test_increment_and_decrement {
        test_increment_and_decrement () {

            test_increment ("0x", "0x01");
            test_increment ("0x00", "0x01");
            test_increment ("0x0000", "0x01");
            test_increment ("0x01", "0x02");
            test_increment ("0x0001", "0x02");
            test_increment ("0x00ff", "0x0100");
            test_increment ("0x80", "0x01");
            test_increment ("0x8000", "0x01");
            test_increment ("0x81", "0x");
            test_increment ("0x82", "0x81");
            test_increment ("0x7f", "0x0080");
            test_increment ("0x8080", "0xff");
            test_increment ("0x800080", "0xff");
        }

        void test_increment (const std::string &from, const std::string &to) {
            Z given = Z::read (from);
            Z expected = Z::read (to);

            Z incremented = increment (given);
            Z decremented = decrement (expected);

            EXPECT_TRUE (is_minimal (incremented));
            EXPECT_TRUE (is_minimal (decremented));

            EXPECT_EQ (expected, incremented);
            EXPECT_EQ (given, decremented);
        }
    };
    
    // adequetly tested in hexidecimal tests
    template <typename Z> requires requires (const Z &a, const Z &b) {
        {a == b} -> std::same_as<bool>;
        {a != b} -> std::same_as<bool>;
        {a < b} -> std::same_as<bool>;
        {a > b} -> std::same_as<bool>;
        {a <= b} -> std::same_as<bool>;
        {a >= b} -> std::same_as<bool>;
    } struct test_compare {
        test_compare () {}
    };
    
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { std::min (a, b) } -> std::convertible_to<Z>;
        { std::max (a, b) } -> std::convertible_to<Z>;
    } struct test_min_max {
        test_min_max () {

        }
    };
    
    // anything other than zero is true. All types of zeros are false.
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { a && b } -> std::same_as<Z>;
        { a || b } -> std::same_as<Z>;
    } && requires (const Z &a) {
        { !a } -> std::same_as<Z>;
    } struct test_logic : virtual test_zero<Z> {
        test_logic () {

            EXPECT_FALSE (bool (Z::zero (1, false)));
            EXPECT_FALSE (bool (Z::zero (2, false)));
            EXPECT_FALSE (bool (Z::zero (3, false)));

            EXPECT_FALSE (bool (Z::zero (1, true)));
            EXPECT_FALSE (bool (Z::zero (2, true)));
            EXPECT_FALSE (bool (Z::zero (3, true)));

            EXPECT_TRUE (bool (Z::read ("0x01")));
            EXPECT_TRUE (bool (Z::read ("0x81")));
            EXPECT_TRUE (bool (Z::read ("0x0002")));
            EXPECT_TRUE (bool (Z::read ("0x8002")));

            EXPECT_EQ (!Z::read ("0x"), Z::read ("0x01"));
            EXPECT_EQ (!Z::read ("0x00"), Z::read ("0x01"));
            EXPECT_EQ (!Z::read ("0x80"), Z::read ("0x01"));
            EXPECT_EQ (!Z::read ("0x01"), Z::read ("0x"));
            EXPECT_EQ (!Z::read ("0x0009"), Z::read ("0x"));
            EXPECT_EQ (!Z::read ("0x80cc"), Z::read ("0x"));

        }
    };

    // adequetly tested elsewhere
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

    template <typename Z>
    struct test_twos_complement :
        test_minimal<Z>, test_negate<Z>, test_abs<Z>,
        test_increment_and_decrement<Z>,
        test_arithmetic<Z>, test_min_max<Z>,
        test_logic<Z>, test_compare<Z> {
        test_twos_complement () {}
    };
    
    TEST (TwosComplementTest, TestTwosComplement) {
        test_twos_complement<hex_int_twos> {};
        test_twos_complement<Z_bytes_twos_big> {};
        test_twos_complement<Z_bytes_twos_little> {};
    }
    
}

