// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/concepts.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"
namespace data::math {
    invert_mod<uint64, uint64> pm {};
}

namespace data {
    
    template <integral NN> requires requires (const NN &a) {
        { sqrt (a) } -> implicitly_convertible_to<set<NN>>;
        { square (a) } -> implicitly_convertible_to<NN>;
        { quadrance (a) } -> implicitly_convertible_to<NN>;
    } && requires (const NN &a, const NN &b) {
        { plus (a, b) } -> implicitly_convertible_to<NN>;
        { minus (a, b) } -> implicitly_convertible_to<NN>;
        { times (a, b) } -> implicitly_convertible_to<NN>;
        { pow (a, b) } -> implicitly_convertible_to<NN>;
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
    implicitly_convertible_to<uint64, NN> &&
    implicitly_convertible_to<uint32, NN> && requires (const NN &n) {
        { uint64 (n) };
        { N (n) };
        { data::abs (n) } -> std::same_as<NN>;
    } && requires (const NN &a, const NN &b) {
        { a % b } -> implicitly_convertible_to<NN>;
    } && requires (const NN &a, const math::nonzero<NN> &b) {
        { divide (a, b) } -> std::same_as<division<NN>>;
        { mod (a, b) } -> implicitly_convertible_to<NN>;
        { negate_mod (a, b) } -> implicitly_convertible_to<NN>;
        { mul_2_mod (a, b) } -> implicitly_convertible_to<NN>;
        { square_mod (a, b) } -> implicitly_convertible_to<NN>;
    } && requires (const NN &a, const NN &b, const math::nonzero<NN> &c) {
        { plus_mod (a, b, c) } -> implicitly_convertible_to<NN>;
        { minus_mod (a, b, c) } -> implicitly_convertible_to<NN>;
        { times_mod (a, b, c) } -> implicitly_convertible_to<NN>;
        { pow_mod (a, b, c) } -> implicitly_convertible_to<NN>;
    } struct test_unsigned_number : test_whole_number<NN> {
        test_unsigned_number (string type = "") {
            EXPECT_EQ (N (NN (0)), N (0)) << " number: " << NN (0) << " vs " << N (0) << " merp " << N (NN (0));

            EXPECT_EQ (math::number::decrement<NN> {} (NN {0u}), NN {0u});
        }
    };
    
    template <signed_integral ZZ> requires std::convertible_to<int64, ZZ> &&
    requires (const ZZ &z) {
        { int64 (z) };
        { Z (z) };
    } struct test_signed_number : test_whole_number<ZZ> {
        test_signed_number () {

            ZZ zz_zero {0};
            EXPECT_EQ (Z (zz_zero), Z (0));

            EXPECT_EQ (math::number::decrement<ZZ> {} (zz_zero), ZZ {-1});

            EXPECT_EQ (math::number::increment<ZZ> {} (zz_zero), ZZ {1});
            EXPECT_EQ (math::number::increment<ZZ> {} (ZZ {-1}), zz_zero);

            EXPECT_EQ (math::number::increment<ZZ> {} (ZZ {0xffu}), ZZ {0x100u});

            EXPECT_EQ (-zz_zero, zz_zero);
            EXPECT_EQ (-ZZ {1}, ZZ {-1});
            EXPECT_EQ (-ZZ {-1}, ZZ {1});
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
        { b % a } -> implicitly_convertible_to<NN>;
    } && requires (const NN &a, const math::nonzero<NN> &b) {
        { mul_2_mod (a, b) } -> implicitly_convertible_to<NN>;
        { square_mod (a, b) } -> implicitly_convertible_to<NN>;
        { invert_mod (a, b) } -> implicitly_convertible_to<maybe<NN>>;
    } && requires (const ZZ &a, const math::nonzero<NN> &b) {
        { data::divide (a, b) } -> std::same_as<division<ZZ, NN>>;
        { mod (a, b) } -> implicitly_convertible_to<NN>;
        { negate_mod (a, b) } -> implicitly_convertible_to<NN>;
        { invert_mod (a, b) } -> implicitly_convertible_to<maybe<NN>>;
        { mul_2_mod (a, b) } -> implicitly_convertible_to<NN>;
        { square_mod (a, b) } -> implicitly_convertible_to<NN>;
    } && requires (const ZZ &a, const ZZ &b, const math::nonzero<NN> &c) {
        { plus_mod (a, b, c) } -> implicitly_convertible_to<NN>;
        { minus_mod (a, b, c) } -> implicitly_convertible_to<NN>;
        { times_mod (a, b, c) } -> implicitly_convertible_to<NN>;
        { pow_mod (a, b, c) } -> implicitly_convertible_to<NN>;
    } struct test_number_system : test_unsigned_number<NN>, test_signed_number<ZZ> {
        test_number_system (string type = ""): test_unsigned_number<NN> {type}, test_signed_number<ZZ> {} {}
    };

    template <typename ZZ>
    struct test_signed_number_system : test_signed_number<ZZ> {};
    
    TEST (NumbersTest, TestNumberSystem) {

        test_number_system<N, Z> {"N"};

        test_number_system<N_bytes_little, Z_bytes_little> {"N_bytes_little"};
        test_number_system<N_bytes_big, Z_bytes_big> {"N_bytes_big"};

        test_unsigned_number<base58_uint> {};

        test_number_system<dec_uint, dec_int> {};
        test_number_system<hex_uint, hex_int> {};

        test_signed_number_system<hex_int_BC> {};

        test_signed_number_system<Z_bytes_BC_little> {};
        test_signed_number_system<Z_bytes_BC_big> {};

    }
    
}
