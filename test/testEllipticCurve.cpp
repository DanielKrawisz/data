// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/math/algebra/finite_field.hpp>
#include <data/math/algebra/elliptic_curve.hpp>

namespace data::math {

    template <auto W> constexpr void test_elliptic_curve () {

        elliptic::point<W> p0 {};/*
        elliptic::compressed_point<W> pc0 {};
        elliptic::projective_point<W> pp0 {};
        elliptic::Jacobian_point<W> j0 {};*/

    }

    template <typename N>
    constexpr void test_toy_curves () {
/*
        test_elliptic_curve<elliptic::Weierstrass<prime_field<N {17}>> {2, 2}> ();
        test_elliptic_curve<elliptic::Weierstrass<prime_field<N {23}>> {9, 17}> ();
        test_elliptic_curve<elliptic::Weierstrass<prime_field<N {43}>> {1, 1}> ();
*/
    }

    TEST (EllipticCurveTest, TestToyCurves) {

        test_toy_curves<byte> ();/*
        test_toy_curves<uint16> ();
        test_toy_curves<uint32> ();
        test_toy_curves<uint64> ();

        test_toy_curves<int8> ();
        test_toy_curves<int16> ();
        test_toy_curves<int32> ();
        test_toy_curves<int64> ();

        test_toy_curves<uint8_little> ();
        test_toy_curves<uint16_little> ();
        test_toy_curves<uint32_little> ();
        test_toy_curves<uint64_little> ();

        test_toy_curves<int8_little> ();
        test_toy_curves<int16_little> ();
        test_toy_curves<int32_little> ();
        test_toy_curves<int64_little> ();

        test_toy_curves<uint8_big> ();
        test_toy_curves<uint16_big> ();
        test_toy_curves<uint32_big> ();
        test_toy_curves<uint64_big> ();

        test_toy_curves<int8_big> ();
        test_toy_curves<int16_big> ();
        test_toy_curves<int32_big> ();
        test_toy_curves<int64_big> ();*/
    }
}

