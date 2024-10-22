// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include "gtest/gtest.h"

// test whether data structures satisfy the correct interfaces. 
namespace data {
    
    TEST (FunctionalInterfaceTest, TestSequence) {
        
        static_assert (sequence<ordered_list<int>>);
        static_assert (sequence<ordered_list<int *>>);
        static_assert (sequence<ordered_list<int &>>);
        
        static_assert (sequence<ordered_list<const int>>);
        static_assert (sequence<ordered_list<const int *>>);
        static_assert (sequence<ordered_list<const int &>>);
        
        static_assert (sequence<priority_queue<int>>);
        static_assert (sequence<priority_queue<int *>>);
        static_assert (sequence<priority_queue<int &>>);
        
        static_assert (sequence<priority_queue<const int>>);
        static_assert (sequence<priority_queue<const int *>>);
        static_assert (sequence<priority_queue<const int &>>);
        
    }
    
    TEST (FunctionalInterfaceTest, TestContainer) {
        
        static_assert (container<ordered_list<int>, const int>);
        static_assert (container<ordered_list<int *>, const int *>);
        static_assert (container<ordered_list<int &>, const int>);
        
        static_assert (container<ordered_list<const int>, const int>);
        static_assert (container<ordered_list<const int *>, const int *>);
        static_assert (container<ordered_list<const int &>, const int>);
        
        static_assert (container<priority_queue<int>, const int>);
        static_assert (container<priority_queue<int *>, const int *>);
        static_assert (container<priority_queue<int &>, const int>);
        
        static_assert (container<priority_queue<const int>, const int>);
        static_assert (container<priority_queue<const int *>, const int *>);
        static_assert (container<priority_queue<const int &>, const int>);
        
    }

    TEST (FunctionalInterfaceTest, TestIterable) {

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const int &>> ().begin ())>);

    }
}
