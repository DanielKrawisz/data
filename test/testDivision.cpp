// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename N>
    void test_division_natural_case (const N &numerator, const N &denominator, const N &quotient, const N &remainder) {

        auto div = divide (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " -> " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " -> " << remainder << " but got " << div.Remainder;
    }

    template <typename Z, typename N>
    void test_division_integer_case (const Z &numerator, const Z &denominator, const Z &quotient, const N &remainder) {

        auto div = divide (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " -> " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " -> " << remainder << " but got " << div.Remainder;
    }

    template <typename Z, typename N>
    void test_division_integer_natural_case (const Z &numerator, const N &denominator, const Z &quotient, const N &remainder) {

        auto div = divide (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " -> " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " -> " << remainder << " but got " << div.Remainder;
    }

    template <typename N>
    requires ring_integral<N>
    struct test_division_natural {
        test_division_natural () {

            EXPECT_THROW (math::number::natural_divide (N {0}, N {0}), math::division_by_zero);
            EXPECT_THROW (math::number::natural_divide (N {1}, N {0}), math::division_by_zero);

            test_division_natural_case<N> (N {0}, N {1}, N {0}, N {0});
            test_division_natural_case<N> (N {1}, N {1}, N {1}, N {0});
            test_division_natural_case<N> (N {4}, N {2}, N {2}, N {0});
            test_division_natural_case<N> (N {1145}, N {916}, N {1}, N {229});

        }
    };

    template <typename Z, typename N = Z>
    requires ring_integral_system<Z, N>
    struct test_division_integer_natural : test_division_natural<N> {
        test_division_integer_natural () {

            EXPECT_THROW (math::number::integer_divide (Z {0}, N {0}), math::division_by_zero);
            EXPECT_THROW (math::number::integer_divide (Z {1}, N {0}), math::division_by_zero);

            test_division_integer_natural_case<Z, N> (Z {0}, N {1}, Z {0}, N {0});
            test_division_integer_natural_case<Z, N> (Z {1}, N {1}, Z {1}, N {0});
            test_division_integer_natural_case<Z, N> (Z {1145}, N {916}, Z {1}, N {229});
            test_division_integer_natural_case<Z, N> (Z {-2}, N {3}, Z {-1}, N {1});
            test_division_integer_natural_case<Z, N> (Z {-2}, N {2}, Z {-1}, N {0});
            test_division_integer_natural_case<Z, N> (Z {-2}, N {1}, Z {-2}, N {0});
            test_division_integer_natural_case<Z, N> (Z {-3}, N {2}, Z {-2}, N {1});

        }
    };

    template <typename Z, typename N = Z>
    requires ring_integral_system<Z, N>
    struct test_division_integer : test_division_integer_natural<Z, N> {
        test_division_integer () {

            EXPECT_THROW (math::number::integer_divide (Z {0}, Z {0}), math::division_by_zero);
            EXPECT_THROW (math::number::integer_divide (Z {1}, Z {0}), math::division_by_zero);

            test_division_integer_case<Z, N> (Z {0}, Z {1}, Z {0}, N {0});
            test_division_integer_case<Z, N> (Z {1}, Z {1}, Z {1}, N {0});
            test_division_integer_case<Z, N> (Z {389}, Z {3}, Z {129}, N {2});
            test_division_integer_case<Z, N> (Z {1145}, Z {916}, Z {1}, N {229});
            test_division_integer_case<Z, N> (Z {-2}, Z {3}, Z {-1}, N {1});
            test_division_integer_case<Z, N> (Z {2}, Z {-3}, Z {0}, N {2});
            test_division_integer_case<Z, N> (Z {-2}, Z {-3}, Z {1}, N {1});
            test_division_integer_case<Z, N> (Z {-2}, Z {2}, Z {-1}, N {0});
            test_division_integer_case<Z, N> (Z {-2}, Z {1}, Z {-2}, N {0});
            test_division_integer_case<Z, N> (Z {2}, Z {-2}, Z {-1}, N {0});
            test_division_integer_case<Z, N> (Z {-3}, Z {2}, Z {-2}, N {1});

        }
    };

    TEST (DivisionTest, TestDivision) {

        test_division_integer<Z, N> {};

        test_division_integer<Z_bytes_big, N_bytes_big> {};
        test_division_integer<Z_bytes_little, N_bytes_little> {};

        test_division_integer<Z_bytes_BC_little> {};
        test_division_integer<Z_bytes_BC_big> {};

        test_division_integer<math::Z_bytes<endian::big, short unsigned>, math::N_bytes<endian::big, short unsigned>> {};
        test_division_integer<math::Z_bytes<endian::little, short unsigned>, math::N_bytes<endian::little, short unsigned>> {};

        test_division_integer<math::Z_bytes_BC<endian::big, short unsigned>> {};
        test_division_integer<math::Z_bytes_BC<endian::little, short unsigned>> {};

        test_division_integer<math::Z_bytes<endian::big, unsigned>, math::N_bytes<endian::big, unsigned>> {};
        test_division_integer<math::Z_bytes<endian::little, unsigned>, math::N_bytes<endian::little, unsigned>> {};

        test_division_integer<math::Z_bytes_BC<endian::big, unsigned>> {};
        test_division_integer<math::Z_bytes_BC<endian::little, unsigned>> {};

        test_division_integer<dec_int, dec_uint> {};
        test_division_integer<hex_int, hex_uint> {};
        test_division_natural<base58_uint> {};

        test_division_integer<hex_int_BC> {};
    }

}

