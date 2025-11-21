// Copyright (c) 2020-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC
#define DATA_ARITHMETIC

#include <data/types.hpp>
#include <data/maybe.hpp>
#include <data/math/nonzero.hpp>
#include <data/sign.hpp>
#include <data/abs.hpp>
#include <data/increment.hpp>
#include <data/divmod.hpp>
#include <data/math/infinite.hpp>

namespace data {

    // basic arithmetic
    template <typename A, typename B> constexpr auto plus (const A &, const B &);
    template <typename A, typename B> constexpr auto minus (const A &, const B &);
    template <typename A, typename B> constexpr auto times (const A &, const B &);
    template <typename A, typename Exp> constexpr A pow (const A &, const Exp &);

    template <typename A> auto constexpr square (const A &);

    template <typename dividend, typename divisor>
    constexpr auto divide (const dividend &a, const math::nonzero<divisor> &b);

    template <typename dividend, typename divisor>
    constexpr bool divides (const dividend &a, const math::nonzero<divisor> &b);

    template <typename A> constexpr bool even (const A &);
    template <typename A> constexpr bool odd (const A &);

    // modular arithmetic
    template <typename A, typename Mod = A> constexpr auto mod (const A &, const math::nonzero<Mod> &);

    template <typename A, typename Mod = A> constexpr auto negate_mod (const A &, const math::nonzero<Mod> &);

    template <typename A, typename B, typename Mod> constexpr auto plus_mod (const A &, const B &, const math::nonzero<Mod> &);
    template <typename A, typename B, typename Mod> constexpr auto minus_mod (const A &, const B &, const math::nonzero<Mod> &);

    template <typename A, typename B, typename Mod> constexpr auto times_mod (const A &, const B &, const math::nonzero<Mod> &);
    template <typename A, typename Exp = A, typename Mod = Exp> constexpr auto pow_mod (const A &, const Exp &, const math::nonzero<Mod> &);

    template <typename A, typename Mod> constexpr auto invert_mod (const A &x, const math::nonzero<Mod> &n);

    // helper functions for computing pow mod and times mod.
    template <typename A, typename Mod = A> constexpr auto mul_2_mod (const A &, const math::nonzero<Mod> &);
    template <typename A, typename Mod = A> constexpr auto square_mod (const A &, const math::nonzero<Mod> &);

    template <typename A> constexpr auto mod_2 (const A &);
    template <typename A> constexpr auto mul_2 (const A &);
    template <typename A> constexpr auto div_2 (const A &);

    template <typename A> constexpr bool inline even (const A &x) {
        return mod_2 (x) == 0;
    }

    template <typename A> constexpr bool inline odd (const A &x) {
        return mod_2 (x) == 1;
    }

    template <typename dividend, typename divisor>
    constexpr bool inline divides (const dividend &a, const math::nonzero<divisor> &b) {
        return divide (a, b) == 0;
    }

    // bit arithmetic
    template <typename A> constexpr auto bit_not (const A &);

    template <typename A, typename B> constexpr auto bit_add (const A &, const B &);
    template <typename A, typename B> constexpr auto bit_or (const A &, const B &);
    template <typename A, typename B> constexpr auto bit_xor (const A &, const B &);
    template <typename A, typename B> constexpr auto bit_nand (const A &, const A &);
    template <typename A, typename B> constexpr auto bit_nor (const A &, const A &);

    template <typename A> constexpr A bit_shift_left (const A &, uint32);
    template <typename A> constexpr A bit_shift_right (const A &, uint32);

    template <typename A> constexpr A inline bit_shift (const A &x, int32 i) {
        return i < 0 ? bit_shift_right (x, static_cast<uint32> (-i)) : bit_shift_left (x, static_cast<uint32> (i));
    }

    // may not exist
    template <typename A, typename Mod = A> constexpr auto invert_mod (const A &, const math::nonzero<Mod> &);

    // count digits in a number.
    template <typename A> size_t constexpr size_in_base_2 (const A &);

    template <typename A> constexpr auto mul_2_pow (const A &, uint32 u);

    template <typename A> constexpr auto mul_2 (const A &x) {
        return mul_2_pow (x, 1);
    }

    // We use two different concepts for numbers, one more
    // alligend with c++ standards and the other more closely
    // aligned with pure mathematics.

    // For numbers resembling c++ built-in numbers, we use the
    // word integral. These kinds of numbers satisfy
    //   * mixing signed and unsigned versions result in unsigned.
    //   * abs and negate return the same type as they were given.

    // For numbers more closely resembling pure mathematics we
    // use the word number.
    //   * mixing signed and unsigned results in signed.
    //   * abs on a signed number results in an unsigned number.
    //   * negate on an unsigned number results in a signed number.

    // NOTE: this page is in an incomplete state of flux. We had
    // an original design that didn't quite work and are in the
    // midst of resolving it.

    // As long as we have constructors for int, long, and long long
    // then the compiler will never be confused by a valid number literal.
    template <typename N> concept BigNumberConstructableSigned =
        requires () {
            { N {0} };
            // the maximum integer that can be represented accurately by a double.
            { N {9007199254740992} };
        } && requires (int x) {
            { N {x} };
        } && requires (long x) {
            { N {x} };
        } && requires (long long x) {
            { N {x} };
        };

    // we don't have an unsigned signed version because in practice
    // nothing would use it that doesn't also have signed.
    template <typename N> concept BigNumberConstructable =
        BigNumberConstructableSigned<N> && requires () {
            { N {0u} };
            { N {9007199254740992u} };
        } && requires (unsigned int x) {
            { N {x} };
        } && requires (unsigned long x) {
            { N {x} };
        } && requires (unsigned long long x) {
            { N {x} };
        };

    template <typename X> concept Incrementable =
        requires (const X &n) {
            { increment (n) };
            { decrement (n) };
        } && requires (X &n) {
            { ++n } -> Same<X &>;
            { n++ } -> Same<X>;
            { --n } -> Same<X &>;
            { n++ } -> Same<X>;
        };

    // a proto_integral type is starting to look like a number.
    template <typename X> concept proto_number =
        std::default_initializable<X> && Incrementable<X> &&
        NumberComparable<X> && requires (const X &n) {
            { abs (n) };
        } && requires (const X &a, int i) {
            { a >> i } -> ImplicitlyConvertible<X>;
            { a << i } -> ImplicitlyConvertible<X>;
        } && requires (X &a, int i) {
            { a >>= i } -> Same<X &>;
            { a <<= i } -> Same<X &>;
        };

    template <typename A> concept proto_signed =
        proto_number<A> && Signed<A> && requires (const A &n) {
            { negate (abs (n)) } -> Same<A>;
        };

    template <typename A> concept proto_unsigned =
        proto_number<A> && Unsigned<A> && requires (const A &n) {
            { size_in_base_2 (n) } -> Same<size_t>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N> concept proto_system =
        proto_number<Z> && proto_number<N> && Signed<Z> &&
        requires (const Z &a) {
            { negate (abs (a)) } -> Same<Z>;
        } && requires (const N &a) {
            { abs (negate (a)) } -> Same<N>;
        } && requires (const N &a) {
            { size_in_base_2 (a) } -> Same<size_t>;
        } && comparable_to<Z, N>;

    template <typename Z, typename N> concept proto_integral_system =
        proto_system<Z, N> && ImplicitlyConvertible<Z, N> &&
        Convertible<N, Z> &&
        math::homo_abs_and_negate<Z> && math::homo_abs_and_negate<N>;

    template <typename Z, typename N = Z> concept proto_number_system =
        proto_system<Z, N> && ImplicitlyConvertible<N, Z> &&
        // NOTE: this clause should be uncommented, but the numbers
        // don't work right ATM so we need to fix them.
        //ExplicitlyConvertible<Z, N> &&
        math::hetero_abs_and_negate<N, Z>;

    // here we look at types with bit operations defined on them.
    template <typename A, typename B> concept bit_algebraic_to =
        requires (const A &a, const B &b) {
            { a & b } -> ImplicitlyConvertible<A>;
            { a | b } -> ImplicitlyConvertible<A>;
            { a ^ b } -> ImplicitlyConvertible<A>;
            { b & a } -> ImplicitlyConvertible<A>;
            { b | a } -> ImplicitlyConvertible<A>;
            { b ^ a } -> ImplicitlyConvertible<A>;
        } && requires (A &a, const B &b) {
            { a &= b } -> Same<A &>;
            { a |= b } -> Same<A &>;
            { a ^= b } -> Same<A &>;
        };

    template <typename NN> concept bit_algebraic =
        bit_algebraic_to<NN, NN>;

    template <typename X> concept proto_bit_number =
        proto_number<X> && bit_algebraic<X>;

    template <typename X, typename Y>
    concept bit_algebraic_signed_to =
        requires (const X &a) {
            { a & 1 } -> ImplicitlyConvertible<Y>;
            { a | 1 } -> ImplicitlyConvertible<Y>;
            { a ^ 1 } -> ImplicitlyConvertible<Y>;
            { 1 & a } -> ImplicitlyConvertible<Y>;
            { 1 | a } -> ImplicitlyConvertible<Y>;
            { 1 & a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept bit_algebraic_unsigned_to =
        requires (const X &a) {
            { a & 1u } -> ImplicitlyConvertible<Y>;
            { a | 1u } -> ImplicitlyConvertible<Y>;
            { a ^ 1u } -> ImplicitlyConvertible<Y>;
            { 1u & a } -> ImplicitlyConvertible<Y>;
            { 1u | a } -> ImplicitlyConvertible<Y>;
            { 1u & a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept bit_algebraic_signed_big_to =
        bit_algebraic_signed_to<X, Y> &&
        requires (const X &a) {
            { a & 9007199254740992 } -> ImplicitlyConvertible<Y>;
            { a | 9007199254740992 } -> ImplicitlyConvertible<Y>;
            { a ^ 9007199254740992 } -> ImplicitlyConvertible<Y>;
            { 9007199254740992 & a } -> ImplicitlyConvertible<Y>;
            { 9007199254740992 | a } -> ImplicitlyConvertible<Y>;
            { 9007199254740992 ^ a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept bit_algebraic_unsigned_big_to =
        bit_algebraic_unsigned_to<X, Y> &&
        requires (const X &a) {
            { a & 9007199254740992u } -> ImplicitlyConvertible<Y>;
            { a | 9007199254740992u } -> ImplicitlyConvertible<Y>;
            { a ^ 9007199254740992u } -> ImplicitlyConvertible<Y>;
            { 9007199254740992u & a } -> ImplicitlyConvertible<Y>;
            { 9007199254740992u | a } -> ImplicitlyConvertible<Y>;
            { 9007199254740992u ^ a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X> concept bit_algebraic_signed =
        bit_algebraic_signed_to<X, X> && requires (X &a) {
            { a &= 1 } -> Same<X &>;
            { a |= 1 } -> Same<X &>;
            { a ^= 1 } -> Same<X &>;
        };

    template <typename X> concept bit_algebraic_unsigned =
        bit_algebraic_unsigned_to<X, X> && requires (X &a) {
            { a &= 1u } -> Same<X &>;
            { a |= 1u } -> Same<X &>;
            { a ^= 1u } -> Same<X &>;
        };

    template <typename X> concept bit_algebraic_signed_big =
        bit_algebraic_signed_big_to<X, X> &&
        bit_algebraic_signed<X> &&
        requires (X &a) {
            { a &= 9007199254740992 } -> Same<X &>;
            { a |= 9007199254740992 } -> Same<X &>;
            { a ^= 9007199254740992 } -> Same<X &>;
        };

    template <typename X> concept bit_algebraic_unsigned_big =
        bit_algebraic_unsigned_big_to<X, X> &&
        bit_algebraic_unsigned<X> &&
        requires (X &a) {
            { a &= 9007199254740992u } -> Same<X &>;
            { a |= 9007199254740992u } -> Same<X &>;
            { a ^= 9007199254740992u } -> Same<X &>;
        };

    template <typename X> concept proto_bit_unsigned =
        proto_bit_number<X> && bit_algebraic_unsigned<X> && Unsigned<X>;

    template <typename X> concept proto_bit_signed =
        proto_bit_number<X> && bit_algebraic_signed<X> && Signed<X>;

    // group_integral has + and - operations.
    // We have this concept because it is possible to define
    // multiplication in terms of addition.

    template <typename A, typename B> concept group_algebraic_to =
        requires (const A &a, const B &b) {
            { a + b } -> ImplicitlyConvertible<A>;
            { a - b } -> ImplicitlyConvertible<A>;
            { b + a } -> ImplicitlyConvertible<A>;
            { b - a } -> ImplicitlyConvertible<A>;
        } && requires (A &a, const B &b) {
            { a += b } -> Same<A &>;
            { a -= b } -> Same<A &>;
        };

    template <typename X> concept group_algebraic =
        group_algebraic_to<X, X> &&
        requires (const X &n) {
            { abs (n) };
            { div_2 (n) } -> ImplicitlyConvertible<X>;
            { mod_2 (n) } -> ImplicitlyConvertible<X>;
        };

    template <typename X> concept group_number =
        proto_number<X> && group_algebraic<X>;

    template <typename A, typename B> concept ring_algebraic_to =
        group_algebraic_to<A, B> &&
        requires (const A &a, const B &b) {
            { a * b } -> ImplicitlyConvertible<A>;
            { b * a } -> ImplicitlyConvertible<A>;
        } && requires (A &a, const B &b) {
            { a *= b } -> Same<A &>;
        };

    template <typename X> concept ring_algebraic =
        ring_algebraic_to<X, X>;

    // This is what we need to ensure that we can
    // construct a division algorithm.
    template <typename X> concept MultiplicativeNumber =
        group_number<X> && ring_algebraic<X> &&
        requires (const X &n) {
            { mul_2 (n) } -> ImplicitlyConvertible<X>;
        };

    template <typename X> concept RingNumber =
        MultiplicativeNumber<X> &&
        requires (const X &n) {
            { negate (n) };
        };

    template <typename A, typename B> concept div_algebraic_to =
        ring_algebraic_to<A, B> &&
        requires (const A &a, const B &b) {
            { a / b } -> ImplicitlyConvertible<A>;
            { b / a } -> ImplicitlyConvertible<A>;
        } && requires (A &a, const B &b) {
            { a /= b } -> Same<A &>;
        };

    template <typename X> concept div_algebraic = div_algebraic_to<X, X>;

    template <typename X> concept div_number =
        MultiplicativeNumber<X> && div_algebraic<X>;

    template <typename X, typename Y>
    concept group_algebraic_signed_to =
        requires (const X &a) {
            { a + 1 } -> ImplicitlyConvertible<Y>;
            { a - 1 } -> ImplicitlyConvertible<Y>;
            { 1 + a } -> ImplicitlyConvertible<Y>;
            { 1 - a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept group_algebraic_unsigned_to =
        requires (const X &a) {
            { a + 1u } -> ImplicitlyConvertible<Y>;
            { a - 1u } -> ImplicitlyConvertible<Y>;
            { 1u + a } -> ImplicitlyConvertible<Y>;
            { 1u - a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept ring_algebraic_signed_to =
        group_algebraic_signed_to<X, Y> &&
        requires (const X &a) {
            { a * 1 } -> ImplicitlyConvertible<Y>;
            { 1 * a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept ring_algebraic_unsigned_to =
        group_algebraic_unsigned_to<X, Y> &&
        requires (const X &a) {
            { a * 1u } -> ImplicitlyConvertible<Y>;
            //{ 1u * a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept div_algebraic_signed_to =
        ring_algebraic_signed_to<X, Y> &&
        requires (const X &a) {
            { a / 1 } -> ImplicitlyConvertible<Y>;
            //{ 1 / a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept div_algebraic_unsigned_to =
        ring_algebraic_unsigned_to<X, Y> /*&&
        requires (const X &a) {
            { a / 1u } -> ImplicitlyConvertible<Y>;
            { 1u / a } -> ImplicitlyConvertible<Y>;
        }*/;

    template <typename X, typename Y>
    concept group_algebraic_signed_big_to =
        group_algebraic_signed_to<X, Y> &&
        requires (const X &a) {
            { a + 9007199254740992 } -> ImplicitlyConvertible<Y>;
            { a - 9007199254740992 } -> ImplicitlyConvertible<Y>;
            { 9007199254740992 + a } -> ImplicitlyConvertible<Y>;
            { 9007199254740992 - a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept group_algebraic_unsigned_big_to =
        group_algebraic_unsigned_to<X, Y> &&
        requires (const X &a) {
            { a + 9007199254740992u } -> ImplicitlyConvertible<Y>;
            { a - 9007199254740992u } -> ImplicitlyConvertible<Y>;
            { 9007199254740992u - a } -> ImplicitlyConvertible<Y>;
            { 9007199254740992u - a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept ring_algebraic_signed_big_to =
        ring_algebraic_signed_to<X, Y> &&
        group_algebraic_signed_big_to<X, Y> &&
        requires (const X &a) {
            { a * 9007199254740992 } -> ImplicitlyConvertible<Y>;
            { 9007199254740992 * a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept ring_algebraic_unsigned_big_to =
        ring_algebraic_unsigned_to<X, Y> &&
        group_algebraic_unsigned_big_to<X, Y> &&
        requires (const X &a) {
            { a * 9007199254740992u } -> ImplicitlyConvertible<Y>;
            { 9007199254740992u * a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept div_algebraic_signed_big_to =
        div_algebraic_signed_to<X, Y> &&
        ring_algebraic_signed_big_to<X, Y> &&
        requires (const X &a) {
            { a / 9007199254740992 } -> ImplicitlyConvertible<Y>;
            //{ 9007199254740992 / a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X, typename Y>
    concept div_algebraic_unsigned_big_to =
        div_algebraic_unsigned_to<X, Y> &&
        ring_algebraic_unsigned_big_to<X, Y> &&
        requires (const X &a) {
            { a / 9007199254740992u } -> ImplicitlyConvertible<Y>;
            //{ 9007199254740992u / a } -> ImplicitlyConvertible<Y>;
        };

    template <typename X> concept group_algebraic_signed =
        group_algebraic_signed_to<X, X> && requires (X &a) {
            { a += 1 } -> Same<X &>;
            { a -= 1 } -> Same<X &>;
        };

    template <typename X> concept group_algebraic_unsigned =
        group_algebraic_unsigned_to<X, X> && requires (X &a) {
            { a += 1u } -> Same<X &>;
            { a -= 1u } -> Same<X &>;
        };

    template <typename X> concept ring_algebraic_signed =
        ring_algebraic_signed_to<X, X> && requires (X &a) {
            { a *= 1 } -> Same<X &>;
        };

    template <typename X> concept ring_algebraic_unsigned =
        ring_algebraic_unsigned_to<X, X> && requires (X &a) {
            { a *= 1u } -> Same<X &>;
        };

    template <typename X> concept div_algebraic_signed =
        div_algebraic_signed_to<X, X> && requires (X &a) {
            { a /= 1 } -> Same<X &>;
        };

    template <typename X> concept div_algebraic_unsigned =
        div_algebraic_unsigned_to<X, X> && requires (X &a) {
            { a /= 1u } -> Same<X &>;
        };

    template <typename X> concept group_algebraic_signed_big =
        group_algebraic_signed_big_to<X, X> &&
        group_algebraic_signed<X> &&
        requires (X &a) {
            { a += 9007199254740992 } -> Same<X &>;
            { a -= 9007199254740992 } -> Same<X &>;
        };

    template <typename X> concept group_algebraic_unsigned_big =
        group_algebraic_unsigned_big_to<X, X> &&
        group_algebraic_unsigned<X> &&
        requires (X &a) {
            { a += 9007199254740992u } -> Same<X &>;
            { a -= 9007199254740992u } -> Same<X &>;
        };

    template <typename X> concept ring_algebraic_signed_big =
        ring_algebraic_signed_big_to<X, X> &&
        ring_algebraic_signed<X> && requires (X &a) {
            { a *= 9007199254740992 } -> Same<X &>;
        };

    template <typename X> concept ring_algebraic_unsigned_big =
        ring_algebraic_unsigned_big_to<X, X> &&
        ring_algebraic_unsigned<X> && requires (X &a) {
            { a *= 9007199254740992u } -> Same<X &>;
        };

    template <typename X> concept div_algebraic_signed_big =
        ring_algebraic_signed_big_to<X, X> &&
        ring_algebraic_signed<X> && requires (X &a) {
            { a /= 9007199254740992 } -> Same<X &>;
        };

    template <typename X> concept div_algebraic_unsigned_big =
        ring_algebraic_unsigned_big_to<X, X> &&
        ring_algebraic_unsigned<X> && requires (X &a) {
            { a /= 9007199254740992u } -> Same<X &>;
        };

    // TODO many number types can use this.
    template <typename X> concept div_algebraic_big =
        div_algebraic_signed_big<X> && div_algebraic_unsigned_big<X>;

    template <typename A> concept group_number_signed =
        group_number<A> && proto_signed<A> && group_algebraic_signed<A>;

    template <typename A> concept group_number_unsigned =
        group_number<A> && proto_unsigned<A> && group_algebraic_unsigned<A>;

    template <typename A> concept group_number_signed_big =
        group_number_signed<A> && group_algebraic_signed_big<A>;

    template <typename A> concept group_number_unsigned_big =
        group_number_unsigned<A> && group_algebraic_unsigned_big<A>;

    template <typename A> concept ring_number_signed =
        group_number_signed<A> && ring_algebraic_signed<A>;

    template <typename A> concept ring_number_unsigned =
        group_number_unsigned<A> && ring_algebraic_unsigned<A>;

    template <typename A> concept ring_number_signed_big =
        ring_number_signed<A> && ring_algebraic_signed_big<A>;

    template <typename A> concept ring_number_unsigned_big =
        ring_number_unsigned<A> && ring_algebraic_unsigned_big<A>;

    template <typename A> concept div_number_signed =
        ring_number_signed<A> && div_algebraic_signed<A>;

    template <typename A> concept div_number_unsigned =
        ring_number_unsigned<A> && div_algebraic_unsigned<A>;

    template <typename A> concept div_number_signed_big =
        div_number_signed<A> && div_algebraic_signed_big<A>;

    template <typename A> concept div_number_unsigned_big =
        div_number_unsigned<A> && div_algebraic_unsigned_big<A>;

    // a number system is more like what a mathematician would expect.
    template <typename Z, typename N> concept group_number_system =
        group_number<Z> && group_number<N> && proto_number_system<Z, N> &&
        group_algebraic_signed<Z> && group_algebraic_unsigned<N> &&
        group_algebraic_to<Z, N>;

    // an integral system resembls a built in c++ number.
    template <typename Z, typename N> concept group_integral_system =
        group_number<Z> && group_number<N> && proto_integral_system<Z, N> &&
        group_algebraic_signed<Z> && group_algebraic_unsigned<N> &&
        group_algebraic_to<N, Z>;

    template <typename Z, typename N> concept group_number_system_big =
        group_number_system<Z, N> && group_number_signed_big<Z> && group_number_unsigned_big<N>;

    template <typename Z, typename N> concept group_integral_system_big =
        group_integral_system<Z, N> && group_number_signed_big<Z> && group_number_unsigned_big<N>;

    template <typename Z, typename N> concept MultiplicativeIntegralSystem =
        group_integral_system<Z, N> &&
        ring_algebraic_signed<Z> && ring_algebraic_unsigned<N> &&
        ring_algebraic_to<N, Z>;

    template <typename Z, typename N> concept MultiplicativeNumberSystem =
        group_number_system<Z, N> &&
        ring_algebraic_signed<Z> && ring_algebraic_unsigned<N> &&
        ring_algebraic_to<Z, N>;

    template <typename Z, typename N> concept ring_number_system_big =
        group_number_system<Z, N> && ring_number_signed_big<Z> && ring_number_unsigned_big<N>;

    template <typename Z, typename N> concept ring_integral_system_big =
        group_integral_system<Z, N> && ring_number_signed_big<Z> && ring_number_unsigned_big<N>;

    // TODO still need rules to say how mod is supposed to work.

    // a whole number could be signed or unsigned
    // and does not require a negate operation.
    template <typename Z> concept WholeNumber =
        div_number<Z> && requires (const Z &a, const Z &b) {
            { a % b };
            // NOTE this should be uncommented.
            //{ divmod (a, b) };
        } && (Signed<Z> || Unsigned<Z>);

    template <typename Z> concept Integer =
        WholeNumber<Z> && RingNumber<Z> && div_number_signed<Z>;

    // TODO require homo modable.
    template <typename Z> concept Natural =
        WholeNumber<Z> && div_number_unsigned<Z>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept number_system =
        MultiplicativeNumberSystem<Z, N> &&
        ring_algebraic_signed<Z> &&
        ring_algebraic_unsigned<N> &&
        requires (const Z &n) {
            { abs (n) } -> ImplicitlyConvertible<N>;
        } && requires (const N &n) {
            { negate (n) } -> ImplicitlyConvertible<Z>;
        } && requires (const Z &a, const N &b) {
            { a % b } -> ImplicitlyConvertible<N>;
            { a / b } -> ImplicitlyConvertible<Z>;
            { b / a } -> ImplicitlyConvertible<Z>;
            { divmod (a, math::nonzero<N> {b}) } -> Same<division<Z, N>>;
        };
}

namespace data::math::def {
    template <typename A, uint32 base> struct size_in_base;

    template <typename A> struct mul_2_pow;
    template <typename A> struct div_2;
    template <typename A> struct mod_2;

    template <typename A> struct square;

    template <typename A, typename B = A> struct plus;
    template <typename A, typename B = A> struct minus;

    template <typename A> struct bit_not;
    template <typename A, typename B = A> struct bit_and;
    template <typename A, typename B = A> struct bit_or;
    template <typename A, typename B = A> struct bit_xor;
    template <typename A> struct bit_shift_left;
    template <typename A> struct bit_shift_right;

    template <typename A, typename B = A> struct times;
    template <typename A, typename Exp = A> struct pow;

    template <typename A, typename B> struct times {
        constexpr auto operator () (const A &x, const B &y) const {
            return x * y;
        }
    };

    template <typename A, typename B = A> struct divide;

    template <typename A, typename Mod = A> struct mod;
    template <typename A, typename Mod = A> struct negate_mod;

    template <typename A, typename Mod = A> struct invert_mod;
    template <typename A, typename B = A, typename Mod = B> struct plus_mod;
    template <typename A, typename B = A, typename Mod = B> struct minus_mod;

    template <typename A, typename B = A, typename Mod = B> struct times_mod;
    template <typename A, typename Exp = A, typename Mod = Exp> struct pow_mod;

    template <typename A, typename Mod = A> struct mul_2_mod;
    template <typename A, typename Mod = A> struct square_mod;

    template <typename A, typename B = A> struct bit_nand {
        auto operator () (const A &x, const B &y) {
            return bit_or (bit_not (x), bit_not (x));
        }
    };

    template <typename A, typename B = A> struct bit_nor {
        auto operator () (const A &x, const B &y) {
            return bit_and (bit_not (x), bit_not (x));
        }
    };
}

namespace data {

    template <typename A> constexpr size_t inline size_in_base_2 (const A &x) {
        return math::def::size_in_base<A, 2> {} (x);
    }

    template <typename A> constexpr auto inline square (const A &x) {
        return math::def::square<A> {} (x);
    }

    template <typename A> constexpr auto inline mul_2_pow (const A &x, uint32 u) {
        return math::def::mul_2_pow<A> {} (x, u);
    }

    template <typename A> constexpr auto inline div_2 (const A &x) {
        return math::def::div_2<A> {} (x);
    }

    template <typename A> constexpr auto inline mod_2 (const A &x) {
        return math::def::mod_2<A> {} (x);
    }

    template <typename A, typename B> constexpr auto inline plus (const A &x, const B &y) {
        return math::def::plus<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto inline minus (const A &x, const B &y) {
        return math::def::minus<A, B> {} (x, y);
    }

    template <typename A> constexpr auto bit_not (const A &x) {
        return math::def::bit_not<A> {} (x);
    }

    template <typename A, typename B> constexpr auto inline bit_and (const A &x, const B &y) {
        return math::def::bit_and<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto inline bit_or (const A &x, const B &y) {
        return math::def::bit_or<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto bit_xor (const A &x, const B &y) {
        return math::def::bit_xor<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto bit_nand (const A &x, const A &y) {
        return math::def::bit_nand<A, B> {} (x, y);
    }

    template <typename A, typename B> constexpr auto bit_nor (const A &x, const A &y) {
        return math::def::bit_nor<A, B> {} (x, y);
    }

    template <typename A> constexpr A inline bit_shift_right (const A &x, uint32 i) {
        return math::def::bit_shift_right<A> {} (x, i);
    }

    template <typename A> constexpr A inline bit_shift_left (const A &x, uint32 i) {
        return math::def::bit_shift_left<A> {} (x, i);
    }

    template <typename A, typename B> constexpr auto inline times (const A &x, const B &y) {
        return math::def::times<A, B> {} (x, y);
    }

    template <typename A, typename Mod> constexpr A inline pow (const A &x, const Mod &y) {
        return math::def::pow<A, Mod> {} (x, y);
    }

    template <typename A, typename Mod> constexpr auto mod (const A &x, const math::nonzero<Mod> &n) {
        return math::def::mod<A, Mod> {} (x, n);
    }

    template <typename A, typename Mod> constexpr auto negate_mod (const A &x, const math::nonzero<Mod> &n) {
        return math::def::negate_mod<A, Mod> {} (x, n);
    }

    template <typename A, typename B, typename Mod> constexpr auto inline plus_mod (const A &x, const B &y, const math::nonzero<Mod> &n) {
        return math::def::plus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename B, typename Mod> constexpr auto inline minus_mod (const A &x, const B &y, const math::nonzero<Mod> &n) {
        return math::def::minus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename Exp, typename Mod> constexpr auto inline times_mod (const A &x, const Exp &y, const math::nonzero<Mod> &n) {
        return math::def::times_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Exp, typename Mod> constexpr auto inline pow_mod (const A &x, const Exp &y, const math::nonzero<Mod> &n) {
        return math::def::pow_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Mod> constexpr auto invert_mod (const A &x, const math::nonzero<Mod> &n) {
        return math::def::invert_mod<A, Mod> {} (x, n);
    }

    template <typename A, typename Mod> constexpr auto inline mul_2_mod (const A &x, const math::nonzero<Mod> &z) {
        return math::def::mul_2_mod<A, Mod> {} (x, z);
    }

    template <typename A, typename Mod> constexpr auto inline square_mod (const A &x, const math::nonzero<Mod> &z) {
        return math::def::square_mod<A, Mod> {} (x, z);
    }
}

namespace data::math {
    // for numbers with bit operations, we can define mul_2 and div_2 in terms of shifts
    template <proto_number A> constexpr A inline bit_mul_2_pow (const A &x, uint32 u) {
        return x << u;
    }

    template <proto_number A> constexpr A inline bit_div_2_positive_mod (const A &x) {
        return x >> 1;
    }

    template <proto_number A> constexpr A inline bit_div_2_negative_mod (const A &x) {
        return (x < 0 ? increment (x) : x) >> 1;
    }

    template <proto_bit_unsigned A> constexpr A inline bit_mod_2 (const A &x) {
        return x & 1u;
    }

    template <proto_bit_number A> constexpr A inline bit_mod_2_positive_mod (const A &x) {
        return abs (x) & 1;
    }

    template <proto_bit_number A> constexpr A inline bit_mod_2_negative_mod (const A &x) {
        auto m = abs (x) & 1;
        return is_negative (x) ? -m : m;
    }
}

// default behaviors for these functions is to use the operators.
namespace data::math::def {

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x + y };
    } struct plus<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x + y;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x * y };
    } struct times<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x * y;
        }
    };

    template <typename A> requires requires (const A &x) {
        { x * x };
    } struct square<A> {
        constexpr A operator () (const A &x) const {
            return x * x;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x & y };
    } struct bit_and<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x & y;
        }
    };

    template <typename A, typename B>
    requires requires (const A &x, const B &y) {
        { x | y };
    } struct bit_or<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x | y;
        }
    };

    // we don't have a similar definition for bit xor because sometimes
    // people use ^ for power instead.

    template <std::unsigned_integral X> struct size_in_base<X, 2> {
        constexpr size_t operator () (X x) {
            return std::bit_width (x);
        }
    };

    template <std::signed_integral X> struct size_in_base<X, 2> {
        constexpr size_t operator () (X x) {
            return std::bit_width (static_cast<std::make_signed_t<X>> (x));
        }
    };

    template <std::integral X> struct times<X, X> {
        constexpr X operator () (X x, X y) {
            return x * y;
        }

        constexpr math::nonzero<X> operator () (math::nonzero<X> x, math::nonzero<X> y) {
            return x.Value * y.Value;
        }
    };

    template <std::integral X> struct bit_not<X> {
        constexpr auto operator () (X n) {
            return static_cast<X> (~n);
        }
    };

    template <std::integral X> struct bit_xor<X> {
        constexpr auto operator () (X a, X b) {
            return static_cast<X> (a ^ b);
        }
    };

    template <std::integral X> struct mod<X> {
        constexpr auto operator () (const X &x, const nonzero<X> &n) const {
            return static_cast<X> (x % n.Value);
        }
    };

    template <typename A, typename Mod> struct negate_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return x >= 0 && x < n.Value ?
            // We need to cast to A here because of the endian arithmetic numbers.
            // We should be able to get rid of that. TODO
            data::mod (A (n.Value - x), n):
            data::mod (A (n.Value - data::mod (x, n)), n);
        }
    };

    template <typename A, typename Mod> struct mul_2_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::mul_2 (x), n);
        }
    };

    template <typename A, typename B, typename Mod> struct plus_mod {
        constexpr auto operator () (const A &x, const B &y, const math::nonzero<Mod> &n) const {
            return data::mod (data::plus (x, y), n);
        }
    };

    template <typename A, typename B, typename Mod> struct minus_mod {
        constexpr auto operator () (const A &x, const B &y, const nonzero<Mod> &n) const {
            return data::mod (data::minus (x, y), n);
        }
    };

    template <typename A, typename Mod> struct square_mod {
        constexpr auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::square (x), n);
        }
    };

    // note: should have a special case for integral that uses twice.
    template <typename A, typename B, typename Mod> struct times_mod {
        constexpr auto operator () (A a, B b, nonzero<Mod> n) {
            return data::mod (a * b, n);
        }
    };

    template <std::signed_integral X> struct div_2<X> {
        constexpr X operator () (X x) {
            return bit_div_2_negative_mod (x);
        }
    };

    template <std::unsigned_integral X> struct div_2<X> {
        constexpr X operator () (X x) {
            return bit_div_2_positive_mod (x);
        }
    };

    template <std::signed_integral X> struct mod_2<X> {
        constexpr X operator () (X x) {
            return bit_mod_2_negative_mod (x);
        }
    };

    template <std::unsigned_integral X> struct mod_2<X> {
        constexpr X operator () (X x) {
            return bit_mod_2 (x);
        }
    };
}

// we define bit operations in terms of arithmetic operations and vice versa.
namespace data::math {

    // we now define shifts in terms of mul_2 and div_2
    template <group_number A> constexpr A arithmetic_shift_right (const A &a, uint32 u) {
        A x = a;
        while (u-- > 0) x = mul_2 (x);
        return x;
    }

    template <group_number_unsigned A> constexpr A arithmetic_shift_left (const A &a, uint32 u) {
        A x = a;
        while (u-- > 0) x = data::div_2 (x);
        return x;
    }

    // we now implement plus and minus in terms of bit operations.
    // this only works for complement one or two.
    template <proto_bit_number A> constexpr A bit_plus (const A &a, const A &b) {
        A x = a;
        A c = b;
        while (c != 0) {
            x ^= c;
            c &= x <<= 1;
        }
        return x;
    }

    template <proto_bit_number A> constexpr A bit_minus (const A &a, const A &b) {
        A x = a;
        A c = b;
        while (c != 0) {
            x ^= c;
            c &= ~x <<= 1;
        }
        return x;
    }

    // for the rest of these, we need to know if A is signed or unsigned.
    template <group_number_unsigned A> constexpr A arithmetic_bit_and (const A &x, const A &y) {
        if (x == 0) return 0;
        if (y == 0) return 0;
        A rx = arithmetic_shift_right (x);
        A ry = arithmetic_shift_right (y);
        A digit = (x - arithmetic_shift_left (rx)) > 0 && (y - arithmetic_shift_left (ry)) > 0 ? 1 : 0;
        return arithmetic_shift_left (arithmatic_bit_and (rx, ry)) + digit;
    }

    template <group_number_unsigned A> constexpr A arithmetic_bit_or (const A &x, const A &y) {
        if (x == 0) return 0;
        if (y == 0) return 0;
        A rx = arithmetic_shift_right (x);
        A ry = arithmetic_shift_right (y);
        A digit = (x - arithmetic_shift_left (rx)) > 0 || (y - arithmetic_shift_left (ry)) > 0 ? 1 : 0;
        return arithmetic_shift_left (arithmatic_bit_or (rx, ry)) + digit;
    }

    template <group_number_unsigned A> constexpr A arithmetic_bit_xor (const A &x, const A &y);
}

namespace data::math::def {

    template <proto_bit_number A> struct bit_shift_right<A> {
        constexpr auto operator () (const A &x, uint32 i) const {
            return x >> i;
        }
    };

    template <proto_bit_number A> struct bit_shift_left<A> {
        constexpr auto operator () (const A &x, uint32 i) const {
            return x << i;
        }
    };

    template <proto_bit_number X> struct mul_2_pow<X> {
        constexpr X operator () (const X &x, uint32 pow) {
            return bit_mul_2_pow (x, pow);
        }
    };

    template <group_number A, group_number B> struct minus<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x - y;
        }
    };

    template <MultiplicativeNumber A, MultiplicativeNumber B> struct divide<A, B> {
        constexpr auto operator () (const A &a, const nonzero<B> &b) const {
            return divmod<A, B> {} (a, b).Quotient;
        }
    };

    template <MultiplicativeNumber A, MultiplicativeNumber B> struct mod<A, B> {
        constexpr auto operator () (const A &a, const nonzero<B> &b) const {
            return divmod<A, B> {} (a, b).Remainder;
        }
    };
}

#endif
