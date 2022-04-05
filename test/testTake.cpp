// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename elem> using vector = std::vector<elem>;

    TEST(TakeTest, TestTake) {
        auto c1r = take(stack<int>{1, 2, 3, 4, 5}, 3);
        auto c1e = stack<int>{1, 2, 3};
        
        auto c2r = take(stack<int>{1, 2, 3}, 4);
        auto c2e = stack<int>{1, 2, 3};
        
        auto c3r = take(list<int>{1, 2, 3, 4, 5}, 3);
        auto c3e = list<int>{1, 2, 3};
        
        EXPECT_EQ(c1r, c1e);
        EXPECT_EQ(c2r, c2e);
        EXPECT_EQ(c3r, c3e);
        
    }
    
}


