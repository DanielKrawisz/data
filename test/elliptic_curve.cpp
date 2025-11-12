// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <data/crypto/elliptic_curve.hpp>
#include <data/math/algebra/finite_field.hpp>
#include <data/numbers.hpp>

namespace data::crypto {

    struct secp192k1 {

        using scalar = uint192;

        // the field
        using coord = math::prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFEE37"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0x000000000000000000000000000000000000000000000000"},
            coord {"0x000000000000000000000000000000000000000000000003"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0xDB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D"},
            coord {"0x9B2F2F6D9C5628A7844163D015BE86344082AA88D95E2F9D"}};

        // the base point order.
        constexpr static const scalar order {"0xFFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;
    };

    struct secp192r1 {

        using scalar = uint192;

        // the field
        using coord = math::prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC"},
            coord {"0x64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0x188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012"},
            coord {"0x07192B95FFC8DA78631011ED6B24CDD573F977A11E794811"}};

        // the base point order.
        constexpr static const scalar order {"0xFFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;

    };

    struct secp224k1 {

        using scalar = uint256;

        // the field
        using coord = math::prime_field<uint224 {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFE56D"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0x000000000000000000000000000000000000000000000000"},
            coord {"0x000000000000000000000000000000000000000000000005"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0xA1455B334DF099DF30FC28A169A467E9E47075A90F7E650EB6B7A45C"},
            coord {"0x7E089FED7FBA344282CAFBD6F7E319F7C0B0BD59E2CA4BDB556D61A5"}};

        // the base point order.
        constexpr static const scalar order {"0x000000010000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;
    };

    struct secp224r1 {

        using scalar = uint224;

        // the field
        using coord = math::prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE"},
            coord {"0xB4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0xB70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21"},
            coord {"0xBD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34"}};

        // the base point order.
        constexpr static const scalar order {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;

    };

    struct secp256k1 {

        using scalar = uint256;

        // the field
        using coord = math::prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0x0000000000000000000000000000000000000000000000000000000000000000"},
            coord {"0x0000000000000000000000000000000000000000000000000000000000000007"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
            coord {"0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"}};

        // the base point order.
        constexpr static const scalar order {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;
    };

    struct secp256r1 {

        using scalar = uint256;

        // the field
        using coord = math::prime_field<scalar {"0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC"},
            coord {"0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296"},
            coord {"0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5"}};

        // the base point order.
        constexpr static const scalar order {"0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;

    };

    struct secp384r1 {

        using scalar = uint384;

        // the field
        using coord = math::prime_field<scalar {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            coord {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC"},
            coord {"0xB3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0xAA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7"},
            coord {"0x3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D7E819D7A431D7C90EA0E5F"}};

        // the base point order.
        constexpr static const scalar order {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;

    };

    struct secp521r1 {

        using scalar = uint544;

        // the field
        using coord = math::prime_field<scalar {"0x000001FF"
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"}>;

        // the curve
        constexpr static const math::Weierstrauss<scalar, coord> curve {
            scalar {"0x000001FF"
                "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC"},
            scalar {"0x00000051"
                "953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E1"
                "56193951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00"}};

        using affine_point = math::elliptic_curve<curve>::affine_point;

        // the base point
        constexpr static const affine_point base {
            coord {"0x000000C6"
                "858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBA"
                "A14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66"},
            coord {"0x00000118"
                "39296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C"
                "97EE72995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650"}};

        // the base point order.
        constexpr static const scalar order {"0x000001FF"
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA"
            "51868783BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386409"};

        constexpr static const uint32 cofactor = 1;

        using system = ECDSA<curve, base, order, cofactor>;

        using pubkey = system::pubkey;

        using secret = system::secret;

    };

    template <typename X>
    struct EllipticCurveCrypto : ::testing::Test {
        using curve = X;
    };

    using elliptic_curve_crypto_cases = ::testing::Types<
        secp192k1, secp192r1,
        secp224k1, secp224r1,
        secp256k1, secp256r1,
        secp384r1, secp521r1>;

    TYPED_TEST_SUITE (EllipticCurveCrypto, elliptic_curve_crypto_cases);


    TYPED_TEST (EllipticCurveCrypto, Crypto) {
        using system = typename TestFixture::curve;
        using scalar = system::scalar;

        math::nonzero<scalar> secret_A {123};
        math::nonzero<scalar> secret_B {456};

        using pubkey = system::pubkey;
/*
        pubkey pubkey_A = system::base * secret_A;
        pubkey pubkey_B = system::base * secret_B;

        // Diffie Helman
        EXPECT_EQ (pubkey_A * secret_B, pubkey_B * secret_A);*/

    }
}

namespace data::crypto {

    // Key here is to test projective point <=> projective vector.

}

