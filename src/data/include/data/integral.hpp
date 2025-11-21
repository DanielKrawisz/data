// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INTEGRAL
#define DATA_MATH_INTEGRAL

// numbers of either endian of any size up to 64 bits using boost.
#include <data/encoding/endian.hpp>

#include <data/encoding/integer.hpp>

// bounded size numbers of any size (bigger than 64 bit)
#include <data/math/number/bounded.hpp>
#include <data/math/number/bytes.hpp>

#include <data/io/exception.hpp>

namespace data {

    template <typename X> concept Integral =
        WholeNumber<X> && bit_algebraic<X> &&
        math::homo_abs_and_negate<X> &&
        requires (const X &a, const X &b) {
            { a % b} -> ImplicitlyConvertible<X>;
            { divmod (a, math::nonzero<X> {b}) } -> Same<division<X, X>>;
        } && requires {
            { math::numeric_limits<X>::max () } -> ImplicitlyConvertible<X>;
            { math::numeric_limits<X>::min () } -> ImplicitlyConvertible<X>;
        };

    template <typename A> concept SignedIntegral =
        Integral<A> && div_number_signed<A> && proto_bit_signed<A>;

    template <typename A> concept UnsignedIntegral =
        Integral<A> && div_number_unsigned<A> && proto_bit_unsigned<A>;

    template <typename Z, typename N = Z> concept IntegralSystem =
        MultiplicativeIntegralSystem<Z, N> && bit_algebraic_to<N, Z> &&
        SignedIntegral<Z> && ring_algebraic_signed<Z> && bit_algebraic_signed<Z> &&
        UnsignedIntegral<N> && ring_algebraic_unsigned<N> && bit_algebraic_unsigned<N> &&
        requires (const Z &n) {
            { abs (n) } -> ImplicitlyConvertible<Z>;
        } && requires (const N &n) {
            { negate (n) } -> ImplicitlyConvertible<N>;
        };

    // fixed size numbers of any size, similar to the
    // built-in types.
    template <size_t size, std::unsigned_integral word = byte>
    using int_little = math::sint<endian::little, size, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using int_big = math::sint<endian::big, size, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using uint_little = math::uint<endian::little, size, word>;

    template <size_t size, std::unsigned_integral word = byte>
    using uint_big = math::uint<endian::big, size, word>;

    using  uint80 = math::uint<endian::little, 5, uint16>;
    using   int80 = math::sint<endian::little, 5, uint16>;

    using uint128 = math::uint<endian::little, 2, uint64>;
    using  int128 = math::sint<endian::little, 2, uint64>;

    using uint160 = math::uint<endian::little, 5, uint32>;
    using  int160 = math::sint<endian::little, 5, uint32>;

    using uint192 = math::uint<endian::little, 3, uint64>;
    using  int192 = math::sint<endian::little, 3, uint64>;

    using uint224 = math::uint<endian::little, 7, uint32>;
    using  int224 = math::sint<endian::little, 7, uint32>;

    using uint256 = math::uint<endian::little, 4, uint64>;
    using  int256 = math::sint<endian::little, 4, uint64>;

    using uint288 = math::uint<endian::little, 9, uint32>;
    using  int288 = math::sint<endian::little, 9, uint32>;

    using uint320 = math::uint<endian::little, 5, uint64>;
    using  int320 = math::sint<endian::little, 5, uint64>;

    using uint352 = math::uint<endian::little, 11, uint32>;
    using  int352 = math::sint<endian::little, 11, uint32>;

    using uint384 = math::uint<endian::little, 6, uint64>;
    using  int384 = math::sint<endian::little, 6, uint64>;

    using uint416 = math::uint<endian::little, 13, uint32>;
    using  int416 = math::sint<endian::little, 13, uint32>;

    using uint448 = math::uint<endian::little, 7, uint64>;
    using  int448 = math::sint<endian::little, 7, uint64>;

    using uint480 = math::uint<endian::little, 15, uint32>;
    using  int480 = math::sint<endian::little, 15, uint32>;

    using uint512 = math::uint<endian::little, 8, uint64>;
    using  int512 = math::sint<endian::little, 8, uint64>;

    using uint544 = math::uint<endian::little, 17, uint32>;
    using  int544 = math::sint<endian::little, 17, uint32>;

    using  uint80_little = uint_little<10>;
    using uint128_little = uint_little<16>;
    using uint160_little = uint_little<20>;
    using uint192_little = uint_little<24>;
    using uint224_little = uint_little<28>;
    using uint256_little = uint_little<32>;
    using uint320_little = uint_little<40>;
    using uint384_little = uint_little<48>;
    using uint448_little = uint_little<56>;
    using uint512_little = uint_little<64>;
    using uint544_little = uint_little<68>;

    using   int80_little = int_little<10>;
    using  int128_little = int_little<16>;
    using  int160_little = int_little<20>;
    using  int192_little = int_little<24>;
    using  int224_little = int_little<28>;
    using  int256_little = int_little<32>;
    using  int320_little = int_little<40>;
    using  int384_little = int_little<48>;
    using  int448_little = int_little<56>;
    using  int512_little = int_little<64>;
    using  int544_little = int_little<68>;

    using  uint80_big = uint_big<10>;
    using uint128_big = uint_big<16>;
    using uint160_big = uint_big<20>;
    using uint192_big = uint_big<24>;
    using uint224_big = uint_big<28>;
    using uint256_big = uint_big<32>;
    using uint320_big = uint_big<40>;
    using uint384_big = uint_big<48>;
    using uint448_big = uint_big<56>;
    using uint512_big = uint_big<64>;
    using uint544_big = uint_big<68>;

    using   int80_big = int_big<10>;
    using  int128_big = int_big<16>;
    using  int160_big = int_big<20>;
    using  int192_big = int_big<24>;
    using  int224_big = int_big<28>;
    using  int256_big = int_big<32>;
    using  int320_big = int_big<40>;
    using  int384_big = int_big<48>;
    using  int448_big = int_big<56>;
    using  int512_big = int_big<64>;
    using  int544_big = int_big<68>;

    static_assert (IntegralSystem<int64, uint64>);
    static_assert (IntegralSystem<int64, uint64>);
/*
    static_assert (IntegralSystem<int32_little, uint32_little>);
    static_assert (IntegralSystem<int64_little, uint64_little>);

    static_assert (IntegralSystem<int32_big, uint32_big>);
    static_assert (IntegralSystem<int64_big, uint64_big>);*/

    static_assert (IntegralSystem<int128, uint128>);
    static_assert (IntegralSystem<int160, uint160>);
    static_assert (IntegralSystem<int256, uint256>);
    static_assert (IntegralSystem<int512, uint512>);

    static_assert (IntegralSystem<int128_big, uint128_big>);
    static_assert (IntegralSystem<int160_big, uint160_big>);
    static_assert (IntegralSystem<int256_big, uint256_big>);
    static_assert (IntegralSystem<int512_big, uint512_big>);

    static_assert (IntegralSystem<int128_little, uint128_little>);
    static_assert (IntegralSystem<int160_little, uint160_little>);
    static_assert (IntegralSystem<int256_little, uint256_little>);
    static_assert (IntegralSystem<int512_little, uint512_little>);
}

#endif

