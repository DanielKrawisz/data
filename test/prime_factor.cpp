// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/number/factor.hpp"
#include "data/numbers.hpp"
#include "data/for_each.hpp"
#include "gtest/gtest.h"

namespace data::math::number {

    template <typename N> void test_prime_factor () {

        struct test_case {
            N Number;
            list<power<N, N>> Expected;

            static list<power<N, N>> read (factorization<N> f) {
                list<power<N, N>> p;
                for (const auto &x : f) p <<= power<N, N> {x.Base.Prime.Value, x.Exponent};
                return p;
            }

            bool test (eratosthenes<N> &e) const {
                auto factors = factorize (nonzero {Number}, e);
                return Expected == read (factors);
            }
        };

        list<test_case> test_cases {{1, {}}, {2, {{2, 1}}}, {4, {{2, 2}}}, {3, {{3, 1}}}, {5, {{5, 1}}},
            {6, {{2, 1}, {3, 1}}}, {7, {{7, 1}}}, {9, {{3, 2}}}, {273, {{3, 1}, {7, 1}, {13, 1}}}};

        eratosthenes<N> e;
        for (const test_case &tc : test_cases) {
            EXPECT_TRUE (tc.test (e));
        }

    }

    template <typename X>
    struct PrimeFactor : ::testing::Test {
        using number = X;
    };

    using test_cases = ::testing::Types<
        uint64, uint64_little, uint64_big,
        uint80, uint80_little, uint80_big,
        uint128, uint128_little, uint128_big,
        uint160, uint256, uint512,
        N, N_bytes_little, N_bytes_big,
        math::N_bytes<endian::little, unsigned short>,
        math::N_bytes<endian::little, unsigned int>,
        math::N_bytes<endian::little, unsigned long>,
        math::N_bytes<endian::little, unsigned long long>,
        dec_uint, hex_uint, base58_uint>;

    TYPED_TEST_SUITE (PrimeFactor, test_cases);

    TYPED_TEST (PrimeFactor, TestType) {
        test_prime_factor<typename TestFixture::number> ();
    }

}

