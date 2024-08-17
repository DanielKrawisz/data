// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/combinatorics.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (CombinatoricsTest, TestPermutations) {

        permutations<int> case_0 {list<int> {}};

        EXPECT_EQ (case_0.size<size_t> (), 0);
        EXPECT_EQ (case_0.begin (), case_0.end ());
        EXPECT_EQ (static_cast<list<list<int>>> (case_0), list<list<int>> {});



    }
}

