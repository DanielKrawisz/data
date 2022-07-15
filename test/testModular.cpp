// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/modular.hpp"
#include "gtest/gtest.h"

namespace data {
    
    // representations of numbers. 
    constexpr auto d2 = decimal{"2"};
    constexpr auto d3 = decimal{"3"};
    constexpr auto d4 = decimal{"4"};
    constexpr auto d5 = decimal{"5"};
    constexpr auto d6 = decimal{"6"};
    constexpr auto d7 = decimal{"7"};
    constexpr auto d8 = decimal{"8"};
    constexpr auto d9 = decimal{"9"};
    constexpr auto d10 = decimal{"10"};
    constexpr auto d11 = decimal{"11"};
    constexpr auto d12 = decimal{"12"};
    constexpr auto d13 = decimal{"13"};
    constexpr auto d14 = decimal{"14"};
    constexpr auto d15 = decimal{"15"};
    constexpr auto d16 = decimal{"16"};
    constexpr auto d17 = decimal{"17"};
    constexpr auto d18 = decimal{"18"};
    constexpr auto d19 = decimal{"19"};
    
    template <typename N> struct test_modular {
        test_modular() {
            math::number::modular<N, d3> m1{1};
            math::number::modular<N, d3> m2{2};
            EXPECT_EQ(m2 + m2, m1);
            EXPECT_EQ(m2 ^ N(5), m1);
        }
    };
    
    TEST(TestModular, TestModularArithmetic) {
        test_modular<uint64>{};
        test_modular<N>{};
        test_modular<N_bytes_little>{};
        test_modular<N_bytes_big>{};
        test_modular<dec_uint>{};
        test_modular<base58_uint>{};
    }
}
