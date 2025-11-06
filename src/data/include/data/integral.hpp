// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INTEGRAL
#define DATA_MATH_INTEGRAL

#include <data/arithmetic.hpp>
#include <data/divmod.hpp>
#include <data/io/exception.hpp>

namespace data {

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

    // group_integral has + and - operations.
    // We have this concept because it is possible to define
    // multiplication in terms of addition.

    // now we have two types that go together as signed and unsigned versions of each other.

    // ring number has * operations.
    template <typename X> concept ring_number =
        group_number<X> && ring_algebraic_to<X, X> &&
        requires (const X &n) {
            { mul_2 (n) } -> ImplicitlyConvertible<X>;
        };

    template <typename A> concept ring_number_signed =
        ring_number<A> && proto_signed<A> && ring_algebraic_signed<A>;

    template <typename A> concept ring_number_unsigned =
        ring_number<A> && proto_unsigned<A> && ring_algebraic_unsigned<A>;

    template <typename A> concept ring_number_signed_big =
        ring_number_signed<A> && ring_algebraic_signed_big<A>;

    template <typename A> concept ring_number_unsigned_big =
        ring_number_unsigned<A> && ring_algebraic_unsigned_big<A>;

    template <typename Z, typename N = Z> concept ring_integral_system =
        group_integral_system<Z, N> &&
        ring_algebraic_signed<Z> && ring_algebraic_unsigned<N> &&
        ring_algebraic_to<N, Z>;

    template <typename Z, typename N = Z> concept ring_number_system =
        group_number_system<Z, N> &&
        ring_algebraic_signed<Z> && ring_algebraic_unsigned<N> &&
        ring_algebraic_to<Z, N>;

    template <typename Z> concept Integer =
        ring_number_signed<Z> &&
        requires (const Z &a, const Z &b) {
            { a / b } -> ImplicitlyConvertible<Z>;
        } && requires (Z &a, const Z &b) {
            { a /= b } -> Same<Z &>;
        };

    template <typename X> concept integral =
        bit_algebraic<X> && ring_number<X> &&
        requires (const X &a, const X &b) {
            { a / b } -> ImplicitlyConvertible<X>;
        } && requires (X &a, const X &b) {
            { a /= b } -> Same<X &>;
        } && requires (const X &a) {
            { a / 1 } -> ImplicitlyConvertible<X>;
            { a % 1 };
        } && requires (X &a) {
            { a /= 1 } -> Same<X &>;
        };

    template <typename A> concept signed_integral =
        integral<A> && ring_number_signed<A> && proto_bit_signed<A> &&
        requires (const A &a, const A &b) {
            { divmod (a, math::nonzero<A> {b}) } -> Same<division<A, to_unsigned<A>>>;
        };

    template <typename A> concept unsigned_integral =
        integral<A> && ring_number_unsigned<A> && proto_bit_unsigned<A> &&
        requires (const A &a, const A &b) {
            { a % b} -> ImplicitlyConvertible<A>;
            { divmod (a, math::nonzero<A> {b}) } -> Same<division<A, A>>;
        } && requires (A &a) {
            { a %= 1 } -> Same<A &>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept number_system =
        ring_number_system<Z, N> &&
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

    template <typename Z, typename N = Z> concept integral_system =
        ring_integral_system<Z, N> && bit_algebraic_to<N, Z> &&
        integral<Z> && ring_algebraic_signed<Z> && bit_algebraic_signed<Z> &&
        integral<N> && ring_algebraic_unsigned<N> && bit_algebraic_unsigned<N> &&
        ImplicitlyConvertible<Z, N> &&
        requires (const Z &n) {
            { abs (n) } -> ImplicitlyConvertible<Z>;
        } && requires (const N &n) {
            { negate (n) } -> ImplicitlyConvertible<N>;
        };

}

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

    template <ring_number A, ring_number B> struct divide<A, B> {
        constexpr auto operator () (const A &a, const nonzero<B> &b) const {
            return divmod<A, B> {} (a, b).Quotient;
        }
    };

    template <ring_number A, ring_number B> struct mod<A, B> {
        constexpr auto operator () (const A &a, const nonzero<B> &b) const {
            return divmod<A, B> {} (a, b).Remainder;
        }
    };
}

#endif

