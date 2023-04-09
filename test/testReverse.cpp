// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools.hpp"
#include "gtest/gtest.h"
#include <iostream>

namespace data {
    
    template <typename list> void test_reverse () {
        list l {1, 2, 3};
        list r {3, 2, 1};
        
        EXPECT_TRUE (l != r);
        
        EXPECT_TRUE (reverse (l) == r);
    }

    TEST (ReverseTest, TestReverse) {
        
        test_reverse<list<int>> ();
        test_reverse<stack<int>> ();
        test_reverse<cycle<int>> ();
        test_reverse<cross<int>> ();
        test_reverse<array<int, 3>> ();
        
    }

}
