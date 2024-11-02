// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/concepts.hpp>
#include <data/io/exception.hpp>
#include <data/math/power.hpp>
#include <data/math/number/natural.hpp>

namespace data::math::number {

    template <typename Z> concept integer = ordered<Z> && integral_domain<Z> && requires (const Z &z) {
        { data::is_zero (z) } -> std::same_as<bool>;
        { data::is_positive (z) } -> std::same_as<bool>;
        { data::is_negative (z) } -> std::same_as<bool>;
        { data::sign (z) } -> std::same_as<math::signature>;
        { data::increment (z) } -> std::same_as<Z>;
        { data::decrement (z) } -> std::same_as<Z>;
    } && requires (const Z &a, const Z &b) {
        { a + b } -> std::same_as<Z>;
        { a - b } -> std::same_as<Z>;
        { a * b } -> std::same_as<Z>;
        { a / b } -> std::same_as<Z>;
    } && requires (Z &z) {
        { ++z } -> std::same_as<Z &>;
        { z++ } -> std::same_as<Z>;
        { --z } -> std::same_as<Z &>;
        { z++ } -> std::same_as<Z>;
    } && requires (Z &a, const Z &b) {
        { a += b } -> std::same_as<Z &>;
        { a -= b } -> std::same_as<Z &>;
        { a *= b } -> std::same_as<Z &>;
    };

    template <typename Z, typename N>
    division<Z, N> integer_natural_divide (const Z &Dividend, const N &Divisor);

    template <typename Z, typename N = decltype (data::abs (std::declval<Z> ()))>
    division<Z, N> integer_divide (const Z &Dividend, const Z &Divisor);

}

namespace data::math {

    template <number::integer Z>
    requires number::natural<decltype (data::abs (std::declval<Z> ()))> || number::integer<decltype (data::abs (std::declval<Z> ()))>
    struct divide<Z> {
        using N = decltype (data::abs (std::declval<Z> ()));
        division<Z, N> operator () (const Z &Dividend, const nonzero<Z> &Divisor) {
            return number::integer_divide (Dividend, Divisor.Value);
        }
    };

    template <number::integer Z, number::natural N>
    struct divide<Z, N> {
        division<Z, N> operator () (const Z &Dividend, const nonzero<N> &Divisor) {
            return number::integer_natural_divide (Dividend, Divisor.Value);
        }
    };

}

namespace data::math::number {

    // implementation of naturals given an implementation of integers.
    template <integer Z> struct N;

    template <integer Z> bool operator == (const N<Z> &, const N<Z> &);

    template <integer Z> auto operator <=> (const N<Z> &, const N<Z> &) -> decltype (std::declval<Z> () <=> std::declval<Z> ());

    template <integer Z> N<Z> operator + (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator - (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator * (const N<Z> &, const N<Z> &);

    template <integer Z> N<Z> operator + (const N<Z> &, uint64);
    template <integer Z> N<Z> operator - (const N<Z> &, uint64);
    template <integer Z> N<Z> operator * (const N<Z> &, uint64);

    template <integer Z> N<Z> operator + (uint64, const N<Z> &);
    template <integer Z> N<Z> operator - (uint64, const N<Z> &);
    template <integer Z> N<Z> operator * (uint64, const N<Z> &);

    template <integer Z> Z operator + (const N<Z> &, const Z &);
    template <integer Z> Z operator - (const N<Z> &, const Z &);
    template <integer Z> Z operator * (const N<Z> &, const Z &);

    template <integer Z> Z operator + (const Z &, const N<Z> &);
    template <integer Z> Z operator - (const Z &, const N<Z> &);
    template <integer Z> Z operator * (const Z &, const N<Z> &);

    template <integer Z> Z operator - (const N<Z> &);

    // divided by
    template <integer Z> N<Z> operator / (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator / (const N<Z> &, uint64);

    // mod
    template <integer Z> N<Z> operator % (const N<Z> &, const N<Z> &);

    // bit operations
    template <integer Z> N<Z> operator | (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator & (const N<Z> &, const N<Z> &);

    // exponential
    template <integer Z> N<Z> operator ^ (const N<Z> &, const N<Z> &);
    template <integer Z> Z operator ^ (const Z &, const N<Z> &);

    template <integer Z> N<Z> operator ^ (const N<Z> &, uint64);
    template <integer Z> Z operator ^ (const Z &, uint64);

    // divided by
    template <integer Z> N<Z> operator / (const N<Z> &, const N<Z> &);
    template <integer Z> Z operator / (const Z &, const N<Z> &);

    template <integer Z> N<Z> operator / (const N<Z> &, uint64);

    // mod
    template <integer Z> N<Z> operator % (const N<Z> &, const N<Z> &);
    template <integer Z> N<Z> operator % (const Z &, const N<Z> &);

    template <integer Z> uint64 operator % (const N<Z> &, uint64);

    // bit shift, which really just means
    // powers of two.
    template <integer Z> N<Z> operator << (const N<Z> &, int);
    template <integer Z> N<Z> operator >> (const N<Z> &, int);

    // pre increment
    template <integer Z> N<Z> &operator ++ (N<Z> &);
    template <integer Z> N<Z> &operator -- (N<Z> &);

    // post increment
    template <integer Z> N<Z> operator ++ (N<Z> &, int);
    template <integer Z> N<Z> operator -- (N<Z> &, int);

    template <integer Z> std::ostream &operator << (std::ostream &o, const N<Z> &n);

    template <integer Z> N<Z> &operator += (N<Z> &, const N<Z> &);
    template <integer Z> N<Z> &operator -= (N<Z> &, const N<Z> &);
    template <integer Z> N<Z> &operator *= (N<Z> &, const N<Z> &);

    template <integer Z> Z &operator += (N<Z> &, const Z &);
    template <integer Z> Z &operator -= (N<Z> &, const Z &);
    template <integer Z> Z &operator *= (N<Z> &, const Z &);

    template <integer Z> Z &operator += (Z &, const N<Z> &);
    template <integer Z> Z &operator -= (Z &, const N<Z> &);
    template <integer Z> Z &operator *= (Z &, const N<Z> &);

    template <integer Z> N<Z> &operator += (N<Z> &, uint64);
    template <integer Z> N<Z> &operator -= (N<Z> &, uint64);
    template <integer Z> N<Z> &operator *= (N<Z> &, uint64);

    template <integer Z> N<Z> &operator &= (N<Z> &, const N<Z> &);
    template <integer Z> N<Z> &operator |= (N<Z> &, const N<Z> &);

    template <integer Z> Z &operator &= (N<Z> &, const Z &);
    template <integer Z> Z &operator |= (N<Z> &, const Z &);

    template <integer Z> Z &operator &= (Z &, const N<Z> &);
    template <integer Z> Z &operator |= (Z &, const N<Z> &);

    template <integer Z> N<Z> &operator ^= (N<Z> &, const N<Z> &);
    template <integer Z> Z &operator ^= (Z &, const N<Z> &);

    template <integer Z> N<Z> &operator ^= (N<Z> &, uint64);

    template <integer Z> N<Z> &operator /= (N<Z> &, const N<Z> &);
    template <integer Z> Z &operator /= (Z &, const Z &);

    template <integer Z> N<Z> &operator /= (N<Z> &, uint64);
    template <integer Z> Z &operator /= (Z &, int64);

    template <integer Z> N<Z> &operator <<= (N<Z> &, int);
    template <integer Z> N<Z> &operator >>= (N<Z> &, int);

    // implementation of naturals given an implementation of integers.
    template <integer Z> struct N {
        Z Value;

        N () : Value {} {}

        explicit N (const Z &z) : Value {z} {}
        explicit N (Z &&z) : Value {z} {}

        // explicit conversions
        template <typename X> requires explicitly_constructible_from<Z, X>
        explicit N (const X &x) : Value { Z (x) } {}

        template <typename X> requires implicitly_constructible_from<Z, X>
        N (const X &x) : Value {Z (x)} {}

        template <typename X, typename Y, typename... P> requires std::constructible_from<Z, X, Y, P...>
        explicit N (const X &x, const Y &y, P... p) : Value {x, y, p...} {}

        operator Z () const {
            return Value;
        }

        bool valid () const {
            return data::valid (Value) && Value >= 0;
        }

    };

}

namespace data::math {

    template <number::integer Z>
    struct divide<number::N<Z>> {
        division<number::N<Z>> operator () (const number::N<Z> &Dividend, const nonzero<number::N<Z>> &Divisor) {
            return number::natural_divide (Dividend, Divisor.Value);
        }
    };

    template <number::integer Z> struct abs<number::N<Z>> {
        number::N<Z> operator () (const number::N<Z> &);
    };

    template <number::integer Z> struct times<number::N<Z>> {
        number::N<Z> operator () (const number::N<Z> &a, const number::N<Z> &b);
        nonzero<number::N<Z>> operator () (const nonzero<number::N<Z>> &a, const nonzero<number::N<Z>> &b);
    };

    template <number::integer Z> struct is_commutative<plus<number::N<Z>>, number::N<Z>> : is_commutative<plus<Z>, Z> {};
    template <number::integer Z> struct is_associative<plus<number::N<Z>>, number::N<Z>> : is_associative<plus<Z>, Z> {};
    template <number::integer Z> struct is_commutative<times<number::N<Z>>, number::N<Z>> : is_commutative<times<Z>, Z> {};
    template <number::integer Z> struct is_associative<times<number::N<Z>>, number::N<Z>> : is_associative<times<Z>, Z> {};

    template <number::integer Z> struct identity<plus<number::N<Z>>, number::N<Z>> : identity<plus<Z>, Z> {
        number::N<Z> operator () ();
    };

    template <number::integer Z> struct identity<times<number::N<Z>>, number::N<Z>> : identity<times<Z>, Z> {
        number::N<Z> operator () ();
    };

    template <number::integer Z> struct sign<number::N<Z>> {
        math::signature operator () (const number::N<Z> &x) {
            return x > 0 ? positive : zero;
        }
    };

}

namespace data::math::number {

    template <integer Z> struct increment<N<Z>> {
        nonzero<N<Z>> operator () (const N<Z> &n) {
            nonzero<N<Z>> x {n};
            ++x.Value;
            return x;
        }
    };

    template <integer Z> struct decrement<N<Z>> {
        N<Z> operator () (const nonzero<N<Z>> &n) {
            auto x = n.Value;
            return --x;
        }

        N<Z> operator () (const N<Z> &n) {
            if (n == 0) return n;
            auto x = n;
            return --x;
        }
    };

    template <typename Z, typename N>
    division<Z, N> integer_natural_divide (const Z &Dividend, const N &Divisor) {
        division<N> d {natural_divide<N> (data::abs (Dividend), Divisor)};

        if (d.Remainder == 0) return {Dividend < 0 ? -Z (d.Quotient) : Z (d.Quotient), d.Remainder};

        if (Dividend < 0) return {-(d.Quotient + 1), Divisor - d.Remainder};

        return {Z (d.Quotient), d.Remainder};
    }

    template <typename Z, typename N>
    division<Z, N> integer_divide (const Z &Dividend, const Z &Divisor) {
        // first we divide the absolute values.
        N divisor = data::abs (Divisor);
        division<N> d {natural_divide<N> (data::abs (Dividend), divisor)};

        // given x == q y + r,
        // if x -> -x, then x == -q y - r
        // if y -> -y, then x == -q y + r
        // if x -> -x and y -> -y, then x = q y - r;

        if (d.Remainder == 0) return {data::sign (Divisor) * data::sign (Dividend) == negative ? -Z (d.Quotient) : Z (d.Quotient), d.Remainder};

        if (Dividend < 0) return {Divisor < 0 ? Z (d.Quotient + 1): -(d.Quotient + 1), divisor - d.Remainder};

        if (Divisor < 0) return {-d.Quotient, d.Remainder};

        return {Z (d.Quotient), d.Remainder};
    }

    template <integer Z> bool inline operator == (const N<Z> &a, const N<Z> &b) {
        return a.Value == b.Value;
    }

    template <integer Z> auto inline operator <=> (const N<Z> &a, const N<Z> &b) -> decltype (std::declval<Z> () <=> std::declval<Z> ()) {
        return a.Value <=> b.Value;
    }

    template <integer Z> N<Z> inline operator + (const N<Z> &a, const N<Z> &b) {
        return a.Value + b.Value;
    }

    template <integer Z> N<Z> inline operator - (const N<Z> &a, const N<Z> &b) {
        return a.Value < b.Value ? 0 : a.Value - b.Value;
    }

    template <integer Z> N<Z> inline operator * (const N<Z> &a, const N<Z> &b) {
        return a.Value * b.Value;
    }

    template <integer Z> N<Z> inline operator + (const N<Z> &a, uint64 b) {
        return a.Value + b;
    }

    template <integer Z> N<Z> inline operator - (const N<Z> &a, uint64 b) {
        return a - N<Z> (b);
    }

    template <integer Z> N<Z> inline operator * (const N<Z> &a, uint64 b) {
        return a.Value * b;
    }

    template <integer Z> N<Z> inline operator + (uint64 a, const N<Z> &b) {
        return b + a;
    }

    template <integer Z> N<Z> inline operator - (uint64 a, const N<Z> &b) {
        return N<Z> (a) - b;
    }

    template <integer Z> N<Z> inline operator * (uint64 a, const N<Z> &b) {
        return b * a;
    }

    template <integer Z> Z inline operator + (const N<Z> &a, const Z &b) {
        return a.Value + b;
    }

    template <integer Z> Z inline operator - (const N<Z> &a, const Z &b) {
        return a.Value - b;
    }

    template <integer Z> Z inline operator * (const N<Z> &a, const Z &b) {
        return a.Value * b;
    }

    template <integer Z> Z inline operator + (const Z &a, const N<Z> &b) {
        return a + b.Value;
    }

    template <integer Z> Z inline operator - (const Z &a, const N<Z> &b) {
        return a - b.Value;
    }

    template <integer Z> Z inline operator * (const Z &a, const N<Z> &b) {
        return a * b.Value;
    }

    template <integer Z> Z inline operator - (const N<Z> &a) {
        return -a.Value;
    }

    // bit operations
    template <integer Z> N<Z> inline operator | (const N<Z> &a, const N<Z> &b) {
        return a.Value | b;
    }

    template <integer Z> N<Z> inline operator & (const N<Z> &a, const N<Z> &b) {
        return a.Value & b;
    }

    // divided by
    template <integer Z> N<Z> inline operator / (const N<Z> &a, const N<Z> &b) {
        if (b == 0) throw division_by_zero {};
        return divide<Z> {} (a.Value, nonzero {b.Value}).Quotient;
    }

    template <integer Z> Z inline operator / (const Z &a, const N<Z> &b) {
        if (b == 0) throw division_by_zero {};
        return divide<Z> {} (a, nonzero {b.Value}).Quotient;
    }

    template <integer Z> N<Z> inline operator / (const N<Z> &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return divide<Z> {} (a.Value, nonzero {Z (b)}).Quotient;
    }

    // mod
    template <integer Z> N<Z> inline operator % (const N<Z> &a, const N<Z> &b) {
        if (b == 0) throw division_by_zero {};
        return divide<N<Z>> {} (a, nonzero {b}).Remainder;
    }

    template <integer Z> N<Z> inline operator % (const Z &a, const N<Z> &b) {
        if (b == 0) throw division_by_zero {};
        return divide<Z> {} (a, nonzero {b.Value}).Remainder;
    }

    template <integer Z> uint64 inline operator % (const N<Z> &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return uint64 (divide<Z> {} (a.Value, nonzero {Z (b)}).Remainder);
    }

    // bit shift, which really just means
    // powers of two.
    template <integer Z> N<Z> inline operator << (const N<Z> &a, int b) {
        return a.Value << b;
    }

    template <integer Z> N<Z> inline operator >> (const N<Z> &a, int b) {
        return a.Value >> b;
    }

    // pre increment
    template <integer Z> N<Z> inline &operator ++ (N<Z> &a) {
        ++a.Value;
        return a;
    }

    template <integer Z> N<Z> inline &operator -- (N<Z> &a) {
        --a.Value;
        return a;
    }

    // post increment
    template <integer Z> N<Z> inline operator ++ (N<Z> &a, int) {
        auto b = a;
        ++a;
        return b;
    }

    template <integer Z> N<Z> inline operator -- (N<Z> &a, int) {
        auto b = a;
        --a;
        return b;
    }

    template <integer Z> std::ostream inline &operator << (std::ostream &o, const N<Z> &n) {
        return o << n.Value;
    }

    template <integer Z> N<Z> inline &operator += (N<Z> &a, const N<Z> &b) {
        a.Value += b.Value;
        return a;
    }

    template <integer Z> N<Z> inline &operator -= (N<Z> &a, const N<Z> &b) {
        return a = a - b;
    }

    template <integer Z> N<Z> inline &operator *= (N<Z> &a, const N<Z> &b) {
        a.Value *= b.Value;
        return a;
    }

    template <integer Z> Z inline &operator += (N<Z> &a, const Z &b) {
        a.Value += b;
        return a;
    }

    template <integer Z> Z inline &operator -= (N<Z> &a, const Z &b) {
        return a = a - b;
    }

    template <integer Z> Z inline &operator *= (N<Z> &a, const Z &b) {
        a.Value *= b.Value;
        return a;
    }

    template <integer Z> Z inline &operator += (Z &a, const N<Z> &b) {
        a += b.Value;
        return a;
    }

    template <integer Z> Z inline &operator -= (Z &a, const N<Z> &b) {
        return a -= b.Value;
    }

    template <integer Z> Z inline &operator *= (Z &a, const N<Z> &b) {
        a *= b.Value;
        return a;
    }

    template <integer Z> N<Z> inline &operator += (N<Z> &a, uint64 b) {
        a.Value += b;
        return a;
    }

    template <integer Z> N<Z> inline &operator -= (N<Z> &a, uint64 b) {
        return a = a - b;
    }

    template <integer Z> N<Z> inline &operator *= (N<Z> &a, uint64 b) {
        a.Value += b;
        return a;
    }

    template <integer Z> N<Z> inline &operator &= (N<Z> &a, const N<Z> &b) {
        a.Value &= b.Value;
        return a;
    }

    template <integer Z> N<Z> inline &operator |= (N<Z> &a, const N<Z> &b) {
        a.Value |= b.Value;
        return a;
    }

    template <integer Z> Z inline &operator &= (N<Z> &a, const Z &b) {
        a.Value &= b;
        return a;
    }

    template <integer Z> Z inline &operator |= (N<Z> &a, const Z &b) {
        a.Value |= b;
        return a;
    }

    template <integer Z> Z inline &operator &= (Z &a, const N<Z> &b) {
        return a &= b.Value;
    }

    template <integer Z> Z inline &operator |= (Z &a, const N<Z> &b) {
        return a |= b.Value;
    }

    template <integer Z> N<Z> inline &operator ^= (N<Z> &a, const N<Z> &b) {
        a.Value ^= b;
        return a;
    }

    template <integer Z> Z inline &operator ^= (Z &a, const N<Z> &b) {
        return a = a ^ b;
    }

    template <integer Z> N<Z> inline &operator ^= (N<Z> &a, uint64 b) {
        a.Value ^= b;
        return a;
    }

    template <integer Z> N<Z> inline &operator /= (N<Z> &a, const N<Z> &b) {
        a.Value /= b.Value;
        return a;
    }

    template <integer Z> N<Z> inline &operator /= (N<Z> &a, uint64 b) {
        a.Value /= b;
        return a;
    }

    template <integer Z> N<Z> inline &operator %= (N<Z> &a, const N<Z> &b) {
        return a = a % b;
    }

    template <integer Z> N<Z> inline &operator %= (Z a, const N<Z> &b) {
        return a = a % b;
    }

    template <integer Z> uint64 inline &operator %= (N<Z> a, uint64 b) {
        return a = a % b;
    }

    template <integer Z> N<Z> inline &operator <<= (N<Z> &a, int i) {
        a.Value <<= i;
        return a;
    }

    template <integer Z> N<Z> inline &operator >>= (N<Z> &a, int i) {
        a.Value >>= i;
        return a;
    }

    // exponential
    template <integer Z> N<Z> inline operator ^ (const N<Z> &a, const N<Z> &b) {
        return data::pow<N<Z>, N<Z>> (a, b);
    }

    template <integer Z> Z inline operator ^ (const Z &a, const N<Z> &b) {
        return data::pow<Z, N<Z>> (a, b);
    }

    template <integer Z> N<Z> inline operator ^ (const N<Z> &a, uint64 b) {
        return data::pow<N<Z>, uint64> (a, b);
    }

}

#endif
