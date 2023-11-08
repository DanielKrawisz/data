// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    
    template <typename NN> requires requires (const NN &n) {
        {math::is_zero (n)} -> std::same_as<bool>;
        {math::is_positive (n)} -> std::same_as<bool>;
        {math::is_negative (n)} -> std::same_as<bool>;
        {sign (n)} -> std::same_as<math::sign>;
        {increment (n)} -> std::same_as<NN>;
        {decrement (n)} -> std::same_as<NN>;
        {sqrt (n)} -> std::same_as<set<NN>>;
    } && requires (NN &n) {
        {++n} -> std::same_as<NN &>;
        {n++} -> std::same_as<NN>;
        {--n} -> std::same_as<NN &>;
        {n++} -> std::same_as<NN>;
    } && requires (const NN &a, const NN &b) {
        {a == b} -> std::same_as<bool>;
        {a != b} -> std::same_as<bool>;
        {a > b} -> std::same_as<bool>;
        {a < b} -> std::same_as<bool>;
        {a <= b} -> std::same_as<bool>;
        {a >= b} -> std::same_as<bool>;
        {a + b} -> std::same_as<NN>;
        {a - b} -> std::same_as<NN>;
        {a * b} -> std::same_as<NN>;
        {a / b} -> std::same_as<NN>;
        {std::min (a, b)} -> std::same_as<const NN &>;
        {std::max (a, b)} -> std::same_as<const NN &>;
    } && requires (NN &a, const NN &b) {
        {a += b} -> std::same_as<NN &>;
        {a -= b} -> std::same_as<NN &>;
        {a *= b} -> std::same_as<NN &>;
    } struct test_whole_number {
        test_whole_number () {}
    };
    
    template <typename NN> requires std::convertible_to<uint64, NN> && 
    requires (const NN &n) {
        {uint64 (n)};
        {NN (n)};
        {n == 0u} -> std::same_as<bool>;
        {n != 0u} -> std::same_as<bool>;
        {n > 0u} -> std::same_as<bool>;
        {n < 0u} -> std::same_as<bool>;
        {n >= 0u} -> std::same_as<bool>;
        {n <= 0u} -> std::same_as<bool>;
    } && requires (const NN &a, const NN &b) {
        {pow (a, b)} -> std::same_as<NN>;
    } struct test_unsigned_number : test_whole_number<NN> {
        test_unsigned_number () {
            EXPECT_EQ (N (NN (0)), N (0)) << " number: " << NN (0) << " vs " << N (0) << " merp " << N (NN (0));

            EXPECT_EQ (decrement (NN {0u}), NN {0u});

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

        }
    };
    
    template <typename ZZ> requires std::convertible_to<int64, ZZ> && 
    requires (const ZZ &z) {
        {int64 (z)};
        {ZZ (z)};
        {z == 0};
        {z == 0};
        {z != 0};
        {z > 0};
        {z < 0};
        {z >= 0};
        {z <= 0};
    } struct test_signed_number : test_whole_number<ZZ> {
        test_signed_number () {
            EXPECT_EQ (Z (ZZ (0)), Z (0));

            EXPECT_EQ (decrement (ZZ {0}), ZZ {-1});

            EXPECT_EQ (increment (ZZ {0}), ZZ {1});
            EXPECT_EQ (increment (ZZ {-1}), ZZ {0});
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
        test_number_system () {}
    };
    
    TEST (NumbersTest, TestNumberSystem) {

        test_number_system<uint64, int64> {};/*
        test_number_system<uint64_little, int64_little> {};
        test_number_system<uint64_big, int64_big> {};

        test_number_system<uint_little<9>, int_little<9>> {};
        test_number_system<uint_big<9>, int_big<9>> {};
        test_number_system<uint_little<10>, int_little<10>> {};
        test_number_system<uint_big<10>, int_big<10>> {};
        test_number_system<uint_little<11>, int_little<11>> {};
        test_number_system<uint_big<11>, int_big<11>> {};
        test_number_system<uint_little<20>, int_little<20>> {};
        test_number_system<uint_big<20>, int_big<20>> {};*
/*
        test_number_system<N, Z> {};

        test_number_system<N_bytes_little, Z_bytes_little> {};
        test_number_system<N_bytes_big, Z_bytes_big> {};

        test_unsigned_number<base58_uint> {};

        test_number_system<dec_uint, dec_int> {};
        test_number_system<hex_uint, hex_int> {};*/
/*
        test_signed_number<hex_int_twos> {};

        test_signed_number<Z_bytes_twos_little> {};
        test_signed_number<Z_bytes_twos_big> {};*/

    }
    
}
