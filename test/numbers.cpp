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
            { N::read ("0") };
            { N::read ("9007199254740992") };
        };

    template <typename N> concept has_string_constructor = requires (const std::string &x) {
            { N::read (x) };
        } && requires (const data::string &x) {
            { N::read (x) };
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
    static_assert (ImplicitlyConvertible<math::N_bytes<endian::little, unsigned short>, math::Z_bytes<endian::little, unsigned short>>);
    static_assert (ImplicitlyConvertible<math::N_bytes<endian::big, unsigned short>, math::Z_bytes<endian::big, unsigned short>>);

    // however, the opposite conversion cannot be implicit.
    static_assert (ExplicitlyConvertible<Z, N>);
    static_assert (ExplicitlyConvertible<Z_bytes_little, N_bytes_little>);
    static_assert (ExplicitlyConvertible<Z_bytes_big, N_bytes_big>);
    static_assert (ExplicitlyConvertible<math::Z_bytes<endian::little, unsigned short>, math::N_bytes<endian::little, unsigned short>>);
    static_assert (ExplicitlyConvertible<math::Z_bytes<endian::big, unsigned short>, math::N_bytes<endian::big, unsigned short>>);

    static_assert (ExplicitlyConvertible<uint64_big, int64_big>);
    static_assert (ImplicitlyConvertible<int64_big, uint64_big>);

    static_assert (ExplicitlyConvertible<uint64_little, int64_little>);
    static_assert (ImplicitlyConvertible<int64_little, uint64_little>);

    static_assert (ExplicitlyConvertible<uint128, int128>);
    static_assert (ImplicitlyConvertible<int128, uint128>);

    static_assert (ExplicitlyConvertible<uint128_little, int128_little>);
    static_assert (ImplicitlyConvertible<int128_little, uint128_little>);

    static_assert (ExplicitlyConvertible<uint128_big, int128_big>);
    static_assert (ImplicitlyConvertible<int128_big, uint128_big>);

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
        bit_arithmetic<NN> && proto_bit_number<NN> && bit_algebraic_unsigned_big<NN>;

    template <typename NN> concept bit_arithmetic_big_signed =
        bit_arithmetic<NN> && proto_bit_number<NN> && bit_algebraic_signed_big<NN>;

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
            { minus (a, b) } -> ImplicitlyConvertible<NN>;
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
        basic_arithmetic_unsigned<NN> &&
        ring_algebraic_unsigned_big<NN> &&
        NumberComparable<NN>;

    template <typename NN> concept basic_arithmetic_big_signed =
        basic_arithmetic_signed<NN> &&
        ring_algebraic_signed_big<NN> &&
        NumberComparable<NN>;

    template <typename NN> concept basic_arithmetic_big =
        basic_arithmetic_big_signed<NN> &&
        basic_arithmetic_big_unsigned<NN>;

    template <typename N> concept basic_number =
        proto_number<N> && basic_arithmetic<N> &&
        requires (const N &a) {
            requires Same<decltype (abs (a)), decltype (quadrance (a))>;
            { math::re (a) } -> Same<N>;
        } && requires (const N &a, const N &b) {
            { math::inner (a, b) } -> ImplicitlyConvertible<N>;
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

    // numbers that are homo modable (can be modded with itself) are
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
        math::homo_abs_and_negate<NN> && homo_modable<NN> &&
        bit_negate_arithmetic<NN>;

    // for number types that resemble built in types, we enforce
    // the non-intuitive rule that when you add signed and unsigned
    // versions together, you get the unsigned version.
    // TODO need to say how divide works.
    template <typename NN, typename ZZ> concept integral_number_system =
        IntegralSystem<ZZ, NN> &&
        integral_number<NN> && integral_number<ZZ> &&
        comparable_to<NN, ZZ> && Unsigned<NN> && Signed<ZZ> &&
        bit_negate_arithmetic<NN> && bit_negate_arithmetic<ZZ> &&
        bit_algebraic_unsigned_to<ZZ, NN> &&
        ring_algebraic_unsigned_to<ZZ, NN>;

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
        math::hetero_abs_and_negate<N, Z> && modable<Z, N> &&
        ring_algebraic_signed_to<N, Z> &&
        requires (const Z &a, const math::nonzero<N> &b) {
            { divmod (a, b) } -> Same<division<Z, N>>;
            { mod (a, b) } -> ImplicitlyConvertible<N>;
            { negate_mod (a, b) } -> ImplicitlyConvertible<N>;
        } && requires (const Z &a, const math::nonzero<Z> &b) {
            { divmod (a, b) } -> Same<division<Z, N>>;
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
        bool_arithmetic<ZZ> && math::homo_abs_and_negate<ZZ> &&
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
        MultiplicativeNumber<N> && requires (const N &x) {
            { negate (x) };
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

    // next we have typed test suites. We have several sets of numbers
    // that we use. The first contains all number types.

    template <typename X> struct Numbers : ::testing::Test {
        using N = X;
    };

    using numbers = ::testing::Types<
        uint32, int32, int32_little, uint32_little, int32_big, uint32_big,
        uint64, int64, int64_little, uint64_little, int64_big, uint64_big,
        uint80, int80, int80_little, uint80_little, int80_big, uint80_big,
        uint128, int128, int128_little, uint128_little, int128_big, uint128_big,
        uint160, int160, int160_little, uint160_little, int160_big, uint160_big,
        uint224, int224, int224_little, uint224_little, int224_big, uint224_big,
        uint256, int256, int256_little, uint256_little, int256_big, uint256_big,
        uint384, int384, int384_little, uint384_little, int384_big, uint384_big,
        uint512, int512, int512_little, uint512_little, int512_big, uint512_big,
        N, Z, N_bytes_little, N_bytes_big, Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        dec_uint, dec_int, hex_uint, hex_int, hex_int_BC, base58_uint>;

    TYPED_TEST_SUITE (Numbers, numbers);

    TYPED_TEST (Numbers, DefaultIsZero) {
        using N = typename TestFixture::N;
        EXPECT_EQ (N {}, N {0});
    }

    TYPED_TEST (Numbers, Multiply) {
        using N = typename TestFixture::N;
        EXPECT_EQ (N {0} * N {0}, N {0});
        EXPECT_EQ (N {0} * N {1}, N {0});
        EXPECT_EQ (N {1} * N {0}, N {0});
        EXPECT_EQ (N {0} * N {2}, N {0});
        EXPECT_EQ (N {2} * N {0}, N {0});
        EXPECT_EQ (N {1} * N {1}, N {1});
        EXPECT_EQ (N {1} * N {2}, N {2});
        EXPECT_EQ (N {2} * N {1}, N {2});
    }

    TYPED_TEST (Numbers, IncrementIsOne) {
        using N = typename TestFixture::N;
        EXPECT_EQ (increment (N {}), N {1});
    }

    TYPED_TEST (Numbers, DecrementOneIsZero) {
        using N = typename TestFixture::N;
        EXPECT_EQ (decrement (N {1}), N {0});
    }

    // signed vs unsigned numbers differ in how
    // decrement works.
    template <typename X> struct Integers : ::testing::Test {
        using Z = X;
    };

    using integers = ::testing::Types<
        int32, int32_little, int32_big,
        int64, int64_little, int64_big,
        int80, int80_little, int80_big,
        int128, int128_little, int128_big,
        int160, int160_little, int160_big,
        int224, int224_little, int224_big,
        int256, int256_little, int256_big,
        int384, int384_little, int384_big,
        int512, int512_little, int512_big,
        Z, Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        dec_int, hex_int, hex_int_BC>;

    template <typename X> struct Naturals : ::testing::Test {
        using N = X;
    };

    using naturals = ::testing::Types<
        uint32, uint32_little, uint32_big,
        uint64, uint64_little, uint64_big,
        uint80, uint80_little, uint80_big,
        uint128, uint128_little, uint128_big,
        uint160, uint160_little, uint160_big,
        uint224, uint224_little, uint224_big,
        uint256, uint256_little, uint256_big,
        uint384, uint384_little, uint384_big,
        uint512, uint512_little, uint512_big,
        N, N_bytes_little, N_bytes_big, dec_uint, hex_uint, base58_uint>;

    TYPED_TEST_SUITE (Naturals, naturals);

    TYPED_TEST_SUITE (Integers, integers);

    TYPED_TEST (Naturals, DecrementZeroIsZero) {
        using N = typename TestFixture::N;
        EXPECT_EQ (decrement (N {0}), N {0});
    }

    TYPED_TEST (Integers, DecrementZeroIsNegOne) {
        using Z = typename TestFixture::Z;
        EXPECT_EQ (decrement (Z {0}), Z {-1});
    }

    // next we take on bit operations.

    TYPED_TEST (Numbers, BitAnd) {
        using N = typename TestFixture::N;
        EXPECT_EQ (Z (5) & Z (2), Z (0));
        EXPECT_EQ (Z (6) & Z (3), Z (2));
    }

    TYPED_TEST (Numbers, BitOr) {
        using N = typename TestFixture::N;
        EXPECT_EQ (Z (5) | Z (2), Z (7));
        EXPECT_EQ (Z (6) | Z (3), Z (7));
    }

    TYPED_TEST (Numbers, BitXor) {
        using N = typename TestFixture::N;
        EXPECT_EQ (Z (5) ^ Z (2), Z (7));
        EXPECT_EQ (Z (6) ^ Z (3), Z (5));
    }

    TYPED_TEST (Numbers, PositiveLeftShiftIsMul2Pow) {
        using N = typename TestFixture::N;
        N test_val_1 {1};
        EXPECT_EQ (test_val_1 << 0, mul_2_pow (test_val_1, 0));
        EXPECT_EQ (test_val_1 << 1, mul_2_pow (test_val_1, 1));
        EXPECT_EQ (test_val_1 << 2, mul_2_pow (test_val_1, 2));
        EXPECT_EQ (test_val_1 << 3, mul_2_pow (test_val_1, 3));
        EXPECT_EQ (test_val_1 << 5, mul_2_pow (test_val_1, 5));
        EXPECT_EQ (test_val_1 << 8, mul_2_pow (test_val_1, 8));
        EXPECT_EQ (test_val_1 << 13, mul_2_pow (test_val_1, 13));
        N test_val_2 {1025973};
        EXPECT_EQ (test_val_2 << 0, mul_2_pow (test_val_2, 0));
        EXPECT_EQ (test_val_2 << 1, mul_2_pow (test_val_2, 1));
        EXPECT_EQ (test_val_2 << 2, mul_2_pow (test_val_2, 2));
        EXPECT_EQ (test_val_2 << 3, mul_2_pow (test_val_2, 3));
        EXPECT_EQ (test_val_2 << 5, mul_2_pow (test_val_2, 5));
        EXPECT_EQ (test_val_2 << 8, mul_2_pow (test_val_2, 8));
        EXPECT_EQ (test_val_2 << 13, mul_2_pow (test_val_2, 13));
    }

    // TODO right shift

    template <typename X> struct IntegersTwos : ::testing::Test {
        using Z = X;
    };

    using integers_twos = ::testing::Types<
        int32, int32_little, int32_big,
        int64, int64_little, int64_big,
        int80, int80_little, int80_big,
        int128, int128_little, int128_big,
        int160, int160_little, int160_big,
        int224, int224_little, int224_big,
        int256, int256_little, int256_big,
        int384, int384_little, int384_big,
        int512, int512_little, int512_big,
        Z, Z_bytes_little, Z_bytes_big,
        dec_int, hex_int>;

    TYPED_TEST_SUITE (IntegersTwos, integers_twos);

    // TODO bit operations on negative numbers and
    // and bit negate.

    using integers_BC = ::testing::Types<
        Z_bytes_BC_little, Z_bytes_BC_big,
        hex_int_BC>;

    template <typename X> struct IntegersBC : ::testing::Test {
        using Z = X;
    };

    TYPED_TEST_SUITE (IntegersBC, integers_BC);

    TYPED_TEST (IntegersBC, BitAnd) {
        using N = typename TestFixture::Z;
        EXPECT_EQ (-Z (5) & Z (3), Z (1));
        EXPECT_EQ (Z (6) & -Z (3), Z (2));
        EXPECT_EQ (-Z (6) & -Z (3), -Z (2));
    }

    TYPED_TEST (IntegersBC, BitOr) {
        using N = typename TestFixture::Z;
        EXPECT_EQ (-Z (5) | Z (2), -Z (7));
        EXPECT_EQ (Z (6) | -Z (3), -Z (7));
        EXPECT_EQ (-Z (4) | -Z (1), -Z (5));
    }

    TYPED_TEST (IntegersBC, BitXor) {
        using N = typename TestFixture::Z;
        EXPECT_EQ (-Z (5) ^ Z (2), -Z (7));
        EXPECT_EQ (Z (6) ^ -Z (3), -Z (5));
        EXPECT_EQ (-Z (4) ^ -Z (1), Z (5));
    }

    TYPED_TEST (IntegersBC, LeftShiftIsDiv2Pow) {
        using N = typename TestFixture::Z;
        N test_val_1 {-1};
        EXPECT_EQ (test_val_1 << 0, mul_2_pow (test_val_1, 0));
        EXPECT_EQ (test_val_1 << 1, mul_2_pow (test_val_1, 1));
        EXPECT_EQ (test_val_1 << 2, mul_2_pow (test_val_1, 2));
        EXPECT_EQ (test_val_1 << 3, mul_2_pow (test_val_1, 3));
        EXPECT_EQ (test_val_1 << 5, mul_2_pow (test_val_1, 5));
        EXPECT_EQ (test_val_1 << 8, mul_2_pow (test_val_1, 8));
        EXPECT_EQ (test_val_1 << 13, mul_2_pow (test_val_1, 13));
        N test_val_2 {-1025973};
        EXPECT_EQ (test_val_2 << 0, mul_2_pow (test_val_2, 0));
        EXPECT_EQ (test_val_2 << 1, mul_2_pow (test_val_2, 1));
        EXPECT_EQ (test_val_2 << 2, mul_2_pow (test_val_2, 2));
        EXPECT_EQ (test_val_2 << 3, mul_2_pow (test_val_2, 3));
        EXPECT_EQ (test_val_2 << 5, mul_2_pow (test_val_2, 5));
        EXPECT_EQ (test_val_2 << 8, mul_2_pow (test_val_2, 8));
        EXPECT_EQ (test_val_2 << 13, mul_2_pow (test_val_2, 13));
    }

    // big numbers are numbers that are not built-in and not boost::endian::arithmetic.
    // (they're all bigger than those numbers.)
    template <typename X> struct BigNumbers : ::testing::Test {
        using N = X;
    };

    using big_numbers = ::testing::Types<
        uint80, int80, int80_little, uint80_little, int80_big, uint80_big,
        uint128, int128, int128_little, uint128_little, int128_big, uint128_big,
        uint160, int160, int160_little, uint160_little, int160_big, uint160_big,
        uint224, int224, int224_little, uint224_little, int224_big, uint224_big,
        uint256, int256, int256_little, uint256_little, int256_big, uint256_big,
        uint384, int384, int384_little, uint384_little, int384_big, uint384_big,
        uint512, int512, int512_little, uint512_little, int512_big, uint512_big,
        N, Z, N_bytes_little, N_bytes_big, Z_bytes_little, Z_bytes_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        dec_uint, dec_int, hex_uint, hex_int, hex_int_BC, base58_uint>;

    TYPED_TEST_SUITE (BigNumbers, big_numbers);

    TYPED_TEST (BigNumbers, DivisionByZero) {
        using Z = typename TestFixture::N;
        EXPECT_THROW (Z {1} / Z {0}, math::division_by_zero);
    }

    TYPED_TEST (Numbers, DivisionByZero) {
        using Z = typename TestFixture::N;
        EXPECT_THROW (divide (Z {1}, math::nonzero {Z {0}}), math::division_by_zero);
    }

    TYPED_TEST (BigNumbers, ModByZero) {
        using N = typename TestFixture::N;
        EXPECT_THROW (N {1} % abs (N {0}), math::division_by_zero);
    }

    TYPED_TEST (Numbers, ModByZero) {
        using N = typename TestFixture::N;
        EXPECT_THROW ((mod (N {1}, math::nonzero {abs (N {0})})), math::division_by_zero);
    }

    TYPED_TEST (Integers, NoNegativePowers) {
        using Z = typename TestFixture::Z;
        EXPECT_THROW (pow (Z {2}, -Z {1}), math::negative_power);
    }

    TYPED_TEST (Numbers, DivMod) {
        using N = typename TestFixture::N;
        EXPECT_EQ ((divmod (Z {10}, math::nonzero {Z {3}})), (division {Z (3), abs (Z (1))}));
    }

    TYPED_TEST (Integers, DivMod) {
        using N = typename TestFixture::Z;
        EXPECT_EQ ((divmod (Z {10}, math::nonzero {Z {-3}})), (division {Z (-3), abs (Z (1))}));
    }

    using integers_neg_mod = ::testing::Types<
        int32, int32_little, int32_big,
        int64, int64_little, int64_big,
        int80, int80_little, int80_big,
        int128, int128_little, int128_big,
        int160, int160_little, int160_big,
        int224, int224_little, int224_big,
        int256, int256_little, int256_big,
        int384, int384_little, int384_big,
        int512, int512_little, int512_big,
        Z_bytes_BC_little, Z_bytes_BC_big,
        hex_int_BC>;

    using integers_pos_mod = ::testing::Types<
        Z, Z_bytes_little, Z_bytes_big,
        dec_int, hex_int>;

    template <typename X> struct IntegersPosMod : ::testing::Test {
        using Z = X;
    };

    template <typename X> struct IntegersNegMod : ::testing::Test {
        using Z = X;
    };

    TYPED_TEST_SUITE (IntegersPosMod, integers_pos_mod);
    TYPED_TEST_SUITE (IntegersNegMod, integers_neg_mod);

    TYPED_TEST (IntegersNegMod, DivMod) {
        using Z = typename TestFixture::Z;
        EXPECT_EQ ((divmod (Z {-10}, math::nonzero {Z {3}})), (division {Z (-3), Z (-1)}));
        EXPECT_EQ ((divmod (Z {-10}, math::nonzero {Z {-3}})), (division {Z (3), Z (-1)}));
    }

    TYPED_TEST (IntegersPosMod, DivMod) {
        using Z = typename TestFixture::Z;
        EXPECT_EQ ((divmod (Z {-10}, math::nonzero {Z {3}})), (division {Z (-4), abs (Z (2))}));
        EXPECT_EQ ((divmod (Z {-10}, math::nonzero {Z {-3}})), (division {Z (4), abs (Z (2))}));
    }

    TYPED_TEST (Naturals, Mod) {
        using N = typename TestFixture::N;
        EXPECT_EQ (N {23} % N {5}, N {3});
        EXPECT_EQ ((mod (N {23}, math::nonzero {N {5}})), N {3});
    }

    TYPED_TEST (IntegersPosMod, Mod) {
        using N = typename TestFixture::Z;
        EXPECT_EQ (N {23} % abs (N {5}), abs (N {3}));
        EXPECT_EQ ((mod (N {23}, math::nonzero {abs (N {5})})), abs (N {3}));
    }

    TYPED_TEST (IntegersNegMod, Mod) {
        using N = typename TestFixture::Z;
        EXPECT_EQ (N {23} % N {5}, N {3});
        EXPECT_EQ ((mod (N {23}, math::nonzero {N {5}})), N {3});
    }

    TYPED_TEST (Integers, Power) {
        using N = typename TestFixture::Z;
        EXPECT_THROW ((pow (Z {1}, -Z {3})), math::negative_power);
    }

}
