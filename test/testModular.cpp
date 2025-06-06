// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/modular.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename X> void test_modular () {
        math::number::modular<plus (times (X {2}, X {3}), minus (X {7}, X {2}))> test_mod_A {3};
        math::number::modular<GCD (X {21}, X {14})> test_mod_B {3};
    }

    TEST (TestModular, TestModularTypes) {

        test_modular<byte> ();
        test_modular<uint16> ();
        test_modular<uint32> ();
        test_modular<uint64> ();

        test_modular<int8> ();
        test_modular<int16> ();
        test_modular<int32> ();
        test_modular<int64> ();

        test_modular<uint8_little> ();
        test_modular<uint16_little> ();
        test_modular<uint32_little> ();
        test_modular<uint64_little> ();

        test_modular<int8_little> ();
        test_modular<int16_little> ();
        test_modular<int32_little> ();
        test_modular<int64_little> ();

        test_modular<uint8_big> ();
        test_modular<uint16_big> ();
        test_modular<uint32_big> ();
        test_modular<uint64_big> ();

        test_modular<int8_big> ();
        test_modular<int16_big> ();
        test_modular<int32_big> ();
        test_modular<int64_big> ();
    }

}
