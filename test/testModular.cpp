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
            modular<square (N {5})> test_mod_square {3};
            modular<conjugate (N {17})> test_mod_conjugate {4};
            modular<abs (N {17})> test_mod_abs {5};
            modular<norm (N {17})> test_mod_norm {6};
            modular<quadrance (N {17})> test_mod_quadrance {7};
            modular<inner (N {17}, N {101})> test_mod_inner {8};
            modular<plus (times (N {2}, N {3}), minus (N {7}, N {2}))> test_mod_exp_A {9};
            modular<N {2} * N {3} + (N {7} - N {2})> test_mod_exp_B {3};
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

    TEST (TestModular, TestModularArithmetic) {

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

        /*

        test_modular<uint_big<9>> {};
        test_modular<uint_little<9>> {};
        test_modular<uint_big<10>> {};
        test_modular<uint_little<10>> {};
        test_modular<uint_big<11>> {};
        test_modular<uint_little<11>> {};
        test_modular<uint_big<12>> {};
        test_modular<uint_little<12>> {};

        test_modular<uint_big<5, short unsigned int>> {};
        test_modular<uint_little<5, short unsigned int>> {};
        test_modular<uint_big<6, short unsigned int>> {};
        test_modular<uint_little<6, short unsigned int>> {};
        test_modular<uint_big<7, short unsigned int>> {};
        test_modular<uint_little<7, short unsigned int>> {};
        test_modular<uint_big<8, short unsigned int>> {};
        test_modular<uint_little<8, short unsigned int>> {};
        test_modular<uint_big<3, unsigned int>> {};
        test_modular<uint_little<3, unsigned int>> {};
        test_modular<uint_big<4, unsigned int>> {};
        test_modular<uint_little<4, unsigned int>> {};
        test_modular<uint_big<5, unsigned int>> {};
        test_modular<uint_little<5, unsigned int>> {};
        test_modular<uint_big<6, unsigned int>> {};
        test_modular<uint_little<6, unsigned int>> {};

        // TODO bigger words
        test_modular<uint_big<2, long unsigned int>> {};
        test_modular<uint_little<2, long unsigned int>> {};
        test_modular<uint_big<3, long unsigned int>> {};
        test_modular<uint_little<3, long unsigned int>> {};
        test_modular<uint_big<4, long unsigned int>> {};
        test_modular<uint_little<4, long unsigned int>> {};
        test_modular<uint_big<5, long unsigned int>> {};
        test_modular<uint_little<5, long unsigned int>> {};
        test_modular<uint_big<1, long long unsigned int>> {};
        test_modular<uint_little<1, long long unsigned int>> {};
        test_modular<uint_big<2, long long unsigned int>> {};
        test_modular<uint_little<2, long long unsigned int>> {};
        test_modular<uint_big<3, long long unsigned int>> {};
        test_modular<uint_little<3, long long unsigned int>> {};
        test_modular<uint_big<4, long long unsigned int>> {};
        test_modular<uint_little<4, long long unsigned int>> {};*/
    }

}
