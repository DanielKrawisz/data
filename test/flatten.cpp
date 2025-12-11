// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/flatten.hpp"
#include "data/list.hpp"
#include "gtest/gtest.h"

namespace data {

    cross<int> a {};
    stack<int> b {};
    list<int> c {};
/*
    static_assert (Same<decltype (flatten (a)), decltype (a)>);
    static_assert (Same<decltype (flatten (b)), decltype (b)>);
    static_assert (Same<decltype (flatten (c)), decltype (c)>);

    static_assert (Same<decltype (flatten<0> (a)), decltype (a)>);
    static_assert (Same<decltype (flatten<0> (b)), decltype (b)>);
    static_assert (Same<decltype (flatten<0> (c)), decltype (c)>);

    cross<cross<int>> d {};
    stack<stack<int>> e {};
    list<list<int>> f {};

    static_assert (Same<decltype (flatten (d)), decltype (a)>);
    static_assert (Same<decltype (flatten (e)), decltype (b)>);
    static_assert (Same<decltype (flatten (f)), decltype (c)>);

    static_assert (Same<decltype (flatten<0> (d)), decltype (d)>);
    static_assert (Same<decltype (flatten<0> (e)), decltype (e)>);
    static_assert (Same<decltype (flatten<0> (f)), decltype (f)>);

    static_assert (Same<decltype (flatten<1> (d)), decltype (a)>);
    static_assert (Same<decltype (flatten<1> (e)), decltype (b)>);
    static_assert (Same<decltype (flatten<1> (f)), decltype (c)>);

    cross<cross<cross<int>>> g {};
    stack<stack<stack<int>>> h {};
    list<list<list<int>>> i {};

    static_assert (Same<decltype (flatten (g)), decltype (a)>);
    static_assert (Same<decltype (flatten (h)), decltype (b)>);
    static_assert (Same<decltype (flatten (i)), decltype (c)>);

    static_assert (Same<decltype (flatten<0> (g)), decltype (g)>);
    static_assert (Same<decltype (flatten<0> (h)), decltype (h)>);
    static_assert (Same<decltype (flatten<0> (i)), decltype (i)>);

    static_assert (Same<decltype (flatten<1> (g)), decltype (d)>);
    static_assert (Same<decltype (flatten<1> (h)), decltype (e)>);
    static_assert (Same<decltype (flatten<1> (i)), decltype (f)>);

    static_assert (Same<decltype (flatten<2> (g)), decltype (a)>);
    static_assert (Same<decltype (flatten<2> (h)), decltype (b)>);
    static_assert (Same<decltype (flatten<2> (i)), decltype (c)>);

    cross<cross<stack<int>>> j {};
    stack<stack<list<int>>> k {};
    list<list<cross<int>>> l {};

    static_assert (Same<decltype (flatten (j)), cross<stack<int>>>);
    static_assert (Same<decltype (flatten (k)), stack<list<int>>>);
    static_assert (Same<decltype (flatten (l)), list<cross<int>>>);

    array<int, 2> a1 {};

    static_assert (Same<decltype (flatten (a1)), array<int, 2>>);
    static_assert (Same<decltype (flatten<0> (a1)), array<int, 2>>);

    array<int, 2, 3> a2 {};

    static_assert (Same<decltype (flatten (a2)), array<int, 6>>);
    static_assert (Same<decltype (flatten<0> (a2)), array<int, 2, 3>>);
    static_assert (Same<decltype (flatten<1> (a2)), array<int, 6>>);

    array<int, 2, 3, 5> a3 {};

    static_assert (Same<decltype (flatten (a3)), array<int, 30>>);
    static_assert (Same<decltype (flatten<0> (a3)), array<int, 2, 3, 5>>);
    static_assert (Same<decltype (flatten<1> (a3)), array<int, 6, 5>>);
    static_assert (Same<decltype (flatten<2> (a3)), array<int, 30>>);*/

}


