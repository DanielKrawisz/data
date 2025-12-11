// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/transpose.hpp"
#include "gtest/gtest.h"

namespace data {

    array<int, 2, 3> x {{1, 2, 3}, {3, 4, 5}};

    static_assert (Same<decltype (transpose (x)), array<int, 3, 2>>);

    static_assert (Same<decltype (transpose<0> (x)), array<int, 2, 3>>);
    static_assert (Same<decltype (transpose<0, 1> (x)), array<int, 2, 3>>);
    static_assert (Same<decltype (transpose<1, 0> (x)), array<int, 3, 2>>);

}

