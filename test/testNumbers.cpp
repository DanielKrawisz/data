// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    
    template <typename NN> requires requires (const NN &a) {
        { data::sqrt (a) } -> std::same_as<set<NN>>;
    } struct test_whole_number {
        test_whole_number () {}
    };
    
    template <typename NN> requires math::number::natural<NN> && std::convertible_to<uint64, NN> &&
    requires (const NN &n) {
        { uint64 (n) };
        { NN (n) };
        { n == 0u } -> std::same_as<bool>;
        { n != 0u } -> std::same_as<bool>;
        { n > 0u } -> std::same_as<bool>;
        { n < 0u } -> std::same_as<bool>;
        { n >= 0u } -> std::same_as<bool>;
        { n <= 0u } -> std::same_as<bool>;
    } && requires (const NN &a, const math::nonzero<NN> &b) {
        { data::divide (a, b) } -> std::same_as<math::division<NN>>;
    } && requires (const NN &a, const NN &b) {
        { pow (a, b) } -> std::same_as<NN>;
    } struct test_unsigned_number : test_whole_number<NN> {
        test_unsigned_number (string type = "") {
            EXPECT_EQ (N (NN (0)), N (0)) << " number: " << NN (0) << " vs " << N (0) << " merp " << N (NN (0));

            EXPECT_EQ (math::number::decrement<NN> {} (NN {0u}), NN {0u});

            auto i0 = math::number::increment<NN> {} (NN {0u});
            EXPECT_EQ (i0, NN {1u});

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
    
    template <typename ZZ> requires math::number::integer<ZZ> && std::convertible_to<int64, ZZ> &&
    requires (const ZZ &z) {
        { int64 (z) };
        { ZZ (z) };
        { z == 0 };
        { z == 0 };
        { z != 0 };
        { z > 0 };
        { z < 0 };
        { z >= 0 };
        { z <= 0 };
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
    
    // we test systems of numbers that are related by the abs function. 
    // types N and Z may be the same. 
    template <typename NN, typename ZZ> 
    requires requires (const NN &n) {
        {data::abs (n)} -> std::same_as<NN>;
        {data::quadrance (n)} -> std::same_as<NN>;
    } && requires (const ZZ &z) {
        {data::abs (z)} -> std::same_as<NN>;
        {data::quadrance (z)} -> std::same_as<NN>;
    } && requires (const ZZ &a, const math::nonzero<ZZ> &b) {
        { data::divide (a, b) } -> std::same_as<math::division<ZZ, NN>>;
    } && requires (const ZZ &a, const math::nonzero<NN> &b) {
        { data::divide (a, b) } -> std::same_as<math::division<ZZ, NN>>;
    } && requires (const NN &a, const ZZ &b) {
        {a == b} -> std::same_as<bool>;
        {a != b} -> std::same_as<bool>;
        {a > b} -> std::same_as<bool>;
        {a < b} -> std::same_as<bool>;
        {a <= b} -> std::same_as<bool>;
        {a >= b} -> std::same_as<bool>;
        {b == a} -> std::same_as<bool>;
        {b != a} -> std::same_as<bool>;
        {b > a} -> std::same_as<bool>;
        {b < a} -> std::same_as<bool>;
        {b <= a} -> std::same_as<bool>;
        {b >= a} -> std::same_as<bool>;
        {pow (b, a)} -> std::same_as<ZZ>;
        {a + b};
        {a - b};
        {a * b};
        {b + a};
        {b - a};
        {b * a};
        {b % a} -> std::convertible_to<NN>;
    } struct test_number_system : test_unsigned_number<NN>, test_signed_number<ZZ> {
        test_number_system (string type = ""): test_unsigned_number<NN> {type}, test_signed_number<ZZ> {} {}
    };
    
    TEST (NumbersTest, TestNumberSystem) {

        //test_number_system<uint64, int64> {};
        test_number_system<uint64_little, int64_little> {};
        test_number_system<uint64_big, int64_big> {};

        test_number_system<uint_little<9>, int_little<9>> {"uint_little<9>"};
        test_number_system<uint_big<9>, int_big<9>> {"uint_big<9>"};
        test_number_system<uint_little<10>, int_little<10>> {"uint_little<10>"};
        test_number_system<uint_big<10>, int_big<10>> {};
        test_number_system<uint_little<11>, int_little<11>> {};
        test_number_system<uint_big<11>, int_big<11>> {};
        test_number_system<uint_little<20>, int_little<20>> {};
        test_number_system<uint_big<20>, int_big<20>> {};

        test_number_system<N, Z> {"N"};

        test_number_system<N_bytes_little, Z_bytes_little> {"N_bytes_little"};
        test_number_system<N_bytes_big, Z_bytes_big> {"N_bytes_big"};

        test_unsigned_number<base58_uint> {};

        test_number_system<dec_uint, dec_int> {};
        test_number_system<hex_uint, hex_int> {};

        test_signed_number<hex_int_twos> {};

        test_signed_number<Z_bytes_twos_little> {};
        test_signed_number<Z_bytes_twos_big> {};

    }
    
}
