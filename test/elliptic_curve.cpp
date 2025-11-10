// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/math/algebra/elliptic_curve.hpp>
#include <data/math/algebra/finite_field.hpp>
#include <data/numbers.hpp>

namespace data::math {
    struct secp192k1 {

        using scalar = uint192;

        // the field
        using coord = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFEE37"}>;

        using secret = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0x000000000000000000000000000000000000000000000000"},
            scalar {"0x000000000000000000000000000000000000000000000003"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {"0xDB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D", parity (0x03)};
    };

    struct secp192r1 {

        using scalar = uint192;

        // the field
        using coord =  prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"}>;

        using secret = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC"},
            scalar {"0x64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {"0x188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012", parity (0x03)};

    };

    struct secp224k1 {

        using scalar = uint256;

        // the field
        using coord = prime_field<uint224 {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFE56D"}>;

        using secret = prime_field<scalar {"0x000000010000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0x000000000000000000000000000000000000000000000000"},
            scalar {"0x000000000000000000000000000000000000000000000005"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {"0xA1455B334DF099DF30FC28A169A467E9E47075A90F7E650EB6B7A45C", parity (0x03)};
    };

    struct secp224r1 {

        using scalar = uint224;

        // the field
        using coord =  prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001"}>;

        using secret = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE"},
            scalar {"0xB4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {"0xB70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21", parity (0x02)};

    };

    struct secp256k1 {

        using scalar = uint256;

        // the field
        using coord =  prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}>;

        using secret = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0x0000000000000000000000000000000000000000000000000000000000000000"},
            scalar {"0x0000000000000000000000000000000000000000000000000000000000000007"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", parity (0x02)};
    };

    struct secp256r1 {

        using scalar = uint256;

        // the field
        using coord =  prime_field<scalar {"0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF"}>;

        using secret = prime_field<scalar {"0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC"},
            scalar {"0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {"0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296", parity (0x03)};

    };

    struct secp384r1 {

        using scalar = uint384;

        // the field
        using coord =  prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF"}>;

        using secret = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC"},
            scalar {"0xB3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {
            "0xAA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7", parity (0x03)};

    };
/*
    struct secp521r1 {

        using scalar = uint544;

        // the field
        using coord =  prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF"}>;

        using secret = prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973"}>;

        // the curve
        constexpr static const Weierstrauss<coord, scalar> curve {
            scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC"},
            scalar {"0xB3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF"}};

        using pubkey = elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const pubkey base {
            "0xAA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7", parity (0x03)};

    };*/

    template <typename X>
    struct EllipticCurveCrypto : ::testing::Test {
        using curve = X;
    };

    using elliptic_curve_crypto_cases = ::testing::Types<
        secp192k1, secp192r1,
        secp224k1, secp224r1,
        secp256k1, secp256r1,
        secp384r1/*, secp521r1*/>;

    TYPED_TEST_SUITE (EllipticCurveCrypto, elliptic_curve_crypto_cases);


    TYPED_TEST (EllipticCurveCrypto, Crypto) {
        using system = typename TestFixture::curve;

        typename system::scalar secret_key {123};

    }
}

namespace data::crypto {

    // Key here is to test projective point <=> projective vector.

}

