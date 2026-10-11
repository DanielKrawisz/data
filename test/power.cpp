// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/power.hpp>

#include <gtest/gtest.h>

namespace data {
    template <typename NN> void test_power_case (const dec_int &base, const dec_int &exp, const dec_int &expected) {
        EXPECT_EQ ((pow (NN (N {base}), NN (N {exp}))), NN (N {expected}));
    }

    template <typename N> void test_power () {
        test_power_case<N> (dec_int {"5"}, dec_int {"20"}, dec_int {"95367431640625"});
        test_power_case<N> (dec_int {"2"}, dec_int {"100"}, dec_int {"1267650600228229401496703205376"});
    }

    template <typename X>
    struct Power : ::testing::Test {
        using number = X;
    };

    using power_test_cases = ::testing::Types<
        N, Z, N_bytes_little, N_bytes_big,
        Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        uint160, int160, uint224, int224, uint256, int256, dec_uint, base58_uint>;

    TYPED_TEST_SUITE (Power, power_test_cases);

    TYPED_TEST (Power, Power) {

        test_power<typename TestFixture::number> ();

    }

    using pow_mod_small_test_cases = ::testing::Types<
        uint32, uint64,
        N, Z, N_bytes_little, N_bytes_big,
        Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        uint160, int160, uint224, int224, uint256, int256, dec_uint, hex_uint, base58_uint>;

    template <typename N> void pow_exp_test_case (N base, N exp, N mod, N expected) {
        auto result = pow_mod (base, exp, math::nonzero {mod});
        EXPECT_EQ (result, expected) << "expected pow_mod (" << base << ", " << exp << ", " << mod << ") -> " << expected << " but got " << result;
    }

    template <typename X>
    struct PowModUInt32 : ::testing::Test {
        using number = X;
    };

    TYPED_TEST_SUITE (PowModUInt32, pow_mod_small_test_cases);

    template <typename N> void pow_mod_small () {

        pow_exp_test_case (N (2), N (10), N (1000), N (24));
        pow_exp_test_case (N (3), N (13), N (17), N (12));
        pow_exp_test_case (N (7), N (1000), N (13), N (9));
        pow_exp_test_case (N (3), N (13), N (7), N (3));
        pow_exp_test_case (N (123456789), N (987654321), N (1000000007), N (652541198));

        pow_exp_test_case (N (0), N (0), N (17), N (1));
        pow_exp_test_case (N (0), N (5), N (17), N (0));
        pow_exp_test_case (N (10), N (5), N (1), N (0));

        // max is 4294967295

        // uint32 overflow tests
        pow_exp_test_case (N (4294967290), N (2), N (4294967291), N (1));
        pow_exp_test_case (N (4000000000), N (2), N (4294967291), N (3430488118));
        pow_exp_test_case (N (4294967290), N (3), N (4294967291), N (4294967290));
        pow_exp_test_case (N (2), N (4294967295), N (4294967291), N (32));

    }

    TYPED_TEST (PowModUInt32, PowMod) {

        pow_mod_small<typename TestFixture::number> ();

    }

    using pow_mod_big_test_cases = ::testing::Types<
        N, Z, N_bytes_little, N_bytes_big,
        Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big>;

    template <typename X>
    struct PowMod : ::testing::Test {
        using number = X;
    };

    TYPED_TEST_SUITE (PowMod, pow_mod_big_test_cases);

    template <typename N> void pow_mod_big () {

        // Big-integer tests
        pow_exp_test_case (
            N (dec_uint {"115792089237316195423570985008687907853269984665640564039457584007913129639935"}),
            N (dec_uint {"57896044618658097711785492504343953926634992332820282019728792003956564832313"}),
            N (dec_uint {"115792089237316195423570985008687907853269984665640564039457584007913129639747"}),
            N (dec_uint {"41161070786103098019366711967885085018669236931811638391973758075981443685081"})
        );

        pow_exp_test_case (
            N (5),
            N (dec_uint {
                "1797693134862315907729305190789024733617976978942306572734300811577326758055009631327084773224075360211"
                "2011387987139335765878976881441662249284743063947412437776789342486548527630221960124609411945308295208"
                "5005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137339"}),
            N (dec_uint {
                "6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977"
                "296311391480858037121987999716643812574028291115057151"}),
            N (dec_uint {
                "9812464990973968361735112739033789989071959496221278098630699069153964442471332929000527787968029766625"
                "4150377508308279542649864245306016886438644805011587"})
        );
    }

    TYPED_TEST (PowMod, PowMod) {

        pow_mod_big<typename TestFixture::number> ();

    }

}

