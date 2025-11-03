// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math.hpp"
#include "gtest/gtest.h"

namespace data {

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

    template <typename N, math::ring M>
    requires requires { pow<M, int>; }
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

    template <auto N> using modular = math::number::modular<N>;

    template <typename N> struct test_modular {
        test_modular () {

            // test that basic arithmetic functions are all constexpr
            modular<N {13}> test_construct {13};
            modular<square (N {5})> test_mod_square {3};
            modular<math::conjugate (N {17})> test_mod_conjugate {4};
            modular<abs (N {17})> test_mod_abs {5};
            modular<quadrance (N {17})> test_mod_quadrance {7};
            modular<math::inner (N {17}, N {101})> test_mod_inner {8};
            modular<plus (times (N {2}, N {3}), minus (N {7}, N {2}))> test_mod_arith_A {9};
            modular<N {2} * N {3} + (N {7} - N {2})> test_mod_arithB {3};
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
    struct Modular : ::testing::Test {
        using number = X;
    };

    using test_cases = ::testing::Types<
        byte, int8, uint16, int16,
        uint16_little, uint16_big,
        int16_little, int16_big,
        uint32, uint32_little, uint32_big,
        int32, int32_little, int32_big,
        uint64, uint64_little, uint64_big,
        int64, int64_little, int64_big/*,
        uint80, uint80_little, uint80_big,
        int80, int80_little, int80_big,
        uint128, uint128_little, uint128_big,
        int128, int128_little, int128_big,
        uint160, uint160_little, uint160_big,
        int160, int160_little, int160_big,
        uint224, uint224_little, uint224_big,
        int224, int224_little, int224_big,
        uint256, uint256_little, uint256_big,
        int256, int256_little, int256_big,
        uint320, uint320_little, uint320_big,
        int320, int320_little, int320_big,
        uint448, uint448_little, uint448_big,
        int448, int448_little, int448_big,
        uint512, uint512_little, uint512_big,
        int512, int512_little, int512_big*/>;

    TYPED_TEST_SUITE (Modular, test_cases);

    TYPED_TEST (Modular, TestType) {
        test_modular<typename TestFixture::number> ();
    }

}
