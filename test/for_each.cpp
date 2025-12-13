// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/for_each.hpp"
#include "data/ordered_sequence.hpp"
#include "data/list.hpp"
#include "data/array.hpp"
#include "data/tree.hpp"
#include "data/string.hpp"
#include "gtest/gtest.h"

namespace data {

    // TODO we need tests for for_each<depth> and for_each_by<depth>

    auto dinner = [] (int &i, const int &j) {
        i = -i * j;
    };

    TEST (ForEach, Cross) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        cross<int> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);

        total = 0;
        for_each (dinner, x, cross<int> {3, 5, -4});
        for_each (sum, x);

        EXPECT_EQ (total, -1);

        // TODO for_each_by
    }

    TEST (ForEach, Array) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        array<int, 3> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);

        total = 0;
        for_each (dinner, x, array<int, 3> {3, 5, -4});
        for_each (sum, x);

        EXPECT_EQ (total, -1);

        // multidimensional arrays.
        total = 0;
        for_each (sum, array<int, 3, 2> {{3, 2}, {5, 1}, {4, 6}});
        EXPECT_EQ (total, 21);

        // for_each_by
        total = 0;
        for_each_by ([&total] (size_t ind, const int &x) {
            total += ind * x;
        }, array<int, 3> {1, 2, 3});
        EXPECT_EQ (total, 8);

        total = 0;
        for_each_by ([&total] (array<size_t, 2> ind, const int &x) {
            total += (ind[0] + ind[1]) * x;
        }, array<int, 2, 2> {{1, 2}, {3, 4}});
        EXPECT_EQ (total, 13);
    }

    TEST (ForEach, Stack) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        stack<int> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);

        total = 0;
        for_each (dinner, x, stack<int> {3, 5, -4});
        for_each (sum, x);

        EXPECT_EQ (total, -1);
        // TODO for_each_by
    }

    TEST (ForEach, Set) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        set<int> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, List) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        list<int> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);
        // TODO need unconst and multiple inputs

        total = 0;
        for_each (dinner, x, list<int> {3, 5, -4});
        for_each (sum, x);

        EXPECT_EQ (total, -1);
        // TODO for_each_by
    }

    TEST (ForEach, Map) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        map<string, int> x {{"A", 1}, {"B", 2}, {"C", 3}};
        for_each (sum, x);

        EXPECT_EQ (total, 6);
        // TODO for_each_by
    }

    TEST (ForEach, OrderedSequence) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        ordered_sequence<int> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);

        total = 0;
        auto inner = [&total] (const int &i, const int &j) {
            total += i * j;
        };

        for_each (inner, x, ordered_sequence<int> {3, 5, -4});
        EXPECT_EQ (total, 17);
    }

    TEST (ForEach, Cycle) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        cycle<int> x {1, 2, 3};
        for_each (sum, x);

        EXPECT_EQ (total, 6);

        auto x2 = [] (int &i) {
            i *= 2;
        };

        for_each (x2, x);
        total = 0;
        for_each (sum, x);
        EXPECT_EQ (total, 12);
    }

    TEST (ForEach, Tree) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        tree<int> x {1, {2, {3}, {}}, {4, {5}, {6, {}, {7}}}};
        for_each (sum, x);

        EXPECT_EQ (total, 28);
    }

}

