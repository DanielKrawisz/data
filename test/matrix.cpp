// Copyright (c) 2024-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/math.hpp>
#include <data/math/linear/matrix.hpp>

namespace data::math::linear {

    TEST (Matrix, Trace) {
        EXPECT_EQ ((tr (matrix<Q, 1, 1> {{5}})), 5);

        EXPECT_EQ ((tr (matrix<Q, 2, 2> {{2, 3},
                                         {7, 11}})), 13);

        EXPECT_EQ ((tr (matrix<Q, 3, 3> {{1, 2, 3},
                                         {4, 5, 6},
                                         {7, 8, 9}})), 15);
    }

    TEST (Matrix, Det) {
        EXPECT_EQ ((det (matrix<Q, 1, 1> {{1}})), 1);
        EXPECT_EQ ((det (matrix<Q, 1, 1> {{5}})), 5);

        EXPECT_EQ ((det (matrix<Q, 2, 2> {{1, 2},
                                          {3, 4}})), -2);

        EXPECT_EQ ((det (matrix<Q, 3, 3> {{1, 0, 0},
                                          {0, 2, 0},
                                          {0, 0, 3}})), 6);

        EXPECT_EQ ((det (matrix<Q, 3, 3> {{1, 2, 3},
                                          {0, 1, 4},
                                          {5, 6, 0}})), 1);
    }

    TEST (Matrix, Multiply) {
        EXPECT_EQ ((
            // assign to index 0
            //   1 * 5 + 2 * 7 = 19
            // assign to index 2
            //   3 * 5 + 4 * 7 = 43
            // assign to index 1
            //   1 * 6 + 2 * 8 = 22
            // assign to index 3
            //   3 * 6 + 4 * 8 = 50
            matrix<Q, 2, 2> {{1, 2},
                             {3, 4}} *
            matrix<Q, 2, 2> {{5, 6},
                             {7, 8}}), (
            matrix<Q, 2, 2> {{19, 22},
                             {43, 50}}));
    }

    TEST (Matrix, Transpose) {

    }

    TEST (Matrix, Invert) {

    }
}
