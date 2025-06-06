// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/modular.hpp"
#include "data/math/algebra/finite_field.hpp"
#include "data/math/algebra/elliptic_curve.hpp"
#include "gtest/gtest.h"

namespace data {

    template <typename W> void test_elliptic_curve () {

        math::elliptic::point<W> p0 {};
        math::elliptic::compressed_point<W> pc0 {};
        math::elliptic::projective_point<W> pp0 {};
        math::elliptic::Jacobian_point<W> j0 {};
    }

    template <typename X> void test_modular () {
        math::number::modular<plus (times (X {2}, X {3}), minus (X {7}, X {2}))> test_mod_A {3};
        math::number::modular<GCD (X {21}, X {14})> test_mod_B {3};

        using F1 = math::prime_field<17>;
        using F2 = math::prime_field<23>;
        using F3 = math::prime_field<43>;

        test_elliptic_curve<math::elliptic::Weierstrass<F1 {2}, F1 {2}>> ();
        test_elliptic_curve<math::elliptic::Weierstrass<F2 {9}, F2 {17}>> ();
        test_elliptic_curve<math::elliptic::Weierstrass<F3 {1}, F3 {1}>> ();

    }

    TEST (TestModular, TestModularTypes) {

        test_modular<byte> ();
        test_modular<uint16> ();
        test_modular<uint32> ();
        test_modular<uint64> ();

        test_modular<int8> ();
        test_modular<int16> ();
        test_modular<int32> ();
        test_modular<int64> ();

        test_modular<uint8_little> ();
        test_modular<uint16_little> ();
        test_modular<uint32_little> ();
        test_modular<uint64_little> ();

        test_modular<int8_little> ();
        test_modular<int16_little> ();
        test_modular<int32_little> ();
        test_modular<int64_little> ();

        test_modular<uint8_big> ();
        test_modular<uint16_big> ();
        test_modular<uint32_big> ();
        test_modular<uint64_big> ();

        test_modular<int8_big> ();
        test_modular<int16_big> ();
        test_modular<int32_big> ();
        test_modular<int64_big> ();
    }

}
