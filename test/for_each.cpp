// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/for_each.hpp"
#include "data/ordered_sequence.hpp"
#include "data/list.hpp"
#include "data/array.hpp"
#include "data/tree.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (ForEach, Cross) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, cross<int> {1, 2, 3});

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, Array) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, array<int, 3> {1, 2, 3});

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, Stack) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, stack<int> {1, 2, 3});

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, List) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, list<int> {1, 2, 3});

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, OrderedSequence) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, ordered_sequence<int> {1, 2, 3});

        EXPECT_EQ (total, 6);
    }
/*
    TEST (ForEach, Tree) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, tree<int> {1, {2, {3}}, {4, {5}, {6, 7}});

        EXPECT_EQ (total, 28);
    }

    TEST (ForEach, PriorityQueue) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, priority_queue<int> {1, 2, 3}, sum);

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, Set) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, set<int> {1, 2, 3}, sum);

        EXPECT_EQ (total, 6);
    }

    TEST (ForEach, Map) {
        int total = 0;
        auto sum = [&total] (const int &i) {
            total += i;
        };

        for_each (sum, map<string, int> {{"A", 1}, {"B", 2}, {"C", 3}}, sum);

        EXPECT_EQ (total, 6);
    }*/

}

