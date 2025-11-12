// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/math.hpp>
#include <data/math/linear/inner.hpp>
#include <data/math/point.hpp>
#include <data/math/algebra/finite_field.hpp>

namespace data::math {

    static_assert (linear::space<Q, space::exterior<Q, 1, 0>>);
    static_assert (linear::space<Q, space::exterior<Q, 1, 1>>);
    static_assert (linear::space<Q, space::exterior<Q, 2, 0>>);
    static_assert (linear::space<Q, space::exterior<Q, 2, 1>>);
    static_assert (linear::space<Q, space::exterior<Q, 2, 2>>);
    static_assert (linear::space<Q, space::exterior<Q, 3, 0>>);
    static_assert (linear::space<Q, space::exterior<Q, 3, 1>>);
    static_assert (linear::space<Q, space::exterior<Q, 3, 2>>);
    static_assert (linear::space<Q, space::exterior<Q, 3, 3>>);


    TEST (Geometry, ExteriorConstructor) {
        // scalars
        space::exterior<Q, 1, 0> {2};
        space::exterior<Q, 2, 0> {2};
        space::exterior<Q, 3, 0> {2};
        space::exterior<Q, 4, 0> {2};
        space::exterior<Q, 5, 0> {2};
        space::exterior<Q, 6, 0> {2};
        space::exterior<Q, 7, 0> {2};
        space::exterior<Q, 8, 0> {2};

        // vectors
        space::exterior<Q, 1, 1> {1};
        space::exterior<Q, 2, 1> {1, 2};
        space::exterior<Q, 3, 1> {1, 2, 3};
        space::exterior<Q, 4, 1> {1, 2, 3, 4};
        space::exterior<Q, 5, 1> {1, 2, 3, 4, 5};
        space::exterior<Q, 6, 1> {1, 2, 3, 4, 5, 6};
        space::exterior<Q, 7, 1> {1, 2, 3, 4, 5, 6, 7};

        // bivectors
        space::exterior<Q, 2, 2> {{1}};
        space::exterior<Q, 3, 2> {{1, 2}, {3}};
        space::exterior<Q, 4, 2> {{1, 2, 3}, {4, 5}, {6}};
        space::exterior<Q, 5, 2> {{1, 2, 3, 4}, {5, 6, 7}, {8, 9}, {10}};
        space::exterior<Q, 6, 2> {{1, 2, 3, 4, 5}, {6, 7, 8, 9}, {10, 11, 12}, {13, 14}, {15}};

        space::exterior<Q, 3, 3> {{{1}}};
        space::exterior<Q, 4, 3> {{{1, 2}, {3}}, {{4}}};
        space::exterior<Q, 5, 3> {{{1, 2, 3}, {4, 5}, {6}}, {{7, 8}, {9}}, {{10}}};
    }


    // test euclidian transformations.

    // Key here is to test projective point <=> projective vector.


}
