// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math.hpp>
#include <data/math/fraction.hpp>
#include <data/tuple.hpp>

#include <gtest/gtest.h>

namespace data::math {

    static_assert (Real<fraction<Z>>);
    static_assert (Real<fraction<Z_bytes_little>>);
    static_assert (Real<fraction<Z_bytes_big>>);
    static_assert (Real<fraction<Z_bytes_BC_little>>);
    static_assert (Real<fraction<Z_bytes_BC_big>>);
    static_assert (Real<fraction<int64>>);
    static_assert (Real<fraction<int128>>);
    static_assert (Real<fraction<int128_little>>);
    static_assert (Real<fraction<int128_big>>);
    static_assert (Real<fraction<dec_int>>);
    static_assert (Real<fraction<hex_int>>);
    static_assert (Real<fraction<hex_int_BC>>);

    static_assert (Complex<complex<fraction<Z>>>);
    static_assert (Complex<complex<fraction<Z_bytes_little>>>);
    static_assert (Complex<complex<fraction<Z_bytes_big>>>);
    static_assert (Complex<complex<fraction<Z_bytes_BC_little>>>);
    static_assert (Complex<complex<fraction<Z_bytes_BC_big>>>);
    static_assert (Complex<complex<fraction<int64>>>);
    static_assert (Complex<complex<fraction<int128>>>);
    static_assert (Complex<complex<fraction<int128_little>>>);
    static_assert (Complex<complex<fraction<int128_big>>>);
    static_assert (Complex<complex<fraction<dec_int>>>);
    static_assert (Complex<complex<fraction<hex_int>>>);
    static_assert (Complex<complex<fraction<hex_int_BC>>>);

    static_assert (Complex<fraction<complex<Z>>>);
    static_assert (Complex<fraction<complex<Z_bytes_little>>>);
    static_assert (Complex<fraction<complex<Z_bytes_big>>>);
    static_assert (Complex<fraction<complex<Z_bytes_BC_little>>>);
    static_assert (Complex<fraction<complex<Z_bytes_BC_big>>>);
    static_assert (Complex<fraction<complex<int64>>>);
    static_assert (Complex<fraction<complex<int128>>>);
    static_assert (Complex<fraction<complex<int128_little>>>);
    static_assert (Complex<fraction<complex<int128_big>>>);
    static_assert (Complex<fraction<complex<dec_int>>>);
    static_assert (Complex<fraction<complex<hex_int>>>);
    static_assert (Complex<fraction<complex<hex_int_BC>>>);

    static_assert (Quaternionic<quaternion<fraction<Z>>>);
    static_assert (Quaternionic<quaternion<fraction<Z_bytes_little>>>);
    static_assert (Quaternionic<quaternion<fraction<Z_bytes_big>>>);
    static_assert (Quaternionic<quaternion<fraction<Z_bytes_BC_little>>>);
    static_assert (Quaternionic<quaternion<fraction<Z_bytes_BC_big>>>);
    static_assert (Quaternionic<quaternion<fraction<int64>>>);
    static_assert (Quaternionic<quaternion<fraction<int128>>>);
    static_assert (Quaternionic<quaternion<fraction<int128_little>>>);
    static_assert (Quaternionic<quaternion<fraction<int128_big>>>);
    static_assert (Quaternionic<quaternion<fraction<dec_int>>>);
    static_assert (Quaternionic<quaternion<fraction<hex_int>>>);
    static_assert (Quaternionic<quaternion<fraction<hex_int_BC>>>);

    static_assert (Quaternionic<fraction<quaternion<Z>>>);
    static_assert (Quaternionic<fraction<quaternion<Z_bytes_little>>>);
    static_assert (Quaternionic<fraction<quaternion<Z_bytes_big>>>);
    static_assert (Quaternionic<fraction<quaternion<Z_bytes_BC_little>>>);
    static_assert (Quaternionic<fraction<quaternion<Z_bytes_BC_big>>>);
    static_assert (Quaternionic<fraction<quaternion<int64>>>);
    static_assert (Quaternionic<fraction<quaternion<int128>>>);
    static_assert (Quaternionic<fraction<quaternion<int128_little>>>);
    static_assert (Quaternionic<fraction<quaternion<int128_big>>>);
    static_assert (Quaternionic<fraction<quaternion<dec_int>>>);
    static_assert (Quaternionic<fraction<quaternion<hex_int>>>);
    static_assert (Quaternionic<fraction<quaternion<hex_int_BC>>>);

    static_assert (Octonionic<octonion<fraction<Z>>>);
    static_assert (Octonionic<octonion<fraction<Z_bytes_little>>>);
    static_assert (Octonionic<octonion<fraction<Z_bytes_big>>>);
    static_assert (Octonionic<octonion<fraction<Z_bytes_BC_little>>>);
    static_assert (Octonionic<octonion<fraction<Z_bytes_BC_big>>>);
    static_assert (Octonionic<octonion<fraction<int64>>>);
    static_assert (Octonionic<octonion<fraction<int128>>>);
    static_assert (Octonionic<octonion<fraction<int128_little>>>);
    static_assert (Octonionic<octonion<fraction<int128_big>>>);
    static_assert (Octonionic<octonion<fraction<dec_int>>>);
    static_assert (Octonionic<octonion<fraction<hex_int>>>);
    static_assert (Octonionic<octonion<fraction<hex_int_BC>>>);

    static_assert (Octonionic<fraction<octonion<Z>>>);
    static_assert (Octonionic<fraction<octonion<Z_bytes_little>>>);
    static_assert (Octonionic<fraction<octonion<Z_bytes_big>>>);
    static_assert (Octonionic<fraction<octonion<Z_bytes_BC_little>>>);
    static_assert (Octonionic<fraction<octonion<Z_bytes_BC_big>>>);
    static_assert (Octonionic<fraction<octonion<int64>>>);
    static_assert (Octonionic<fraction<octonion<int128>>>);
    static_assert (Octonionic<fraction<octonion<int128_little>>>);
    static_assert (Octonionic<fraction<octonion<int128_big>>>);
    static_assert (Octonionic<fraction<octonion<dec_int>>>);
    static_assert (Octonionic<fraction<octonion<hex_int>>>);
    static_assert (Octonionic<fraction<octonion<hex_int_BC>>>);
}

namespace data {

    template <typename Q> void test_divide_by_zero () {

        EXPECT_THROW (Q {1} / Q {0}, math::division_by_zero);

    }

    template <typename Q> void test_lowest_terms () {

        EXPECT_EQ (Q {14} / Q {6}, Q {7} / Q {3});

    }

    template <typename Q> void test_compare () {}

    template <typename Q> void test_arithmetic () {}

    template <typename num, typename denum>
    requires requires (const num &z, const num &n) {
        { math::over<num, denum> (z, n) } -> Same<math::fraction<num, denum>>;
    } && math::rational<math::fraction<num, denum>> struct test_fraction {
        void operator () () {
            using Q = math::fraction<num, denum>;

            EXPECT_TRUE (Q {0}.valid ());
            EXPECT_TRUE (Q {1}.valid ());
            EXPECT_TRUE (Q {-1}.valid ());

            test_divide_by_zero<Q> ();
            test_lowest_terms<Q> ();
            test_compare<Q> ();
            test_arithmetic<Q> ();
        }
    };

    template <typename Q>
    concept RationalBasic =
    requires (Q q) {
        { Q {0} };
        { Q {1} };
        { -q } -> Same<Q>;
        { ~q } -> Same<Q>;

        { q + q } -> Same<Q>;
        { q - q } -> Same<Q>;
        { q * q } -> Same<Q>;
        { q / q } -> Same<Q>;

        { q += q } -> Same<Q&>;
        { q -= q } -> Same<Q&>;
        { q *= q } -> Same<Q&>;
        { q /= q } -> Same<Q&>;

        { q == q } -> ImplicitlyConvertible<bool>;
        { q != q } -> ImplicitlyConvertible<bool>;
        { is_whole (q) } -> ImplicitlyConvertible<bool>;
    };

    template <typename Q>
    concept RationalOrdered =
    requires (Q q) {
        { q < q } -> ImplicitlyConvertible<bool>;
        { q <= q } -> ImplicitlyConvertible<bool>;
        { q > q } -> ImplicitlyConvertible<bool>;
        { q >= q } -> ImplicitlyConvertible<bool>;

        { abs (q) } -> ImplicitlyConvertible<Q>;
        { sign (q) } -> ImplicitlyConvertible<math::sign>;
    };

    template <typename Q>
    concept RationalWithLiterals =
    requires (Q q) {
        { q + 1 } -> Same<Q>;
        { 1 + q } -> Same<Q>;

        { q - 1 } -> Same<Q>;
        { 1 - q } -> Same<Q>;

        { q * 2 } -> Same<Q>;
        { 2 * q } -> Same<Q>;

        { q / 2 } -> Same<Q>;
        { 2 / q } -> Same<Q>;
    };

    template <typename Q>
    concept RationalConstructible =
    requires {
        Q {0};
        Q {1};
        Q {0, 1};
        Q {1, 2};
    };

    template <typename N, typename D>
    concept RationalConstruction =
    requires (N n, N d) {
        { math::over (n, d) } -> Same<math::fraction<N, D>>;
    };

    template <typename N, typename D>
    struct Fraction {
        using num = N;
        using den = D;

        using Q = math::fraction<num, den>;

        static_assert (!WholeNumber<Q>);
        static_assert (math::Field<Q>);
        static_assert (RationalConstruction<num, den>);
        static_assert (RationalConstructible<Q>);
        static_assert (RationalBasic<Q>);
        static_assert (RationalWithLiterals<Q>);

        static_assert (requires (Q q) {
            { numerator (q) } -> ImplicitlyConvertible<num>;
            { denominator (q) } -> ImplicitlyConvertible<den>;
        });

        static_assert (requires (Q q) {
            { round (q) } -> ImplicitlyConvertible<num>;
            { frac (q) } -> ImplicitlyConvertible<Q>;
        });

        static_assert (requires (Q q, den d) {
            { pow (q, d) } -> ImplicitlyConvertible<Q>;
        });

        static_assert (requires (Q q) {
            { math::re (q) } -> ImplicitlyConvertible<Q>;
            { math::im (q) } -> ImplicitlyConvertible<Q>;
            { math::ev (q) } -> ImplicitlyConvertible<Q>;
            { math::od (q) } -> ImplicitlyConvertible<Q>;
            { math::conjugate (q) } -> ImplicitlyConvertible<Q>;
        });

    };

    template <typename tuple>
    struct RationalFraction : Fraction<
        typename std::tuple_element<0, tuple>::type,
        typename std::tuple_element<1, tuple>::type>, ::testing::Test {
        using num = typename std::tuple_element<0, tuple>::type;
        using den = typename std::tuple_element<1, tuple>::type;

        using Q = math::fraction<num, den>;

        static_assert (RationalOrdered<Q>);

        static_assert (requires (Q q) {
            { floor (q) } -> ImplicitlyConvertible<num>;
            { ceiling (q) } -> ImplicitlyConvertible<num>;
        });

        static_assert (requires (Q q, math::nonzero<den> d) {
            { mod (q, d) } -> ImplicitlyConvertible<Q>;
        });

    };

    template <typename tuple>
    struct ComplexRationalFraction : Fraction<
        math::complex<typename std::tuple_element<0, tuple>::type>,
        typename std::tuple_element<1, tuple>::type>, ::testing::Test {};

    template <typename tuple>
    struct QuaternionicRationalFraction : Fraction<
        math::quaternion<typename std::tuple_element<0, tuple>::type>,
        typename std::tuple_element<1, tuple>::type>, ::testing::Test {};

    template <typename tuple>
    struct OctonionicRationalFraction : Fraction<
        math::octonion<typename std::tuple_element<0, tuple>::type>,
        typename std::tuple_element<1, tuple>::type>, ::testing::Test {};

    using test_cases = ::testing::Types<
        tuple<int32, int32>,
        tuple<int32_little, int32_little>,
        tuple<int32_big, int32_big>,
        tuple<int64, int64>,
        tuple<int64_little, int64_little>,
        tuple<int64_big, int64_big>,
        tuple<int80, int80>,
        tuple<int80_little, int80_little>,
        tuple<int80_big, int80_big>,
        tuple<int128, int128>,
        tuple<int128_little, int128_little>,
        tuple<int128_big, int128_big>,
        tuple<Z, N>,
        tuple<Z_bytes_little, N_bytes_little>,
        tuple<Z_bytes_big, N_bytes_big>,
        tuple<Z_bytes_BC_little, Z_bytes_BC_little>,
        tuple<Z_bytes_BC_big, Z_bytes_BC_big>,
        tuple<math::Z_bytes<endian::little, unsigned short>, math::N_bytes<endian::little, unsigned short>>,
        tuple<math::Z_bytes<endian::big, unsigned int>, math::N_bytes<endian::big, unsigned int>>,
        tuple<math::Z_bytes<endian::little, unsigned long>, math::N_bytes<endian::little, unsigned long>>,
        tuple<math::Z_bytes<endian::big, unsigned long long>, math::N_bytes<endian::big, unsigned long long>>,
        tuple<math::Z_bytes_BC<endian::big, unsigned short>, math::Z_bytes_BC<endian::big, unsigned short>>,
        tuple<math::Z_bytes_BC<endian::little, unsigned int>, math::Z_bytes_BC<endian::little, unsigned int>>,
        tuple<math::Z_bytes_BC<endian::big, unsigned long>, math::Z_bytes_BC<endian::big, unsigned long>>,
        tuple<math::Z_bytes_BC<endian::little, unsigned long long>, math::Z_bytes_BC<endian::little, unsigned long long>>,
        tuple<dec_int, dec_uint>,
        tuple<hex_int, hex_uint>,
        tuple<hex_int_BC, hex_int_BC>>;

    TYPED_TEST_SUITE (RationalFraction, test_cases);
/*
    TYPED_TEST_SUITE (ComplexRationalFraction, test_cases);

    TYPED_TEST_SUITE (QuaternionicRationalFraction, test_cases);

    TYPED_TEST_SUITE (OctonionicRationalFraction, test_cases);*/

    TYPED_TEST (RationalFraction, Basics) {
        using denominator = typename TestFixture::den;
        using numerator = typename TestFixture::num;

        test_fraction<numerator, denominator> {} ();

    }
/*
    TYPED_TEST (ComplexRationalFraction, Basics) {
        using denominator = typename TestFixture::den;
        using numerator = typename TestFixture::num;

        test_fraction<numerator, denominator> {} ();

    }

    TYPED_TEST (QuaternionicRationalFraction, Basics) {
        using denominator = typename TestFixture::den;
        using numerator = typename TestFixture::num;

        test_fraction<numerator, denominator> {} ();

    }

    TYPED_TEST (OctonionicRationalFraction, Basics) {
        using denominator = typename TestFixture::den;
        using numerator = typename TestFixture::num;

        test_fraction<numerator, denominator> {} ();

    }*/

    TYPED_TEST (RationalFraction, Arithmetic) {
        using Q = typename TestFixture::Q;
        using numerator = typename TestFixture::num;
        using denominator = typename TestFixture::den;

        const Q a = math::over<numerator, denominator> (1, 2);
        const Q b = math::over<numerator, denominator> (1, 3);

        EXPECT_EQ (a + b, (math::over<numerator, denominator> (5, 6)));
        EXPECT_EQ (a - b, (math::over<numerator, denominator> (1, 6)));
        EXPECT_EQ (a * b, (math::over<numerator, denominator> (1, 6)));
        EXPECT_EQ (a / b, (math::over<numerator, denominator> (3, 2)));

        EXPECT_EQ (-a, (math::over<numerator, denominator> (-1, 2)));

        EXPECT_EQ (a + 1, (math::over<numerator, denominator> (3, 2)));
        EXPECT_EQ (1 + a, (math::over<numerator, denominator> (3, 2)));

        EXPECT_EQ (a - 1, (math::over<numerator, denominator> (-1, 2)));
        EXPECT_EQ (1 - a, (math::over<numerator, denominator> (1, 2)));

        EXPECT_EQ (a * 2, Q {1});
        EXPECT_EQ (2 * a, Q {1});

        EXPECT_EQ (a / 2, (math::over<numerator, denominator> (1, 4)));
        EXPECT_EQ (2 / a, Q {4});

    }

    TYPED_TEST (RationalFraction, Comparisons) {
        using Q = typename TestFixture::Q;
        using numerator = typename TestFixture::num;
        using denominator = typename TestFixture::den;

        const Q a = math::over<numerator, denominator> (1, 2);
        const Q b = math::over<numerator, denominator> (2, 3);

        EXPECT_LT (a, b);
        EXPECT_LE (a, b);
        EXPECT_GT (b, a);
        EXPECT_GE (b, a);

        EXPECT_EQ (a, (math::over<numerator, denominator> (2, 4)));
        EXPECT_NE (a, b);

        EXPECT_LT (-a, 0);
        EXPECT_GT (a, 0);
        EXPECT_EQ (Q {0}, 0);
        EXPECT_EQ (Q {1}, 1);

        EXPECT_LT (a, 1);
        EXPECT_LT (0, a);
        EXPECT_GT (1, a);
        EXPECT_GT (a, 0);

        EXPECT_EQ (a, (math::over<numerator, denominator> (1, 2)));
        EXPECT_EQ ((math::over<numerator, denominator> (1, 2)), a);

    }

    TYPED_TEST (RationalFraction, CielFloor) {
        using Q = typename TestFixture::Q;
        using numerator = typename TestFixture::num;
        using denominator = typename TestFixture::den;

        EXPECT_EQ ((floor (math::over<numerator, denominator> (7, 3))), 2);
        EXPECT_EQ ((floor (math::over<numerator, denominator> (-7, 3))), -3);
        EXPECT_EQ ((floor (math::over<numerator, denominator> (6, 3))), 2);
        EXPECT_EQ ((floor (math::over<numerator, denominator> (-6, 3))), -2);

        EXPECT_EQ ((ceiling (math::over<numerator, denominator> (7, 3))), 3);
        EXPECT_EQ ((ceiling (math::over<numerator, denominator> (-7, 3))), -2);
        EXPECT_EQ ((ceiling (math::over<numerator, denominator> (6, 3))), 2);
        EXPECT_EQ ((ceiling (math::over<numerator, denominator> (-6, 3))), -2);

    }

    TYPED_TEST (RationalFraction, FractionalPart) {
        using Q = typename TestFixture::Q;
        using numerator = typename TestFixture::num;
        using denominator = typename TestFixture::den;

        EXPECT_EQ (frac (Q {0}), 0);

        EXPECT_EQ ((frac (math::over<numerator, denominator> (1, 3))), (math::over<numerator, denominator> (1, 3)));
        EXPECT_EQ ((frac (math::over<numerator, denominator> (2, 3))), (math::over<numerator, denominator> (2, 3)));
        EXPECT_EQ ((frac (math::over<numerator, denominator> (4, 3))), (math::over<numerator, denominator> (1, 3)));
        EXPECT_EQ ((frac (math::over<numerator, denominator> (5, 3))), (math::over<numerator, denominator> (2, 3)));

        EXPECT_EQ ((frac (math::over<numerator, denominator> (-1, 3))), (math::over<numerator, denominator> (2, 3)));
        EXPECT_EQ ((frac (math::over<numerator, denominator> (-2, 3))), (math::over<numerator, denominator> (1, 3)));
        EXPECT_EQ ((frac (math::over<numerator, denominator> (-4, 3))), (math::over<numerator, denominator> (2, 3)));
        EXPECT_EQ ((frac (math::over<numerator, denominator> (-5, 3))), (math::over<numerator, denominator> (1, 3)));

        EXPECT_EQ (frac (Q {1}), 0);
        EXPECT_EQ (frac (Q {-1}), 0);
        EXPECT_EQ (frac (Q {10}), 0);
        EXPECT_EQ (frac (Q {-10}), 0);

    }

    TYPED_TEST (RationalFraction, Round) {
        using Q = typename TestFixture::Q;
        using numerator = typename TestFixture::num;
        using denominator = typename TestFixture::den;

        EXPECT_EQ (round (Q {0}), 0);

        EXPECT_EQ ((round (math::over<numerator, denominator> (1, 4))), 0);
        EXPECT_EQ ((round (math::over<numerator, denominator> (2, 5))), 0);
        EXPECT_EQ ((round (math::over<numerator, denominator> (1, 2))), 0);

        EXPECT_EQ ((round (math::over<numerator, denominator> (3, 5))), 1);
        EXPECT_EQ ((round (math::over<numerator, denominator> (3, 4))), 1);

        EXPECT_EQ ((round (math::over<numerator, denominator> (3, 2))), 2);
        EXPECT_EQ ((round (math::over<numerator, denominator> (5, 2))), 2);

        EXPECT_EQ ((round (math::over<numerator, denominator> (7, 2))), 4);
        EXPECT_EQ ((round (math::over<numerator, denominator> (9, 2))), 4);

        EXPECT_EQ ((round (math::over<numerator, denominator> (-1, 2))), 0);
        EXPECT_EQ ((round (math::over<numerator, denominator> (-3, 2))), -2);
        EXPECT_EQ ((round (math::over<numerator, denominator> (-5, 2))), -2);
        EXPECT_EQ ((round (math::over<numerator, denominator> (-7, 2))), -4);
        EXPECT_EQ ((round (math::over<numerator, denominator> (-9, 2))), -4);
        EXPECT_EQ ((round (math::over<numerator, denominator> (-11, 2))), -6);

        EXPECT_EQ ((round (math::over<numerator, denominator> (4, 2) - math::over<numerator, denominator> (1, 100))), 2);
        EXPECT_EQ ((round (math::over<numerator, denominator> (4, 2) + math::over<numerator, denominator> (1, 100))), 2);

        EXPECT_EQ ((round (math::over<numerator, denominator> (199, 100))), 2);
        EXPECT_EQ ((round (math::over<numerator, denominator> (201, 100))), 2);

        EXPECT_EQ ((round (math::over<numerator, denominator> (299, 100))), 3);
        EXPECT_EQ ((round (math::over<numerator, denominator> (301, 100))), 3);

    }

    TYPED_TEST (RationalFraction, Mod) {
        using Q = typename TestFixture::Q;
        using numerator = typename TestFixture::num;
        using denominator = typename TestFixture::den;

        const math::nonzero<denominator> n {denominator {3}};

        EXPECT_EQ (mod (Q {0}, n), Q {0});

        EXPECT_EQ (mod (Q {1}, n), Q {1});
        EXPECT_EQ (mod (Q {2}, n), Q {2});
        EXPECT_EQ (mod (Q {3}, n), Q {0});
        EXPECT_EQ (mod (Q {4}, n), Q {1});
        EXPECT_EQ (mod (Q {5}, n), Q {2});

        EXPECT_EQ (mod (Q {-1}, n), Q {2});
        EXPECT_EQ (mod (Q {-2}, n), Q {1});
        EXPECT_EQ (mod (Q {-3}, n), Q {0});
        EXPECT_EQ (mod (Q {-4}, n), Q {2});
        EXPECT_EQ (mod (Q {-5}, n), Q {1});

        EXPECT_EQ ((mod (math::over<numerator, denominator> (1, 2), n)),
            (math::over<numerator, denominator> (1, 2)));

        EXPECT_EQ ((mod (math::over<numerator, denominator> (7, 2), n)),
            (math::over<numerator, denominator> (1, 2)));

        EXPECT_EQ ((mod (math::over<numerator, denominator> (-1, 2), n)),
            (math::over<numerator, denominator> (5, 2)));

        EXPECT_EQ ((mod (math::over<numerator, denominator> (-7, 2), n)),
            (math::over<numerator, denominator> (5, 2)));

    }

    using fixed_test_cases = ::testing::Types<
        tuple<int32, int32>,
        tuple<int32_little, int32_little>,
        //tuple<int32_big, int32_big>,
        tuple<int64, int64>,
        //tuple<int64_little, int64_little>,
        tuple<int64_big, int64_big>/*,
        tuple<int128, int128>,
        //tuple<int128_little, int128_little>,
        tuple<int128_big, int128_big>*/>;

    template <typename tuple>
    struct FixedRationalFraction : RationalFraction<tuple> {
        static_assert (Same<typename RationalFraction<tuple>::num, typename RationalFraction<tuple>::den>);
    };

    TYPED_TEST_SUITE (FixedRationalFraction, fixed_test_cases);

    TYPED_TEST (FixedRationalFraction, Overflow) {
        using Q = typename TestFixture::Q;
        using integer = typename TestFixture::num;

        EXPECT_GT ((Q {math::numeric_limits<integer>::max (), math::numeric_limits<integer>::max () - 1}),
            (Q {math::numeric_limits<integer>::max () - 1, math::numeric_limits<integer>::max ()}));
    }

    TYPED_TEST (FixedRationalFraction, Constexpr) {

        using Q = typename TestFixture::Q;
        using integer = typename TestFixture::num;

        constexpr Q z {};
        constexpr Q a {integer {1}};
        constexpr Q b {integer {2}};
        constexpr Q c {integer {1}, integer {2}};

        // comparison and unary
        static_assert (~b == c);
        static_assert (~a == a);
        static_assert (c < b);
        static_assert (~c > a);
        static_assert (-b < c);

        // arithmetic
        static_assert (a + c == math::over (integer {3}, integer {2}));
        static_assert (b - c == math::over (integer {3}, integer {2}));
        static_assert (c * b == a);
        static_assert (a / b == c);

        // other functions
        static_assert (sign (b) == math::positive);
        static_assert (abs (b) == b);
        static_assert (abs (-b) == b);
        static_assert (floor (c) == 0);
        static_assert (ceiling (c) == 1);
        static_assert (round (c) == 0);
        static_assert (frac (c) == c);
        //static_assert (pow (c, 2) == math::over (integer {3}, integer {2}));

    }
}
