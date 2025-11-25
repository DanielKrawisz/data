// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/lift.hpp"
#include "data/string.hpp"
#include "gtest/gtest.h"

namespace data {
    
    string f (uint32 x) {
        return std::to_string (x);
    }

    TEST (ForEach, ForEach) {

        bool lift_test_stack = lift (&f, stack<uint32> {1u, 2u, 3u, 4u}) == stack<string> {"1", "2", "3", "4"};
        bool lift_test_list = lift (&f, list<uint32> {2, 4, 6}) == stack<string> {"2", "4", "6"};
        //bool lift_test_priority_queue = lift (&f, priority_queue<uint32> {})
        bool lift_test_vector = lift (&f, cross<uint32> {3, 5, 7}) == cross<string> {"3", "5", "7"};
        //bool lift_test_set = lift (&f, set<uint32> {4, 5, 6, 999}) == list<string>{"4", "5", "6", "999"};

        map<int, uint32> test_map_from {{1, 1}, {2, 2}, {3, 3}};
        map<int, string> test_map_to {{1, "1"}, {2, "2"}, {3, "3"}};
        map<int, string> test_map_test = lift (&f, test_map_from);
        bool lift_test_map = test_map_test == test_map_to;
        bool lift_test_tree = lift (&f, tree<uint32> {55004}) == tree<string> {"55004"};
        
        EXPECT_TRUE (lift_test_stack);
        EXPECT_TRUE (lift_test_list);
        //EXPECT_TRUE (lift_test_priority_queue)
        EXPECT_TRUE (lift_test_vector);
        //EXPECT_TRUE (lift_test_set);
        EXPECT_TRUE (lift_test_map) << "expected " << test_map_from << " => " << test_map_to << " but got " << test_map_test;
        EXPECT_TRUE (lift_test_tree);

    }
}

