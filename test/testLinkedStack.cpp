// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST(LinkedStackTest, TestLinkedStack1) {
        
        EXPECT_TRUE(stack<int>{} == stack<int>::make());
        
        EXPECT_TRUE(stack<int>::make(1) == stack<int>::make(1));
        EXPECT_FALSE(stack<int>::make(1) == stack<int>{});
        EXPECT_FALSE(stack<int>::make(1) == stack<int>::make(0));
        
        EXPECT_TRUE(stack<int>::make(1).first() == 1);
        EXPECT_TRUE(stack<int>::make(1).rest() == stack<int>{});
        EXPECT_TRUE(stack<int>::make(1).size() == 1);
        
    }

    TEST(LinkedStackTest, TestLinkedStack2) {
        
        auto L1 = stack<int>::make(1, 2, 3);
        auto L2 = stack<int>::make(3, 2, 1);
        
        EXPECT_FALSE(L1 == L2);
        EXPECT_TRUE(L1 == data::reverse(L2));
        
    }

    // TODO
    TEST(LinkedStackTest, TestLinkedStackSort) {
        
        /*EXPECT_TRUE(stack<int>::make(1, 2, 3, 4, 5) == sort(stack<int>::make(4, 5, 1, 3, 2)));
        EXPECT_TRUE(stack<int>::make(1, 2, 3, 3, 4, 5) == sort(stack<int>::make(4, 3, 5, 1, 3, 2)));*/
        
    }

    // c++ containers aren't allowed to contain references. 
    // Functional data structures don't have to be c++ 
    // containers so we also have a version of functional
    // linked list for references. 
    TEST(LinkedStackTest, TestLinkedStackR) {
        
        int One = 1;
        int Zero = 0;
        
        auto empty = stack<int&>{};
        auto zero = stack<int&>{Zero, {}};
        auto one = stack<int&>{One, {}};
        auto one_zero = stack<int&>{One, {Zero,{}}};
        
        EXPECT_TRUE(empty == empty);
        EXPECT_TRUE(one == one);
        EXPECT_FALSE(one == empty);
        EXPECT_FALSE(one == zero);
        EXPECT_TRUE(one.first() == One);
        
        //EXPECT_TRUE(stack<int&>::make(One).first() == One);
        EXPECT_TRUE(one.rest() == empty);
        EXPECT_TRUE(one_zero.rest() == zero);
        //EXPECT_TRUE(stack<int&>::make(One).size() == 1);
        
    }
}
