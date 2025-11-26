// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/set.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {

    static_assert (OrderedSet<set<int>, int>);
    static_assert (OrderedSet<set<int &>, int &>);
    static_assert (OrderedSet<set<int *>, int *>);
    static_assert (OrderedSet<set<int *const>, int *const>);

    static_assert (OrderedSet<set<const int>, const int>);
    static_assert (OrderedSet<set<const int &>, const int &>);
    static_assert (OrderedSet<set<const int *>, const int *>);
    static_assert (OrderedSet<set<const int *const>, const int *const>);

    static_assert (Sack<set<int>, int>);
    static_assert (Sack<set<int &>, int &>);
    static_assert (Sack<set<int *>, int *>);
    static_assert (Sack<set<int *const>, int *const>);

    static_assert (Sack<set<const int>, int>);
    static_assert (Sack<set<const int &>, int &>);
    static_assert (Sack<set<const int *>, const int *>);
    static_assert (Sack<set<const int *const>, const int *const>);

    static_assert (Sack<const set<int>, int>);
    static_assert (Sack<const set<int &>, int &>);
    static_assert (Sack<const set<int *>, int *const>);
    static_assert (Sack<const set<int *const>, int *const>);

    static_assert (Sack<const set<const int>, int>);
    static_assert (Sack<const set<const int &>, int &>);
    static_assert (Sack<const set<const int *>, const int *>);
    static_assert (Sack<const set<const int *const>, const int *const>);

    static_assert (ConstIterable<set<int>>);
    static_assert (ConstIterable<set<int &>>);
    static_assert (ConstIterable<set<int *>>);
    static_assert (ConstIterable<set<int *const>>);

    static_assert (ConstIterable<set<const int>>);
    static_assert (ConstIterable<set<const int &>>);
    static_assert (ConstIterable<set<const int *>>);
    static_assert (ConstIterable<set<const int *const>>);

    TEST (Set, Empty) {
        set<int> a;
        set<string> b;
        set<Z> c;
        set<N> d;

        set<const int> ax;
        set<const string> bx;
        set<const Z> cx;
        set<const N> dx;
    }

    // TODO a lot more tests are needed here.

}
