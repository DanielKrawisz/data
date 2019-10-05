// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"

TEST(LinkedListTest, TestLinkedList) {
    using namespace data::exported;
    
    auto L1 = list<int>::make(1, 2, 3);
    auto L2 = list<int>::make(3, 2, 1);
    
    EXPECT_FALSE(L1 == L2);
    EXPECT_TRUE(L1 == reverse(L2));
    
    EXPECT_TRUE(list<int>::make(1, 2, 3, 4, 5) == data::sort(list<int>::make(4, 5, 1, 3, 2)));
    EXPECT_TRUE(list<int>::make(1, 2, 3, 3, 4, 5) == data::sort(list<int>::make(4, 3, 5, 1, 3, 2)));
    
}
