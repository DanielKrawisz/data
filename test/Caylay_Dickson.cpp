
// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/fraction.hpp>
#include <data/math.hpp>

#include <gtest/gtest.h>

namespace data::math {

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

    template <typename R>
    void test_round_integral (R unit) {
        EXPECT_EQ (round (R {0} * unit), R {0} * unit);
        EXPECT_EQ (round (R {1} * unit), R {1} * unit);
        EXPECT_EQ (round (R {-1} * unit), R {-1} * unit);
        EXPECT_EQ (round (R {2} * unit), R {2} * unit);
        EXPECT_EQ (round (R {-2} * unit), R {-2} * unit);
    }

    template <typename U>
    void test_round_fractional (U unit) {
        // Ordinary rounding.
        EXPECT_EQ (round (unit * 6 / 5), unit);
        EXPECT_EQ (round (unit * 9 / 5), unit * 2);
        EXPECT_EQ (round (unit * -6 / 5), unit * -1);
        EXPECT_EQ (round (unit * -9 / 5), unit * -2);

        // Ties to even.
        EXPECT_EQ (round (unit * 3 / 2), unit * 2);
        EXPECT_EQ (round (unit * 5 / 2), unit * 2);
        EXPECT_EQ (round (unit * -3 / 2), unit * -2);
        EXPECT_EQ (round (unit * -5 / 2), unit * -2);
    }

    template <typename X>
    void test_complex_whole (X zero, X one, X i) {
        EXPECT_EQ (zero, 0);
        EXPECT_EQ (one, 1);

        EXPECT_NE (zero, one);
        EXPECT_NE (zero, i);
        EXPECT_NE (one, i);

        EXPECT_EQ (*zero, zero);
        EXPECT_EQ (*one, one);
        EXPECT_EQ (*i, -i);

        EXPECT_EQ (zero * zero, zero);
        EXPECT_EQ (zero * one, zero);
        EXPECT_EQ (one * zero, zero);
        EXPECT_EQ (zero * i, zero);
        EXPECT_EQ (i * zero, zero);
        EXPECT_EQ (one * one, one);
        EXPECT_EQ (i * one, i);
        EXPECT_EQ (one * i, i);
        EXPECT_EQ (i * i, -one);

        EXPECT_EQ (quadrance (zero), 0);
        EXPECT_EQ (quadrance (one), 1);
        EXPECT_EQ (quadrance (i), 1);
        EXPECT_EQ (quadrance (one + one), 4);
        EXPECT_EQ (quadrance (i + i), 4);
        EXPECT_EQ (quadrance (one + i), 2);

        EXPECT_EQ (re (zero), 0);
        EXPECT_EQ (re (one), 1);
        EXPECT_EQ (re (i), 0);

        test_round_integral<X> (one);
        test_round_integral<X> (i);
    };

    template <typename X> void test_quaternion_whole (X zero, X one, X i, X j) {
        test_complex_whole<X> (zero, one, i);
        test_complex_whole<X> (zero, one, j);

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

    template <typename X> void test_octonion_whole (X zero, X one, X i, X j, X k) {
        test_quaternion_whole<X> (zero, one, i, j);
        test_quaternion_whole<X> (zero, one, j, k);
        test_quaternion_whole<X> (zero, one, k, i);

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

    using test_cases_ring = ::testing::Types<
        int32, int64, int32_little, int64_big,
        int128, int128_little, int160, int160_big,
        Z, Z_bytes_little, Z_bytes_BC_big,
        dec_int, hex_int, hex_int_BC>;

    using test_cases_field = ::testing::Types<
        float32, float64,
        fraction<int32>, fraction<int64>,
        fraction<int32_little>, fraction<int64_big>,
        fraction<int128>, fraction<int128_little>,
        fraction<int160>, fraction<int160_big>,
        fraction<Z>, fraction<Z_bytes_little>,
        fraction<Z_bytes_BC_big>,
        fraction<dec_int>, fraction<hex_int>,
        fraction<hex_int_BC>>;

}

namespace data {

    template <typename N>
    struct CayleyDicksonRing : ::testing::Test {
        using base_ring = N;
    };

    template <typename N>
    struct CayleyDicksonField : ::testing::Test {
        using base_field = N;
    };

    TYPED_TEST_SUITE (CayleyDicksonRing, math::test_cases_ring);

    TYPED_TEST_SUITE (CayleyDicksonField, math::test_cases_field);

    template <typename R, typename CR>
    concept RingSubAlgebra = ImplicitlyConvertible<R, CR> && requires {
        CR {1};
    } && requires (const R &x, const CR &z) {
        { x == z } -> ImplicitlyConvertible<bool>;
        { z == x } -> ImplicitlyConvertible<bool>;
        { x + z } -> ImplicitlyConvertible<CR>;
        { z + x } -> ImplicitlyConvertible<CR>;
        { x - z } -> ImplicitlyConvertible<CR>;
        { z - x } -> ImplicitlyConvertible<CR>;
        { x * z } -> ImplicitlyConvertible<CR>;
        { z * x } -> ImplicitlyConvertible<CR>;
    } && requires (const CR &z) {
        { 1 == z } -> ImplicitlyConvertible<bool>;
        { z == 1 } -> ImplicitlyConvertible<bool>;
        { 1 + z } -> ImplicitlyConvertible<CR>;
        { z + 1 } -> ImplicitlyConvertible<CR>;
        { 1 - z } -> ImplicitlyConvertible<CR>;
        { z - 1 } -> ImplicitlyConvertible<CR>;
        { 1 * z } -> ImplicitlyConvertible<CR>;
        { z * 1 } -> ImplicitlyConvertible<CR>;
    };

    template <typename R, typename CR>
    concept FieldSubAlgebra = RingSubAlgebra<R, CR> &&
    requires (const R &x, const CR &z) {
        { x / z } -> ImplicitlyConvertible<CR>;
        { z / x } -> ImplicitlyConvertible<CR>;
    } && requires (const CR &z) {
        { 1 / z } -> ImplicitlyConvertible<CR>;
        { z / 1 } -> ImplicitlyConvertible<CR>;
    };

    template <typename R, typename CR>
    concept RealSubAlgebra = RingSubAlgebra<R, CR> &&
    requires (const CR &z) {
        { re (z) } -> Same<R>;
    };

    template <typename R, typename CR>
    requires RealSubAlgebra<R, CR>
    void test_complex_ring () {

        EXPECT_EQ (CR {}, CR {R {}});
        EXPECT_EQ (CR {}, CR {0});
        EXPECT_EQ (CR {0}, R {});
        EXPECT_EQ (R {}, CR {0});

        math::test_complex_whole<CR> (CR {0}, CR {1}, CR::I ());
    }

    template <typename R, typename CR>
    requires FieldSubAlgebra<R, CR>
    void test_complex_field () {

        test_complex_ring<R, CR> ();

        math::test_round_fractional<CR> (CR {1});
        math::test_round_fractional<CR> (CR::I ());
    }

    TYPED_TEST (CayleyDicksonRing, Complex) {
        using R = typename TestFixture::base_ring;
        using CR = math::complex<R>;

        test_complex_ring<R, CR> ();
    }
/*
    TYPED_TEST (CayleyDicksonField, Complex) {
        using R = typename TestFixture::base_field;
        using CR = math::complex<R>;

        test_complex_field<R, CR> ();
    }*/
    // TODO uncommenting this requires a lot of work.
    // we need round before we can do this.
/*
    TYPED_TEST (CayleyDicksonRing, Rationalize) {
        using G = math::complex<typename TestFixture::base_ring>;
        using C = math::fraction<G>;

        math::test_complex_whole<C> (G {0}, G {1}, G::I ());
    }*/

    template <typename R, typename CR, typename HR>
    requires RingSubAlgebra<R, HR> && RingSubAlgebra<CR, HR>
    void test_quaternionic_ring () {

        EXPECT_EQ (HR {}, HR {R {}});
        EXPECT_EQ (HR {}, HR {CR {}});
        EXPECT_EQ (HR {}, HR {0});
        EXPECT_EQ (HR {0}, R {});
        EXPECT_EQ (R {}, HR {0});

        math::test_quaternion_whole<HR> (HR {0}, HR {1}, HR::I (), HR::J ());
    }

    template <typename R, typename CR, typename HR>
    requires FieldSubAlgebra<R, HR> && FieldSubAlgebra<CR, HR>
    void test_quaternionic_field () {

        test_quaternionic_ring<R, CR, HR> ();
    }

    TYPED_TEST (CayleyDicksonRing, Quaternion) {
        using R = typename TestFixture::base_ring;
        using CR = math::complex<R>;
        using HR = math::quaternion<R>;

        test_quaternionic_ring<R, CR, HR> ();
    }
/*
    TYPED_TEST (CayleyDicksonField, Quaternion) {
        using R = typename TestFixture::base_field;
        using CR = math::complex<R>;
        using HR = math::quaternion<R>;

        test_quaternionic_field<R, CR, HR> ();

        math::test_round_fractional<HR> (HR {1});
        math::test_round_fractional<HR> (HR::I ());
        math::test_round_fractional<HR> (HR::J ());
    }*/

    template <typename R, typename CR, typename HR, typename OR>
    requires RealSubAlgebra<R, OR> && RingSubAlgebra<CR, OR> && RingSubAlgebra<HR, OR>
    void test_octonionic_ring () {

        EXPECT_EQ (OR {}, OR {R {}});
        EXPECT_EQ (OR {}, OR {HR {}});
        EXPECT_EQ (OR {}, OR {CR {}});
        EXPECT_EQ (OR {}, R {});
        EXPECT_EQ (OR {}, OR {0});
        EXPECT_EQ (OR {0}, R {0});
        EXPECT_EQ (R {0}, OR {0});

        math::test_octonion_whole<OR> (OR {0}, OR {1}, OR::E1 (), OR::E2 (), OR::E4 ());
    }

    template <typename R, typename CR, typename HR, typename OR>
    requires RingSubAlgebra<R, OR> && RingSubAlgebra<CR, OR> && RingSubAlgebra<HR, OR>
    void test_octonionic_field () {

        test_octonionic_ring<R, CR, HR, OR> ();
    }

    TYPED_TEST (CayleyDicksonRing, Octonion) {
        using R = typename TestFixture::base_ring;
        using CR = math::complex<R>;
        using HR = math::quaternion<R>;
        using OR = math::octonion<R>;

        test_octonionic_ring<R, CR, HR, OR> ();
    }
/*
    TYPED_TEST (CayleyDicksonField, Octonion) {
        using R = typename TestFixture::base_field;
        using CR = math::complex<R>;
        using HR = math::quaternion<R>;
        using OR = math::octonion<R>;

        test_octonionic_field<R, CR, HR, OR> ();

        math::test_round_fractional<OR> (OR {1});
        math::test_round_fractional<OR> (OR::E1 ());
        math::test_round_fractional<OR> (OR::E2 ());
        math::test_round_fractional<OR> (OR::E3 ());
    }*/

    // TODO constexpr

}
