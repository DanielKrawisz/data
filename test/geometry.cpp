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


    TEST (Geometry, ScalarConstructor) {
        space::exterior<Q, 1, 0> {2};
        space::exterior<Q, 2, 0> {2};
        space::exterior<Q, 3, 0> {2};
        space::exterior<Q, 4, 0> {2};
        space::exterior<Q, 5, 0> {2};
        space::exterior<Q, 6, 0> {2};
        space::exterior<Q, 7, 0> {2};
    }

    // test euclidian transformations.

    // Key here is to test projective point <=> projective vector.


}
