// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/integer.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename N>
    void test_division_natural_case (const N &numerator, const N &denominator, const N &quotient, const N &remainder) {
        auto div = divide (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " = " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " = " << remainder << " but got " << div.Remainder;
    }

    template <typename Z, typename N>
    void test_division_integer_case (const Z &numerator, const Z &denominator, const Z &quotient, const N &remainder) {
        auto div = divide (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " = " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " = " << remainder << " but got " << div.Remainder;
    }

    template <typename N>
    struct test_division_natural {
        test_division_natural () {

            EXPECT_THROW (math::number::natural::divide (N {0}, N {0}), math::division_by_zero);
            EXPECT_THROW (math::number::natural::divide (N {1}, N {0}), math::division_by_zero);

            test_division_natural_case<N> (N {0}, N {1}, N {0}, N {0});
            test_division_natural_case<N> (N {1}, N {1}, N {1}, N {0});
            test_division_natural_case<N> (N {4}, N {2}, N {2}, N {0});
            test_division_natural_case<N> (N {1145}, N {916}, N {1}, N {229});

        }
    };

    template <typename Z, typename N = Z>
    struct test_division_integer : test_division_natural<N> {
        test_division_integer () {

            EXPECT_THROW (math::number::integer::divide (Z {0}, Z {0}), math::division_by_zero);
            EXPECT_THROW (math::number::integer::divide (Z {1}, Z {0}), math::division_by_zero);

            test_division_integer_case<Z, N> (Z {0}, Z {1}, Z {0}, N {0});
            test_division_integer_case<Z, N> (Z {1}, Z {1}, Z {1}, N {0});
            test_division_integer_case<Z, N> (Z {1145}, Z {916}, Z {1}, N {229});

        }
    };

    TEST (DivisionTest, TestDivision) {

        //test_division_integer<int64, uint32> {};
        test_division_integer<int64_little, uint64_little> {};
        test_division_integer<int64_big, uint64_big> {};
        test_division_integer<Z, N> {};

        test_division_integer<Z_bytes_big, N_bytes_big> {};
        test_division_integer<Z_bytes_little, N_bytes_little> {};

        test_division_integer<Z_bytes_twos_little> {};
        test_division_integer<Z_bytes_twos_big> {};

        test_division_integer<int_big<9>, uint_big<9>> {};
        test_division_integer<int_big<10>, uint_big<10>> {};
        test_division_integer<int_big<11>, uint_big<11>> {};
        test_division_integer<int_big<20>, uint_big<20>> {};

        test_division_integer<int_little<9>, uint_little<9>> {};
        test_division_integer<int_little<10>, uint_little<10>> {};
        test_division_integer<int_little<11>, uint_little<11>> {};
        test_division_integer<int_little<20>, uint_little<20>> {};

        test_division_integer<dec_int, dec_uint> {};
        test_division_integer<hex_int, hex_uint> {};
        test_division_natural<base58_uint> {};

        test_division_integer<hex_int_twos> {};
    }

}

