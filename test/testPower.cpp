// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/power.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename N> void test_power_case (string_view base, string_view exp, string_view expected) {
        EXPECT_EQ ((pow (N {base}, N {exp})), N {expected});
    }

    template <typename N> void test_power () {
        test_power_case<N> ("5", "20", "95367431640625");
        test_power_case<N> ("2", "100", "1267650600228229401496703205376");
    }

    TEST (TestPower, TestPower) {

        test_power<N> ();
        test_power<N_bytes_little> ();
        test_power<N_bytes_big> ();
        test_power<dec_uint> ();
        test_power<base58_uint> ();

    }

}

