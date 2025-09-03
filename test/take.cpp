// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/stack.hpp"
#include "data/list.hpp"
#include "data/cross.hpp"
#include "data/ordered_sequence.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename elem> using vector = std::vector<elem>;

    template <typename X> struct take_test : ::testing::Test {
        using list = X;
    };

    // priority_queue does not have take and drop because iterating through the queue is not
    // as fast as these other things.
    using test_cases = ::testing::Types<stack<int>, list<int>, cross<int>, ordered_sequence<int>>;

    TYPED_TEST_SUITE (take_test, test_cases);

    TYPED_TEST (take_test, TestTake) {

        using list = typename TestFixture::list;
        
        auto c1r = take (list {1, 2, 3, 4, 5}, 3);
        auto c1e = list {1, 2, 3};
        
        auto c2r = take (list {1, 2, 3}, 4);
        auto c2e = list {1, 2, 3};
        
        auto c3r = take (list {1, 2, 3, 4, 5}, 1);
        auto c3e = list {1};
        
        EXPECT_EQ (c1r, c1e);
        EXPECT_EQ (c2r, c2e);
        EXPECT_EQ (c3r, c3e);
    }
    
    TYPED_TEST (take_test, TestDrop) {

        using list = typename TestFixture::list;
        
        auto c1r = drop (list {1, 2, 3, 4, 5}, 3);
        auto c1e = list {4, 5};
        
        auto c2r = drop (list {1, 2, 3}, 4);
        auto c2e = list {};
        
        auto c3r = drop (list {1, 2, 3, 4, 5}, 1);
        auto c3e = list {2, 3, 4, 5};
        
        EXPECT_EQ (c1r, c1e);
        EXPECT_EQ (c2r, c2e);
        EXPECT_EQ (c3r, c3e);
        
    }
    
}
