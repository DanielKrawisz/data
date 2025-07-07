// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {

    TEST (ListTest, TestListInterfaces) {
        static_assert (Same<decltype (std::declval<const list<int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<const list<const int>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<const list<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<const list<const int &>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<const list<int *>> ().first ()), int *const &>);
        static_assert (Same<decltype (std::declval<const list<const int *>> ().first ()), const int *const &>);

        static_assert (Same<decltype (std::declval<const list<string>> ().first ()), const string &>);
        static_assert (Same<decltype (std::declval<const list<const string>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<const list<string &>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<const list<const string &>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<const list<string *>> ().first ()), string *const &>);
        static_assert (Same<decltype (std::declval<const list<const string *>> ().first ()), const string *const &>);
    
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<int>> ().first ()), int>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<int &>> ().first ()), int>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<int *>> ().first ()), int *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<const int *>> ().first ()), const int *>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<string>> ().first ()), const string &>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<string &>> ().first ()), const string &>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<string *>> ().first ()), string *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<const string *>> ().first ()), const string *>);

        static_assert (Container<list<int>, int>);
        static_assert (Container<list<int *>, int *>);
        static_assert (Container<list<const int *>, const int *>);
        static_assert (Container<list<int *const>, int *const>);
        static_assert (Container<list<const int *const>, const int *const>);
        static_assert (Container<list<int &>, int &>);
        static_assert (Container<list<const int &>, const int &>);

        static_assert (List<list<int>, int>);
        static_assert (List<list<int *>, int *>);
        static_assert (List<list<const int *>, const int *>);
        static_assert (List<list<int *const>, int *const>);
        static_assert (List<list<const int *const>, const int *const>);
        static_assert (List<list<int &>, int &>);
        static_assert (List<list<const int &>, const int &>);

        static_assert (std::forward_iterator<decltype (std::declval<const list<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int *const >> ().begin ())>);

    }

    void accept_stack_of_string_views (list<string_view>) {}

    TEST (ListTest, TestListConvert) {
        list<string> test {"1", "2", "3", "4"};

        accept_stack_of_string_views (test);

        list<N> numbers {1, 2, 3, 4};

        EXPECT_EQ (list<N> (test), numbers);

    }

    TEST (ListTest, TestListRotate) {

        list<string> test {"1", "2", "3", "4"};
        list<string> rl {"2", "3", "4", "1"};
        list<string> rr {"4", "1", "2", "3"};

        EXPECT_EQ (rotate_right (test), rr);
        EXPECT_EQ (rotate_left (test), rl);

    }

    TEST (ListTest, TestListOrdering) {

        list<int> l0 {};
        list<int> l1 {1};
        list<int> l2 {1, 2};
        list<int> l3 {1, 2, 3};

        EXPECT_EQ (shuffle (l0, {}), l0);
        EXPECT_EQ (shuffle (l1, {0}), l1);
        EXPECT_EQ (shuffle (l2, {0, 1}), l2);
        EXPECT_EQ (shuffle (l2, {1, 0}), reverse (l2));
        EXPECT_EQ (shuffle (l3, {0, 1, 2}), l3);
        EXPECT_EQ (shuffle (l3, {2, 1, 0}), reverse (l3));
        EXPECT_EQ (shuffle (l3, {0, 2, 1}), (list<int> {1, 3, 2}));
        EXPECT_EQ (shuffle (l3, {1, 0, 2}), (list<int> {2, 1, 3}));
        EXPECT_EQ (shuffle (l3, {1, 2, 0}), (list<int> {3, 1, 2}));
        EXPECT_EQ (shuffle (l3, {2, 0, 1}), (list<int> {2, 3, 1}));

    }

    TEST (ListTest, TestListConstruct) {

        list<int> l {1, 2, 3};
        list<int> r {};
        list<int> a {1};
        list<int> b {1, 2};

        EXPECT_EQ (l.size (), 3);
        EXPECT_EQ (r.size (), 0);
        EXPECT_EQ (a.size (), 1);
        EXPECT_EQ (b.size (), 2);
        EXPECT_TRUE (l != r);
        EXPECT_TRUE (l == r << 1 << 2 << 3);

        int val[] {1, 2, 3, 4, 5, 6, 7, 8, 9};

        list<int> {1, 2, 3, 4, 5, 6, 7};
        list<int *> {val, val + 2, val + 4, val + 3, val + 8, val + 1};
        list<int &> {val[0], val[1], val[3], val[7], val[4], val[5], val[0]};

    }

}
