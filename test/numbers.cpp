// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/concepts.hpp>
#include <data/numbers.hpp>
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
        can_use_string_literals<N> && BigNumberConstructableSigned<N>;

    template <typename N> concept conveniently_constructable =
        can_use_literals<N> && has_string_constructor<N> &&
        std::default_initializable<N> &&
        requires (std::istream &i, N &x) {
            { i >> x };
        };

    template <typename N> concept conveniently_constructable_unsigned =
        conveniently_constructable<N> && BigNumberConstructable<N>;

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

    static_assert (ImplicitlyConvertible<uint128, uint160>);
    static_assert (ImplicitlyConvertible<uint128, int160>);
    static_assert (ImplicitlyConvertible<int128, int160>);

    static_assert (ExplicitlyConvertible<uint160, uint128>);
    static_assert (ExplicitlyConvertible<int160, int128>);

    static_assert (ImplicitlyConvertible<uint128_little, uint160_little>);
    static_assert (ImplicitlyConvertible<uint128_little, int160_little>);
    static_assert (ImplicitlyConvertible<int128_little, int160_little>);

    static_assert (ImplicitlyConvertible<uint128_big, uint160_big>);
    static_assert (ImplicitlyConvertible<uint128_big, int160_big>);
    static_assert (ImplicitlyConvertible<int128_big, int160_big>);

    static_assert (comparable_to<N, Z>);
    static_assert (comparable_to<N_bytes_little, Z_bytes_little>);
    static_assert (comparable_to<Z_bytes_big, N_bytes_big>);
    static_assert (comparable_to<math::N_bytes<endian::little, unsigned short>, math::Z_bytes<endian::little, unsigned short>>);
    static_assert (comparable_to<math::Z_bytes<endian::big, unsigned short>, math::N_bytes<endian::big, unsigned short>>);
    static_assert (comparable_to<math::N_bytes<endian::little, unsigned int>, math::Z_bytes<endian::little, unsigned int>>);
    static_assert (comparable_to<math::Z_bytes<endian::big, unsigned int>, math::N_bytes<endian::big, unsigned int>>);
    static_assert (comparable_to<uint256, int256>);
    static_assert (comparable_to<int160, uint160>);
    static_assert (comparable_to<uint64_little, int64_little>);
    static_assert (comparable_to<int64_big, uint64_big>);
    // TODO we could expect many more of these!

    // we can consistently define bit_and and bit_or on any number type
    // regardless of whether it really is a byte sequence.
    template <typename NN> concept bit_arithmetic =
        bit_algebraic<NN> && requires (const NN &a, const NN &b) {
            { a & b } -> ImplicitlyConvertible<NN>;
            { a | b } -> ImplicitlyConvertible<NN>;
            { a ^ b } -> ImplicitlyConvertible<NN>;
            { bit_and (a, b) } -> ImplicitlyConvertible<NN>;
            { bit_or (a, b) } -> ImplicitlyConvertible<NN>;
            { bit_xor (a, b) } -> ImplicitlyConvertible<NN>;
        };

    template <typename NN> concept bit_arithmetic_big_unsigned =
        bit_arithmetic<NN> && proto_bit_number<NN> && bit_algebraic_unsigned<NN> &&
        requires (const NN &a) {
            { a & 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a | 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a ^ 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u & a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u | a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u ^ a } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a) {
            { a &= 9007199254740992u } -> Same<NN &>;
            { a |= 9007199254740992u } -> Same<NN &>;
            { a ^= 9007199254740992u } -> Same<NN &>;
        };

    template <typename NN> concept bit_arithmetic_big_signed =
        bit_arithmetic<NN> && proto_bit_number<NN> && bit_algebraic_signed<NN> &&
        requires (const NN &a) {
            { a & 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a | 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a ^ 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 & a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 | a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 ^ a } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a) {
            { a &= 0 } -> Same<NN &>;
            { a |= 0 } -> Same<NN &>;
            { a ^= 0 } -> Same<NN &>;
            { a &= 9007199254740992 } -> Same<NN &>;
            { a |= 9007199254740992 } -> Same<NN &>;
            { a ^= 9007199254740992 } -> Same<NN &>;
        };

    template <typename NN> concept bit_arithmetic_big =
        bit_arithmetic_big_unsigned<NN> &&
        bit_arithmetic_big_signed<NN>;

    template <typename NN, typename ZZ = NN> concept bit_negate_arithmetic =
        requires (const NN &n) {
            { ~n } -> ImplicitlyConvertible<ZZ>;
        };

    template <typename NN> concept complete_bit_arithmetic =
        bit_arithmetic<NN> && bit_negate_arithmetic<NN>;

    template <typename NN> concept complete_bit_arithmetic_big_signed =
        complete_bit_arithmetic<NN> && bit_arithmetic_big_signed<NN>;

    template <typename NN> concept complete_bit_arithmetic_big =
        complete_bit_arithmetic_big_signed<NN> && bit_arithmetic_big_unsigned<NN>;

    static_assert (complete_bit_arithmetic<uint32>);
    static_assert (complete_bit_arithmetic<uint32_little>);
    static_assert (complete_bit_arithmetic<uint32_big>);
    static_assert (complete_bit_arithmetic<int32>);
    static_assert (complete_bit_arithmetic<int32_little>);
    static_assert (complete_bit_arithmetic<int32_big>);
    static_assert (complete_bit_arithmetic<uint64>);
    static_assert (complete_bit_arithmetic<uint64_little>);
    static_assert (complete_bit_arithmetic<uint64_big>);
    static_assert (complete_bit_arithmetic<int64>);
    static_assert (complete_bit_arithmetic<int64_little>);
    static_assert (complete_bit_arithmetic<int64_big>);
    static_assert (complete_bit_arithmetic_big<uint80>);
    static_assert (complete_bit_arithmetic_big<uint80_little>);
    static_assert (complete_bit_arithmetic_big<uint80_big>);
    static_assert (complete_bit_arithmetic_big_signed<int80>);
    static_assert (complete_bit_arithmetic_big_signed<int80_little>);
    static_assert (complete_bit_arithmetic_big_signed<int80_big>);
    static_assert (complete_bit_arithmetic_big<uint128>);
    static_assert (complete_bit_arithmetic_big<uint128_little>);
    static_assert (complete_bit_arithmetic_big<uint128_big>);
    static_assert (complete_bit_arithmetic_big_signed<int128>);
    static_assert (complete_bit_arithmetic_big_signed<int128_little>);
    static_assert (complete_bit_arithmetic_big_signed<int128_big>);
    static_assert (complete_bit_arithmetic_big<uint160>);
    static_assert (complete_bit_arithmetic_big<uint160_little>);
    static_assert (complete_bit_arithmetic_big<uint160_big>);
    static_assert (complete_bit_arithmetic_big_signed<int160>);
    static_assert (complete_bit_arithmetic_big_signed<int160_little>);
    static_assert (complete_bit_arithmetic_big_signed<int160_big>);
    static_assert (complete_bit_arithmetic_big<uint256>);
    static_assert (complete_bit_arithmetic_big<uint256_little>);
    static_assert (complete_bit_arithmetic_big<uint256_big>);
    static_assert (complete_bit_arithmetic_big_signed<int256>);
    static_assert (complete_bit_arithmetic_big_signed<int256_little>);
    static_assert (complete_bit_arithmetic_big_signed<int256_big>);
    static_assert (complete_bit_arithmetic_big<uint512>);
    static_assert (complete_bit_arithmetic_big<uint512_little>);
    static_assert (complete_bit_arithmetic_big<uint512_big>);
    static_assert (complete_bit_arithmetic_big_signed<int512>);
    static_assert (complete_bit_arithmetic_big_signed<int512_little>);
    static_assert (complete_bit_arithmetic_big_signed<int512_big>);

    // natural numbers can have bit operations
    // defined on them that work all the same
    // way for each number.
    static_assert (bit_arithmetic_big_unsigned<N>);
    static_assert (bit_arithmetic_big_unsigned<N_bytes_little>);
    static_assert (bit_arithmetic_big_unsigned<N_bytes_big>);
    static_assert (bit_arithmetic_big_unsigned<dec_uint>);
    static_assert (bit_arithmetic_big_unsigned<hex_uint>);
    static_assert (bit_arithmetic_big_unsigned<base58_uint>);

    static_assert (!proto_bit_number<hex_int_BC>);

    template <typename NN> concept basic_arithmetic =
        requires (const NN &a) {
            { sign (a) };
            { is_positive (a) } -> Same<bool>;
            { is_negative (a) } -> Same<bool>;
            { is_zero (a) } -> Same<bool>;
            { square (a) } -> ImplicitlyConvertible<NN>;
        } && requires (const NN &a, const NN &b) {
            { a + b } -> ImplicitlyConvertible<NN>;
            { a - b } -> ImplicitlyConvertible<NN>;
            { a * b } -> ImplicitlyConvertible<NN>;
            { a / b } -> ImplicitlyConvertible<NN>;
            { plus (a, b) } -> ImplicitlyConvertible<NN>;
            //{ minus (a, b) } -> ImplicitlyConvertible<NN>;
            { times (a, b) } -> ImplicitlyConvertible<NN>;
            { pow (a, b) } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a, const NN &b) {
            { a += b } -> Same<NN &>;
            { a -= b } -> Same<NN &>;
            { a *= b } -> Same<NN &>;
            { a /= b } -> Same<NN &>;
        };

    template <typename N> concept basic_arithmetic_signed =
        basic_arithmetic<N> && ring_algebraic_signed<N>;

    template <typename N> concept basic_arithmetic_unsigned =
        basic_arithmetic<N> && ring_algebraic_unsigned<N>;

    // add division
    template <typename NN> concept basic_arithmetic_big_unsigned =
        basic_arithmetic_unsigned<NN> /*&& requires (const NN &a) {
            { a + 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a - 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { a * 9007199254740992u } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u + a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u - a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992u * a } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a) {
            { a += 9007199254740992u } -> Same<NN &>;
            { a -= 9007199254740992u } -> Same<NN &>;
            { a *= 9007199254740992u } -> Same<NN &>;
        }*/;

    template <typename NN> concept basic_arithmetic_big_signed =
        basic_arithmetic_signed<NN> /*&& requires (const NN &a) {
            { a + 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a - 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { a * 9007199254740992 } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 + a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 - a } -> ImplicitlyConvertible<NN>;
            { 9007199254740992 * a } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a) {
            { a += 9007199254740992 } -> Same<NN &>;
            { a -= 9007199254740992 } -> Same<NN &>;
            { a *= 9007199254740992 } -> Same<NN &>;
        }*/;

    template <typename NN> concept basic_arithmetic_big =
        basic_arithmetic_big_signed<NN> &&
        basic_arithmetic_big_unsigned<NN>;

    template <typename N> concept basic_number =
        proto_number<N> && basic_arithmetic<N> &&
        requires (const N &a) {
            requires Same<decltype (abs (a)), decltype (quadrance (a))>;
            { re (a) } -> Same<N>;
        } && requires (const N &a, const N &b) {
            { inner (a, b) } -> ImplicitlyConvertible<N>;
        };

    template <typename N> concept basic_number_big_unsigned =
        basic_number<N> && basic_arithmetic_big_unsigned<N>;

    template <typename N> concept basic_number_big_signed =
        basic_number<N> && basic_arithmetic_big_signed<N>;

    template <typename N> concept basic_number_big =
        basic_number<N> && basic_arithmetic_big<N>;

    static_assert (basic_number<uint32>);
    static_assert (basic_number<uint32_little>);
    static_assert (basic_number<uint32_big>);

    static_assert (basic_number<int32>);
    static_assert (basic_number<int32_little>);
    static_assert (basic_number<int32_big>);

    static_assert (basic_number<uint64>);
    static_assert (basic_number<uint64_little>);
    static_assert (basic_number<uint64_big>);

    static_assert (basic_number<int64>);
    static_assert (basic_number<int64_little>);
    static_assert (basic_number<int64_big>);

    static_assert (basic_number_big<uint128>);
    static_assert (basic_number_big_signed<int128>);
    static_assert (basic_number_big<uint128_little>);
    static_assert (basic_number_big_signed<int128_little>);
    static_assert (basic_number_big<uint128_big>);
    static_assert (basic_number_big_signed<int128_big>);

    static_assert (basic_number_big_unsigned<N>);
    static_assert (basic_number_big_unsigned<N_bytes_little>);
    static_assert (basic_number_big_unsigned<N_bytes_big>);
    static_assert (basic_number_big_unsigned<dec_uint>);
    static_assert (basic_number_big_unsigned<hex_uint>);
    static_assert (basic_number_big_unsigned<base58_uint>);

    static_assert (basic_number_big<Z>);
    static_assert (basic_number_big<Z_bytes_little>);
    static_assert (basic_number_big<Z_bytes_big>);
    static_assert (basic_number_big<Z_bytes_BC_little>);
    static_assert (basic_number_big<Z_bytes_BC_big>);
    static_assert (basic_number_big<dec_int>);
    static_assert (basic_number_big<hex_int>);
    static_assert (basic_number_big<hex_int_BC>);

    // numbers resembling built-in types will have homo_abs_and_negate
    template <typename N> concept homo_abs_and_negate =
        requires (const N &n) {
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
    // it may require an unsigned version of that number.
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

    // numbers that are homo modable are
    //   * built-in-like numbers
    //   * natural numbers.

    template <typename NN> concept homo_modable =
        modable<NN> && requires (NN &a, const NN &b) {
            { a %= b } -> Same<NN &>;
        };

    // an unaccompanied_number works without having to know
    // about any corresponding signed or unsigned type.
    template <typename N> concept unaccompanied_number =
        basic_number<N> && homo_modable<N> &&
        requires (const N &a, const math::nonzero<N> &b) {
            { divmod (a, b) } -> Same<division<N>>;
        };

    // a number resembling a built-in number.
    template <typename NN> concept natural_number =
        unaccompanied_number<NN> && Unsigned<NN>;

    template <typename NN> concept unaccompanied_integer =
        unaccompanied_number<NN> && Signed<NN>;

    template <typename NN> concept natural_number_big =
        natural_number<NN> && basic_number_big_unsigned<NN>;

    static_assert (ring_number_system<Z, N>);
    static_assert (ring_number_system<dec_int, dec_uint>);

    static_assert (natural_number_big<N>);
    static_assert (natural_number_big<N_bytes_little>);
    static_assert (natural_number_big<N_bytes_big>);
    static_assert (natural_number_big<math::N_bytes<endian::little, unsigned short>>);
    static_assert (natural_number_big<math::N_bytes<endian::big, unsigned short>>);
    static_assert (natural_number_big<math::N_bytes<endian::little, unsigned int>>);
    static_assert (natural_number_big<math::N_bytes<endian::big, unsigned int>>);
    static_assert (natural_number_big<math::N_bytes<endian::little, unsigned long>>);
    static_assert (natural_number_big<math::N_bytes<endian::big, unsigned long>>);
    static_assert (natural_number_big<math::N_bytes<endian::little, unsigned long long>>);
    static_assert (natural_number_big<math::N_bytes<endian::big, unsigned long long>>);
    static_assert (natural_number_big<dec_uint>);
    static_assert (natural_number_big<hex_uint>);
    static_assert (natural_number_big<base58_uint>);

    template <typename NN> concept complement_twos_number =
        basic_number<NN> && bit_arithmetic<NN>;

    // a number resembling a built-in number.
    template <typename NN> concept integral_number =
        complement_twos_number<NN> &&
        homo_abs_and_negate<NN> && homo_modable<NN> &&
        bit_negate_arithmetic<NN>;

    // for number types that resemble built in types, we enforce
    // the non-intuitive rule that when you add signed and unsigned
    // versions together, you get the unsigned version.
    // TODO need to say how divide works.
    template <typename NN, typename ZZ> concept integral_number_system =
        integral_number<NN> && integral_number<ZZ> && comparable_to<NN, ZZ> &&
        Unsigned<NN> && Signed<ZZ> &&
        bit_negate_arithmetic<NN> && bit_negate_arithmetic<ZZ> /*&&
        requires (const ZZ &a, const math::nonzero<NN> &b) {
            { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<NN>>;
        }*/ && requires (const NN &n, const ZZ &z) {
            // TODO need a way to say that we also have these
            // kinds of conversions when we use a number literal!
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

    static_assert (integral_number_system<uint32, int32>);
    static_assert (integral_number_system<uint64, int64>);
/*
    static_assert (integral_number_system<uint32_little, int32_little>);
    static_assert (integral_number_system<uint64_little, int64_little>);

    static_assert (integral_number_system<uint32_big, int32_big>);
    static_assert (integral_number_system<uint64_big, int64_big>);*/

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

    // TODO need a basic arithmetic system to say
    // that the result of adding a signed and unsigned number
    // will be signed, etc.
    template <typename N, typename Z> concept pure_number_system =
        natural_number<N> && basic_number<Z> &&
        Unsigned<N> && Signed<Z> &&
        hetero_abs_and_negate<N, Z> && modable<Z, N> &&
        requires (const Z &a, const math::nonzero<N> &b) {
            { divmod (a, b) } -> Same<division<Z, N>>;
            { mod (a, b) } -> ImplicitlyConvertible<N>;
            { negate_mod (a, b) } -> ImplicitlyConvertible<N>;
            { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<N>>;
        } && requires (const Z &a, const math::nonzero<Z> &b) {
            { divmod (a, b) } -> Same<division<Z, N>>;
            { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<Z>>;
        };

    template <typename N, typename Z> concept bytes_number_system =
        pure_number_system<N, Z> && bit_arithmetic<N> && complete_bit_arithmetic<Z> &&
        bit_negate_arithmetic<N, Z>;

    static_assert (bytes_number_system<N, Z>);
    static_assert (pure_number_system<dec_uint, dec_int>);
    static_assert (bytes_number_system<hex_uint, hex_int>);
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

    // TODO this ought to go in Bitcoin_numbers.cpp
    template <typename ZZ> concept bitcoin_arithmetic =
        unaccompanied_integer<ZZ> && basic_number_big<ZZ> && Signed<ZZ> &&
        bool_arithmetic<ZZ> && homo_abs_and_negate<ZZ> &&
        requires (const ZZ &a) {
            { is_positive_zero (a) } -> ImplicitlyConvertible<bool>;
            { is_negative_zero (a) } -> ImplicitlyConvertible<bool>;
            { mul_2 (a) } -> ImplicitlyConvertible<ZZ>;
            { div_2 (a) } -> ImplicitlyConvertible<ZZ>;
            // we still have bit_invert and bit_xor but they
            // don't correspond to numerical calculations since
            // they are different for different representations
            // of the same number.
            { bit_not (a) } -> ImplicitlyConvertible<ZZ>;
        } && requires (const ZZ &a, const ZZ &b) {
            { bit_xor (a, b) } -> ImplicitlyConvertible<ZZ>;
            { bit_or (a, b) } -> ImplicitlyConvertible<ZZ>;
            { bit_and (a, b) } -> ImplicitlyConvertible<ZZ>;
        };

    static_assert (bitcoin_arithmetic<Z_bytes_BC_big>);
    static_assert (bitcoin_arithmetic<Z_bytes_BC_little>);
    static_assert (bitcoin_arithmetic<hex_int_BC>);

    // number types suitable for number theory functions.
    // essentially, the number needs to be able to go negative in some way.
    template <typename N> concept number_theory_number =
        ring_integral<N> &&
        (homo_abs_and_negate<N> || Signed<N> || (Unsigned<N> && requires () {
            make_signed<N> {};
        })) && requires (const N &a, const math::nonzero<N> &b) {
            { invert_mod (a, b) } -> ImplicitlyConvertible<maybe<N>>;
        };

    static_assert (number_theory_number<N>);
    static_assert (number_theory_number<Z>);
    static_assert (number_theory_number<int32>);
    static_assert (number_theory_number<uint32>);
    static_assert (number_theory_number<int64>);
    static_assert (number_theory_number<uint64>);
    static_assert (number_theory_number<int128>);
    static_assert (number_theory_number<uint128>);
    static_assert (number_theory_number<int128>);
    static_assert (number_theory_number<uint128>);

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

    template <typename N>
    void test_natural_decrement_zero_is_zero () {
        EXPECT_EQ (decrement (N {0}), N {0});
    }

    TEST (Numbers, DecrementZeroIsZero) {

        test_natural_decrement_zero_is_zero<N> ();
        test_natural_decrement_zero_is_zero<N_bytes_little> ();
        test_natural_decrement_zero_is_zero<N_bytes_big> ();
        test_natural_decrement_zero_is_zero<dec_uint> ();
        test_natural_decrement_zero_is_zero<hex_uint> ();
        test_natural_decrement_zero_is_zero<base58_uint> ();

    }

    // TODO Need a decrement test for other kinds of numbers.

    // TODO need a test to ensure that ^ means power for
    // the bitcoin numbers but not the other types.
    
}
