// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/math.hpp>
#include <data/math/linear/inner.hpp>
#include <data/math/linear/exterior.hpp>
#include <data/math/algebra/finite_field.hpp>

namespace data::math {
    using namespace space;

    static_assert (linear::space<Q, exterior<Q, 1, 0>>);
    static_assert (linear::space<Q, exterior<Q, 1, 1>>);
    static_assert (linear::space<Q, exterior<Q, 2, 0>>);
    static_assert (linear::space<Q, exterior<Q, 2, 1>>);
    static_assert (linear::space<Q, exterior<Q, 2, 2>>);
    static_assert (linear::space<Q, exterior<Q, 3, 0>>);
    static_assert (linear::space<Q, exterior<Q, 3, 1>>);
    static_assert (linear::space<Q, exterior<Q, 3, 2>>);
    static_assert (linear::space<Q, exterior<Q, 3, 3>>);

    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 1, 0>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 1, 1>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 2, 0>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 2, 1>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 2, 2>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 3, 0>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 3, 1>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 3, 2>> ().begin ())>);
    static_assert (std::forward_iterator<decltype (std::declval<exterior<Q, 3, 3>> ().begin ())>);

    // scalars
    exterior<Q, 1, 0> q10 {2};
    exterior<Q, 2, 0> q20 {2};
    exterior<Q, 3, 0> q30 {2};
    exterior<Q, 4, 0> q40 {2};
    exterior<Q, 5, 0> q50 {2};
    exterior<Q, 6, 0> q60 {2};
    exterior<Q, 7, 0> q70 {2};
    exterior<Q, 8, 0> q80 {2};

    // vectors
    exterior<Q, 1, 1> q11 {1};
    exterior<Q, 2, 1> q21 {1, 2};
    exterior<Q, 3, 1> q31 {1, 2, 3};
    exterior<Q, 4, 1> q41 {1, 2, 3, 4};
    exterior<Q, 5, 1> q51 {1, 2, 3, 4, 5};
    exterior<Q, 6, 1> q61 {1, 2, 3, 4, 5, 6};
    exterior<Q, 7, 1> q71 {1, 2, 3, 4, 5, 6, 7};

    // bivectors
    exterior<Q, 2, 2> q22 {{1}};
    exterior<Q, 3, 2> q32 {{1, 2}, {3}};
    exterior<Q, 4, 2> q42 {{1, 2, 3}, {4, 5}, {6}};
    exterior<Q, 5, 2> q52 {{1, 2, 3, 4}, {5, 6, 7}, {8, 9}, {10}};
    exterior<Q, 6, 2> q62 {{1, 2, 3, 4, 5}, {6, 7, 8, 9}, {10, 11, 12}, {13, 14}, {15}};

    exterior<Q, 3, 3> q33 {{{1}}};
    exterior<Q, 4, 3> q43 {{{1, 2}, {3}}, {{4}}};
    exterior<Q, 5, 3> q53 {{{1, 2, 3}, {4, 5}, {6}}, {{7, 8}, {9}}, {{10}}};

    exterior<Q, 4, 4> q44 {{{{1}}}};

    TEST (Exterior, Access) {

        EXPECT_EQ (q10[], 2);
        EXPECT_EQ (q51[3], 4);

        EXPECT_THROW ((q32[0, 0]), exception);

        EXPECT_EQ ((q32[0, 1]), 1);
        EXPECT_EQ ((q32[1, 0]), -1);
        EXPECT_EQ ((q32[0, 1]), 1);
        EXPECT_EQ ((q32[1, 0]), -1);

        EXPECT_EQ ((q53[0, 2, 3]), 5);
        EXPECT_EQ ((q53[0, 3, 2]), -5);
        EXPECT_EQ ((q53[3, 0, 2]), 5);

        auto q = q32;

        q[0, 1] = 5;
        EXPECT_EQ ((q[0, 1]), 5);
        EXPECT_EQ ((q[1, 0]), -5);

        q[1, 0] = 8;
        EXPECT_EQ ((q[0, 1]), -8);
        EXPECT_EQ ((q[1, 0]), 8);
    }
/*
    TEST (Exterior, ScalarMultiplication) {
        EXPECT_EQ (q10 * Q {2}, (exterior<Q, 1, 0> {4}));
        EXPECT_EQ (q21 * Q {3}, (exterior<Q, 2, 1> {3, 6}));
        EXPECT_EQ (q32 * Q {5}, (exterior<Q, 3, 2> {{5, 10}, {15}}));
    }

    TEST (Exterior, Addition) {

    }

    TEST (Exterior, Wedge) {

    }

    TEST (Exterior, Star) {

    }*/

    // test euclidian transformations.

    // Key here is to test projective point <=> projective vector.


}
