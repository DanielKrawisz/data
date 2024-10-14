// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/modular.hpp"
#include "gtest/gtest.h"

namespace data {
    
    // representations of numbers. 
    constexpr auto d2 = decimal {"2"};
    constexpr auto d3 = decimal {"3"};
    constexpr auto d4 = decimal {"4"};
    constexpr auto d5 = decimal {"5"};
    constexpr auto d6 = decimal {"6"};
    constexpr auto d7 = decimal {"7"};
    constexpr auto d8 = decimal {"8"};
    constexpr auto d9 = decimal {"9"};
    constexpr auto d10 = decimal {"10"};
    constexpr auto d11 = decimal {"11"};
    constexpr auto d12 = decimal {"12"};
    constexpr auto d13 = decimal {"13"};
    constexpr auto d14 = decimal {"14"};
    constexpr auto d15 = decimal {"15"};
    constexpr auto d16 = decimal {"16"};
    constexpr auto d17 = decimal {"17"};
    constexpr auto d18 = decimal {"18"};
    constexpr auto d19 = decimal {"19"};

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
        return pow (N {3}, 7);
    }

    template <typename N> N f5 () {
        return N {5} * N {17};
    }

    template <typename N, typename M> void test_suite () {

        EXPECT_EQ (M (N {0}), M {0});

        auto f1m = f1<M> ();
        M f1nm (f1<N> ());
        std::cout << " comparing " << f1nm << " to " << f1m << std::endl;
        EXPECT_EQ (f1nm, f1m) << "expected " << f1nm << " to equal " << f1m;

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
    
    template <typename N> struct test_modular {
        test_modular () {

            test_suite<N, math::number::modular<N, d2>> ();
            test_suite<N, math::number::modular<N, d3>> ();
            test_suite<N, math::number::modular<N, d4>> ();
            test_suite<N, math::number::modular<N, d5>> ();
            test_suite<N, math::number::modular<N, d6>> ();
            test_suite<N, math::number::modular<N, d7>> ();
            test_suite<N, math::number::modular<N, d8>> ();
            test_suite<N, math::number::modular<N, d9>> ();
            test_suite<N, math::number::modular<N, d10>> ();
            test_suite<N, math::number::modular<N, d11>> ();
            test_suite<N, math::number::modular<N, d12>> ();
            test_suite<N, math::number::modular<N, d13>> ();
            test_suite<N, math::number::modular<N, d14>> ();
            test_suite<N, math::number::modular<N, d15>> ();
            test_suite<N, math::number::modular<N, d16>> ();
            test_suite<N, math::number::modular<N, d17>> ();
            test_suite<N, math::number::modular<N, d18>> ();
            test_suite<N, math::number::modular<N, d19>> ();

        }
    };
    
    TEST (TestModular, TestModularArithmetic) {

        test_modular<uint64> {};
        test_modular<N> {};
        //test_modular<CryptoPP::Integer> {};
        test_modular<N_bytes_little> {};
        test_modular<N_bytes_big> {};/*
        test_modular<uint_big<9>> {};
        test_modular<uint_little<9>> {};
        test_modular<uint_big<10>> {};
        test_modular<uint_little<10>> {};
        test_modular<uint_big<11>> {};
        test_modular<uint_little<11>> {};
        test_modular<uint_big<12>> {};
        test_modular<uint_little<12>> {};*/
        test_modular<dec_uint> {};
        test_modular<base58_uint> {};

    }
}
