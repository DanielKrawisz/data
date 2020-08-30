// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools.hpp"
#include "gtest/gtest.h"
#include <iostream>

namespace data {

    TEST(ReverseTest, TestReverse) {
        
        list<int> ll{1, 2, 3};
        list<int> lr{3, 2, 1};
        stack<int> sl{1, 2, 3};
        stack<int> sr{3, 2, 1};
        cycle<int> cl{1, 2, 3};
        cycle<int> cr{3, 2, 1};
        
        EXPECT_TRUE(ll != lr);
        EXPECT_TRUE(sl != sr);
        EXPECT_TRUE(cl != cr);
        
        EXPECT_TRUE(reverse(ll) == lr);
        EXPECT_TRUE(reverse(sl) == sr);
        EXPECT_TRUE(reverse(cl) == cr);
        
    }

}
