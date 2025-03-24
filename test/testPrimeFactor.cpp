// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/number/factor.hpp"
#include "data/numbers.hpp"
#include "data/for_each.hpp"
#include "gtest/gtest.h"

namespace data::math::number {

    template <typename N> void prime_factor_test () {

        struct test_case {
            N Number;
            list<power<N, N>> Expected;

            static list<power<N, N>> read (factorization<N> f) {
                list<power<N, N>> p;
                for (const auto &x : f) p <<= power<N, N> {x.Base.Prime, x.Exponent};
                return p;
            }

            bool test (eratosthenes<N> &e) const {
                auto factors = factor (Number, e);
                return Expected == read (factors);
            }
        };

        list<test_case> test_cases {{0, {}}, {1, {}}, {2, {{2, 1}}}, {4, {{2, 2}}}, {3, {{3, 1}}}, {5, {{5, 1}}},
            {6, {{2, 1}, {3, 1}}}, {7, {{7, 1}}}, {9, {{3, 2}}}, {273, {{3, 1}, {7, 1}, {13, 1}}}};

        eratosthenes<N> e;
        for (const test_case &tc : test_cases) {
            EXPECT_TRUE (tc.test (e));
        }

    }

    TEST (PrimeFactorTest, TestPrimeFactor) {

        prime_factor_test<uint64> ();
        prime_factor_test<uint64_little> ();
        prime_factor_test<uint64_big> ();
        prime_factor_test<uint80> ();
        prime_factor_test<N> ();
        prime_factor_test<N_bytes_little> ();
        prime_factor_test<N_bytes_big> ();
        prime_factor_test<dec_uint> ();
        prime_factor_test<hex_uint> ();
        prime_factor_test<base58_uint> ();
        //prime_factor_test<CryptoPP::Integer> ();

    }

}

