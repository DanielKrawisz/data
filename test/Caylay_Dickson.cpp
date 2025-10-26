
// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/fraction.hpp"
#include "data/math.hpp"
#include "gtest/gtest.h"

namespace data::math {

    // TODO we need to be able to compare fractions and

    static_assert (Real<Z>);
    static_assert (Real<Z_bytes_little>);
    static_assert (Real<Z_bytes_big>);
    static_assert (Real<Z_bytes_BC_little>);
    static_assert (Real<Z_bytes_BC_big>);
    static_assert (Real<int64>);
    static_assert (Real<int128>);
    static_assert (Real<int128_little>);
    static_assert (Real<int128_big>);
    static_assert (Real<dec_int>);
    static_assert (Real<hex_int>);
    static_assert (Real<hex_int_BC>);

    static_assert (Complex<complex<Z>>);
    static_assert (Complex<complex<Z_bytes_little>>);
    static_assert (Complex<complex<Z_bytes_big>>);
    static_assert (Complex<complex<Z_bytes_BC_little>>);
    static_assert (Complex<complex<Z_bytes_BC_big>>);
    static_assert (Complex<complex<int64>>);
    static_assert (Complex<complex<int128>>);
    static_assert (Complex<complex<int128_little>>);
    static_assert (Complex<complex<int128_big>>);
    static_assert (Complex<complex<dec_int>>);
    static_assert (Complex<complex<hex_int>>);
    static_assert (Complex<complex<hex_int_BC>>);

    static_assert (Quaternionic<quaternion<Z>>);
    static_assert (Quaternionic<quaternion<Z_bytes_little>>);
    static_assert (Quaternionic<quaternion<Z_bytes_big>>);
    static_assert (Quaternionic<quaternion<Z_bytes_BC_little>>);
    static_assert (Quaternionic<quaternion<Z_bytes_BC_big>>);
    static_assert (Quaternionic<quaternion<int64>>);
    static_assert (Quaternionic<quaternion<int128>>);
    static_assert (Quaternionic<quaternion<int128_little>>);
    static_assert (Quaternionic<quaternion<int128_big>>);
    static_assert (Quaternionic<quaternion<dec_int>>);
    static_assert (Quaternionic<quaternion<hex_int>>);
    static_assert (Quaternionic<quaternion<hex_int_BC>>);

    static_assert (Octonionic<octonion<Z>>);
    static_assert (Octonionic<octonion<Z_bytes_little>>);
    static_assert (Octonionic<octonion<Z_bytes_big>>);
    static_assert (Octonionic<octonion<Z_bytes_BC_little>>);
    static_assert (Octonionic<octonion<Z_bytes_BC_big>>);
    static_assert (Octonionic<octonion<int64>>);
    static_assert (Octonionic<octonion<int128>>);
    static_assert (Octonionic<octonion<int128_little>>);
    static_assert (Octonionic<octonion<int128_big>>);
    static_assert (Octonionic<octonion<dec_int>>);
    static_assert (Octonionic<octonion<hex_int>>);
    static_assert (Octonionic<octonion<hex_int_BC>>);

    template <typename X>
    // requires NumberComparable<X>
    void test_complex (X zero, X one, X i) {
        EXPECT_NE (zero, one);
        EXPECT_NE (zero, i);
        EXPECT_NE (one, i);

        EXPECT_EQ (~zero, zero);
        EXPECT_EQ (~one, one);
        EXPECT_EQ (~i, -i);

        EXPECT_EQ (zero * zero, zero);
        EXPECT_EQ (zero * one, zero);
        EXPECT_EQ (one * zero, zero);
        EXPECT_EQ (zero * i, zero);
        EXPECT_EQ (i * zero, zero);
        EXPECT_EQ (one * one, one);
        EXPECT_EQ (i * one, i);
        EXPECT_EQ (one * i, i);
        EXPECT_EQ (i * i, -one);
    };

    template <typename X> void test_quaternion (X zero, X one, X i, X j) {
        test_complex<X> (zero, one, i);
        test_complex<X> (zero, one, j);

        EXPECT_NE (i, j);
        EXPECT_NE (-i, j);

        auto k = j * i;
        EXPECT_NE (zero, k);
        EXPECT_NE (one, k);
        EXPECT_NE (i, k);
        EXPECT_NE (j, k);

        EXPECT_EQ (k * k, -one);
        EXPECT_EQ (i * j, -k);
        EXPECT_EQ (j * k, -i);
        EXPECT_EQ (k * i, -j);
        EXPECT_EQ (k * j * i, -one);
    };

    template <typename X> void test_octonion (X zero, X one, X i, X j, X k) {
        test_quaternion<X> (zero, one, i, j);
        test_quaternion<X> (zero, one, j, k);
        test_quaternion<X> (zero, one, k, i);

        auto e3 = i * j;
        auto e5 = j * k;
        auto e6 = k * i;
        auto e7 = i * (j * k);

        EXPECT_NE (i, e3);
        EXPECT_NE (-i, e3);
        EXPECT_NE (j, e3);
        EXPECT_NE (-j, e3);
        EXPECT_NE (k, e3);
        EXPECT_NE (-k, e3);

        EXPECT_NE (i, e5);
        EXPECT_NE (-i, e5);
        EXPECT_NE (j, e5);
        EXPECT_NE (-j, e5);
        EXPECT_NE (k, e5);
        EXPECT_NE (-k, e5);

        EXPECT_NE (i, e6);
        EXPECT_NE (-i, e6);
        EXPECT_NE (j, e6);
        EXPECT_NE (-j, e6);
        EXPECT_NE (k, e6);
        EXPECT_NE (-k, e6);

        EXPECT_NE (i, e7);
        EXPECT_NE (-i, e7);
        EXPECT_NE (j, e7);
        EXPECT_NE (-j, e7);
        EXPECT_NE (k, e7);
        EXPECT_NE (-k, e7);

        EXPECT_NE (e3, e5);
        EXPECT_NE (e3, e6);
        EXPECT_NE (e3, e7);

        EXPECT_NE (-e3, e5);
        EXPECT_NE (-e3, e6);
        EXPECT_NE (-e3, e7);

        EXPECT_NE (e5, e6);
        EXPECT_NE (e5, e7);

        EXPECT_NE (-e5, e6);
        EXPECT_NE (-e5, e7);

        EXPECT_NE (e6, e7);
        EXPECT_NE (-e6, e7);

        auto x = (i * j) * k;

        EXPECT_EQ (e7, -x);
    };

    using complex_test_cases = ::testing::Types<
        complex64, complex128,
        complex<fraction<Z>>,
        complex<fraction<Z_bytes_little>>,
        complex<fraction<Z_bytes_big>>,
        complex<fraction<Z_bytes_BC_little>>,
        complex<fraction<Z_bytes_BC_big>>,
        complex<fraction<dec_int>>,
        complex<fraction<hex_int>>,
        complex<fraction<hex_int_BC>>,
        complex<fraction<int64>>,/*
        complex<fraction<int64_little>>,
        complex<fraction<int64_big>>,*/
        complex<fraction<int128>>,
        complex<fraction<int128_little>>,
        complex<fraction<int128_big>>/*,
        fraction<complex<Z>>,
        fraction<complex<Z_bytes_little>>,
        fraction<complex<Z_bytes_big>>,
        fraction<complex<Z_bytes_BC_little>>,
        fraction<complex<Z_bytes_BC_big>>,
        fraction<complex<int64>>,
        fraction<complex<int64_little>>,
        fraction<complex<int64_big>>,
        fraction<complex<int128>>,
        fraction<complex<int128_little>>,
        fraction<complex<int128_big>>*/>;

    using quat_test_cases = ::testing::Types<
        quat128, quat256,
        quaternion<fraction<Z>>,
        quaternion<fraction<Z_bytes_little>>,
        quaternion<fraction<Z_bytes_big>>,
        quaternion<fraction<Z_bytes_BC_little>>,
        quaternion<fraction<Z_bytes_BC_big>>,
        quaternion<fraction<dec_int>>,
        quaternion<fraction<hex_int>>,
        quaternion<fraction<hex_int_BC>>,
        quaternion<fraction<int64>>,/*
        quaternion<fraction<int64_little>>,
        quaternion<fraction<int64_big>>,*/
        quaternion<fraction<int128>>,
        quaternion<fraction<int128_little>>,
        quaternion<fraction<int128_big>>/*,
        fraction<quaternion<Z>>,
        fraction<quaternion<Z_bytes_little>>,
        fraction<quaternion<Z_bytes_big>>,
        fraction<quaternion<Z_bytes_BC_little>>,
        fraction<quaternion<Z_bytes_BC_big>>,
        fraction<quaternion<int64>>,
        fraction<quaternion<int64_little>>,
        fraction<quaternion<int64_big>>,
        fraction<quaternion<int128>>,
        fraction<quaternion<int128_little>>,
        fraction<quaternion<int128_big>>*/>;

    using oct_test_cases = ::testing::Types<
        oct256, oct512,
        octonion<fraction<Z>>,
        octonion<fraction<Z_bytes_little>>,
        octonion<fraction<Z_bytes_big>>,
        octonion<fraction<Z_bytes_BC_little>>,
        octonion<fraction<Z_bytes_BC_big>>,
        octonion<fraction<dec_int>>,
        octonion<fraction<hex_int>>,
        octonion<fraction<hex_int_BC>>,
        octonion<fraction<int64>>,/*
        octonion<fraction<int64_little>>,
        octonion<fraction<int64_big>>,*/
        octonion<fraction<int128>>,
        octonion<fraction<int128_little>>,
        octonion<fraction<int128_big>>/*,
        fraction<octonion<Z>>,
        fraction<octonion<Z_bytes_little>>,
        fraction<octonion<Z_bytes_big>>,
        fraction<octonion<Z_bytes_BC_little>>,
        fraction<octonion<Z_bytes_BC_big>>,
        fraction<octonion<int64>>,
        fraction<octonion<int64_little>>,
        fraction<octonion<int64_big>>,
        fraction<octonion<int128>>,
        fraction<octonion<int128_little>>,
        fraction<octonion<int128_big>>*/>;
}

namespace data {

    template <typename N>
    struct Complex : ::testing::Test {
        using complex = N;
    };

    TYPED_TEST_SUITE (Complex, math::complex_test_cases);

    TYPED_TEST (Complex, Complex) {
        using C = typename TestFixture::complex;
        test_complex<C> (C {0}, C {1}, C::I ());
    }

    template <typename N>
    struct Quaternion : ::testing::Test {
        using quat = N;
    };

    TYPED_TEST_SUITE (Quaternion, math::quat_test_cases);

    TYPED_TEST (Quaternion, Quaternion) {
        using H = typename TestFixture::quat;
        test_quaternion<H> (H {0}, H {1}, H::I (), H::J ());
    }

    template <typename N>
    struct Octonion : ::testing::Test {
        using oct = N;
    };

    TYPED_TEST_SUITE (Octonion, math::oct_test_cases);

    TYPED_TEST (Octonion, Octonion) {
        using O = typename TestFixture::oct;
        test_octonion<O> (O {0}, O {1}, O::E1 (), O::E2 (), O::E4 ());
    }

}
