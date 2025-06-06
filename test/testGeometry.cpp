// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/math/linear/inner.hpp>
#include <data/math/point.hpp>
#include <data/math/algebra/finite_field.hpp>

namespace data::math {

    static_assert (std::is_trivially_copy_constructible<array<uint64, 0>>::value);
    static_assert (std::is_trivially_copy_constructible<array<uint64, 1>>::value);
    static_assert (std::is_trivially_copy_constructible<array<uint64, 2>>::value);
    static_assert (std::is_trivially_copy_constructible<array<uint64, 3>>::value);
    static_assert (std::is_trivially_copy_constructible<array<uint64, 4>>::value);

    static_assert (std::is_trivially_copy_constructible<array<prime_field<uint64 {17}>, 0>>::value);
    static_assert (std::is_trivially_copy_constructible<array<prime_field<uint64 {19}>, 1>>::value);
    static_assert (std::is_trivially_copy_constructible<array<prime_field<uint64 {23}>, 2>>::value);
    static_assert (std::is_trivially_copy_constructible<array<prime_field<uint64 {29}>, 3>>::value);
    static_assert (std::is_trivially_copy_constructible<array<prime_field<uint64 {31}>, 4>>::value);
    static_assert (std::is_trivially_copy_constructible<array<prime_field<uint64 {37}>, 5>>::value);

    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {41}>, 1, 0>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {43}>, 1, 1>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {47}>, 2, 0>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {53}>, 2, 1>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {59}>, 2, 2>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {61}>, 3, 0>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {67}>, 3, 1>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {71}>, 3, 2>>::value);
    static_assert (std::is_trivially_copy_constructible<space::exterior<prime_field<uint64 {73}>, 3, 3>>::value);

    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {79}>, 1>::simplex<0>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {83}>, 1>::simplex<1>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {89}>, 2>::simplex<0>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {97}>, 2>::simplex<1>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {101}>, 2>::simplex<2>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {103}>, 3>::simplex<0>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {107}>, 3>::simplex<1>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {109}>, 3>::simplex<2>>::value);
    static_assert (std::is_trivially_copy_constructible<space::affine<prime_field<uint64 {113}>, 3>::simplex<3>>::value);

    static_assert (linear::space<prime_field<uint64 {127}>, space::exterior<prime_field<uint64 {127}>, 1, 0>>);
    static_assert (linear::space<prime_field<uint64 {131}>, space::exterior<prime_field<uint64 {131}>, 1, 1>>);
    static_assert (linear::space<prime_field<uint64 {137}>, space::exterior<prime_field<uint64 {137}>, 2, 0>>);
    static_assert (linear::space<prime_field<uint64 {139}>, space::exterior<prime_field<uint64 {139}>, 2, 1>>);
    static_assert (linear::space<prime_field<uint64 {149}>, space::exterior<prime_field<uint64 {149}>, 2, 2>>);
    static_assert (linear::space<prime_field<uint64 {151}>, space::exterior<prime_field<uint64 {151}>, 3, 0>>);
    static_assert (linear::space<prime_field<uint64 {157}>, space::exterior<prime_field<uint64 {157}>, 3, 1>>);
    static_assert (linear::space<prime_field<uint64 {163}>, space::exterior<prime_field<uint64 {163}>, 3, 2>>);
    static_assert (linear::space<prime_field<uint64 {167}>, space::exterior<prime_field<uint64 {167}>, 3, 3>>);


    TEST (GeometryTest, TestScalarConstructor) {
        space::exterior<prime_field<uint64 {17}>, 1, 0> {2};
        space::exterior<prime_field<uint64 {17}>, 2, 0> {2};
        space::exterior<prime_field<uint64 {17}>, 3, 0> {2};
        space::exterior<prime_field<uint64 {17}>, 4, 0> {2};
        space::exterior<prime_field<uint64 {17}>, 5, 0> {2};
        space::exterior<prime_field<uint64 {17}>, 6, 0> {2};
        space::exterior<prime_field<uint64 {17}>, 7, 0> {2};
    }

/*
    TEST (GeometryTest, TestVectorConstructor) {

        space::exterior<prime_field<uint64 {17}>, 1, 1> {2};
        space::exterior<prime_field<uint64 {17}>, 2, 1> {2, 3};
        space::exterior<prime_field<uint64 {17}>, 3, 1> {2, 3, 4};
        space::exterior<prime_field<uint64 {17}>, 4, 1> {2, 3, 4, 5};
        space::exterior<prime_field<uint64 {17}>, 5, 1> {2, 3, 4, 5, 6};
        space::exterior<prime_field<uint64 {17}>, 6, 1> {2, 3, 4, 5, 6, 7};
        space::exterior<prime_field<uint64 {17}>, 7, 1> {2, 3, 4, 5, 6, 7, 8};
    }*/

    // test euclidian transformations.

    // Key here is to test projective point <=> projective vector.


}
