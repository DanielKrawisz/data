// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CRYPTO_SEC
#define DATA_MATH_CRYPTO_SEC

#include <data/integral.hpp>
#include <data/math/algebra/finite_field.hpp>
#include <data/crypto/elliptic_curve.hpp>

namespace data::crypto::SEC {

    template <size_t z> struct hexidecimal {
        bool Valid {false};
        char Value[z] = {};

        constexpr hexidecimal (const char (&input)[z]) {
            if (z <= 1) return;
            if (z % 2 != 0) return;
            if (input[0] != '0' || input[1] != 'x') return;

            for (size_t i {2}; i < z - 1; i++)
                if (!(input[i] >= '0' && input[i] <= '9') || !(input[i] >= 'a' && input[i] <= 'f')) return;

            if (input[z - 1] != '\0') return;

            Valid = true;
            for (size_t i {0}; i < z; i++) Value[i] = input[i];
        }

        operator N () const {
            N x {0};
            N digit {1};

            for (int i {z - 2}; i >= 0; --i) {
                uint64 d = static_cast<uint64> (Value[i] >= 'a' && Value[i] <= 'f' ? Value[i] - 'a' + 10 : Value[i] - '0');
                x += digit * d;
                digit <<= 4;
            }

            return x;
        }
    };

    // make sure these two won't work.
    template <> struct hexidecimal<0>;
    template <> struct hexidecimal<1>;

    template <size_t N> hexidecimal (const char (&)[N]) -> hexidecimal<N>;
    template <size_t N> hexidecimal (hexidecimal<N>) -> hexidecimal<N>;

    template <size_t z>
    struct point {
        char Value[z] = {};
        bool Valid {false};

        constexpr point (const char (&input)[z]) {
            if (z <= 1) return;
            if (z % 2 != 0) return;
            if (input[0] != '0' || (input[1] != '2' && input[1] != '3' && input[1] != '4')) return;

            for (size_t i {2}; i < z - 1; i++)
                if (!(input[i] >= '0' && input[i] <= '9') || !(input[i] >= 'a' && input[i] <= 'f')) return;

            if (input[z - 1] != '\0') return;

            Valid = true;
            for (size_t i {0}; i < z; i++) Value[i] = input[i];
        }
    };

    // make sure these two won't work.
    template <> struct point<0>;
    template <> struct point<1>;

    template <size_t N> point (const char (&)[N]) -> point<N>;
    template <size_t N> point (point<N>) -> point<N>;

    template <size_t size> struct P {
        constexpr const static size_t pstr =
            // number of bytes
            (size / 8)
            // two characters for one byte
            * 2 +
            // the prefix, which will be 0x for numbers or 02, 03, or 04 for points.
            2 +
            // some extra if size doesn't fit perfectly into a list of bytes
            (size % 8 > 0 ? 2 : 0) +
            // the \0 at the end of the string.
            1;

        // the curve modulus
        hexidecimal<pstr> M;
        // the curve parameters
        hexidecimal<pstr> A;
        hexidecimal<pstr> B;
        // the base point.
        point<pstr> G;
        // order of G
        hexidecimal<pstr + 2> N;
        // the cofactor
        uint32 C;

        constexpr P (hexidecimal<pstr> m, hexidecimal<pstr> a, hexidecimal<pstr> b, point<pstr> g, hexidecimal<pstr + 2> n, uint32 c):
            M {m}, A {a}, B {b}, G {g}, N {n}, C {c} {}
    };

    constexpr P<192> secp192k1 {
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFEE37"},
        hexidecimal {"0x000000000000000000000000000000000000000000000000"},
        hexidecimal {"0x000000000000000000000000000000000000000000000003"},
        point       {"03DB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D"},
        hexidecimal {"0x00FFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D"},
        1};

    constexpr P<192> secp192r1 {
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"},
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC"},
        hexidecimal {"0x64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1"},
        point       {"03188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012"},
        hexidecimal {"0x00FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831"},
        1};

    constexpr P<224> secp224k1 {
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFE56D"},
        hexidecimal {"0x00000000000000000000000000000000000000000000000000000000"},
        hexidecimal {"0x00000000000000000000000000000000000000000000000000000005"},
        point       {"03A1455B334DF099DF30FC28A169A467E9E47075A90F7E650EB6B7A45C"},
        hexidecimal {"0x010000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7"},
        1};

    constexpr P<224> secp224r1 {
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001"},
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE"},
        hexidecimal {"0xB4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4"},
        point       {"02B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21"},
        hexidecimal {"0x00FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D"},
        1};

    constexpr P<256> secp256k1 {
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"},
        hexidecimal {"0x0000000000000000000000000000000000000000000000000000000000000000"},
        hexidecimal {"0x0000000000000000000000000000000000000000000000000000000000000007"},
        point       {"0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"},
        hexidecimal {"0x00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"},
        1};

    constexpr P<256> secp256r1 {
        hexidecimal {"0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF"},
        hexidecimal {"0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC"},
        hexidecimal {"0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B"},
        point {"036B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296"},
        hexidecimal {"0x00FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551"},
        1};

    constexpr P<384> secp384r1 {
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF"},
        hexidecimal {"0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC"},
        hexidecimal {"0xB3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF"},
        point {"03AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7"},
        hexidecimal {"0x00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973"},
        1};

    constexpr P<521> secp521r1 {
        hexidecimal {"0x"
                "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFFFFFFFFFF"},
        hexidecimal {"0x"
                "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFFFFFFFFFC"},
        hexidecimal {"0x"
                "0051953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3"
            "B8B489918EF109E156193951EC7E937B1652C0BD3BB1BF073573DF88"
            "3D2C34F1EF451FD46B503F00"},
        point {
              "0200C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521"
            "F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1"
            "856A429BF97E7E31C2E5BD66"},
        hexidecimal {"0x"
              "0001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
            "FFFFFFFFFFFFFFFA51868783BF2F966B7FCC0148F709A5D03BB5C9B8"
            "899C47AEBB6FB71E91386409"},
        1};

}

#endif

