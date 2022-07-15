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
        EXPECT_EQ (M (f1<N> ()), f1<M> ());
        EXPECT_EQ (M (f2<N> ()), f2<M> ());
        EXPECT_EQ (M (f3<N> ()), f3<M> ());
        EXPECT_EQ (M (f4<N> ()), f4<M> ());
        EXPECT_EQ (M (f5<N> ()), f5<M> ());

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
        test_modular<N_bytes_big> {};
        test_modular<dec_uint> {};
        test_modular<base58_uint> {};

    }
}
