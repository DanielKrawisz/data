// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INTEGRAL
#define DATA_MATH_INTEGRAL

#include <data/abs.hpp>
#include <data/sign.hpp>
#include <data/arithmetic.hpp>
#include <data/divmod.hpp>
#include <data/increment.hpp>
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

    template <typename X> concept NumberComparableSigned =
        Ordered<X> &&
        comparable_to<X, int> &&
        comparable_to<X, long> &&
        comparable_to<X, long long>;

    template <typename X> concept NumberComparable =
        NumberComparableSigned<X> &&
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
    template <typename X> concept proto_number =
        NumberComparable<X> && std::default_initializable<X> &&
        requires (const X &n) {
            { is_zero (n) } -> Same<bool>;
            { is_positive (n) } -> Same<bool>;
            { is_negative (n) } -> Same<bool>;
            { sign (n) } -> ImplicitlyConvertible<math::sign>;
            { abs (n) };
        } && incrementable<X>;

    template <typename A> concept proto_signed =
        proto_number<A> && Signed<A> && requires (const A &n) {
            { negate (abs (n)) } -> Same<A>;
        };

    template <typename A> concept proto_unsigned =
        proto_number<A> && Unsigned<A> && requires (const A &n) {
            { size_in_base_2 (n) } -> Same<size_t>;
        };

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept proto_integral_system =
        proto_number<Z> && Signed<Z> && proto_number<N> && Convertible<N, Z> &&
        requires (const Z &a) {
            { negate (abs (a)) } -> Same<Z>;
        } && requires (const N &a) {
            { size_in_base_2 (a) } -> Same<size_t>;
            { negate (a) };
        } && comparable_to<Z, N>;

    template <typename NN> concept bit_algebraic =
        requires (const NN &a, const NN &b) {
            { a & b } -> ImplicitlyConvertible<NN>;
            { a | b } -> ImplicitlyConvertible<NN>;
            { a ^ b } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a, const NN &b) {
            { a &= b } -> Same<NN &>;
            { a |= b } -> Same<NN &>;
            { a ^= b } -> Same<NN &>;
        } && requires (const NN &a, int i) {
            { a >> i } -> ImplicitlyConvertible<NN>;
            { a << i } -> ImplicitlyConvertible<NN>;
        } && requires (NN &a, int i) {
            { a >>= i } -> Same<NN &>;
            { a <<= i } -> Same<NN &>;
        };

    template <typename X> concept proto_bit_number =
        proto_number<X> && bit_algebraic<X>;

    template <typename X> concept bit_algebraic_signed =
        requires (const X &a) {
            { a & 1 } -> ImplicitlyConvertible<X>;
            { a | 1 } -> ImplicitlyConvertible<X>;
            { a ^ 1 } -> ImplicitlyConvertible<X>;
            { 1 & a } -> ImplicitlyConvertible<X>;
            { 1 | a } -> ImplicitlyConvertible<X>;
            { 1 & a } -> ImplicitlyConvertible<X>;
        } && requires (X &a) {
            { a &= 1 } -> Same<X &>;
            { a |= 1 } -> Same<X &>;
            { a ^= 1 } -> Same<X &>;
        };

    template <typename X> concept bit_algebraic_unsigned =
        requires (const X &a) {
            { a & 1u } -> ImplicitlyConvertible<X>;
            { a | 1u } -> ImplicitlyConvertible<X>;
            { a ^ 1u } -> ImplicitlyConvertible<X>;
            { 1u & a } -> ImplicitlyConvertible<X>;
            { 1u | a } -> ImplicitlyConvertible<X>;
            { 1u & a } -> ImplicitlyConvertible<X>;
        } && requires (X &a) {
            { a &= 1u } -> Same<X &>;
            { a |= 1u } -> Same<X &>;
            { a ^= 1u } -> Same<X &>;
        };

    template <typename X> concept proto_bit_unsigned =
        proto_bit_number<X> && bit_algebraic_unsigned<X> && Unsigned<X>;

    template <typename X> concept proto_bit_signed =
        proto_bit_number<X> && bit_algebraic_signed<X> && Signed<X>;

    // group_integral has + and - operations.
    // We have this concept because it is possible to define
    // multiplication in terms of addition.

    template <typename X> concept group_algebraic =
        requires (const X &a, const X &b) {
            { a + b } -> ImplicitlyConvertible<X>;
            { a - b } -> ImplicitlyConvertible<X>;
        } && requires (const X &n) {
            { div_2 (n) } -> ImplicitlyConvertible<X>;
            { abs (n) };
            { mod_2 (n) } -> ImplicitlyConvertible<X>;
            { div_2 (n) } -> ImplicitlyConvertible<X>;
        } && requires (X &a, const X &b) {
            { a += b } -> Same<X &>;
            { a -= b } -> Same<X &>;
        };

    template <typename X> concept group_algebraic_signed =
        requires (const X &a) {
            { a + 1 } -> ImplicitlyConvertible<X>;
            { a - 1 } -> ImplicitlyConvertible<X>;/*
            { 1 + a } -> ImplicitlyConvertible<X>;
            { 1 - a } -> ImplicitlyConvertible<X>;*/
        } && requires (X &a) {
            { a += 1 } -> Same<X &>;
            { a -= 1 } -> Same<X &>;
        };

    template <typename X> concept group_algebraic_unsigned =
        requires (const X &a) {
            { a + 1u } -> ImplicitlyConvertible<X>;
            { a - 1u } -> ImplicitlyConvertible<X>;/*
            { 1u + a } -> ImplicitlyConvertible<X>;
            { 1u - a } -> ImplicitlyConvertible<X>;*/
        } && requires (X &a) {
            { a += 1u } -> Same<X &>;
            { a -= 1u } -> Same<X &>;
        };

    template <typename X> concept group_integral =
        proto_number<X> && group_algebraic<X>;

    template <typename A> concept signed_group_integral =
        group_integral<A> && proto_signed<A> && group_algebraic_signed<A>;

    template <typename A> concept unsigned_group_integral =
        group_integral<A> && proto_unsigned<A> && group_algebraic_unsigned<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept group_number_system =
        proto_integral_system<Z, N> && group_integral<Z> && group_integral<N> &&
        group_algebraic_signed<Z> && group_algebraic_unsigned<N> &&
        requires (const Z &a, const N &b) {
            { a + b } -> ImplicitlyConvertible<Z>;
            { a - b } -> ImplicitlyConvertible<Z>;
            { b + a } -> ImplicitlyConvertible<Z>;
            { b - a } -> ImplicitlyConvertible<Z>;
        } && requires (Z &a, const N &b) {
            { a += b } -> Same<Z &>;
            { a -= b } -> Same<Z &>;
        };

    template <typename Z, typename N = Z> concept group_integral_system =
        proto_integral_system<Z, N> && group_integral<Z> && group_integral<N> &&
        group_algebraic_signed<Z> && group_algebraic_unsigned<N> &&
        requires (const Z &a, const N &b) {
            { a + b } -> ImplicitlyConvertible<N>;
            { a - b } -> ImplicitlyConvertible<N>;
            { b + a } -> ImplicitlyConvertible<N>;
            { b - a } -> ImplicitlyConvertible<N>;
        } /*&& requires (Z &a, const N &b) {
            { a += b } -> Same<Z &>;
            { a -= b } -> Same<Z &>;
        }*/;

    // ring integral has * operations.
    template <typename X> concept ring_integral =
        group_integral<X> &&
        requires (const X &a, const X &b) {
            { a * b } -> ImplicitlyConvertible<X>;
        } && requires (const X &n) {
            { mul_2 (n) } -> ImplicitlyConvertible<X>;
        } && requires (X &a, const X &b) {
            { a *= b } -> Same<X &>;
        };

    template <typename X> concept ring_algebraic_signed =
        group_algebraic_signed<X> &&
        requires (const X &a) {
            { a * 1 } -> ImplicitlyConvertible<X>;/*
            { 1 * a } -> ImplicitlyConvertible<X>;*/
        } && requires (X &a) {
            { a *= 1 } -> Same<X &>;
        };

    template <typename X> concept ring_algebraic_unsigned =
        group_algebraic_unsigned<X> &&
        requires (const X &a) {
            { a * 1u } -> ImplicitlyConvertible<X>;/*
            { 1u * a } -> ImplicitlyConvertible<X>;*/
        } && requires (X &a) {
            { a *= 1u } -> Same<X &>;
        };

    template <typename A> concept signed_ring_integral =
        ring_integral<A> && proto_signed<A> && ring_algebraic_signed<A>;

    template <typename A> concept unsigned_ring_integral =
        ring_integral<A> && proto_unsigned<A> && ring_algebraic_unsigned<A>;

    // now we have two types that go together as signed and unsigned versions of each other.
    template <typename Z, typename N = Z> concept ring_integral_system =
        group_integral_system<Z, N> &&
        ring_algebraic_signed<Z> && ring_algebraic_unsigned<N> &&
        requires (const Z &a, const N &b) {
            { a * b } -> ImplicitlyConvertible<N>;
        } /*&& requires (Z &a, const N &b) {
            { a *= b } -> ImplicitlyConvertible<Z &>;
        }*/;

    template <typename Z, typename N = Z> concept ring_number_system =
        group_number_system<Z, N> &&
        ring_algebraic_signed<Z> && ring_algebraic_unsigned<N> &&
        requires (const Z &a, const N &b) {
            { a * b } -> ImplicitlyConvertible<Z>;
        } && requires (Z &a, const N &b) {
            { a *= b } -> ImplicitlyConvertible<Z &>;
        };

    template <typename Z> concept Integer =
        signed_ring_integral<Z> &&
        requires (const Z &a, const Z &b) {
            { a / b } -> ImplicitlyConvertible<Z>;
        } && requires (Z &a, const Z &b) {
            { a /= b } -> Same<Z &>;
        };

    template <typename X> concept integral =
        bit_algebraic<X> && ring_integral<X> &&
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
        integral<A> && signed_ring_integral<A> && proto_bit_signed<A> &&
        requires (const A &a, const A &b) {
            { divmod (a, math::nonzero<A> {b}) } -> Same<division<A, to_unsigned<A>>>;
        };

    template <typename A> concept unsigned_integral =
        integral<A> && unsigned_ring_integral<A> && proto_bit_unsigned<A> &&
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
        ring_integral_system<Z, N> &&
        integral<Z> && ring_algebraic_signed<Z> && bit_algebraic_signed<Z> &&
        integral<N> && ring_algebraic_unsigned<N> && bit_algebraic_unsigned<N> &&
        //ImplicitlyConvertible<Z, N> &&
        requires (const Z &n) {
            { abs (n) } -> ImplicitlyConvertible<Z>;
        } && requires (const N &n) {
            { negate (n) } -> ImplicitlyConvertible<N>;
        };

}

namespace data::math {
    // for numbers with bit operations, we can define mul_2 and div_2 in terms of shifts
    template <proto_bit_number A> constexpr A inline bit_mul_2_pow (const A &x, uint32 u) {
        return x << u;
    }

    template <proto_bit_unsigned A> constexpr A inline bit_div_2 (const A &x) {
        return x >> 1;
    }

    template <proto_bit_number A> constexpr A inline bit_mod_2 (const A &x) {
        return abs (x) & 1;
    }

    template <proto_bit_unsigned A> constexpr A inline bit_mod_2 (const A &x) {
        return x & 1u;
    }

    template <proto_bit_signed A> constexpr A inline bit_div_2 (const A &x) {
        return (data::is_negative (x) ? data::increment (x) : x) >> 1;
    }

    // we now define shifts in terms of mul_2 and div_2
    template <group_integral A> constexpr A arithmetic_shift_right (const A &a, uint32 u) {
        A x = a;
        while (u-- > 0) x = mul_2 (x);
        return x;
    }

    template <unsigned_group_integral A> constexpr A arithmetic_shift_left (const A &a, uint32 u) {
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
    template <unsigned_group_integral A> constexpr A arithmetic_bit_and (const A &x, const A &y) {
        if (x == 0) return 0;
        if (y == 0) return 0;
        A rx = arithmetic_shift_right (x);
        A ry = arithmetic_shift_right (y);
        A digit = (x - arithmetic_shift_left (rx)) > 0 && (y - arithmetic_shift_left (ry)) > 0 ? 1 : 0;
        return arithmetic_shift_left (arithmatic_bit_and (rx, ry)) + digit;
    }

    template <unsigned_group_integral A> constexpr A arithmetic_bit_or (const A &x, const A &y) {
        if (x == 0) return 0;
        if (y == 0) return 0;
        A rx = arithmetic_shift_right (x);
        A ry = arithmetic_shift_right (y);
        A digit = (x - arithmetic_shift_left (rx)) > 0 || (y - arithmetic_shift_left (ry)) > 0 ? 1 : 0;
        return arithmetic_shift_left (arithmatic_bit_or (rx, ry)) + digit;
    }

    template <unsigned_group_integral A> constexpr A arithmetic_bit_xor (const A &x, const A &y);
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

    template <proto_bit_number X> struct div_2<X> {
        constexpr X operator () (const X &x) {
            return bit_div_2 (x);
        }
    };

    template <proto_bit_number X> struct mod_2<X> {
        constexpr X operator () (const X &x) {
            return bit_mod_2 (x);
        }
    };

    template <group_integral A, group_integral B> struct minus<A, B> {
        constexpr auto operator () (const A &x, const B &y) const {
            return x - y;
        }
    };

    template <ring_integral A, ring_integral B> struct divide<A, B> {
        constexpr auto operator () (const A &a, const nonzero<B> &b) const {
            return divmod<A, B> {} (a, b).Quotient;
        }
    };

    template <ring_integral A, ring_integral B> struct mod<A, B> {
        constexpr auto operator () (const A &a, const nonzero<B> &b) const {
            return divmod<A, B> {} (a, b).Remainder;
        }
    };
}

#endif

