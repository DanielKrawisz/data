
// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename Q> void test_divide_by_zero () {}
    template <typename Q> void test_lowest_terms () {}
    template <typename Q> void test_compare () {}
    template <typename Q> void test_arithmetic () {}

    template <typename Z> void test_fraction () {
        using Q = math::fraction<Z>;



        test_divide_by_zero<Q> ();
        test_lowest_terms<Q> ();
        test_compare<Q> ();
        test_arithmetic<Q> ();
    }
}
