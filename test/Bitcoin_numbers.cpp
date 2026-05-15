// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

// We test that number types work like Bitcoin numbers. 
namespace data {
    // NOTE: Bitcoin bit logic is not a numeric function
    // (in other words, it operates on byte strings rather
    // than on numbers). Nevertheless, we test it here.
    template <typename Z> requires requires (const Z &z) {
        { bit_not (z) } -> ImplicitlyConvertible<Z>;
    } && requires (const Z &a, const Z &b) {
        { bit_xor (a, b) } -> ImplicitlyConvertible<Z>;
        { bit_or (a, b) } -> ImplicitlyConvertible<Z>;
        { bit_and (a, b) } -> ImplicitlyConvertible<Z>;
    } struct test_bit_logic {
        test_bit_logic () {}
    };

    template <typename Z> requires requires (const Z &z) {
        { math::number::is_minimal (z) } -> Same<bool>;
    } struct test_minimal {
        test_minimal () {

            EXPECT_TRUE (math::number::is_minimal (Z {string {"0x"}}));
            EXPECT_FALSE (math::number::is_minimal (Z {string {"0x00"}}));
            EXPECT_TRUE (math::number::is_minimal (Z {string {"0x01"}}));
            EXPECT_FALSE (math::number::is_minimal (Z {string {"0x0001"}}));
            EXPECT_TRUE (math::number::is_minimal (Z {string {"0x81"}}));
            EXPECT_FALSE (math::number::is_minimal (Z {string {"0x8001"}}));
        }
    };
    
    template <typename Z> requires requires (const Z &z) {
        { is_zero (z) } -> Same<bool>;
        { is_positive_zero (z) } -> Same<bool>;
        { is_negative_zero (z) } -> Same<bool>;
    } && requires (const Z &a, const Z &b) {
        { identical (a, b) } -> Same<bool>;
    } && requires (size_t size, bool negative) {
        { Z::zero (size, negative) } -> Same<Z>;
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

    template <typename Z> requires requires (const uint64 u) {
        {Z {u}};
    } && requires (const uint32 u) {
        {Z {u}};
    } && requires (const uint16 u) {
        {Z {u}};
    } && requires (const byte u) {
        {Z {u}};
    } && requires (const int64 z) {
        {Z {z}};
    } && requires (const int32 z) {
        {Z {z}};
    } && requires (const int16 z) {
        {Z {z}};
    } && requires (const int8 z) {
        {Z {z}};
    } struct test_construct {
        test_construct () {
            EXPECT_EQ (Z (byte   (0)), Z::read ("0x"));
            EXPECT_EQ (Z (int8   (0)), Z::read ("0x"));
            EXPECT_EQ (Z (uint16 (0)), Z::read ("0x"));
            EXPECT_EQ (Z (int16  (0)), Z::read ("0x"));
            EXPECT_EQ (Z (uint32 (0)), Z::read ("0x"));
            EXPECT_EQ (Z (int32  (0)), Z::read ("0x"));
            EXPECT_EQ (Z (uint64 (0)), Z::read ("0x"));
            EXPECT_EQ (Z (int64  (0)), Z::read ("0x"));

            EXPECT_EQ (Z (byte   (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (int8   (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (uint16 (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (int16  (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (uint32 (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (int32  (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (uint64 (1)), Z::read ("0x01"));
            EXPECT_EQ (Z (int64  (1)), Z::read ("0x01"));

            EXPECT_EQ (Z (int8   (-1)), Z::read ("0x81"));
            EXPECT_EQ (Z (int16  (-1)), Z::read ("0x81"));
            EXPECT_EQ (Z (int32  (-1)), Z::read ("0x81"));
            EXPECT_EQ (Z (int64  (-1)), Z::read ("0x81"));

            EXPECT_EQ (Z (byte   (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (int8   (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (uint16 (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (int16  (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (uint32 (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (int32  (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (uint64 (127)), Z::read ("0x7f"));
            EXPECT_EQ (Z (int64  (127)), Z::read ("0x7f"));

            EXPECT_EQ (Z (int8   (-127)), Z::read ("0xff"));
            EXPECT_EQ (Z (int16  (-127)), Z::read ("0xff"));
            EXPECT_EQ (Z (int32  (-127)), Z::read ("0xff"));
            EXPECT_EQ (Z (int64  (-127)), Z::read ("0xff"));

            EXPECT_EQ (Z (byte   (128)), Z::read ("0x0080"));
            EXPECT_EQ (Z (uint16 (128)), Z::read ("0x0080"));
            EXPECT_EQ (Z (int16  (128)), Z::read ("0x0080"));
            EXPECT_EQ (Z (uint32 (128)), Z::read ("0x0080"));
            EXPECT_EQ (Z (int32  (128)), Z::read ("0x0080"));
            EXPECT_EQ (Z (uint64 (128)), Z::read ("0x0080"));
            EXPECT_EQ (Z (int64  (128)), Z::read ("0x0080"));

            EXPECT_EQ (Z (int8   (-128)), Z::read ("0x8080"));
            EXPECT_EQ (Z (int16  (-128)), Z::read ("0x8080"));
            EXPECT_EQ (Z (int32  (-128)), Z::read ("0x8080"));
            EXPECT_EQ (Z (int64  (-128)), Z::read ("0x8080"));

            EXPECT_EQ (Z (int16  (-130)), Z::read ("0x8082"));
            EXPECT_EQ (Z (int32  (-130)), Z::read ("0x8082"));
            EXPECT_EQ (Z (int64  (-130)), Z::read ("0x8082"));

            EXPECT_EQ (Z (byte   (255)), Z::read ("0x00ff"));
            EXPECT_EQ (Z (uint16 (255)), Z::read ("0x00ff"));
            EXPECT_EQ (Z (int16  (255)), Z::read ("0x00ff"));
            EXPECT_EQ (Z (uint32 (255)), Z::read ("0x00ff"));
            EXPECT_EQ (Z (int32  (255)), Z::read ("0x00ff"));
            EXPECT_EQ (Z (uint64 (255)), Z::read ("0x00ff"));
            EXPECT_EQ (Z (int64  (255)), Z::read ("0x00ff"));

            EXPECT_EQ (Z (int16  (-255)), Z::read ("0x80ff"));
            EXPECT_EQ (Z (int32  (-255)), Z::read ("0x80ff"));
            EXPECT_EQ (Z (int64  (-255)), Z::read ("0x80ff"));

            EXPECT_EQ (Z (uint16 (256)), Z::read ("0x0100"));
            EXPECT_EQ (Z (int16  (256)), Z::read ("0x0100"));
            EXPECT_EQ (Z (uint32 (256)), Z::read ("0x0100"));
            EXPECT_EQ (Z (int32  (256)), Z::read ("0x0100"));
            EXPECT_EQ (Z (uint64 (256)), Z::read ("0x0100"));
            EXPECT_EQ (Z (int64  (256)), Z::read ("0x0100"));

            EXPECT_EQ (Z (int16  (-256)), Z::read ("0x8100"));
            EXPECT_EQ (Z (int32  (-256)), Z::read ("0x8100"));
            EXPECT_EQ (Z (int64  (-256)), Z::read ("0x8100"));

            EXPECT_EQ (Z (uint16 (32767)), Z::read ("0x7fff"));
            EXPECT_EQ (Z (int16  (32767)), Z::read ("0x7fff"));
            EXPECT_EQ (Z (uint32 (32767)), Z::read ("0x7fff"));
            EXPECT_EQ (Z (int32  (32767)), Z::read ("0x7fff"));
            EXPECT_EQ (Z (uint64 (32767)), Z::read ("0x7fff"));
            EXPECT_EQ (Z (int64  (32767)), Z::read ("0x7fff"));

            EXPECT_EQ (Z (int16  (-32767)), Z::read ("0xffff"));
            EXPECT_EQ (Z (int32  (-32767)), Z::read ("0xffff"));
            EXPECT_EQ (Z (int64  (-32767)), Z::read ("0xffff"));

            EXPECT_EQ (Z (uint16 (32768)), Z::read ("0x008000"));
            EXPECT_EQ (Z (uint32 (32768)), Z::read ("0x008000"));
            EXPECT_EQ (Z (int32  (32768)), Z::read ("0x008000"));
            EXPECT_EQ (Z (uint64 (32768)), Z::read ("0x008000"));
            EXPECT_EQ (Z (int64  (32768)), Z::read ("0x008000"));

            EXPECT_EQ (Z (int16  (-32768)), Z::read ("0x808000"));
            EXPECT_EQ (Z (int32  (-32768)), Z::read ("0x808000"));
            EXPECT_EQ (Z (int64  (-32768)), Z::read ("0x808000"));

            EXPECT_EQ (Z (uint16 (65535)), Z::read ("0x00ffff"));
            EXPECT_EQ (Z (uint32 (65535)), Z::read ("0x00ffff"));
            EXPECT_EQ (Z (int32  (65535)), Z::read ("0x00ffff"));
            EXPECT_EQ (Z (uint64 (65535)), Z::read ("0x00ffff"));
            EXPECT_EQ (Z (int64  (65535)), Z::read ("0x00ffff"));

            EXPECT_EQ (Z (int32  (-65535)), Z::read ("0x80ffff"));
            EXPECT_EQ (Z (int64  (-65535)), Z::read ("0x80ffff"));

            EXPECT_EQ (Z (uint32 (65536)), Z::read ("0x010000"));
            EXPECT_EQ (Z (int32  (65536)), Z::read ("0x010000"));
            EXPECT_EQ (Z (uint64 (65536)), Z::read ("0x010000"));
            EXPECT_EQ (Z (int64  (65536)), Z::read ("0x010000"));

            EXPECT_EQ (Z (int32  (-65536)), Z::read ("0x810000"));
            EXPECT_EQ (Z (int64  (-65536)), Z::read ("0x810000"));

        }
    };

    template <typename Z> requires requires (const Z &z) {
        { -z } -> Same<Z>;
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
        { abs (z) } -> std::convertible_to<Z>;
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
        { increment (z) } -> Same<Z>;
        { decrement (z) } -> Same<Z>;
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

            EXPECT_TRUE (math::number::is_minimal (incremented));
            EXPECT_TRUE (math::number::is_minimal (decremented));

            EXPECT_EQ (expected, incremented);
            EXPECT_EQ (given, decremented);
        }
    };
    
    // adequetly tested in hexidecimal tests
    template <typename Z> requires requires (const Z &a, const Z &b) {
        {a == b} -> Same<bool>;
        {a != b} -> Same<bool>;
        {a < b} -> Same<bool>;
        {a > b} -> Same<bool>;
        {a <= b} -> Same<bool>;
        {a >= b} -> Same<bool>;
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

    template <typename Z> requires requires (const Z &z, uint32 u) {
        { z << u } -> std::convertible_to<Z>;
        { z >> u } -> std::convertible_to<Z>;
    } struct test_bit_shift {
        test_bit_shift () {
            EXPECT_EQ (Z::read ("0x") << 0, Z::read ("0x"));
            EXPECT_EQ (Z::read ("0x") >> 0, Z::read ("0x"));

            EXPECT_EQ (Z::read ("0x01") << 0, Z::read ("0x01"));
            EXPECT_EQ (Z::read ("0x01") >> 0, Z::read ("0x01"));

            EXPECT_EQ (Z::read ("0x81") << 0, Z::read ("0x81"));
            EXPECT_EQ (Z::read ("0x81") >> 0, Z::read ("0x81"));

            EXPECT_EQ (Z::read ("0x01") << 1, Z::read ("0x02"));
            EXPECT_EQ (Z::read ("0x01") >> 1, Z::read ("0x"));

            EXPECT_EQ (Z::read ("0x81") << 1, Z::read ("0x82"));
            EXPECT_EQ (Z::read ("0x81") >> 1, Z::read ("0x"));

            EXPECT_EQ (Z::read ("0x02") << 1, Z::read ("0x04"));
            EXPECT_EQ (Z::read ("0x02") >> 1, Z::read ("0x01"));

            EXPECT_EQ (Z::read ("0x82") << 1, Z::read ("0x84"));
            EXPECT_EQ (Z::read ("0x82") >> 1, Z::read ("0x81"));
        }
    };
    
    // anything other than zero is true. All types of zeros are false.
    template <typename Z> requires requires (const Z &a, const Z &b) {
        { a && b } -> Same<Z>;
        { a || b } -> Same<Z>;
    } && requires (const Z &a) {
        { !a } -> Same<Z>;
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
        { a + b } -> Same<Z>;
        { a - b } -> Same<Z>;
        { a * b } -> Same<Z>;
        { a / b } -> Same<Z>;
        { a % b } -> Same<Z>;
    } struct test_arithmetic {
        test_arithmetic () {

            EXPECT_EQ (Z::read ("0x") + Z::read ("0x"), Z::read ("0x"));
            EXPECT_EQ (Z::read ("0x") - Z::read ("0x"), Z::read ("0x"));
            EXPECT_EQ (Z::read ("0x") * Z::read ("0x"), Z::read ("0x"));

            EXPECT_EQ (Z::read ("0x") * Z::read ("0x01"), Z::read ("0x"));
            EXPECT_EQ (Z::read ("0x") / Z::read ("0x01"), Z::read ("0x"));
        }
    };

    template <typename Z>
    struct test_BC :
        test_bit_logic<Z>,
        test_minimal<Z>,
        test_construct<Z>,
        test_negate<Z>,
        test_abs<Z>,
        test_increment_and_decrement<Z>,
        test_arithmetic<Z>,
        test_min_max<Z>,
        test_logic<Z>,
        test_compare<Z>,
        test_bit_shift<Z> {
        test_BC () {}
    };
    
    // TODO make typed test.
    TEST (BitcoinNumbers, BitcoinNumbers) {
        test_BC<hex_int_BC> {};
        test_BC<Z_bytes_BC_big> {};
        test_BC<Z_bytes_BC_little> {};
    }
    
}

