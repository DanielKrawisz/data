// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/float.hpp>
#include <data/numbers.hpp>

#include <gtest/gtest.h>

namespace data {

    TEST (Float, Make) {
        // valid finite values
        EXPECT_TRUE (is_positive_zero (make_float<double> (math::positive, 0u, 0u)));   // +0
        EXPECT_TRUE (is_negative_zero (make_float<double> (math::negative, 0u, 0u)));   // -0

        make_float<double> (math::positive, 1u,    0u);   // smallest positive subnormal
        make_float<double> (math::positive, 2046u, 0u); // largest finite exponent field

        // invalid
        EXPECT_THROW (make_float<double> (math::positive, 2047u, 0u), std::exception); // +inf
        EXPECT_THROW (make_float<double> (math::negative, 2047u, 0u), std::exception); // -inf
        EXPECT_THROW (make_float<double> (math::positive, 2047u, 1u), std::exception); // NaN
        EXPECT_THROW (make_float<double> (math::negative, 2047u, 1u), std::exception);  // NaN

        EXPECT_THROW (make_float<double> (math::positive, 2048u, 0u), std::exception); // exponent doesn't fit
        EXPECT_THROW (make_float<double> (math::positive,    1u, uint64_t {1} << 52), std::exception); // mantissa doesn't fit
    }

    TEST (Float, Inf) {
        auto positive = make_float_inf<double> ();
        auto negative = make_float_inf<double> (math::negative);

        EXPECT_TRUE (std::isinf (positive));
        EXPECT_TRUE (std::isinf (negative));
        EXPECT_TRUE (positive > 0);
        EXPECT_TRUE (negative < 0);
    }

    TEST (Float, Nan) {
        auto positive = make_float_nan<double> ();
        auto negative = make_float_nan<double> (math::negative);

        EXPECT_TRUE (std::isnan (positive));
        EXPECT_TRUE (std::isnan (negative));
    }

    TEST (Float, Double) {
        EXPECT_EQ (double (N_bytes_big {dec_int ("0")}), 0.0);

        EXPECT_EQ (double (N_bytes_big {dec_int ("1")}), 1.0);

        EXPECT_EQ (double (N_bytes_big {dec_int ("2")}), 2.0);

        EXPECT_EQ (double (N_bytes_big {dec_int ("3")}), 3.0);

        EXPECT_EQ (double (N_bytes_big {dec_int ("42")}), 42.0);

        EXPECT_EQ (double (N_bytes_big {dec_int ("500")}), 500.0);

        EXPECT_EQ (double (N_bytes_big {dec_int ("9007199254740992")}), 9007199254740992.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("0")}), 0.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("1")}), 1.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("2")}), 2.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("3")}), 3.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("42")}), 42.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("500")}), 500.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("9007199254740992")}), 9007199254740992.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("0")}), 0.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("1")}), 1.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("2")}), 2.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("3")}), 3.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("42")}), 42.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("500")}), 500.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("9007199254740992")}), 9007199254740992.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("-1")}), -1.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("-2")}), -2.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("-3")}), -3.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("-42")}), -42.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("-500")}), -500.0);

        EXPECT_EQ (double (Z_bytes_big {dec_int ("-9007199254740992")}), -9007199254740992.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("-1")}), -1.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("-2")}), -2.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("-3")}), -3.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("-42")}), -42.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("-500")}), -500.0);

        EXPECT_EQ (double (Z_bytes_BC_big {dec_int ("-9007199254740992")}), -9007199254740992.0);
    }
}
