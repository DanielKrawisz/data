// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/for_each.hpp"
#include "gtest/gtest.h"

namespace data {
    
    string f(uint32 x) {
        return std::to_string(x);
    }

    TEST(ForEachTest, TestForEach) {
        bool for_each_test_stack = for_each(&f, stack<uint32>{1u, 2u, 3u, 4u}) == stack<string>{"1", "2", "3", "4"};
        bool for_each_test_list = for_each(&f, list<uint32>{2, 4, 6}) == stack<string>{"2", "4", "6"};
        //bool for_each_test_priority_queue = for_each(&f, priority_queue<uint32>{})
        bool for_each_test_vector = for_each(&f, cross<uint32>{3, 5, 7}) == cross<string>{"3", "5", "7"};
        //bool for_each_test_set = for_each(&f, set<uint32>{4, 5, 6, 999}) == list<string>{"4", "5", "6", "999"};
        bool for_each_test_map = for_each(&f, map<int, uint32>{{1, 1}, {2, 2}, {3, 3}}) == map<int, string>{{1, "1"}, {2, "2"}, {3, "3"}};
        bool for_each_test_tree = for_each(&f, tree<uint32>{55004}) == tree<string>{"55004"};
        
        EXPECT_TRUE(for_each_test_stack);
        EXPECT_TRUE(for_each_test_list);
        //EXPECT_TRUE(for_each_test_priority_queue)
        EXPECT_TRUE(for_each_test_vector);
        //EXPECT_TRUE(for_each_test_set);
        EXPECT_TRUE(for_each_test_map);
        EXPECT_TRUE(for_each_test_tree);
    }
}

