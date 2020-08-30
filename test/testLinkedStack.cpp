// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST(LinkedStackTest, TestLinkedStack1) {
        
        EXPECT_TRUE(stack<int>{} == stack<int>());
        
        EXPECT_TRUE(stack<int>(1) == stack<int>(1));
        EXPECT_FALSE(stack<int>(1) == stack<int>{});
        EXPECT_FALSE(stack<int>(1) == stack<int>(0));
        
        EXPECT_TRUE(stack<int>(1).first() == 1);
        EXPECT_TRUE(stack<int>(1).rest() == stack<int>{});
        EXPECT_TRUE(stack<int>(1).size() == 1);
        
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
        
        EXPECT_TRUE(stack<int&>(One).first() == One);
        EXPECT_TRUE(one.rest() == empty);
        EXPECT_TRUE(one_zero.rest() == zero);
        EXPECT_TRUE(stack<int&>(One).size() == 1);
        
    }

    TEST(LinkedStackTest, TestLinkedStackConstruct) {
    
        stack<int> l{1, 2, 3};
        stack<int> r{};
        stack<int> a{1};
        stack<int> b{1, 2};
        EXPECT_EQ(l.size(), 3);
        EXPECT_EQ(r.size(), 0);
        EXPECT_EQ(a.size(), 1);
        EXPECT_EQ(b.size(), 2);
        EXPECT_TRUE(l != r);
        EXPECT_TRUE(l == r << 3 << 2 << 1);
        
    }
    
    TEST(LinkedStackTest, TestLinkedStackEqual) {
        
        stack<int> t1{2, 1, 3, 5, 1, 7};
        stack<int> t2{2, 1, 3, 5, 1, 7};
        stack<int> t3{5, 2, 3, 5, 8, 3};
        stack<int> t4{5, 2, 3, 5};
        
        EXPECT_TRUE(t1 == t1);
        EXPECT_TRUE(t1 == t2);
        EXPECT_TRUE(t1 != t3);
        EXPECT_TRUE(t3 != t4);
        EXPECT_TRUE(t4 != t1);
    }
    
    void test_copy_linked_stack(stack<int>& p, int max) {
        p = stack<int>{};
        stack<int> new_stack{};
        for(int i = 1; i <= max; i++) new_stack = new_stack << i;
        p = new_stack;
    }
    
    // There was a bug in which linked lists would be destroyed
    // if they were copied. This test ensures that this does not happen.
    TEST(LinkedStackTest, TestLinkedStackCopy) {
        stack<int> p;
        test_copy_linked_stack(p, 7);
        EXPECT_EQ(p.size(), 7);
        EXPECT_EQ(p.first(), 7);
        EXPECT_EQ(p.rest().size(), 6);
        EXPECT_EQ(p.rest().first(), 6);
    }
    
    TEST(LinkedStackTest, TestLinkedStackIteration) {
        
        stack<int> t{1, 2, 3};
        auto i = t.begin();
        EXPECT_NE(i, t.end());
        EXPECT_EQ(*i, 1);
        i++;
        EXPECT_NE(i, t.end());
        EXPECT_EQ(*i, 2);
        i++;
        EXPECT_NE(i, t.end());
        EXPECT_EQ(*i, 3);
        i++;
        EXPECT_EQ(i, t.end());
        
        for (int x : t) ;
        for (const int& x : t) ;
    }

    // TODO
    TEST(LinkedStackTest, TestLinkedStackSort) {
        
        /*EXPECT_TRUE(stack<int>(1, 2, 3, 4, 5) == sort(stack<int>(4, 5, 1, 3, 2)));
        EXPECT_TRUE(stack<int>(1, 2, 3, 3, 4, 5) == sort(stack<int>(4, 3, 5, 1, 3, 2)));*/
        
    }
}
