// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INTEGRAL
#define DATA_MATH_INTEGRAL

#include <data/abs.hpp>
#include <data/sign.hpp>
#include <data/arithmetic.hpp>
#include <data/divide.hpp>
#include <data/increment.hpp>
#include <data/io/exception.hpp>

namespace data {

    // a proto_integral type is starting to look like a number.
    // It can be assigned from a integer literal, has a concept
    // of being signed or unsigned.
    template <typename X> concept proto_integral =
        ordered<X> && requires {
            { X (0) };
        } && requires (const X &n) {
            { is_zero (n) } -> same_as<bool>;
            { is_positive (n) } -> same_as<bool>;
            { is_negative (n) } -> same_as<bool>;
            { sign (n) };
            { increment (n) };
        } && requires (X &n) {
            { ++n } -> same_as<X &>;
            { n++ } -> same_as<X>;
            { --n } -> same_as<X &>;
            { n++ } -> same_as<X>;
        } && requires (const X &a) {
            {a == 0} -> same_as<bool>;
            {a != 0} -> same_as<bool>;
            {a > 0} -> same_as<bool>;
            {a < 0} -> same_as<bool>;
            {a >= 0} -> same_as<bool>;
            {a <= 0} -> same_as<bool>;
        };

    template <typename A> concept signed_proto_integral =
        proto_integral<A> && signed_type<A> && requires (const A &n) {
            { negate (abs (n)) } -> same_as<A>;
            { decrement (n) } -> same_as<A>;
        };

    template <typename A> concept unsigned_proto_integral =
        proto_integral<A> && unsigned_type<A> && requires (const A &n) {
            { digits_base_2 (n) } -> same_as<size_t>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept proto_integral_system =
        signed_proto_integral<Z> && proto_integral<N> &&
        convertible_to<N, Z> &&
        requires (const Z &a) {
            { abs (a) } -> same_as<N>;
        } && requires (const N &a) {
            { negate (a) } -> same_as<Z>;
        } && requires (const Z &a, const N &b) {
            {a == b} -> same_as<bool>;
            {a < b} -> same_as<bool>;
            {a > b} -> same_as<bool>;
            {a <= b} -> same_as<bool>;
            {a >= b} -> same_as<bool>;
            {b == a} -> same_as<bool>;
            {b < a} -> same_as<bool>;
            {b > a} -> same_as<bool>;
            {b <= a} -> same_as<bool>;
            {b >= a} -> same_as<bool>;
        };

    // bit_integral has bit operations.
    template <typename X> concept bit_integral =
        proto_integral<X> &&
        requires (const X &a) {
            {a << 1} -> implicitly_convertible_to<X>;
            {a >> 1} -> implicitly_convertible_to<X>;
            {a & 1} -> implicitly_convertible_to<X>;
            {a | 1} -> implicitly_convertible_to<X>;
        } && requires (X &a) {
            {a <<= 1} -> same_as<X &>;
            {a >>= 1} -> same_as<X &>;
            {a &= 1} -> same_as<X &>;
            {a |= 1} -> same_as<X &>;
        };

    template <typename A> concept signed_bit_integral = bit_integral<A> && signed_proto_integral<A>;
    template <typename A> concept unsigned_bit_integral = bit_integral<A> && unsigned_proto_integral<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept bit_integral_system =
        proto_integral_system<Z, N> && signed_bit_integral<Z> && bit_integral<N>;

    // group_integral has + and - operations.
    template <typename X> concept group_integral =
        proto_integral<X> &&
        requires (const X &a, const X &b) {
            { a + b } -> implicitly_convertible_to<X>;
            { a - b } -> implicitly_convertible_to<X>;
        } && requires (const X &n) {
            { div_2 (n) } -> implicitly_convertible_to<X>;
            { n + 1 } -> implicitly_convertible_to<X>;
            { n - 1 } -> implicitly_convertible_to<X>;
        } && requires (X &a, const X &b) {
            { a += b } -> same_as<X &>;
            { a -= b } -> same_as<X &>;
        } && requires (X &a) {
            { a += 1 } -> same_as<X &>;
            { a -= 1 } -> same_as<X &>;
        };

    template <typename A> concept signed_group_integral = group_integral<A> && signed_proto_integral<A>;
    template <typename A> concept unsigned_group_integral = group_integral<A> && unsigned_proto_integral<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept group_integral_system =
        proto_integral_system<Z, N> && signed_group_integral<Z> && group_integral<N> &&
        requires (const Z &a, const N &b) {
            { a + b } -> implicitly_convertible_to<Z>;
            { a - b } -> implicitly_convertible_to<Z>;
            { b + a } -> implicitly_convertible_to<Z>;
            { b - a } -> implicitly_convertible_to<Z>;
        } && requires (Z &a, const N &b) {
            { a += b } -> same_as<Z &>;
            { a -= b } -> same_as<Z &>;
        };

    // ring integral has * operations.
    template <typename X> concept ring_integral =
        group_integral<X> &&
        requires (const X &a, const X &b) {
            { a * b } -> implicitly_convertible_to<X>;
        } && requires (const X &n) {
            { mul_2 (n) } -> implicitly_convertible_to<X>;
            { n * 1 } -> implicitly_convertible_to<X>;
        } && requires (X &a, const X &b) {
            { a *= b } -> same_as<X &>;
        } && requires (X &a) {
            { a *= 1 } -> same_as<X &>;
        };

    template <typename A> concept signed_ring_integral =
        ring_integral<A> && signed_type<A>;

    template <typename A> concept unsigned_ring_integral =
        proto_integral<A> && unsigned_type<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept ring_integral_system =
        group_integral_system<Z, N> && signed_ring_integral<Z> && ring_integral<N> &&
        requires (const Z &a, const N &b) {
            { a * b } -> implicitly_convertible_to<Z>;
        } && requires (Z &a, const N &b) {
            { a *= b } -> implicitly_convertible_to<Z &>;
        };

    template <typename X> concept integral =
        bit_integral<X> && ring_integral<X> &&
        requires (const X &a, const X &b) {
            { a / b } -> implicitly_convertible_to<X>;
        } && requires (X &a, const X &b) {
            { a /= b } -> same_as<X &>;
        } && requires (const X &a) {
            { a / 1 } -> implicitly_convertible_to<X>;
            { a % 1 };
        } && requires (X &a) {
            { a /= 1 } -> same_as<X &>;
        };

    template <typename A> concept signed_integral =
        integral<A> && signed_ring_integral<A> && signed_bit_integral<A> &&
        requires (const A &a, const A &b) {
            { divide (a, math::nonzero<A> {b}) } -> same_as<division<A, to_unsigned<A>>>;
        };

    template <typename A> concept unsigned_integral =
        integral<A> && unsigned_ring_integral<A> && unsigned_bit_integral<A> &&
        requires (const A &a, const A &b) {
            { a % b} -> implicitly_convertible_to<A>;
            { divide (a, math::nonzero<A> {b}) } -> same_as<division<A, A>>;
        } && requires (A &a) {
            { a %= 1 } -> same_as<A &>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept integral_system =
        signed_integral<Z> && integral<N> &&
        requires (const Z &a, const N &b) {
            { a % b } -> implicitly_convertible_to<N>;
            { a / b } -> implicitly_convertible_to<Z>;
            { b / a } -> implicitly_convertible_to<Z>;
            { divide (a, math::nonzero<N> {b}) } -> same_as<division<Z, N>>;
        };

}

namespace data::math {
    // for numbers with bit operations, we can define mul_2 and div_2 in terms of shifts
    template <bit_integral A> constexpr A inline bit_mul_2 (const A &x) {
        return x << 1;
    }

    template <unsigned_bit_integral A> constexpr A inline bit_div_2 (const A &x) {
        return x >> 1;
    }

    template <signed_bit_integral A> constexpr A inline bit_div_2 (const A &x) {
        return (data::is_negative (x) ? data::increment (x) : x) >> 1;
    }

    // we now define shifts in terms of mul_2 and div_2
    template <group_integral A> constexpr A arithmatic_shift_right (const A &a, uint32 u) {
        A x = a;
        while (u-- > 0) x = data::mul_2 (x);
        return x;
    }

    template <unsigned_group_integral A> constexpr A arithmatic_shift_left (const A &a, uint32 u) {
        A x = a;
        while (u-- > 0) x = data::div_2 (x);
        return x;
    }

    // we now implement plus and minus in terms of bit operations.
    // this only works for complement one or two.
    template <bit_integral A> constexpr A bit_plus (const A &a, const A &b) {
        A x = a;
        A c = b;
        while (c != 0) {
            x ^= c;
            c &= x <<= 1;
        }
        return x;
    }

    template <bit_integral A> constexpr A bit_minus (const A &a, const A &b) {
        A x = a;
        A c = b;
        while (c != 0) {
            x ^= c;
            c &= ~x <<= 1;
        }
        return x;
    }

    // for the rest of these, we need to know if A is signed or unsigned.
    template <unsigned_group_integral A> constexpr A arithmatic_bit_and (const A &x, const A &y) {
        if (x == 0) return 0;
        if (y == 0) return 0;
        A rx = arithmatic_shift_right (x);
        A ry = arithmatic_shift_right (y);
        A digit = (x - arithmatic_shift_left (rx)) > 0 && (y - arithmatic_shift_left (ry)) > 0 ? 1 : 0;
        return arithmatic_shift_left (arithmatic_bit_and (rx, ry)) + digit;
    }

    template <unsigned_group_integral A> constexpr A algebraic_bit_or (const A &x, const A &y) {
        if (x == 0) return 0;
        if (y == 0) return 0;
        A rx = arithmatic_shift_right (x);
        A ry = arithmatic_shift_right (y);
        A digit = (x - arithmatic_shift_left (rx)) > 0 || (y - arithmatic_shift_left (ry)) > 0 ? 1 : 0;
        return arithmatic_shift_left (arithmatic_bit_or (rx, ry)) + digit;
    }

    template <bit_integral A> struct bit_shift_right<A> {
        constexpr auto operator () (const A &x, uint32 i) const {
            return x >> i;
        }
    };

    template <bit_integral A> struct bit_shift_left<A> {
        constexpr auto operator () (const A &x, uint32 i) const {
            return x << i;
        }
    };

    template <bit_integral X> struct mul_2<X> {
        constexpr X operator () (const X &x) {
            return bit_mul_2 (x);
        }
    };

    template <bit_integral X> struct div_2<X> {
        constexpr X operator () (const X &x) {
            return bit_div_2 (x);
        }
    };

    template <group_integral A, group_integral B> struct minus<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x - y;
        }
    };
}

#endif

