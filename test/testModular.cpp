// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/natural.hpp"
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

    template <unsigned_integral N, math::ring M>
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
    
    template <typename N> struct test_modular {
        test_modular () {

            test_suite<N, math::number::modular<d2, N>> ();/*
            test_suite<N, math::number::modular<d3, N>> ();
            test_suite<N, math::number::modular<d4, N>> ();
            test_suite<N, math::number::modular<d5, N>> ();
            test_suite<N, math::number::modular<d6, N>> ();
            test_suite<N, math::number::modular<d7, N>> ();
            test_suite<N, math::number::modular<d8, N>> ();
            test_suite<N, math::number::modular<d9, N>> ();
            test_suite<N, math::number::modular<d10, N>> ();
            test_suite<N, math::number::modular<d11, N>> ();
            test_suite<N, math::number::modular<d12, N>> ();
            test_suite<N, math::number::modular<d13, N>> ();
            test_suite<N, math::number::modular<d14, N>> ();
            test_suite<N, math::number::modular<d15, N>> ();
            test_suite<N, math::number::modular<d16, N>> ();
            test_suite<N, math::number::modular<d17, N>> ();
            test_suite<N, math::number::modular<d18, N>> ();
            test_suite<N, math::number::modular<d19, N>> ();*/

        }
    };

    // TODO something with a modulus that is nearly the same size as the max value of the number type.
    
    TEST (TestModular, TestModularArithmetic) {

        test_modular<uint64> {};/*
        test_modular<N> {};
        //test_modular<CryptoPP::Integer> {};
        test_modular<N_bytes_little> {};
        test_modular<N_bytes_big> {};
        test_modular<dec_uint> {};
        test_modular<base58_uint> {};

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
        test_modular<uint_little<6, unsigned int>> {};*/

        // TODO bigger words
        /*
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
        test_modular<uint_little<4, long long unsigned int>> {};
        */

    }
}
