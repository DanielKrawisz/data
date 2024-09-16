// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {

    void accept_stack_of_string_views (list<string_view>) {}

    TEST (ListTest, TestListConvert) {
        list<string> test {"1", "2", "3", "4"};

        accept_stack_of_string_views (test);

        list<N> numbers {1, 2, 3, 4};

        EXPECT_EQ (list<N> (test), numbers);

    }

    TEST (ListTest, TestListRotate) {

        list<string> test {"1", "2", "3", "4"};
        list<string> rl {"2", "3", "4", "1"};
        list<string> rr {"4", "1", "2", "3"};

        EXPECT_EQ (rotate_right (test), rr);
        EXPECT_EQ (rotate_left (test), rl);

    }

    TEST (ListTest, TestListOrdering) {
        list<int> l0 {};
        list<int> l1 {1};
        list<int> l2 {1, 2};
        list<int> l3 {1, 2, 3};

        EXPECT_EQ (shuffle (l0, {}), l0);
        EXPECT_EQ (shuffle (l1, {0}), l1);
        EXPECT_EQ (shuffle (l2, {0, 1}), l2);
        EXPECT_EQ (shuffle (l2, {1, 0}), reverse (l2));
        EXPECT_EQ (shuffle (l3, {0, 1, 2}), l3);
        EXPECT_EQ (shuffle (l3, {2, 1, 0}), reverse (l3));
        EXPECT_EQ (shuffle (l3, {0, 2, 1}), (list<int> {1, 3, 2}));
        EXPECT_EQ (shuffle (l3, {1, 0, 2}), (list<int> {2, 1, 3}));
        EXPECT_EQ (shuffle (l3, {1, 2, 0}), (list<int> {3, 1, 2}));
        EXPECT_EQ (shuffle (l3, {2, 0, 1}), (list<int> {2, 3, 1}));

    }
}
