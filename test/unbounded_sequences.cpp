// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>

#include "gtest/gtest.h"

namespace data {

    using twos_complement = ::testing::Types<hex_int, Z_bytes_little, Z_bytes_big,
        math::number::Z_bytes<endian::little, negativity::twos, unsigned short>,
        math::number::Z_bytes<endian::big, negativity::twos, unsigned short>/*,
        math::number::Z_bytes<endian::little, negativity::twos, unsigned>,
        math::number::Z_bytes<endian::big, negativity::twos, unsigned>,
        math::number::Z_bytes<endian::little, negativity::twos, unsigned long>,
        math::number::Z_bytes<endian::big, negativity::twos, unsigned long>,
        math::number::Z_bytes<endian::little, negativity::twos, unsigned long long>,
        math::number::Z_bytes<endian::big, negativity::twos, unsigned long long>*/>;

    using sign_and_magnitude = ::testing::Types<hex_int_BC, Z_bytes_BC_little, Z_bytes_BC_big,
        math::number::Z_bytes<endian::little, negativity::BC, unsigned short>,
        math::number::Z_bytes<endian::big, negativity::BC, unsigned short>/*,
        math::number::Z_bytes<endian::little, negativity::BC, unsigned>,
        math::number::Z_bytes<endian::big, negativity::BC, unsigned>,
        math::number::Z_bytes<endian::little, negativity::BC, unsigned long>,
        math::number::Z_bytes<endian::big, negativity::BC, unsigned long>,
        math::number::Z_bytes<endian::little, negativity::BC, unsigned long long>,
        math::number::Z_bytes<endian::big, negativity::BC, unsigned long long>*/>;


    template <typename X> struct TwosComplement : ::testing::Test {
        using N = X;
    };

    template <typename X> struct SignMagnitude : ::testing::Test {
        using N = X;
    };

    TYPED_TEST_SUITE (TwosComplement, twos_complement);

    TYPED_TEST_SUITE (SignMagnitude, sign_and_magnitude);

    template <typename N> void test_plus (const N &left, const N &right, const N &expected) {
        N result = plus (left, right);
        EXPECT_EQ (result, expected) << "expected " << left << " + " << right << " -> " << expected << " but got " << result;
    }

    template <typename N> void test_times (const N &left, const N &right, const N &expected) {
        N result = times (left, right);
        EXPECT_EQ (result, expected) << "expected " << left << " * " << right << " -> " << expected << " but got " << result;
    }

    template <typename N> void test_mul_2_pow (const N &base, const uint32 exp2, const N &expected) {
        N result = mul_2_pow (base, exp2);
        EXPECT_EQ (result, expected) << "expected " << base << " * 2 ^ " << exp2 << " -> " << expected << " but got " << result;
    }

    template <typename N> void test_bit_shift_left (const N &base, const uint32 shift, const N &expected) {
        N result = bit_shift_left (base, shift);
        EXPECT_EQ (result, expected) << "expected " << base << " << " << shift << " -> " << expected << " but got " << result;
    }

    TYPED_TEST (TwosComplement, PlusSignBit) {
        using N = typename TestFixture::N;

        test_plus (
            N (hex_int {"0x7fffffffffffffff"}),
            N (hex_int {"0x0000000000000001"}),
            N (hex_int {"0x00000000000000008000000000000000"}));

        test_plus (
            N (hex_int {"0x8000000000000000"}),
            N (hex_int {"0xffffffffffffffff"}),
            N (hex_int {"0xffffffffffffffff7fffffffffffffff"}));
    }

    TYPED_TEST (TwosComplement, TimesSignBit) {
        using N = typename TestFixture::N;

        test_times (
            N (hex_int {"0x7fffffffffffffff"}),
            N (hex_int {"0x0000000000000002"}),
            N (hex_int {"0x0000000000000000fffffffffffffffe"}));

        test_times (
            N (hex_int {"0x8000000000000000"}),
            N (hex_int {"0xffffffffffffffff"}),
            N (hex_int {"0x00000000000000008000000000000000"}));
    }

    TYPED_TEST (TwosComplement, Mul2PowSignBit) {
        using N = typename TestFixture::N;

        test_mul_2_pow (
            N (hex_int {"0x4000000000000000"}),
            1u,
            N (hex_int {"0x00000000000000008000000000000000"}));

        test_mul_2_pow (
            N (hex_int {"0x8000000000000000"}),
            1u,
            N (hex_int {"0xffffffffffffffff0000000000000000"}));

        test_mul_2_pow (
            N (hex_int {"0x4000000000000000"}),
            1u,
            N (hex_int {"0x00000000000000008000000000000000"}));
    }

    TYPED_TEST (TwosComplement, BitShiftLeftSignBit) {
        using N = typename TestFixture::N;

        test_bit_shift_left (
            N (hex_int {"0x4000000000000000"}),
            1u,
            N (hex_int {"0x00000000000000008000000000000000"}));

        test_bit_shift_left (
            N (hex_int {"0x8000000000000000"}),
            1u,
            N (hex_int {"0xffffffffffffffff0000000000000000"}));
    }

    TYPED_TEST (SignMagnitude, PlusSignBit) {
        using N = typename TestFixture::N;

        test_plus (
            N (hex_int_BC {"0x7fffffffffffffff"}),
            N (hex_int_BC {"0x0000000000000001"}),
            N (hex_int_BC {"0x00000000000000008000000000000000"}));
/*
        test_plus (
            N (dec_int {"-9223372036854775808"}),
            N (dec_int {"-1"}),
            N (dec_int {"-9223372036854775809"}));*/
    }

    TYPED_TEST (SignMagnitude, TimesSignBit) {
        using N = typename TestFixture::N;

        test_times (
            N (hex_int_BC {"0x7fffffffffffffff"}),
            N (hex_int_BC {"0x0000000000000002"}),
            N (hex_int_BC {"0x0000000000000000fffffffffffffffe"}));
/*
        test_times (
            N (dec_int {"-9223372036854775808"}),
            N (dec_int {"-1"}),
            N (hex_int_BC {"0x8000000000000000"}));*/
    }

    TYPED_TEST (SignMagnitude, Mul2PowSignBit) {
        using N = typename TestFixture::N;

        test_mul_2_pow (
            N (hex_int_BC {"0x4000000000000000"}),
            1u,
            N (hex_int_BC {"0x00000000000000008000000000000000"}));
/*
        test_mul_2_pow (
            N (dec_int {"-9223372036854775808"}),
            1u,
            N (dec_int {"-18446744073709551616"}));*/
    }

    TYPED_TEST (SignMagnitude, BitShiftLeftSignBit) {
        using N = typename TestFixture::N;

        test_bit_shift_left (
            N (hex_int_BC {"0x4000000000000000"}),
            1u,
            N (hex_int_BC {"0x00000000000000008000000000000000"}));
/*
        test_bit_shift_left (
            N (dec_int {"-9223372036854775808"}),
            1u,
            N (dec_int {"-18446744073709551616"}));*/
    }
}
