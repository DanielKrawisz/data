// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include "gtest/gtest.h"

namespace data {

    TEST (OrderedListTest, TestOrderedList) {

        ordered_list<int> a {1};
        ordered_list<int> b {1, 2};
        ordered_list<int> c {1, 2, 3};
        ordered_list<int> d;

        EXPECT_NE (a, d);
        EXPECT_NE (b, d);
        EXPECT_NE (c, d);

        // we had a problem with this earlier.
        d = c;

        EXPECT_NE (a, d);
        EXPECT_NE (b, d);
        EXPECT_EQ (c, d);

        ordered_list<int> e {d};

        EXPECT_EQ (e, d);
    }

}

