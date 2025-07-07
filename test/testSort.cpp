// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include "gtest/gtest.h"

namespace data {
    
    template <typename list> void sort_test_case (list given, list expected) {

        EXPECT_TRUE (sorted (expected));
        EXPECT_EQ (sorted (given), given == expected);
        EXPECT_EQ (sort (given), expected);

    }
    
    template <typename list> void sort_test () {

        sort_test_case (list {4, 5, 1, 3, 2}, list {1, 2, 3, 4, 5});
        sort_test_case (list {4, 3, 5, 1, 3, 2}, list {1, 2, 3, 3, 4, 5});

    }
    
    TEST (SortTest, TestSort) {

        sort_test<stack<int>> ();
        sort_test<list<int>> ();
        sort_test<cross<int>> ();

    }
    
    template <typename list> requires Sequence<list> && std::equality_comparable_with<decltype (std::declval<list> ().first ()), int> 
    void test_sorted_list () {

        list oA {4, 5, 1, 3, 2};
        list oB {4, 3, 5, 1, 3, 2};
        list oC {33, 24};
        list oD {3, 5, 4};
        
        stack<int> sA {1, 2, 3, 4, 5};
        stack<int> sB {1, 2, 3, 3, 4, 5};
        stack<int> sC {24, 33};
        stack<int> sD {3, 4, 5};
        
        bool eqC = oC == sC;
        EXPECT_TRUE (eqC);
        bool eqD = oD == sD;
        EXPECT_TRUE (eqD);
        bool eqA = oA == sA;
        EXPECT_TRUE (eqA);
        bool eqB = oB == sB;
        EXPECT_TRUE (eqB);
        bool neAB = oA != sB;
        EXPECT_TRUE (neAB);
        bool neBA = oB != sA;
        EXPECT_TRUE (neBA);
    }
    
    TEST (SortTest, TestOrderedStack) {

        test_sorted_list<ordered_list<int>> ();
        test_sorted_list<priority_queue<int>> ();

    }
    
}
