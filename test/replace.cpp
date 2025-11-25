// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/replace.hpp"
#include "data/string.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (Replace, Cross) {
        EXPECT_EQ ((replace (cross<int> {}, {})), (cross<int> {}));
        EXPECT_EQ ((replace (cross<int> {1}, {})), (cross<int> {1}));
        EXPECT_EQ ((replace (cross<int> {1}, {{1, 2}})), (cross<int> {2}));
        EXPECT_EQ ((replace (cross<int> {1, 3}, {{1, 2}})), (cross<int> {2, 3}));
    }

    TEST (Replace, Array) {
        EXPECT_EQ ((replace (array<int, 0> {}, {})), (array<int, 0> {}));
        EXPECT_EQ ((replace (array<int, 1> {1}, {})), (array<int, 1> {1}));
        EXPECT_EQ ((replace (array<int, 1> {1}, {{1, 2}})), (array<int, 1> {2}));
        EXPECT_EQ ((replace (array<int, 2> {1, 3}, {{1, 2}})), (array<int, 2> {2, 3}));
    }

    TEST (Replace, Tree) {
        EXPECT_EQ ((replace (tree<int> {}, {})), (tree<int> {}));
        EXPECT_EQ ((replace (tree<int> {1}, {})), (tree<int> {1}));
        EXPECT_EQ ((replace (tree<int> {1}, {{1, 2}})), (tree<int> {2}));
    }

    TEST (Replace, Stack) {
        EXPECT_EQ ((replace (stack<int> {}, {})), (stack<int> {}));
        EXPECT_EQ ((replace (stack<int> {1}, {})), (stack<int> {1}));
        EXPECT_EQ ((replace (stack<int> {1}, {{1, 2}})), (stack<int> {2}));
        EXPECT_EQ ((replace (stack<int> {1, 3}, {{1, 2}})), (stack<int> {2, 3}));
    }

    TEST (Replace, List) {
        EXPECT_EQ ((replace (list<int> {}, {})), (list<int> {}));
        EXPECT_EQ ((replace (list<int> {1}, {})), (list<int> {1}));
        EXPECT_EQ ((replace (list<int> {1}, {{1, 2}})), (list<int> {2}));
        EXPECT_EQ ((replace (list<int> {1, 3}, {{1, 2}})), (list<int> {2, 3}));
    }

    TEST (Replace, Cycle) {
        EXPECT_EQ ((replace (cycle<int> {}, {})), (cycle<int> {}));
        EXPECT_EQ ((replace (cycle<int> {1}, {})), (cycle<int> {1}));
        EXPECT_EQ ((replace (cycle<int> {1}, {{1, 2}})), (cycle<int> {2}));
        EXPECT_EQ ((replace (cycle<int> {1, 3}, {{1, 2}})), (cycle<int> {2, 3}));
    }
/*
    TEST (Replace, Map) {
        EXPECT_EQ ((replace (map<string, int> {}, {})), (map<string, int> {}));
        EXPECT_EQ ((replace (map<string, int> {{"A", 1}}, {})), (map<string, int> {{"A", 1}}));
        EXPECT_EQ ((replace (map<string, int> {{"A", 1}}, {{1, 2}})), (map<string, int> {{"A", 2}}));
    }*/

}
