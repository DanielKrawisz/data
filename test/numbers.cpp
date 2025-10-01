// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/concepts.hpp>
#include <data/numbers.hpp>
#include <data/math.hpp>
#include "gtest/gtest.h"

namespace data {

    // We have some concepts that we use to define how our
    // number types should be constructable.

    // TODO make sure the functions we use are all defined!
    template <typename N> concept can_use_string_literals = requires () {
            { N {"0"} };
            { N {"9007199254740992"} };
        };

    template <typename N> concept has_string_constructor = requires (const std::string &x) {
            { N {x} };
        } && requires (const data::string &x) {
            { N {x} };
        };

    template <typename N> concept has_bytes_constructor = requires (slice<const byte> x) {
            { N {x} };
        } && requires (const data::bytes &x) {
            { N {x} };
        };

    template <typename N> concept can_use_literals =
        can_use_string_literals<N> && NumberConstructable<N>;

    template <typename N> concept conveniently_constructable =
        can_use_literals<N> && has_string_constructor<N> &&
        std::default_initializable<N> &&
        requires (std::istream &i, N &x) {
            { i >> x };
        };

    template <typename N> concept conveniently_constructable_unsigned =
        conveniently_constructable<N> && UnsignedNumberConstructable<N>;

    template <typename N> concept conveniently_constructable_bytes =
        conveniently_constructable<N> && has_bytes_constructor<N>;

    template <typename N> concept conveniently_constructable_bytes_unsigned =
        conveniently_constructable<N> && has_bytes_constructor<N>;

    static_assert (conveniently_constructable_unsigned<N>);
    static_assert (conveniently_constructable<Z>);
    static_assert (conveniently_constructable_bytes_unsigned<N_bytes_little>);
    static_assert (conveniently_constructable_bytes<Z_bytes_little>);
    static_assert (conveniently_constructable_bytes_unsigned<N_bytes_big>);
    static_assert (conveniently_constructable_bytes<Z_bytes_big>);
    static_assert (conveniently_constructable_bytes<Z_bytes_BC_little>);
    static_assert (conveniently_constructable_bytes<Z_bytes_BC_big>);
    static_assert (conveniently_constructable_unsigned<math::N_bytes<endian::little, unsigned short>>);
    static_assert (conveniently_constructable<math::Z_bytes<endian::little, unsigned short>>);
    static_assert (conveniently_constructable_unsigned<math::N_bytes<endian::big, unsigned int>>);
    static_assert (conveniently_constructable<math::Z_bytes<endian::big, unsigned int>>);
    static_assert (conveniently_constructable_unsigned<uint128_little>);
    static_assert (conveniently_constructable_unsigned<uint128_big>);
    static_assert (conveniently_constructable<uint128>);
    static_assert (conveniently_constructable_unsigned<dec_uint>);
    static_assert (conveniently_constructable<dec_int>);
    static_assert (conveniently_constructable_unsigned<hex_uint>);
    static_assert (conveniently_constructable<hex_int>);
    static_assert (conveniently_constructable<hex_int_BC>);
    static_assert (conveniently_constructable_unsigned<base58_uint>);

    // We have some rules about how number types can
    // be converted into one another. NOTE: these are incomplete.

    // It's possible to convert a natural type implicitly to an integer type.
    static_assert (ImplicitlyConvertible<N, Z>);
    static_assert (ImplicitlyConvertible<N_bytes_little, Z_bytes_little>);
    static_assert (ImplicitlyConvertible<N_bytes_big, Z_bytes_big>);

    // however, the opposite conversion cannot be implicit.
    static_assert (ExplicitlyConvertible<Z, N>);/*
    static_assert (ExplicitlyConvertible<Z_bytes_little, N_bytes_little>);
    static_assert (ExplicitlyConvertible<Z_bytes_big, N_bytes_big>);
    static_assert (ExplicitlyConvertible<math::Z_bytes<endian::little, unsigned short>, N_bytes<endian::little, unsigned short>>);
    static_assert (ExplicitlyConvertible<math::Z_bytes<endian::big, unsigned short>, N_bytes<endian::big, unsigned short>>);*/

    // for built-in-like numbers, both conversions are explicit.
    static_assert (ExplicitlyConvertible<uint128, int128>);
    static_assert (ExplicitlyConvertible<int128, uint128>);

    static_assert (ExplicitlyConvertible<uint128_little, int128_little>);
    static_assert (ExplicitlyConvertible<int128_little, uint128_little>);

    static_assert (ExplicitlyConvertible<uint128_big, int128_big>);
    static_assert (ExplicitlyConvertible<int128_big, uint128_big>);
/*
    static_assert (ImplicitlyConvertible<uint128, uint160>);
    static_assert (ImplicitlyConvertible<uint128, int160>);
    static_assert (ImplicitlyConvertible<int128, int160>);

    static_assert (ExplicitlyConvertible<uint160, uint128>);
    static_assert (ExplicitlyConvertible<int160, int128>);*/

    static_assert (ImplicitlyConvertible<uint128_little, uint160_little>);
    static_assert (ImplicitlyConvertible<uint128_little, int160_little>);
    static_assert (ImplicitlyConvertible<int128_little, int160_little>);

    static_assert (ImplicitlyConvertible<uint128_big, uint160_big>);
    static_assert (ImplicitlyConvertible<uint128_big, int160_big>);
    static_assert (ImplicitlyConvertible<int128_big, int160_big>);

    static_assert (comparable_to<N, Z>);
    static_assert (comparable_to<N_bytes_little, Z_bytes_little>);
    static_assert (comparable_to<Z_bytes_little, N_bytes_little>);
    static_assert (comparable_to<uint256, int256>);
    static_assert (comparable_to<int256, uint256>);
    // TODO we could expect many more of these!

    // we can consistently define bit_and and bit_or on any number type
    // regardless of whether it really is a byte sequence.
    template <typename NN> concept bit_arithmetic =
        requires (const NN &a, const NN &b) {
            { a & b } -> ImplicitlyConvertible<NN>;
            { a | b } -> ImplicitlyConvertible<NN>;
            { bit_and (a, b) } -> ImplicitlyConvertible<NN>;
            { bit_or (a, b) } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a, const NN &b) {
            { a &= b } -> Same<NN &>;
            { a |= b } -> Same<NN &>;
        } && requires (const NN &a, int i) {
            { a >> i } -> ImplicitlyConvertible<NN>;
            { a << i } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a, int i) {
            { a >>= i } -> Same<NN &>;
            { a <<= i } -> Same<NN &>;
        };

    template <typename NN> concept bit_arithmetic_big =
        bit_arithmetic<NN> /*&& requires (const NN &a) {
            { a & 0 } -> ImplicitlyConvertible<NN>;
            { a | 0 } -> ImplicitlyConvertible<NN>;
            { a ^ 0 } -> ImplicitlyConvertible<NN>;
            { 0 & a } -> ImplicitlyConvertible<NN>;
            { 0 | a } -> ImplicitlyConvertible<NN>;
            { 0 ^ a } -> ImplicitlyConvertible<NN>;
            { a & 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a | 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a ^ 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 & a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 | a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 ^ a } -> ImplicitlyConvertible<NN>;
            { a & 0u } -> ImplicitlyConvertible<NN>;
            { a | 0u } -> ImplicitlyConvertible<NN>;
            { a ^ 0u } -> ImplicitlyConvertible<NN>;
            { 0u & a } -> ImplicitlyConvertible<NN>;
            { 0u | a } -> ImplicitlyConvertible<NN>;
            { 0u ^ a } -> ImplicitlyConvertible<NN>;
            { a & 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a | 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a ^ 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u & a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u | a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u ^ a } -> ImplicitlyConvertible<NN>;
        }*/;

    //bit xor and invert operations cannot be defined consistently
    //on a number without complement twos.
    template <typename ZZ> concept signed_bit_arithmetic =
        bit_arithmetic<ZZ> && requires (const ZZ &a) {
            { ~a } -> ImplicitlyConvertible<ZZ>;
        } && requires (const ZZ &a, const ZZ &b) {
            { a ^ b } -> ImplicitlyConvertible<ZZ>;
            { bit_xor (a, b) } -> ImplicitlyConvertible<ZZ>;
        } && requires (ZZ &a, const ZZ &b) {
            { a ^= b } -> Same<ZZ &>;
        };

    template <typename NN> concept basic_arithmetic =
        requires (const NN &a) {
            { sign (a) };
            { is_positive (a) } -> Same<bool>;
            { is_negative (a) } -> Same<bool>;
            { is_zero (a) } -> Same<bool>;
            { square (a) } -> ImplicitlyConvertible<NN>;
            { quadrance (a) } -> ImplicitlyConvertible<NN>;
        } && requires (const NN &a, const NN &b) {
            { a + b } -> ImplicitlyConvertible<NN>;
            { a - b } -> ImplicitlyConvertible<NN>;
            { a * b } -> ImplicitlyConvertible<NN>;
            { a / b } -> ImplicitlyConvertible<NN>;
            { plus (a, b) } -> ImplicitlyConvertible<NN>;
            { minus (a, b) } -> ImplicitlyConvertible<NN>;
            { times (a, b) } -> ImplicitlyConvertible<NN>;
            { pow (a, b) } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a, const NN &b) {
            { a += b } -> Same<NN &>;
            { a -= b } -> Same<NN &>;
            { a *= b } -> Same<NN &>;
            { a /= b } -> Same<NN &>;
        };

    template <typename NN> concept basic_arithmetic_big =
        basic_arithmetic<NN> /*&& requires (const NN &a) {
            { a + 0 } -> ImplicitlyConvertible<NN>;
            { a - 0 } -> ImplicitlyConvertible<NN>;
            { a * 0 } -> ImplicitlyConvertible<NN>;
            { 0 + a } -> ImplicitlyConvertible<NN>;
            { 0 - a } -> ImplicitlyConvertible<NN>;
            { 0 * a } -> ImplicitlyConvertible<NN>;
            { a + 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a - 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a * 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 + a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 - a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 * a } -> ImplicitlyConvertible<NN>;
            { a + 0u } -> ImplicitlyConvertible<NN>;
            { a - 0u } -> ImplicitlyConvertible<NN>;
            { a * 0u } -> ImplicitlyConvertible<NN>;
            { 0u + a } -> ImplicitlyConvertible<NN>;
            { 0u - a } -> ImplicitlyConvertible<NN>;
            { 0u * a } -> ImplicitlyConvertible<NN>;
            { a + 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a - 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a * 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u + a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u - a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u * a } -> ImplicitlyConvertible<NN>;
        }*/;

    // the traits satisfied by all our numbers.
    template <typename N> concept basic_number =
        UnsignedNumberComparable<N> && incrementable<N> &&
        bit_arithmetic_big<N> && basic_arithmetic_big<N>;

    static_assert (basic_number<N>);
    static_assert (basic_number<Z>);
    static_assert (basic_number<N_bytes_little>);
    static_assert (basic_number<Z_bytes_little>);
    static_assert (basic_number<N_bytes_big>);
    static_assert (basic_number<Z_bytes_big>);
    static_assert (basic_number<Z_bytes_BC_little>);
    static_assert (basic_number<Z_bytes_BC_big>);
    static_assert (basic_number<uint128>);
    static_assert (basic_number<int128>);
    static_assert (basic_number<uint128_little>);
    static_assert (basic_number<int128_little>);
    static_assert (basic_number<uint128_big>);
    static_assert (basic_number<int128_big>);
    static_assert (basic_number<dec_uint>);
    static_assert (basic_number<dec_int>);
    static_assert (basic_number<hex_uint>);
    static_assert (basic_number<hex_int>);
    static_assert (basic_number<hex_int_BC>);
    static_assert (basic_number<base58_uint>);

    // numbers resembling built-in types will have homo_abs_and_negate
    template <typename N> concept homo_abs_and_negate = requires (const N &n) {
            { abs (n) } -> Same<N>;
            { negate (n) } -> Same<N>;
            { -n } -> Same<N>;
        };

    // number systems will use hetero.
    template <typename N, typename Z> concept hetero_abs_and_negate =
        requires (const N &n, const Z &z) {
            { abs (n) } -> Same<N>;
            { abs (z) } -> Same<N>;
            { negate (n) } -> Same<Z>;
            { negate (z) } -> Same<Z>;
            { -n } -> Same<Z>;
            { -z } -> Same<Z>;
        };

    // a number does not necessarily have mod operations because
    // it may require a signed version of that number.
    template <typename ZZ, typename NN = ZZ> concept modable =
        requires (const ZZ &a, const NN &b) {
            { a % b } -> ImplicitlyConvertible<NN>;
        } && requires (const ZZ &a, const math::nonzero<NN> &b) {
            { mod (a, b) } -> ImplicitlyConvertible<NN>;
            { negate_mod (a, b) } -> ImplicitlyConvertible<NN>;
        } && requires (const ZZ &a, const ZZ &b, const math::nonzero<NN> &c) {
            { plus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
            { minus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
            { times_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
            { pow_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        };

    template <typename NN> concept homo_modable =
        modable<NN> && requires (NN &a, const NN &b) {
            { a %= b } -> Same<NN &>;
        };

    // a number resembling a built-in number.
    template <typename NN> concept natural_number =
        basic_number<NN> && homo_modable<NN> &&
        requires (const NN &a, const math::nonzero<NN> &b) {
            { divide (a, b) } -> Same<division<NN>>;
        };

    static_assert (natural_number<base58_uint>);

    // a number resembling a built-in number.
    template <typename NN> concept integral_number =
        natural_number<NN> && homo_abs_and_negate<NN> && signed_bit_arithmetic<NN>;

    // for number types that resemble built in types, we enforce
    // the non-intuitive rule that when you add signed and unsigned
    // versions together, you get the unsigned version.
    template <typename NN, typename ZZ> concept integral_number_system =
        integral_number<NN> && integral_number<ZZ> && comparable_to<NN, ZZ> &&
        requires (const NN &n, const ZZ &z) {
            { n ^ z } -> ImplicitlyConvertible<NN>;
            { z ^ n } -> ImplicitlyConvertible<NN>;
            { n | z } -> ImplicitlyConvertible<NN>;
            { z | n } -> ImplicitlyConvertible<NN>;
            { n & z } -> ImplicitlyConvertible<NN>;
            { z & n } -> ImplicitlyConvertible<NN>;
            { n + z } -> ImplicitlyConvertible<NN>;
            { z + n } -> ImplicitlyConvertible<NN>;
            { n - z } -> ImplicitlyConvertible<NN>;
            { z - n } -> ImplicitlyConvertible<NN>;
            { n * z } -> ImplicitlyConvertible<NN>;
            { z * n } -> ImplicitlyConvertible<NN>;
            { n / z } -> ImplicitlyConvertible<NN>;
            { z / n } -> ImplicitlyConvertible<NN>;
            { n % z } -> ImplicitlyConvertible<NN>;
            { z % n } -> ImplicitlyConvertible<NN>;
        };

    static_assert (integral_number_system<uint128, int128>);
    static_assert (integral_number_system<uint160, int160>);
    static_assert (integral_number_system<uint256, int256>);
    static_assert (integral_number_system<uint512, int512>);

    static_assert (integral_number_system<uint128_big, int128_big>);
    static_assert (integral_number_system<uint160_big, int160_big>);
    static_assert (integral_number_system<uint256_big, int256_big>);
    static_assert (integral_number_system<uint512_big, int512_big>);

    static_assert (integral_number_system<uint128_little, int128_little>);
    static_assert (integral_number_system<uint160_little, int160_little>);
    static_assert (integral_number_system<uint256_little, int256_little>);
    static_assert (integral_number_system<uint512_little, int512_little>);

    // refering to pairs of numbers representing naturals and integers.
    // Bit invert and xor should return an integer if they take a natural.
    template <typename NN, typename ZZ> concept signed_bit_arithmetic_system =
        bit_arithmetic<NN> && signed_bit_arithmetic<ZZ> &&
        requires (const NN &a) {
            { ~a } -> ImplicitlyConvertible<ZZ>;
        } && requires (const NN &a, const NN &b) {
            { a ^ b } -> ImplicitlyConvertible<ZZ>;
            { bit_xor (a, b) } -> ImplicitlyConvertible<ZZ>;
        } && requires (const NN &a, const ZZ &b) {
            { a ^ b } -> ImplicitlyConvertible<NN>;
            { bit_xor (a, b) } -> ImplicitlyConvertible<NN>;
            { b ^ a } -> ImplicitlyConvertible<NN>;
            { bit_xor (b, a) } -> ImplicitlyConvertible<NN>;
        };

    // TODO need a basic arithmetic system to say
    // that the result of adding a signed and unsigned number
    // will be signed, etc.
    template <typename N, typename Z> concept number_system =
        natural_number<N> && basic_number<Z> &&
        hetero_abs_and_negate<N, Z> && modable<Z, N> &&
        requires (const Z &a, const math::nonzero<N> &b) {
            { divide (a, b) } -> Same<division<Z, N>>;
            { mod (a, b) } -> ImplicitlyConvertible<N>;
            { negate_mod (a, b) } -> ImplicitlyConvertible<N>;
        } && requires (const Z &a, const math::nonzero<Z> &b) {
            { divide (a, b) } -> Same<division<Z, N>>;
        };

    template <typename N, typename Z> concept bytes_number_system =
        number_system<N, Z> && signed_bit_arithmetic_system<N, Z>;

    static_assert (number_system<N, Z>);
    static_assert (number_system<dec_uint, dec_int>);
    static_assert (number_system<hex_uint, hex_int>);
    static_assert (bytes_number_system<N_bytes_little, Z_bytes_little>);
    static_assert (bytes_number_system<N_bytes_big, Z_bytes_big>);

    template <typename ZZ> concept bool_arithmetic =
        requires (bool b) {
            { ZZ {b} };
        } && requires (const ZZ &n) {
            { !n } -> ImplicitlyConvertible<ZZ>;
        } && requires (const ZZ &a, const ZZ &b) {
            { a && b } -> ImplicitlyConvertible<ZZ>;
            { a || b } -> ImplicitlyConvertible<ZZ>;
        };

    template <typename ZZ> concept bitcoin_arithmetic =
        natural_number<ZZ> && bit_arithmetic<ZZ> &&
        bool_arithmetic<ZZ> && homo_abs_and_negate<ZZ> &&
        requires (const ZZ &a) {
            { mul_2 (a) } -> ImplicitlyConvertible<ZZ>;
            { div_2 (a) } -> ImplicitlyConvertible<ZZ>;
            // we still have bit_invert and bit_xor but they
            // don't correspond to numerical calculations since
            // they are different for different representations
            // of the same number.
            { bit_not (a) } -> ImplicitlyConvertible<ZZ>;
        } && requires (const ZZ &a, const ZZ &b) {
            { bit_xor (a, b) } -> ImplicitlyConvertible<ZZ>;
        };

    static_assert (bitcoin_arithmetic<Z_bytes_BC_big>);
    static_assert (bitcoin_arithmetic<Z_bytes_BC_little>);
    static_assert (bitcoin_arithmetic<hex_int_BC>);

    template <typename N>
    void test_default_is_zero () {
        EXPECT_EQ (N {}, N {0});
    }

    TEST (Numbers, DefaultIsZero) {

        test_default_is_zero<N> ();
        test_default_is_zero<Z> ();
        test_default_is_zero<N_bytes_little> ();
        test_default_is_zero<N_bytes_big> ();
        test_default_is_zero<Z_bytes_little> ();
        test_default_is_zero<Z_bytes_big> ();
        test_default_is_zero<Z_bytes_BC_little> ();
        test_default_is_zero<Z_bytes_BC_big> ();
        test_default_is_zero<uint128> ();
        test_default_is_zero<int128> ();
        test_default_is_zero<dec_int> ();
        test_default_is_zero<dec_uint> ();
        test_default_is_zero<hex_int> ();
        test_default_is_zero<hex_uint> ();
        test_default_is_zero<hex_int_BC> ();
        test_default_is_zero<base58_uint> ();

    }

    // TODO need a test to ensure that ^ means power for
    // N and Z and the bitcoin numbers but not the other types.
    
    template <integral NN> requires basic_arithmetic<NN>
    struct test_whole_number {
        test_whole_number () {

            EXPECT_EQ (increment (NN {0u}), NN {1u});

            EXPECT_FALSE (NN {0u} < NN {0u});
            EXPECT_TRUE (NN {0u} <= NN {0u});
            EXPECT_FALSE (NN {0u} > NN {0u});
            EXPECT_TRUE (NN {0u} >= NN {0u});

            EXPECT_FALSE (NN {1u} < NN {0u});
            EXPECT_FALSE (NN {1u} <= NN {0u});
            EXPECT_TRUE (NN {1u} > NN {0u});
            EXPECT_TRUE (NN {1u} >= NN {0u});

            EXPECT_TRUE (NN {394u} < NN {479u});
            EXPECT_TRUE (NN {394u} <= NN {479u});
            EXPECT_FALSE (NN {394u} > NN {479u});
            EXPECT_FALSE (NN {394u} >= NN {479u});

            EXPECT_TRUE (NN {0x394u} < NN {0x479u});
            EXPECT_TRUE (NN {0x394u} <= NN {0x479u});
            EXPECT_FALSE (NN {0x394u} > NN {0x479u});
            EXPECT_FALSE (NN {0x394u} >= NN {0x479u});

            EXPECT_EQ (NN {0x479u} - NN {0x394u}, NN {0xe5u});

            EXPECT_EQ (NN {0x858u} + NN {0x1edu}, NN {0xa45u});

            EXPECT_EQ (NN {0x64u} * NN {0xau}, NN {0x3e8u});

        }
    };
    
    template <unsigned_integral NN> requires
    ImplicitlyConvertible<uint64, NN> &&
    ImplicitlyConvertible<uint32, NN> && requires () {
        { NN {0} };
        { NN {9223372036854775807} };
        { NN {"0"} };
    } && requires (const NN &n) {
        { uint64 (n) };
        { static_cast<N> (n) };
        { data::abs (n) } -> std::same_as<NN>;
    } && modable<NN> struct test_unsigned_number : test_whole_number<NN> {
        test_unsigned_number (string type = "") {
            EXPECT_EQ (N (NN (0)), N (0)) << " number: " << NN (0) << " vs " << N (0) << " merp " << N (NN (0));

            EXPECT_EQ (math::number::decrement<NN> {} (NN {0u}), NN {0u});
        }
    };
    
    template <signed_integral ZZ> requires std::convertible_to<int64, ZZ> &&
    requires (const ZZ &z) {
        { int64 (z) };
        { static_cast<Z> (z) };
    } struct test_signed_number : test_whole_number<ZZ> {
        test_signed_number () {

            ZZ zz_zero {0};
            ZZ zz_minus_one {-1};

            // convert to Z
            EXPECT_EQ (Z (zz_zero), Z (0));
            EXPECT_EQ (Z (zz_minus_one), Z (-1));

            EXPECT_EQ (math::number::decrement<ZZ> {} (zz_zero), zz_minus_one);

            EXPECT_EQ (math::number::increment<ZZ> {} (zz_zero), ZZ {1});
            EXPECT_EQ (math::number::increment<ZZ> {} (zz_minus_one), zz_zero);

            EXPECT_EQ (math::number::increment<ZZ> {} (ZZ {0xffu}), ZZ {0x100u});

            EXPECT_EQ (-zz_zero, zz_zero);
            EXPECT_EQ (-ZZ {1}, zz_minus_one);
            EXPECT_EQ (-zz_minus_one, ZZ {1});
        }
    };

    template <typename NN, typename ZZ> 
    requires requires (const ZZ &z) {
        { data::abs (z) } -> std::same_as<NN>;
    } && requires (const ZZ &a, const math::nonzero<ZZ> &b) {
        { data::divide (a, b) } -> std::same_as<division<ZZ, NN>>;
    } && requires (const NN &a, const ZZ &b) {
        { pow (b, a) } -> std::same_as<ZZ>;
        { a + b };
        { a - b };
        { a * b };
        { b + a };
        { b - a };
        { b * a };
        { b % a } -> ImplicitlyConvertible<NN>;
    } && requires (const NN &a, const math::nonzero<NN> &b) {
        { mul_2_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { square_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<NN>>;
    } && requires (const ZZ &a, const math::nonzero<NN> &b) {
        { data::divide (a, b) } -> std::same_as<division<ZZ, NN>>;
        { mod (a, b) } -> ImplicitlyConvertible<NN>;
        { negate_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<NN>>;
        { mul_2_mod (a, b) } -> ImplicitlyConvertible<NN>;
        { square_mod (a, b) } -> ImplicitlyConvertible<NN>;
    } && requires (const ZZ &a, const ZZ &b, const math::nonzero<NN> &c) {
        { plus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { minus_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { times_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
        { pow_mod (a, b, c) } -> ImplicitlyConvertible<NN>;
    } struct test_number_system : test_unsigned_number<NN>, test_signed_number<ZZ> {
        test_number_system (string type = ""): test_unsigned_number<NN> {type}, test_signed_number<ZZ> {} {}
    };

    template <typename ZZ>
    struct test_signed_number_system : test_signed_number<ZZ> {};
    
    TEST (Numbers, NumberSystem) {
        test_number_system<N, Z> {"N"};

        test_number_system<N_bytes_little, Z_bytes_little> {"N_bytes_little"};
        test_number_system<N_bytes_big, Z_bytes_big> {"N_bytes_big"};

        test_signed_number_system<Z_bytes_BC_little> {};
        test_signed_number_system<Z_bytes_BC_big> {};

        test_number_system<dec_uint, dec_int> {};

        test_number_system<hex_uint, hex_int> {};

        test_signed_number_system<hex_int_BC> {};

        test_unsigned_number<base58_uint> {};

    }
    
}
