// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include "gtest/gtest.h"

// test whether data structures satisfy the correct interfaces. 
namespace data {
    
    TEST(FunctionalInterfaceTest, TestSequence) {
        
        is_sequence<ordered_list<int>>();
        is_sequence<ordered_list<int*>>();
        is_sequence<ordered_list<int&>>();
        
        is_sequence<ordered_list<const int>>();
        is_sequence<ordered_list<const int*>>();
        is_sequence<ordered_list<const int&>>();
        
        is_sequence<priority_queue<int>>();
        is_sequence<priority_queue<int*>>();
        is_sequence<priority_queue<int&>>();
        
        is_sequence<priority_queue<const int>>();
        is_sequence<priority_queue<const int*>>();
        is_sequence<priority_queue<const int&>>();
        
    }
    
    TEST(FunctionalInterfaceTest, TestContainer) {
        
        is_container<ordered_list<int>, const int>();
        is_container<ordered_list<int*>, const int*>();
        is_container<ordered_list<int&>, const int>();
        
        is_container<ordered_list<const int>, const int>();
        is_container<ordered_list<const int*>, const int*>();
        is_container<ordered_list<const int&>, const int>();
        
        is_container<priority_queue<int>, const int>();
        is_container<priority_queue<int*>, const int*>();
        is_container<priority_queue<int&>, const int>();
        
        is_container<priority_queue<const int>, const int>();
        is_container<priority_queue<const int*>, const int*>();
        is_container<priority_queue<const int&>, const int>();
        
    }
    
    TEST(FunctionalInterfaceTest, TestList) {
        
        is_list<list<int>>();
        is_list<list<int*>>();
        is_list<list<int&>>();
        
        is_list<list<const int>>();
        is_list<list<const int*>>();
        is_list<list<const int&>>();
        
    }
    
    TEST(FunctionalInterfaceTest, TestTree) {
        
        is_tree<tree<int>>();
        is_tree<tree<int*>>();
        is_tree<tree<int&>>();
        
        is_tree<tree<const int>>();
        is_tree<tree<const int*>>();
        is_tree<tree<const int&>>();
        
    }
}
