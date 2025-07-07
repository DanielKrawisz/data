// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/string.hpp"
#include "gtest/gtest.h"

TEST (PriorityQueueTest, TestPriorityQueue) {
    using namespace data;
    
    using pq = priority_queue<int>;
    
    EXPECT_EQ (pq {}, pq {});
    EXPECT_EQ (first (pq {} << 1), 1);
    EXPECT_NE (pq {} << 1, pq {});
    EXPECT_EQ (pq {} << 1 << 2, pq {} << 2 << 1);
    EXPECT_EQ (1, first (pq {} << 2 << 1));
}

