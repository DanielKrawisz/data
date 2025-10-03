// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/concepts.hpp>
#include <data/numbers.hpp>
#include <data/math.hpp>
#include "gtest/gtest.h"
namespace data::math {
    invert_mod<uint64, uint64> pm {};
}

namespace data {

    template <typename A, typename B> requires requires (const N &n, uint64 u) {
        {n == u};
        {n <=> u};
        {u == n};
        {u <=> n};/*
        {n + u};
        {n - u};
        {n * u};
        {n / u};
        {n % u};
        {u + n};
        {u - n};
        {u * n};
        {u / n};
        {u % n};
        {n += u};
        {n -= u};
        {n *= u};
        {n /= u};*/
    } struct test_number_conversion {};

    TEST (Numbers, NumberConversion) {
        test_number_conversion<N, Z> {};
        test_number_conversion<Z, N> {};
        test_number_conversion<N, uint64> {};
        test_number_conversion<N, int64> {};
        test_number_conversion<Z, uint64> {};
        test_number_conversion<Z, int64> {};
        test_number_conversion<N, N_bytes_little> {};
        test_number_conversion<N, Z_bytes_little> {};
        test_number_conversion<Z, N_bytes_little> {};
        test_number_conversion<Z, Z_bytes_little> {};
        test_number_conversion<N, uint256> {};
        test_number_conversion<N, int256> {};
        test_number_conversion<Z, uint256> {};
        test_number_conversion<Z, int256> {};
        test_number_conversion<N_bytes_little, Z_bytes_little> {};
        test_number_conversion<Z_bytes_little, N_bytes_little> {};
        test_number_conversion<N_bytes_little, uint64> {};
        test_number_conversion<N_bytes_little, int64> {};
        test_number_conversion<Z_bytes_little, uint64> {};
        test_number_conversion<Z_bytes_little, int64> {};
        test_number_conversion<N_bytes_little, uint256> {};
        test_number_conversion<N_bytes_little, int256> {};
        test_number_conversion<Z_bytes_little, uint256> {};
        test_number_conversion<Z_bytes_little, int256> {};
        test_number_conversion<uint256, int256> {};
        test_number_conversion<int256, uint256> {};
    }
    
    template <integral NN> requires requires (const NN &a) {
        { square (a) } -> ImplicitlyConvertible<NN>;
        { quadrance (a) } -> ImplicitlyConvertible<NN>;
    } && requires (const NN &a, const NN &b) {
        { plus (a, b) } -> ImplicitlyConvertible<NN>;
        { minus (a, b) } -> ImplicitlyConvertible<NN>;
        { times (a, b) } -> ImplicitlyConvertible<NN>;
        { pow (a, b) } -> ImplicitlyConvertible<NN>;
    } struct test_whole_number {
        test_whole_number () {

            EXPECT_EQ (increment (NN {0u}), NN {1u});

            EXPECT_FALSE (NN {0u} < NN {0u});
            EXPECT_TRUE (NN {0u} <= NN {0u});
            EXPECT_FALSE (NN {0u} > NN {0u});
            EXPECT_TRUE (NN {0u} >= NN {0u});

            EXPECT_FALSE (NN {1u} < NN {0u});
            EXPECT_FALSE (NN {1u} <= NN {0u});
            EXPECT_TRUE (NN {1u} > NN {0u});
            EXPECT_TRUE (NN {1u} >= NN {0u});

            EXPECT_TRUE (NN {394u} < NN {479u});
            EXPECT_TRUE (NN {394u} <= NN {479u});
            EXPECT_FALSE (NN {394u} > NN {479u});
            EXPECT_FALSE (NN {394u} >= NN {479u});

            EXPECT_TRUE (NN {0x394u} < NN {0x479u});
            EXPECT_TRUE (NN {0x394u} <= NN {0x479u});
            EXPECT_FALSE (NN {0x394u} > NN {0x479u});
            EXPECT_FALSE (NN {0x394u} >= NN {0x479u});

            EXPECT_EQ (NN {0x479u} - NN {0x394u}, NN {0xe5u});

            EXPECT_EQ (NN {0x858u} + NN {0x1edu}, NN {0xa45u});

            EXPECT_EQ (NN {0x64u} * NN {0xau}, NN {0x3e8u});

        }
    };

    
    template <unsigned_integral NN> requires
    ImplicitlyConvertible<uint64, NN> &&
    ImplicitlyConvertible<uint32, NN> && requires (const NN &n) {
        { uint64 (n) };
        { static_cast<N> (n) };
        { data::abs (n) } -> std::same_as<NN>;
    } && requires (const NN &a, const NN &b) {
        { a % b } -> ImplicitlyConvertible<NN>;
    } && requires (const NN &a, const math::nonzero<NN> &b) {
        { divide (a, b) } -> std::same_as<division<NN>>;
        { mod (a, b) } -> ImplicitlyConvertible<NN>;
        { negate_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { mul_2_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { square_mod (a, b) } -> ImplicitlyConvertible<NN>;
    } && requires (const NN &a, const NN &b, const math::nonzero<NN> &c) {
        { plus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { minus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { times_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { pow_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
    } struct test_unsigned_number : test_whole_number<NN> {
        test_unsigned_number (string type = "") {
            EXPECT_EQ (N (NN (0)), N (0)) << " number: " << NN (0) << " vs " << N (0) << " merp " << N (NN (0));

            EXPECT_EQ (math::number::decrement<NN> {} (NN {0u}), NN {0u});
        }
    };
    
    template <signed_integral ZZ> requires std::convertible_to<int64, ZZ> &&
    requires (const ZZ &z) {
        { int64 (z) };
        { static_cast<Z> (z) };
    } struct test_signed_number : test_whole_number<ZZ> {
        test_signed_number () {

            ZZ zz_zero {0};
            ZZ zz_minus_one {-1};

            // convert to Z
            EXPECT_EQ (Z (zz_zero), Z (0));
            EXPECT_EQ (Z (zz_minus_one), Z (-1));

            EXPECT_EQ (math::number::decrement<ZZ> {} (zz_zero), zz_minus_one);

            EXPECT_EQ (math::number::increment<ZZ> {} (zz_zero), ZZ {1});
            EXPECT_EQ (math::number::increment<ZZ> {} (zz_minus_one), zz_zero);

            EXPECT_EQ (math::number::increment<ZZ> {} (ZZ {0xffu}), ZZ {0x100u});

            EXPECT_EQ (-zz_zero, zz_zero);
            EXPECT_EQ (-ZZ {1}, zz_minus_one);
            EXPECT_EQ (-zz_minus_one, ZZ {1});
        }
    };

    template <typename NN, typename ZZ> 
    requires requires (const ZZ &z) {
        { data::abs (z) } -> std::same_as<NN>;
    } && requires (const ZZ &a, const math::nonzero<ZZ> &b) {
        { data::divide (a, b) } -> std::same_as<division<ZZ, NN>>;
    } && requires (const NN &a, const ZZ &b) {
        { pow (b, a) } -> std::same_as<ZZ>;
        { a + b };
        { a - b };
        { a * b };
        { b + a };
        { b - a };
        { b * a };
        { b % a } -> ImplicitlyConvertible<NN>;
    } && requires (const NN &a, const math::nonzero<NN> &b) {
        { mul_2_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { square_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<NN>>;
    } && requires (const ZZ &a, const math::nonzero<NN> &b) {
        { data::divide (a, b) } -> std::same_as<division<ZZ, NN>>;
        { mod (a, b) } -> ImplicitlyConvertible<NN>;
        { negate_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<NN>>;
        { mul_2_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { square_mod (a, b) } -> ImplicitlyConvertible<NN>;
    } && requires (const ZZ &a, const ZZ &b, const math::nonzero<NN> &c) {
        { plus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { minus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { times_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { pow_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
    } struct test_number_system : test_unsigned_number<NN>, test_signed_number<ZZ> {
        test_number_system (string type = ""): test_unsigned_number<NN> {type}, test_signed_number<ZZ> {} {}
    };

    template <typename ZZ>
    struct test_signed_number_system : test_signed_number<ZZ> {};
    
    TEST (Numbers, NumberSystem) {

        test_number_system<N, Z> {"N"};

        test_number_system<N_bytes_little, Z_bytes_little> {"N_bytes_little"};
        test_number_system<N_bytes_big, Z_bytes_big> {"N_bytes_big"};

        test_signed_number_system<Z_bytes_BC_little> {};
        test_signed_number_system<Z_bytes_BC_big> {};

        test_unsigned_number<base58_uint> {};

        test_number_system<dec_uint, dec_int> {};
        test_number_system<hex_uint, hex_int> {};

        test_signed_number_system<hex_int_BC> {};

    }
    
}
