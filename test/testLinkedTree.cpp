// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include "gtest/gtest.h"

namespace data {
    
    // test whether these data structures satisfy the correct interfaces. 
    TEST(LinkedTreeTest, TestTreeInterfaces) {
        
        is_tree<tree<int>>();
        is_tree<tree<int*>>();
        is_tree<tree<int&>>();
        
        is_tree<tree<const int>>();
        is_tree<tree<const int*>>();
        is_tree<tree<const int&>>();
        
    }
    
    TEST(LinkedTreeTest, TestTreeSize) {
        tree<int> t0 = tree<int>{};
        tree<int> t1 = tree<int>{6};
        tree<int> t2 = tree<int>{4, tree<int>{}, tree<int>{3}};
        tree<int> t3 = tree<int>{2, tree<int>{1}, tree<int>{}};
        
        EXPECT_EQ(t0.size(), 0);
        EXPECT_EQ(t1.size(), 1);
        EXPECT_EQ(t2.size(), 2);
        EXPECT_EQ(t3.size(), 2);
        
        EXPECT_EQ(t0.right().size(), 0);
        EXPECT_EQ(t1.right().size(), 0);
        EXPECT_EQ(t2.right().size(), 1);
        EXPECT_EQ(t3.right().size(), 0);
        
        EXPECT_EQ(t0.left().size(), 0);
        EXPECT_EQ(t1.left().size(), 0);
        EXPECT_EQ(t2.left().size(), 0);
        EXPECT_EQ(t3.left().size(), 1);
        
        EXPECT_EQ(t0.begin(), t0.end());
        EXPECT_NE(t1.begin(), t1.end());
        EXPECT_EQ(*t1.begin(), 6);
        
    }
    
    TEST(LinkedTreeTest, TestTreeEqual) {
        
        tree<int> t1{1, tree<int>{2, tree<int>{3}, tree<int>{}}, tree<int>{4}};
        tree<int> t2{1, tree<int>{2, tree<int>{3}, tree<int>{}}, tree<int>{4}};
        tree<int> t3{2, tree<int>{3, tree<int>{4}, tree<int>{}}, tree<int>{5}};
        tree<int> t4{3, tree<int>{3, tree<int>{4}, tree<int>{7}}, tree<int>{5, tree<int>{6}, tree<int>{}}};
        
        EXPECT_EQ(t1, t1);
        EXPECT_EQ(t1, t2);
        EXPECT_NE(t1, t3);
        EXPECT_NE(t3, t4);
        EXPECT_NE(t4, t1);
        
        stack<int> v1{1, 2, 3, 4};
        stack<int> v3{2, 3, 4, 5};
        stack<int> v4{3, 3, 4, 7, 5, 6};
        
        EXPECT_EQ(t1.values(), v1);
        EXPECT_EQ(t2.values(), v1);
        EXPECT_EQ(t3.values(), v3);
        EXPECT_EQ(t4.values(), v4);
        
        EXPECT_NE(t1.values(), v3);
        EXPECT_NE(t2.values(), v4);
        EXPECT_NE(t3.values(), v1);
        EXPECT_NE(t4.values(), v1);
        
    }
    
    void test_copy_linked_tree(tree<int>& p, int max) {
        p = tree<int>{};
        tree<int> new_tree{};
        for (int i = 0; i < max; i++) new_tree = tree<int>{i, new_tree, tree<int>{}};
        p = new_tree;
    }
    
    TEST(LinkedTreeTest, TestLinkedTreeCopy) {
        tree<int> p;
        test_copy_linked_tree(p, 7);
        EXPECT_EQ(p.size(), 7);
        EXPECT_EQ(p.left().size(), 6);
        EXPECT_EQ(p.right().size(), 0);
    }
    
}
