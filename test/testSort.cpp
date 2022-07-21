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
    
    template <typename list> void sort_test() {
        sort_test_case(list{4, 5, 1, 3, 2}, list{1, 2, 3, 4, 5});
        sort_test_case(list{4, 3, 5, 1, 3, 2}, list{1, 2, 3, 3, 4, 5});
    }
    
    TEST(SortTest, TestSort) {
        sort_test<stack<int>>();
        sort_test<list<int>>();
        sort_test<cross<int>>();
    }
    
    TEST(SortTest, TestOrderedStack) {
        ordered_list<int> oA{4, 5, 1, 3, 2};
        std::cout << "adding OB" << std::endl;
        ordered_list<int> oB{4, 3, 5, 1, 3, 2};
        ordered_list<int> oC{33, 24};
        ordered_list<int> oD{3, 5, 4};
        
        stack<int> sA{1, 2, 3, 4, 5};
        stack<int> sB{1, 2, 3, 3, 4, 5};
        stack<int> sC{24, 33};
        stack<int> sD{3, 4, 5};
        
        EXPECT_EQ(oC, sC);
        EXPECT_EQ(oD, sD);
        
        EXPECT_EQ(oA, sA);
        EXPECT_EQ(oB, sB);
        
        EXPECT_NE(oA, sB);
        EXPECT_NE(oB, sA);
    }
    
}
