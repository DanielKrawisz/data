// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include "gtest/gtest.h"

namespace data {
    
    template <typename list> void sort_test_case(list given, list expected) {
        EXPECT_TRUE(sorted(expected));
        EXPECT_EQ(sorted(given), given == expected);
        EXPECT_EQ(sort(given), expected);
    }
    
    template <typename list> requires ordered<int> && sequence<list> && ordered<element_of<list>> void sort_test() {
        sort_test_case(list{4, 5, 1, 3, 2}, list{1, 2, 3, 4, 5});
        sort_test_case(list{4, 3, 5, 1, 3, 2}, list{1, 2, 3, 3, 4, 5});
    }
    
    TEST(LinkedStackTest, TestLinkedStackSort) {
        sort_test<stack<int>>();
        sort_test<list<int>>();
        //sort_test<cross<int>>();
    }
}
