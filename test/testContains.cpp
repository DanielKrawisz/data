// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/string.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename elem> using vector = std::vector<elem>;

    TEST (ContainsTest, TestContains) {

        EXPECT_TRUE (contains (stack<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (stack<string> {"a", "b", "c", "d"}, "f"));
        
        EXPECT_TRUE (contains (stack<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (stack<string> {"a", "b", "c", "d"}, "f"));
        
        EXPECT_TRUE (contains (std::vector<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (std::vector<string> {"a", "b", "c", "d"}, "f"));
/*
        EXPECT_TRUE (contains (list<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (list<string> {"a", "b", "c", "d"}, "f"));
*/
    }
}

