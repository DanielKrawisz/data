// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/number/division.hpp"
#include "data/math/number/bytes.hpp"
#include "data/numbers.hpp"
#include "data/tuple.hpp"
#include "gtest/gtest.h"

namespace data {
    template <typename N>
    void test_size_in_base_2 () {
        // TODO
    }

    template <typename Z, typename N>
    void test_div_mod_2_positive () {
        EXPECT_EQ (div_2 (Z {0}), Z {0});
        EXPECT_EQ (mod_2 (Z {0}), N {0});
        EXPECT_EQ (div_2 (Z {1}), Z {0});
        EXPECT_EQ (mod_2 (Z {1}), N {1});
        EXPECT_EQ (div_2 (Z {2}), Z {1});
        EXPECT_EQ (mod_2 (Z {2}), N {0});
        EXPECT_EQ (div_2 (Z {3}), Z {1});
        EXPECT_EQ (mod_2 (Z {3}), N {1});
    }

    template <typename Z, typename N>
    void test_div_mod_2_integer () {
        test_div_mod_2_positive<Z, N> ();
        EXPECT_EQ (div_2 (Z {-1}), Z {-1});
        EXPECT_EQ (mod_2 (Z {-1}), N {1});
        EXPECT_EQ (div_2 (Z {-2}), Z {-1});
        EXPECT_EQ (mod_2 (Z {-2}), N {0});
    }

    template <typename Z>
    void test_div_mod_2_signed () {
        test_div_mod_2_positive<Z, Z> ();
        EXPECT_EQ (div_2 (Z {-1}), Z {0});
        EXPECT_EQ (mod_2 (Z {-1}), Z {-1});
        EXPECT_EQ (div_2 (Z {-2}), Z {-1});
        EXPECT_EQ (mod_2 (Z {-2}), Z {0});
    }

    template <typename N>
    void test_division_natural_case (const N &numerator, const N &denominator, const N &quotient, const N &remainder) {

        auto div = divmod (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " -> " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " -> " << remainder << " but got " << div.Remainder;
    }

    template <typename Z, typename N>
    void test_division_integer_case (const Z &numerator, const Z &denominator, const Z &quotient, const N &remainder) {

        auto div = divmod (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " -> " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " -> " << remainder << " but got " << div.Remainder;
    }

    template <typename Z, typename N>
    void test_division_integer_natural_case (const Z &numerator, const N &denominator, const Z &quotient, const N &remainder) {

        auto div = divmod (numerator, math::nonzero {denominator});
        EXPECT_EQ (div.Quotient, quotient)
            << "expected " << numerator << " / " << denominator << " -> " << quotient << " but got " << div.Quotient;
        EXPECT_EQ (div.Remainder, remainder)
            << "expected " << numerator << " % " << denominator << " -> " << remainder << " but got " << div.Remainder;
    }

    template <ring_number N>
    struct test_division_natural {
        test_division_natural () {

            EXPECT_THROW (divmod (N {0}, math::nonzero<N> {N {0}}), math::division_by_zero);
            EXPECT_THROW (divmod (N {1}, math::nonzero<N> {N {0}}), math::division_by_zero);

            test_division_natural_case<N> (N {0}, N {1}, N {0}, N {0});
            test_division_natural_case<N> (N {1}, N {1}, N {1}, N {0});
            test_division_natural_case<N> (N {4}, N {2}, N {2}, N {0});
            test_division_natural_case<N> (N {1145}, N {916}, N {1}, N {229});

        }
    };

    template <ring_number Z, ring_number N = Z>
    struct test_division_integer_natural {
        test_division_integer_natural () {

            EXPECT_THROW (divmod (Z {0}, math::nonzero {N {0}}), math::division_by_zero);
            EXPECT_THROW (divmod (Z {1}, math::nonzero {N {0}}), math::division_by_zero);

            test_division_integer_natural_case<Z, N> (Z {0}, N {1}, Z {0}, N {0});
            test_division_integer_natural_case<Z, N> (Z {1}, N {1}, Z {1}, N {0});
            test_division_integer_natural_case<Z, N> (Z {1145}, N {916}, Z {1}, N {229});
            test_division_integer_natural_case<Z, N> (Z {-2}, N {3}, Z {-1}, N {1});
            test_division_integer_natural_case<Z, N> (Z {-2}, N {2}, Z {-1}, N {0});
            test_division_integer_natural_case<Z, N> (Z {-2}, N {1}, Z {-2}, N {0});
            test_division_integer_natural_case<Z, N> (Z {-3}, N {2}, Z {-2}, N {1});

        }
    };

    template <ring_number Z, ring_number N = Z>
    struct test_division_integer {
        test_division_integer () {

            EXPECT_THROW (divmod (Z {0}, math::nonzero {Z {0}}), math::division_by_zero);
            EXPECT_THROW (divmod (Z {1}, math::nonzero {Z {0}}), math::division_by_zero);

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

    template <ring_number Z>
    struct test_division_signed {
        test_division_signed () {

            EXPECT_THROW (divmod (Z {0}, math::nonzero {Z {0}}), math::division_by_zero);
            EXPECT_THROW (divmod (Z {1}, math::nonzero {Z {0}}), math::division_by_zero);

            test_division_integer_case<Z, Z> (Z {-2}, Z {3}, Z {0}, Z {-2});
            test_division_integer_case<Z, Z> (Z {2}, Z {-3}, Z {0}, Z {2});
            test_division_integer_case<Z, Z> (Z {-2}, Z {-3}, Z {0}, Z {-2});
            test_division_integer_case<Z, Z> (Z {-2}, Z {2}, Z {-1}, Z {0});
            test_division_integer_case<Z, Z> (Z {-2}, Z {1}, Z {-2}, Z {0});
            test_division_integer_case<Z, Z> (Z {2}, Z {-2}, Z {-1}, Z {0});
            test_division_integer_case<Z, Z> (Z {-3}, Z {2}, Z {-1}, Z {-1});

        }
    };

    template <ring_number N>
    struct UnsignedDivision : ::testing::Test {
        using unsigned_int = N;
    };

    template <ring_number Z>
    struct IntegerDivision : ::testing::Test {
        using integer = Z;
    };

    template <ring_number Z>
    struct SignedDivision : ::testing::Test {
        using signed_int = Z;
    };

    template <typename tuple>
    struct Division : ::testing::Test {
        using natural = typename std::tuple_element<1, tuple>::type;
        using integer = typename std::tuple_element<0, tuple>::type;
    };

    using unsigned_test_cases = ::testing::Types<
        uint64, uint64_little, uint64_big,
        uint80, uint80_little, uint80_big,
        uint128, uint128_little, uint128_big>;

    using signed_test_cases = ::testing::Types<
        int64, int64_little, int64_big,
        int80, int80_little, int80_big,
        int128, int128_little, int128_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        math::Z_bytes_BC<endian::big, unsigned short>,
        math::Z_bytes_BC<endian::little, unsigned int>,
        math::Z_bytes_BC<endian::big, unsigned long>,
        math::Z_bytes_BC<endian::little, unsigned long long>,
        hex_int_BC>;

    using test_cases = ::testing::Types<
        tuple<Z, N>,
        tuple<Z_bytes_little, N_bytes_little>,
        tuple<Z_bytes_big, N_bytes_big>,
        tuple<math::Z_bytes<endian::little, unsigned short>, math::N_bytes<endian::little, unsigned short>>,
        tuple<math::Z_bytes<endian::big, unsigned int>, math::N_bytes<endian::big, unsigned int>>,
        tuple<math::Z_bytes<endian::little, unsigned long>, math::N_bytes<endian::little, unsigned long>>,
        tuple<math::Z_bytes<endian::big, unsigned long long>, math::N_bytes<endian::big, unsigned long long>>,
        tuple<dec_int, dec_uint>,
        tuple<hex_int, hex_uint>>;

    TYPED_TEST_SUITE (UnsignedDivision, unsigned_test_cases);
    TYPED_TEST_SUITE (SignedDivision, signed_test_cases);
    TYPED_TEST_SUITE (Division, test_cases);

    TYPED_TEST (UnsignedDivision, SizeInBase2) {
        using N = typename TestFixture::unsigned_int;

        test_size_in_base_2<N> ();
    }

    TYPED_TEST (SignedDivision, SizeInBase2) {
        using Z = typename TestFixture::signed_int;

        test_size_in_base_2<Z> ();
    }

    TYPED_TEST (Division, SizeInBase2) {
        using N = typename TestFixture::natural;
        using Z = typename TestFixture::integer;

        test_size_in_base_2<N> ();
    }

    TYPED_TEST (UnsignedDivision, DivMod2) {
        using N = typename TestFixture::unsigned_int;

        test_div_mod_2_positive<N, N> ();
    }

    TYPED_TEST (SignedDivision, DivMod2) {
        using Z = typename TestFixture::signed_int;

        test_div_mod_2_signed<Z> ();
    }

    TYPED_TEST (Division, DivMod2) {
        using N = typename TestFixture::natural;
        using Z = typename TestFixture::integer;

        test_div_mod_2_integer<Z, N> ();
    }

    TYPED_TEST (UnsignedDivision, Division) {
        using N = typename TestFixture::unsigned_int;

        test_division_natural<N> {};
    }

    TYPED_TEST (SignedDivision, Division) {
        using Z = typename TestFixture::signed_int;

        test_division_natural<Z> {};
        test_division_signed<Z> {};
    }

    TYPED_TEST (Division, Division) {
        using N = typename TestFixture::natural;
        using Z = typename TestFixture::integer;

        test_division_natural<N> {};
        test_division_integer_natural<Z, N> {};
        test_division_integer<Z, N> {};
    }

}

