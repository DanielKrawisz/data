// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include "gtest/gtest.h"

namespace data {
    
    TEST(MapTest, TestMapInterface) {
        
        is_map<map<uint32, int>>();
        is_map<map<uint32, int*>>();
        
        // These are commented out because I can't get them to work. 
        //is_map<map<uint32, const int>>();
        //is_map<map<uint32, const int*>>();
        
    }
    
    TEST(MapTest, TestOrderedSetInterface) {
        
        is_ordered_set<set<int>>();
        is_ordered_set<set<int*>>();
        
        // same here. 
        //is_ordered_set<set<const int>>();
        //is_ordered_set<set<const int*>>();
        
    }
    
    TEST(MapTest, TestMapEqual) {
        
        map<int, int> m1{{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m2{{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m3{{5, 2}, {3, 5}, {8, 3}};
        map<int, int> m4{{5, 2}, {3, 5}};
        
        EXPECT_EQ(m1, m1);
        EXPECT_EQ(m1, m2);
        EXPECT_NE(m1, m3);
        EXPECT_NE(m3, m4);
        EXPECT_NE(m4, m1);
        
        stack<entry<int, int>> v1{entry<int, int>{1, 7}, entry<int, int>{2, 1}, entry<int, int>{3, 5}};
        stack<entry<int, int>> v3{entry<int, int>{3, 5}, entry<int, int>{5, 2}, entry<int, int>{8, 3}};
        stack<entry<int, int>> v4{entry<int, int>{3, 5}, entry<int, int>{5, 2}};
        
        EXPECT_TRUE(m1.values() == v1);
        EXPECT_TRUE(m2.values() == v1);
        EXPECT_TRUE(m3.values() == v3);
        EXPECT_TRUE(m4.values() == v4);
        
        EXPECT_TRUE(m1.values() != v3);
        EXPECT_TRUE(m2.values() != v4);
        EXPECT_TRUE(m3.values() != v1);
        EXPECT_TRUE(m4.values() != v1);
    }
    
    TEST(MapTest, TestMapEmpty) {
        int i = 3;
        map<int, int*> m{{0, &i}};
        EXPECT_EQ(m[0], &i);
        EXPECT_EQ(m[1], nullptr);
    }
    
    TEST(MapTest, TestRemoveFromMap) {
        
        map<int, int> m1{{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m2{{2, 1}, {1, 7}};
        
        auto m1r2 = m1.remove(3);
        
        EXPECT_EQ(m1.remove(3), m2);
    }
}

