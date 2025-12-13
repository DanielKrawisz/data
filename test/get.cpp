// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/get.hpp"
#include "data/ordered_sequence.hpp"
#include "data/cross.hpp"
#include "data/array.hpp"
#include "data/list.hpp"
#include "data/stack.hpp"
#include "data/map.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (Get, Get) {
        EXPECT_EQ ((get (cross<int> {})), (cross<int> {}));
        EXPECT_EQ ((get (list<int> {})), (list<int> {}));
        EXPECT_EQ ((get (stack<int> {})), (stack<int> {}));
        EXPECT_EQ ((get (map<int, int> {})), (map<int, int> {}));
        EXPECT_EQ ((get (ordered_sequence<int> {})), (ordered_sequence<int> {}));
        EXPECT_EQ ((get (array<int, 3> {1, 2, 3})), (array<int, 3> {1, 2, 3}));

        EXPECT_EQ ((get (cross<int> {1, 2, 3, 4}, 2)), (3));
        EXPECT_EQ ((get (list<int> {1, 2, 3, 4}, 2)), (3));
        EXPECT_EQ ((get (stack<int> {1, 2, 3, 4}, 2)), (3));
        EXPECT_EQ ((get (map<int, int> {{2, 3}}, 2)), (3));
        EXPECT_EQ ((get (ordered_sequence<int> {1, 2, 3, 4}, 2)), (3));

        EXPECT_EQ ((get (cross<cross<int>> {{1}, {2}, {3}, {4}}, 2)), (cross<int> {3}));
        EXPECT_EQ ((get (list<list<int>> {{1}, {2}, {3}, {4}}, 2)), (list<int> {3}));
        EXPECT_EQ ((get (stack<stack<int>> {{1}, {2}, {3}, {4}}, 2)), (stack<int> {3}));

        EXPECT_EQ ((get (list<cross<int>> {{1}, {2}, {3, 4}, {5}}, 2, 1)), (4));
        EXPECT_EQ ((get (stack<list<int>> {{1}, {2}, {3, 2, 1}, {4}}, 2, 0)), (3));
        EXPECT_EQ ((get (map<int, stack<int>> {{1, {1}}, {2, {2, 3}}, {3, {3, 7}}, {4, {4}}}, 3, 1)), (7));
        EXPECT_EQ ((get (cross<map<int, int>> {{{1, 1}}, {{2, 2}}, {{3, 3}}, {{4, 4}}}, 2, 3)), (3));
    }

    static_assert (requires {
        array_info<array<int, 6>> {};
    });

    static_assert (requires {
        typename array_info<array<int, 6>>::dims;
    });

    TEST (Get, Assign) {

        cross<int> A {1, 2, 3, 4};
        list<int> B {1, 2, 3, 4};
        stack<int> C {1, 2, 3, 4};
        map<int, int> D {{2, 3}, {4, 5}};
        array<int, 6> E {1, 2, 3, 4, 5, 6};
        array<int, 3, 2> F {{1, 2}, {3, 4}, {5, 6}};

        get (A, 1) = 5;
        get (B, 2) = 8;
        get (C, 3) = 13;
        get (D, 4) = 21;
        get (E, 5) = 77;
        get (F, 1, 1) = 23;

    }

    TEST (Get, Array) {
        EXPECT_EQ ((get (array<int, 3> {1, 2, 3}, 2)), (3));
        EXPECT_EQ ((get (array<int, 3, 2> {{1, 2}, {3, 4}, {5, 6}}, 2, 1)), (6));
        EXPECT_THROW ((get (array<int, 3, 2, 1> {{{1}, {2}}, {{3}, {4}}, {{5}, {6}}}, 1, 2, 0)), exception);
        EXPECT_EQ ((get (array<int, 3, 2, 1> {{{1}, {2}}, {{3}, {4}}, {{5}, {6}}}, 2, 1, 0)), (6));
    }
}
