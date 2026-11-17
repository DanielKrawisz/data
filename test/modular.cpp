// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math.hpp>

#include <gtest/gtest.h>

namespace data {
    template <typename T>
    concept ModularNumber = requires (
        T &a,
        T b,
        int i,
        modular<int {17}, T> x,
        modular<int {17}, T> y
    ) {
        // Underlying number type.

        a % b;
        a %= b;

        // Modular with modular.

        { x == y } -> ImplicitlyConvertible<bool>;
        { y == x } -> ImplicitlyConvertible<bool>;

        { x + y } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x - y } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x * y } -> ImplicitlyConvertible<modular<int {17}, T>>;

        // Modular with underlying number.

        { x == a } -> ImplicitlyConvertible<bool>;
        { a == x } -> ImplicitlyConvertible<bool>;

        { x + a } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x - a } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x * a } -> ImplicitlyConvertible<modular<int {17}, T>>;

        { a + x } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { a - x } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { a * x } -> ImplicitlyConvertible<modular<int {17}, T>>;

        // Modular with int.

        { x == i } -> ImplicitlyConvertible<bool>;
        { i == x } -> ImplicitlyConvertible<bool>;

        { x + i } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x - i } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x * i } -> ImplicitlyConvertible<modular<int {17}, T>>;

        { i + x } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { i - x } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { i * x } -> ImplicitlyConvertible<modular<int {17}, T>>;

        { x << i } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { x >> i } -> ImplicitlyConvertible<modular<int {17}, T>>;

        { pow (x, i) } -> ImplicitlyConvertible<modular<int {17}, T>>;
        { pow (x, b) } -> ImplicitlyConvertible<modular<int {17}, T>>;
    };

    template <typename T>
    concept PrimeFieldNumber = requires (
        T &a,
        T b,
        int i,
        prime_field<int {17}, T> x,
        prime_field<int {17}, T> y
    ) {
        // Underlying number type.

        a % b;
        a %= b;

        // Modular with modular.

        { x == y } -> ImplicitlyConvertible<bool>;
        { y == x } -> ImplicitlyConvertible<bool>;

        { x + y } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x - y } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x * y } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        // Modular with underlying number.

        { x == a } -> ImplicitlyConvertible<bool>;
        { a == x } -> ImplicitlyConvertible<bool>;

        { x + a } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x - a } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x * a } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        { a + x } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { a - x } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { a * x } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        // Modular with int.

        { x == i } -> ImplicitlyConvertible<bool>;
        { i == x } -> ImplicitlyConvertible<bool>;

        { x + i } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x - i } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x * i } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        { i + x } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { i - x } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { i * x } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        { x << i } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { x >> i } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        { pow (x, i) } -> ImplicitlyConvertible<prime_field<int {17}, T>>;
        { pow (x, b) } -> ImplicitlyConvertible<prime_field<int {17}, T>>;

        x / y;
        x / b;
        x / i;
    };

    template <ModularNumber T>
    struct Modular : testing::Test {
        using N = T;
    };

    template <PrimeFieldNumber T>
    struct PrimeField : testing::Test {
        using N = T;
    };

    using ModularTypes = testing::Types<
        uint256, int256,
        N, //Z,
        Z_bytes_BC_little, Z_bytes_BC_big,
        N_bytes_little, N_bytes_big,
        math::N_bytes<endian::little, unsigned short>,
        math::N_bytes<endian::big, unsigned>,
        math::N_bytes<endian::little, unsigned long>,
        math::N_bytes<endian::big, unsigned long long>>;

    TYPED_TEST_SUITE (Modular, ModularTypes);

    TYPED_TEST_SUITE (PrimeField, ModularTypes);

    TYPED_TEST (Modular, Negate) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;
            const M a {5};
            const Z expected {12};
            const auto z = -a;

            EXPECT_EQ (z.Value, expected)
                << "expected -" << a << " -> " << expected
                << " but got " << z << " instead";
        }

        {
            const auto modulus = uint32 {3999999999};

            using M = modular<modulus, N>;

            const M a {5};

            const Z expected = mod (-Z (5), math::nonzero {Z (modulus)});
            const auto z = -a;

            EXPECT_EQ (z.Value, expected)
                << "expected -" << a << " -> " << expected
                << " but got " << z << " instead";
        }

    }

    TYPED_TEST (Modular, Add) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;
            const M a {13};
            const M b {15};
            const N expected {11};
            const auto z = a + b;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " + " << b
                << " -> " << expected << " but got " << z << " instead";
        }

        {
            using M = modular<int32 {17}, N>;
            const M a {8};
            const M b {9};
            const Z expected {0};
            const auto z = a + b;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " + " << b
                << " -> " << expected << " but got " << z << " instead";
        }
    }

    TYPED_TEST (Modular, Subtract) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;
            const M a {3};
            const M b {8};
            const Z expected {12};
            const auto z = a - b;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " - " << b
                << " -> " << expected << " but got " << z << " instead";
        }

        {
            using M = modular<int32 {19}, N>;
            const M a {15};
            const M b {4};
            const Z expected {11};
            const auto z = a - b;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " - " << b
                << " -> " << expected << " but got " << z << " instead";
        }
    }

    TYPED_TEST (Modular, Multiply) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;
            const M a {13};
            const M b {15};
            const N expected {8};
            const auto z = a * b;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " x " << b
                << " -> " << expected << " but got " << z << " instead";
        }

        {
            using M = modular<int32 {23}, N>;
            const M a {19};
            const M b {21};
            const N expected {8};
            const auto z = a * b;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " x " << b
                << " -> " << expected << " but got " << z << " instead";
        }
    }

    TYPED_TEST (Modular, ShiftLeft) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;
            const M a {5};
            const unsigned shift {2};

            const Z expected = mod (
                Z (a.Value) << shift,
                math::nonzero {Z (17)}
            );

            const auto z = a << shift;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " << " << shift
                << " -> " << expected << " but got " << z << " instead";
        }

        {
            const auto modulus = uint32 {3999999999};

            using M = modular<modulus, N>;

            const M a {123456789};
            const unsigned shift {17};

            const Z expected = mod (
                Z (a.Value) << shift,
                math::nonzero {Z (modulus)}
            );

            const auto z = a << shift;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " << " << shift
                << " -> " << expected << " but got " << z << " instead";
        }
    }

    TYPED_TEST (Modular, ShiftRight) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;
            const M a {15};
            const unsigned shift {2};

            const Z expected = Z (a.Value) >> shift;

            const auto z = a >> shift;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " >> " << shift
                << " -> " << expected << " but got " << z << " instead";
        }

        {
            const auto modulus = uint32 {3999999999};

            using M = modular<modulus, N>;

            const M a {123456789};
            const unsigned shift {17};

            const Z expected = Z (a.Value) >> shift;

            const auto z = a >> shift;

            EXPECT_EQ (z.Value, expected)
                << "expected " << a << " >> " << shift
                << " -> " << expected << " but got " << z << " instead";
        }
    }

    TYPED_TEST (Modular, Pow) {
        using N = typename TestFixture::N;

        {
            using M = modular<uint32 {17}, N>;

            const M base {3};
            const N exponent {5};

            const Z expected = pow_mod (
                Z (base.Value),
                Z (exponent),
                math::nonzero {Z (17)}
            );

            const auto z = pow (base, exponent);

            EXPECT_EQ (z.Value, expected)
                << "expected " << base << "^" << exponent
                << " mod 17 -> " << expected
                << " but got " << z << " instead";
        }

        {
            using M = modular<uint32 {3999999999}, N>;

            const M base {123456789};
            const N exponent {12345};

            const Z expected = pow_mod (
                Z (base.Value),
                Z (exponent),
                math::nonzero {Z (3999999999)}
            );

            const auto z = pow (base, exponent);

            EXPECT_EQ (z.Value, expected)
                << "expected " << base << "^" << exponent
                << " mod 3999999999 -> " << expected
                << " but got " << z << " instead";
        }

        {
            using M = modular<uint32 {17}, N>;

            const M base {16};
            const N exponent {256};

            const Z expected = pow_mod (
                Z (base.Value),
                Z (exponent),
                math::nonzero {Z (17)});

            const auto z = pow (base, exponent);

            EXPECT_EQ (z.Value, expected)
                << "expected " << base << "^" << exponent
                << " mod 17 -> " << expected
                << " but got " << z << " instead";
        }

        {
            constexpr auto modulus = (uint256 {1} << 200) - uint256 {235};

            using M = modular<modulus, N>;

            const M base {(uint256 {1} << 180) + uint256 {12345}};
            const N exponent {12345};

            const Z expected = pow_mod (
                Z (base.Value),
                Z (exponent),
                math::nonzero {Z (modulus)});

            const auto z = pow (base, exponent);

            EXPECT_EQ (z.Value, expected)
                << "expected " << base << "^" << exponent
                << " mod " << modulus << " -> " << expected
                << " but got " << z << " instead";
        }
    }

    struct division_test_case {
        int numerator;
        int denominator;
        int expected;
    };

    constexpr std::array division_test_cases {
        division_test_case { 1, 1, 1 },
        division_test_case { 1, 2, 9 },
        division_test_case { 1, 3, 6 },
        division_test_case { 1, 4, 13 },
        division_test_case { 1, 8, 15 },
        division_test_case { 2, 2, 1 },
        division_test_case { 4, 2, 2 },
        division_test_case { 6, 3, 2 },
        division_test_case { 10, 2, 5 },
        division_test_case { 16, 2, 8 },
        division_test_case { 16, 16, 1 },
        division_test_case { 0, 1, 0 },
        division_test_case { 0, 16, 0 },
        division_test_case { 1, 16, 16 },
        division_test_case { 8, 16, 9 },
        division_test_case { 16, 1, 16 },
        division_test_case { 17, 2, 0 },
        division_test_case { 18, 2, 9 },
        division_test_case { 34, 2, 0 },
        division_test_case { 35, 2, 9 }
    };

    TYPED_TEST (PrimeField, Division) {
        using N = typename TestFixture::N;

        for (const auto &test : division_test_cases) {

            using M = prime_field<uint32 {17}, N>;

            const M a {test.numerator};
            const M b {test.denominator};
            const M expected {test.expected};

            EXPECT_EQ (a / b, expected)
                << test.numerator << " / " << test.denominator;
        }
    }

    using constexpr_test_cases = ::testing::Types<
        byte, int8, uint16, int16,
        uint16_little, uint16_big,
        int16_little, int16_big,
        uint32, uint32_little, uint32_big,
        int32, int32_little, int32_big,
        uint64, uint64_little, uint64_big,
        int64, int64_little, int64_big,
        uint80, uint80_little, uint80_big,
        int80, int80_little, int80_big,
        uint128, uint128_little, uint128_big,
        int128, int128_little, int128_big,
        uint160, uint160_little, uint160_big,
        int160, int160_little, int160_big,
        uint224, uint224_little, uint224_big,
        int224, int224_little, int224_big,
        uint256, uint256_little, uint256_big,
        int256, int256_little, int256_big/*,
        uint320, uint320_little, uint320_big,
        int320, int320_little, int320_big,
        uint448, uint448_little, uint448_big,
        int448, int448_little, int448_big,
        uint512, uint512_little, uint512_big,
        int512, int512_little, int512_big*/>;

    template <typename N> N f1 () {
        return N {1} + N {1};
    }

    template <typename N> N f2 () {
        return N {2} + N {3};
    }

    template <typename N> N f3 () {
        return pow (N {2}, 5);
    }

    template <typename N> N f4 () {
        return pow (N {3}, 3);
    }

    template <typename N> N f5 () {
        return N {5} * N {17};
    }

    template <typename N> N f6 () {
        return pow (N {3}, 7);
    }

    template <typename N, math::Ring M>
    requires RingNumber<M> &&
    requires { pow<M, int>; }
    void test_suite () {

        EXPECT_EQ (M (N {0}), M {0});

        auto f1m = f1<M> ();
        M f1nm (f1<N> ());

        EXPECT_EQ (f1nm, f1m) << "expected " << std::hex << f1nm << " to equal " << f1m;

        auto f2m = f2<M> ();
        M f2nm (f2<N> ());
        EXPECT_EQ (f2nm, f2m) << "expected " << f2nm << " to equal " << f2m;

        auto f3m = f3<M> ();
        M f3nm (f3<N> ());
        EXPECT_EQ (f3nm, f3m) << "expected " << f3nm << " to equal " << f3m;

        auto f4m = f4<M> ();
        M f4nm (f4<N> ());
        EXPECT_EQ (f4nm, f4m) << "expected " << f4nm << " to equal " << f4m;

        auto f5m = f5<M> ();
        M f5nm (f5<N> ());
        EXPECT_EQ (f5nm, f5m) << "expected " << f5nm << " to equal " << f5m;

    }

    template <RingNumber N>
    requires requires {
        typename math::numeric_limits<N>;
    } struct test_modular {
        test_modular () {

            // test that basic arithmetic functions are all constexpr
            modular<N {13}> test_construct {13};
            modular<square (N {5})> test_mod_square {3};
            modular<math::conjugate (N {17})> test_mod_conjugate {4};
            modular<abs (N {17})> test_mod_abs {5};
            modular<quadrance (N {17})> test_mod_quadrance {7};
            modular<math::inner (N {17}, N {101})> test_mod_inner {8};
            modular<plus (times (N {2}, N {3}), minus (N {7}, N {2}))> test_mod_arith_A {9};
            modular<N {2} * N {3} + (N {7} - N {2})> test_mod_arith_B {3};
            modular<(N {13} / N {5}) % N {17}> test_mod_exp_C {3};
            modular<~(-N {13})> test_mod_exp_D {3};
            modular<bit_not (negate (N {13}))> test_mod_exp_E {3};
            modular<mod (bit_xor (negate (N {17}), negate (N {19})), math::nonzero<N> {N {37}})> test_mod_exp_F {4};
            modular<GCD (N {21}, N {14})> test_mod_GCD {3};

            test_suite<N, modular<N {2}>> ();
            test_suite<N, modular<N {3}>> ();
            test_suite<N, modular<N {4}>> ();
            test_suite<N, modular<N {5}>> ();
            test_suite<N, modular<N {6}>> ();
            test_suite<N, modular<N {7}>> ();
            test_suite<N, modular<N {8}>> ();
            test_suite<N, modular<N {9}>> ();
            test_suite<N, modular<N {10}>> ();
            test_suite<N, modular<N {11}>> ();
            test_suite<N, modular<N {12}>> ();
            test_suite<N, modular<N {13}>> ();
            test_suite<N, modular<N {14}>> ();
            test_suite<N, modular<N {15}>> ();
            test_suite<N, modular<N {16}>> ();
            test_suite<N, modular<N {17}>> ();
            test_suite<N, modular<N {18}>> ();
            test_suite<N, modular<N {19}>> ();

        }
    };

    template <typename X>
    struct ModularConstexpr : ::testing::Test {
        using number = X;
    };

    TYPED_TEST_SUITE (ModularConstexpr, constexpr_test_cases);

    TYPED_TEST (ModularConstexpr, TestType) {
        test_modular<typename TestFixture::number> ();
    }

}
