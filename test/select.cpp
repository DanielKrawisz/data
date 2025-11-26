// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/string.hpp"
#include "data/ordered_sequence.hpp"
#include "data/cycle.hpp"
#include "data/priority_queue.hpp"
#include "data/map.hpp"
#include "data/set.hpp"
#include "data/list.hpp"
#include "data/select.hpp"
#include "gtest/gtest.h"

namespace data {

    auto nonnegative = [](int x) -> bool { return x > 0; };

    TEST (Select, Stack) {
        EXPECT_EQ ((select (stack<int> {}, nonnegative)), (stack<int> {}));
        EXPECT_EQ ((select (stack<int> {-1}, nonnegative)), (stack<int> {}));
        EXPECT_EQ ((select (stack<int> {1}, nonnegative)), (stack<int> {1}));
    }

    TEST (Select, List) {
        EXPECT_EQ ((select (list<int> {}, nonnegative)), (list<int> {}));
        EXPECT_EQ ((select (list<int> {-1}, nonnegative)), (list<int> {}));
        EXPECT_EQ ((select (list<int> {1}, nonnegative)), (list<int> {1}));
    }

    TEST (Select, OrderedSequence) {
        EXPECT_EQ ((select (ordered_sequence<int> {}, nonnegative)), (ordered_sequence<int> {}));
    }

    TEST (Select, Map) {
        EXPECT_EQ ((select (map<string, int> {}, nonnegative)), (map<string, int> {}));
    }
/*
    TEST (Select, Set) {
        EXPECT_EQ ((select (set<int> {}, nonnegative)), (set<int> {}));
    }

    TEST (Select, Cross) {
        EXPECT_EQ ((select (cross<int> {}, nonnegative)), (cross<int> {}));
    }*/

}

