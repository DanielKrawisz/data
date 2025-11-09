// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/SEC.hpp>

namespace data::crypto {

    // Key here is to test projective point <=> projective vector.

    constexpr const char *prime =  "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
    constexpr const char *order =  "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141";

    using coordinate = data::math::prime_field<data::uint256 {prime}>;

    using scalar = data::math::prime_field<data::uint256 {order}>;

    constexpr const char *base_X = "0x" "79BE667EF9DCBBAC" "55A06295CE870B07" "029BFCDB2DCE28D9" "59F2815B16F81798";
    constexpr const char *base_Y = "0x" "483ADA7726A3C465" "5DA4FBFC0E1108A8" "FD17B448A6855419" "9C47D08FFB10D4B8​";

    constexpr coordinate BaseX {base_X};
    constexpr coordinate BaseY {base_Y};



    TEST (EllipticCurve, SECP256k1) {}

}

