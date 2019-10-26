// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "gtest/gtest.h"

TEST(LinkedListTest, TestLinkedList1) {
    using namespace data::exported;
    
    EXPECT_TRUE(list<int>{} == list<int>::make());
    
    EXPECT_TRUE(list<int>::make(1) == list<int>::make(1));
    EXPECT_FALSE(list<int>::make(1) == list<int>{});
    EXPECT_FALSE(list<int>::make(1) == list<int>::make(0));
    
    EXPECT_TRUE(list<int>::make(1).first() == 1);
    EXPECT_TRUE(list<int>::make(1).rest() == list<int>{});
    EXPECT_TRUE(list<int>::make(1).size() == 1);
    
}

TEST(LinkedListTest, TestLinkedList2) {
    using namespace data::exported;
    
    auto L1 = list<int>::make(1, 2, 3);
    auto L2 = list<int>::make(3, 2, 1);
    
    EXPECT_FALSE(L1 == L2);
    EXPECT_TRUE(L1 == reverse(L2));
    
}

TEST(LinkedListTest, DISABLED_TestLinkedListSort) {
    using namespace data::exported;
    
    EXPECT_TRUE(list<int>::make(1, 2, 3, 4, 5) == data::sort(list<int>::make(4, 5, 1, 3, 2)));
    EXPECT_TRUE(list<int>::make(1, 2, 3, 3, 4, 5) == data::sort(list<int>::make(4, 3, 5, 1, 3, 2)));
    
}

// c++ containers aren't allowed to contain references. 
// Functional data structures don't have to be c++ 
// containers so we also have a version of functional
// linked list for references. 
TEST(LinkedListTest, TestLinkedListR) {
    using namespace data::exported;
    
    int One = 1;
    int Zero = 0;
    
    EXPECT_TRUE(list<int&>{} == list<int&>{});
    EXPECT_TRUE(list<int&>{}.prepend(One) == list<int&>{}.prepend(One));
    EXPECT_FALSE(list<int&>{}.prepend(One) == list<int&>{});
    EXPECT_FALSE(list<int&>{}.prepend(One) == list<int&>{}.prepend(Zero));
    EXPECT_TRUE(list<int&>{}.prepend(One).first() == One);
    
    EXPECT_TRUE(list<int&>::make(One).first() == One);
    EXPECT_TRUE(list<int&>{}.prepend(One).rest() == list<int&>{});
    EXPECT_TRUE(list<int&>{}.prepend(Zero).prepend(One).rest() == list<int&>{}.prepend(Zero));
    EXPECT_TRUE(list<int&>::make(One).size() == 1);
    
}
