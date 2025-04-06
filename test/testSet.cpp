// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {

    TEST (SetTest, TestSetEmpty) {
        set<int> a;
        set<string> b;
        set<Z> c;
        set<N> d;

        set<const int> ax;
        set<const string> bx;
        set<const Z> cx;
        set<const N> dx;
    }

}
