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
    // As long as we have constructors for int, long, and long long
    // then the compiler will never be confused by a valid number literal.
    template <typename N> concept NumberConstructable =
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

    template <typename N> concept UnsignedNumberConstructable =
        NumberConstructable<N> && requires () {
            { N {0u} };
            { N {9007199254740992u} };
        } && requires (unsigned int x) {
            { N {x} };
        } && requires (unsigned long x) {
            { N {x} };
        } && requires (unsigned long long x) {
            { N {x} };
        };

    template <typename N, typename M> concept comparable_to =
        requires (const N &n, const M &m) {
            { n == m } -> Same<bool>;
            { n != m } -> Same<bool>;
            { n > m } -> Same<bool>;
            { n < m } -> Same<bool>;
            { n >= m } -> Same<bool>;
            { n <= m } -> Same<bool>;
            { m == n } -> Same<bool>;
            { m != n } -> Same<bool>;
            { m > n } -> Same<bool>;
            { m < n } -> Same<bool>;
            { m > n } -> Same<bool>;
            { m <= n } -> Same<bool>;
            { m >= n } -> Same<bool>;
        };

    template <typename X> concept NumberComparable =
        Ordered<X> &&
        comparable_to<X, int> &&
        comparable_to<X, long> &&
        comparable_to<X, long long>;

    template <typename X> concept UnsignedNumberComparable =
        NumberComparable<X> &&
        comparable_to<X, unsigned int> &&
        comparable_to<X, unsigned long> &&
        comparable_to<X, unsigned long long>;

    template <typename X> concept incrementable =
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
    // It can be assigned from a integer literal, has a concept
    // of being signed or unsigned.
    template <typename X> concept proto_integral =
        NumberComparable<X> && std::default_initializable<X> &&
        requires (const X &n) {
            { is_zero (n) } -> Same<bool>;
            { is_positive (n) } -> Same<bool>;
            { is_negative (n) } -> Same<bool>;
            { sign (n) };
        } && incrementable<X>;

    // NOTE: these definitions don't make sense anymore.
    template <typename A> concept signed_proto_integral =
        proto_integral<A> && signed_type<A> && requires (const A &n) {
            { negate (abs (n)) } -> Same<A>;
        };

    template <typename A> concept unsigned_proto_integral =
        proto_integral<A> && unsigned_type<A> && requires (const A &n) {
            { digits_base_2 (n) } -> Same<size_t>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept proto_integral_system =
        signed_proto_integral<Z> && proto_integral<N> && Convertible<N, Z> &&
        requires (const Z &a) {
            { abs (a) } -> Same<N>;
        } && requires (const N &a) {
            { negate (a) } -> Same<Z>;
        } && comparable_to<Z, N>;

    // bit_integral has bit operations.
    template <typename X> concept bit_integral =
        proto_integral<X> &&
        requires (const X &a) {
            {a << 1} -> ImplicitlyConvertible<X>;
            {a >> 1} -> ImplicitlyConvertible<X>;
            {a & 1} -> ImplicitlyConvertible<X>;
            {a | 1} -> ImplicitlyConvertible<X>;
        } && requires (X &a) {
            {a <<= 1} -> Same<X &>;
            {a >>= 1} -> Same<X &>;
            {a &= 1} -> Same<X &>;
            {a |= 1} -> Same<X &>;
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
            { a + b } -> ImplicitlyConvertible<X>;
            { a - b } -> ImplicitlyConvertible<X>;
        } && requires (const X &n) {
            { div_2 (n) } -> ImplicitlyConvertible<X>;
            { n + 1 } -> ImplicitlyConvertible<X>;
            { n - 1 } -> ImplicitlyConvertible<X>;
        } && requires (X &a, const X &b) {
            { a += b } -> Same<X &>;
            { a -= b } -> Same<X &>;
        } && requires (X &a) {
            { a += 1 } -> Same<X &>;
            { a -= 1 } -> Same<X &>;
        };

    template <typename A> concept signed_group_integral = group_integral<A> && signed_proto_integral<A>;
    template <typename A> concept unsigned_group_integral = group_integral<A> && unsigned_proto_integral<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept group_integral_system =
        proto_integral_system<Z, N> && signed_group_integral<Z> && group_integral<N> &&
        requires (const Z &a, const N &b) {
            { a + b } -> ImplicitlyConvertible<Z>;
            { a - b } -> ImplicitlyConvertible<Z>;
            { b + a } -> ImplicitlyConvertible<Z>;
            { b - a } -> ImplicitlyConvertible<Z>;
        } && requires (Z &a, const N &b) {
            { a += b } -> Same<Z &>;
            { a -= b } -> Same<Z &>;
        };

    // ring integral has * operations.
    template <typename X> concept ring_integral =
        group_integral<X> &&
        requires (const X &a, const X &b) {
            { a * b } -> ImplicitlyConvertible<X>;
        } && requires (const X &n) {
            { mul_2 (n) } -> ImplicitlyConvertible<X>;
            { n * 1 } -> ImplicitlyConvertible<X>;
        } && requires (X &a, const X &b) {
            { a *= b } -> Same<X &>;
        } && requires (X &a) {
            { a *= 1 } -> Same<X &>;
        };

    template <typename A> concept signed_ring_integral =
        ring_integral<A> && signed_type<A>;

    template <typename A> concept unsigned_ring_integral =
        proto_integral<A> && unsigned_type<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept ring_integral_system =
        group_integral_system<Z, N> && signed_ring_integral<Z> && ring_integral<N> &&
        requires (const Z &a, const N &b) {
            { a * b } -> ImplicitlyConvertible<Z>;
        } && requires (Z &a, const N &b) {
            { a *= b } -> ImplicitlyConvertible<Z &>;
        };

    template <typename X> concept integral =
        bit_integral<X> && ring_integral<X> &&
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
        integral<A> && signed_ring_integral<A> && signed_bit_integral<A> &&
        requires (const A &a, const A &b) {
            { divide (a, math::nonzero<A> {b}) } -> Same<division<A, to_unsigned<A>>>;
        };

    template <typename A> concept unsigned_integral =
        integral<A> && unsigned_ring_integral<A> && unsigned_bit_integral<A> &&
        requires (const A &a, const A &b) {
            { a % b} -> ImplicitlyConvertible<A>;
            { divide (a, math::nonzero<A> {b}) } -> Same<division<A, A>>;
        } && requires (A &a) {
            { a %= 1 } -> Same<A &>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept integral_system =
        signed_integral<Z> && integral<N> &&
        requires (const Z &a, const N &b) {
            { a % b } -> ImplicitlyConvertible<N>;
            { a / b } -> ImplicitlyConvertible<Z>;
            { b / a } -> ImplicitlyConvertible<Z>;
            { divide (a, math::nonzero<N> {b}) } -> Same<division<Z, N>>;
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

