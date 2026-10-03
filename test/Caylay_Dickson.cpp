
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

    template <typename X>
    void test_complex (X zero, X one, X i) {
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

    using test_cases = ::testing::Types<
        float32, float64,
        int32, int64, int32_little, int64_big,
        int128, int128_little, int160, int160_big,
        Z, Z_bytes_little, Z_bytes_BC_big,
        dec_int, hex_int, hex_int_BC,
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
    struct CayleyDickson : ::testing::Test {
        using base_ring = N;
    };

    TYPED_TEST_SUITE (CayleyDickson, math::test_cases);

    TYPED_TEST (CayleyDickson, Complex) {
        using R = typename TestFixture::base_ring;
        using CR = math::complex<R>;

        static_assert (requires {
            CR {1};
        });

        static_assert (ImplicitlyConvertible<R, CR>);

        static_assert (requires (const R &x, const CR &z) {
            { x == z } -> ImplicitlyConvertible<bool>;
            { z == x } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const CR &z) {
            { 1 == z } -> ImplicitlyConvertible<bool>;
            { z == 1 } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const R &x, const CR &z) {
            { x + z } -> ImplicitlyConvertible<CR>;
            { z + x } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const R &x, const CR &z) {
            { x - z } -> ImplicitlyConvertible<CR>;
            { z - x } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const R &x, const CR &z) {
            { x * z } -> ImplicitlyConvertible<CR>;
            { z * x } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const R &x, const CR &z) {
            { x / z } -> ImplicitlyConvertible<CR>;
            { z / x } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const CR &z) {
            { 1 + z } -> ImplicitlyConvertible<CR>;
            { z + 1 } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const CR &z) {
            { 1 - z } -> ImplicitlyConvertible<CR>;
            { z - 1 } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const CR &z) {
            { 1 * z } -> ImplicitlyConvertible<CR>;
            { z * 1 } -> ImplicitlyConvertible<CR>;
        });

        static_assert (requires (const CR &z) {
            { 1 / z } -> ImplicitlyConvertible<CR>;
            { z / 1 } -> ImplicitlyConvertible<CR>;
        });

        EXPECT_EQ (CR {}, CR {R {}});
        EXPECT_EQ (CR {}, CR {0});
        EXPECT_EQ (CR {0}, R {});
        EXPECT_EQ (R {}, CR {0});

        test_complex<CR> (CR {0}, CR {1}, CR::I ());
    }

    TYPED_TEST (CayleyDickson, ComplexDivMod) {
        //TODO
    }

    TYPED_TEST (CayleyDickson, Quaternion) {
        using R = typename TestFixture::base_ring;
        using CR = math::complex<R>;
        using HR = math::quaternion<R>;

        static_assert (requires {
            HR {1};
        });

        static_assert (ImplicitlyConvertible<R, HR>);
        static_assert (ImplicitlyConvertible<CR, HR>);

        static_assert (requires (const R &x, const HR &z) {
            { x == z } -> ImplicitlyConvertible<bool>;
            { z == x } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const CR &x, const HR &z) {
            { x == z } -> ImplicitlyConvertible<bool>;
            { z == x } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const HR &z) {
            { 1 == z } -> ImplicitlyConvertible<bool>;
            { z == 1 } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const R &x, const HR &z) {
            { x + z } -> ImplicitlyConvertible<HR>;
            { z + x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const R &x, const HR &z) {
            { x - z } -> ImplicitlyConvertible<HR>;
            { z - x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const R &x, const HR &z) {
            { x * z } -> ImplicitlyConvertible<HR>;
            { z * x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const R &x, const HR &z) {
            { x / z } -> ImplicitlyConvertible<HR>;
            { z / x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const CR &x, const HR &z) {
            { x + z } -> ImplicitlyConvertible<HR>;
            { z + x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const CR &x, const HR &z) {
            { x - z } -> ImplicitlyConvertible<HR>;
            { z - x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const CR &x, const HR &z) {
            { x * z } -> ImplicitlyConvertible<HR>;
            { z * x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const CR &x, const HR &z) {
            { x / z } -> ImplicitlyConvertible<HR>;
            { z / x } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const HR &z) {
            { 1 + z } -> ImplicitlyConvertible<HR>;
            { z + 1 } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const HR &z) {
            { 1 - z } -> ImplicitlyConvertible<HR>;
            { z - 1 } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const HR &z) {
            { 1 * z } -> ImplicitlyConvertible<HR>;
            { z * 1 } -> ImplicitlyConvertible<HR>;
        });

        static_assert (requires (const HR &z) {
            { 1 / z } -> ImplicitlyConvertible<HR>;
            { z / 1 } -> ImplicitlyConvertible<HR>;
        });

        EXPECT_EQ (HR {}, HR {R {}});
        EXPECT_EQ (HR {}, HR {CR {}});
        EXPECT_EQ (HR {}, HR {0});
        EXPECT_EQ (HR {0}, R {});
        EXPECT_EQ (R {}, HR {0});

        test_quaternion<HR> (HR {0}, HR {1}, HR::I (), HR::J ());
    }

    TYPED_TEST (CayleyDickson, Octonion) {
        using R = typename TestFixture::base_ring;
        using CR = math::complex<R>;
        using HR = math::quaternion<R>;
        using OR = math::octonion<R>;

        static_assert (requires {
            OR {1};
        });

        static_assert (ImplicitlyConvertible<R, OR>);
        static_assert (ImplicitlyConvertible<CR, OR>);
        static_assert (ImplicitlyConvertible<HR, OR>);

        static_assert (requires (const R &x, const OR &z) {
            { x == z } -> ImplicitlyConvertible<bool>;
            { z == x } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const CR &x, const OR &z) {
            { x == z } -> ImplicitlyConvertible<bool>;
            { z == x } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const HR &x, const OR &z) {
            { x == z } -> ImplicitlyConvertible<bool>;
            { z == x } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const OR &z) {
            { 1 == z } -> ImplicitlyConvertible<bool>;
            { z == 1 } -> ImplicitlyConvertible<bool>;
        });

        static_assert (requires (const R &x, const OR &z) {
            { x + z } -> ImplicitlyConvertible<OR>;
            { z + x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const R &x, const OR &z) {
            { x - z } -> ImplicitlyConvertible<OR>;
            { z - x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const R &x, const OR &z) {
            { x * z } -> ImplicitlyConvertible<OR>;
            { z * x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const R &x, const OR &z) {
            { x / z } -> ImplicitlyConvertible<OR>;
            { z / x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const CR &x, const OR &z) {
            { x + z } -> ImplicitlyConvertible<OR>;
            { z + x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const CR &x, const OR &z) {
            { x - z } -> ImplicitlyConvertible<OR>;
            { z - x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const CR &x, const OR &z) {
            { x * z } -> ImplicitlyConvertible<OR>;
            { z * x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const CR &x, const OR &z) {
            { x / z } -> ImplicitlyConvertible<OR>;
            { z / x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const HR &x, const OR &z) {
            { x + z } -> ImplicitlyConvertible<OR>;
            { z + x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const HR &x, const OR &z) {
            { x - z } -> ImplicitlyConvertible<OR>;
            { z - x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const HR &x, const OR &z) {
            { x * z } -> ImplicitlyConvertible<OR>;
            { z * x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const HR &x, const OR &z) {
            { x / z } -> ImplicitlyConvertible<OR>;
            { z / x } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const OR &z) {
            { 1 + z } -> ImplicitlyConvertible<OR>;
            { z + 1 } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const OR &z) {
            { 1 - z } -> ImplicitlyConvertible<OR>;
            { z - 1 } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const OR &z) {
            { 1 * z } -> ImplicitlyConvertible<OR>;
            { z * 1 } -> ImplicitlyConvertible<OR>;
        });

        static_assert (requires (const OR &z) {
            { 1 / z } -> ImplicitlyConvertible<OR>;
            { z / 1 } -> ImplicitlyConvertible<OR>;
        });

        EXPECT_EQ (OR {}, OR {R {}});
        EXPECT_EQ (OR {}, OR {HR {}});
        EXPECT_EQ (OR {}, OR {CR {}});
        EXPECT_EQ (OR {}, R {});
        EXPECT_EQ (OR {}, OR {0});
        EXPECT_EQ (OR {0}, R {0});
        EXPECT_EQ (R {0}, OR {0});

        test_octonion<OR> (OR {0}, OR {1}, OR::E1 (), OR::E2 (), OR::E4 ());
    }

}
