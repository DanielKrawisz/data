// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/arithmetic/negativity.hpp>
#include <data/arithmetic.hpp>

#include <data/math/power.hpp>

#include <NTL/Z.hpp>

namespace data::math::number {
    struct Z final {

        Z () : Value {} {}

        template <std::integral I> Z (I u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <bool is_signed, endian r, std::size_t size>
        Z (endian_integral<is_signed, r, size> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        static Z read (string_view x);

        Z (const dec_int &u): Value {NTL::conv<NTL::ZZ> (u)} {}
        Z (const dec_uint &u): Value {NTL::conv<NTL::ZZ> (u)} {}
        template <negativity n, hex_case zz> Z (const hex::integer<n, zz> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian r, negativity c, std::unsigned_integral word>
        explicit Z (const Z_bytes<r, c, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian r, std::unsigned_integral word>
        explicit Z (const N_bytes<r, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<true, r, size, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<false, r, size, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <std::integral I>
        explicit operator I () const {
            return NTL::conv<I> (Value);
        }

        explicit operator double () const {
            return NTL::conv<double> (Value);
        }

        explicit operator dec_int () const {
            return NTL::conv<dec_int> (Value);
        }

        // TODO get rid of these operators and make them constructors.
        template <endian r, negativity c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const;

        template <endian r, size_t size, std::unsigned_integral word>
        explicit operator bounded<true, r, size, word> () const;

        template <hex_case zz> explicit operator hex::int2<zz> () const {
            return encoding::hexidecimal::write<zz> (Z_bytes<endian::little, negativity::twos, byte> (*this));
        }

        template <hex_case zz> explicit operator hex::intBC<zz> () const {
            return encoding::hexidecimal::write<zz> (Z_bytes<endian::little, negativity::BC, byte> (*this));
        }

        explicit Z (const NTL::ZZ &z): Value {z} {}
        explicit Z (NTL::ZZ &&z): Value {z} {}

        NTL::ZZ Value;
    };

    // implementation of naturals given an implementation of integers.
    struct N {

        N () : Value {} {}

        template <std::integral I> N (I u): Value {NTL::conv<NTL::ZZ> (u)} {
            if (u < 0) throw exception {} << "cannot instantiate N with negative number " << u;
        }

        template <bool is_signed, data::endian r, std::size_t size>
        N (endian_integral<is_signed, r, size> &u): Value {NTL::conv<NTL::ZZ> (u)} {
            if (u < 0) throw exception {} << "cannot instantiate N with negative number " << u;
        }

        explicit N (const Z &z) : Value {z.Value} {
            if (Value < 0) throw exception {} << "cannot instantiate N with negative number " << Value;
        }

        explicit N (Z &&z) : Value {z.Value} {
            if (Value < 0) throw exception {} << "cannot instantiate N with negative number " << Value;
        }

        static N read (string_view);
        N (const dec_uint &u): Value {NTL::conv<NTL::ZZ> (u)} {}
        template <hex_case zz> N (const hex::uint<zz> &u): Value {NTL::conv<NTL::ZZ> (u)} {}
        N (const base58_uint &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian r, std::unsigned_integral word>
        explicit N (const N_bytes<r, word> &n) : Value {NTL::conv<NTL::ZZ> (n)} {}

        template <endian r, size_t size, std::unsigned_integral word>
        explicit N (const bounded<false, r, size, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        operator Z () const {
            return Z (Value);
        }

        bool valid () const {
            return Value >= 0;
        }

        template <std::integral I>
        explicit operator I () const {
            return NTL::conv<I> (Value);
        }

        explicit operator double () const {
            return NTL::conv<double> (Value);
        }

        explicit operator dec_uint () const {
            return NTL::conv<dec_uint> (Value);
        }

        explicit operator dec_int () const {
            return NTL::conv<dec_int> (Value);
        }

        template <negativity n, hex_case zz> explicit operator hex::integer<n, zz> () const {
            if constexpr (n == negativity::nones)
                return encoding::hexidecimal::write<zz> (N_bytes<endian::little, byte> (*this));
            else return encoding::hexidecimal::write<zz> (Z_bytes<endian::little, n, byte> (*this));
        }

        template <endian r, std::unsigned_integral word>
        explicit operator N_bytes<r, word> () const;

        template <endian r, negativity c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const;

        template <bool u, endian r, size_t size, std::unsigned_integral word>
        explicit operator bounded<u, r, size, word> () const;

        explicit N (const NTL::ZZ &z): Value {z} {}
        explicit N (NTL::ZZ &&z): Value {z} {}
        NTL::ZZ Value;
    };
}

// TODO we need to fill these in. Right now we are using defaults.
namespace data::math::def {

    template <> struct sign<N> {
        math::sign operator () (const N &);
    };

    template <> struct sign<Z> {
        math::sign operator () (const Z &);
    };

    template <> struct square<N> {
        N operator () (const N &);
    };

    template <> struct square<Z> {
        Z operator () (const Z &);
    };

    template <> struct negate_mod<N> {
        N operator () (const N &, const nonzero<N> &);
    };

    template <> struct negate_mod<Z, N> {
        N operator () (const Z &, const nonzero<N> &);
    };

    template <> struct invert_mod<N> {
        N operator () (const N &, const nonzero<N> &);
    };

    template <> struct invert_mod<Z, N> {
        N operator () (const Z &, const nonzero<N> &);
    };

    template <> struct GCD<N, Z> {
        N operator () (const Z &, const Z &);
        N operator () (const N &, const N &);
    };

    template <> struct GCD<Z, Z> {
        Z operator () (const Z &, const Z &);
    };

    template <> struct divides<Z> {
        bool operator () (const Z &a, const nonzero<Z> &b);
    };

    template <> struct divides<N> {
        bool operator () (const N &a, const nonzero<N> &b);
    };

    template <> struct square_mod<N> {
        N operator () (const N &, const nonzero<N> &);
    };

    template <> struct square_mod<Z, N> {
        N operator () (const Z &, const nonzero<N> &);
    };

    template <> struct plus_mod<N> {
        N operator () (const N &, const N &, const nonzero<N> &);
    };

    template <> struct plus_mod<Z, Z, N> {
        N operator () (const Z &, const Z &, const nonzero<N> &);
    };

    template <> struct minus_mod<N> {
        N operator () (const N &, const N &, const nonzero<N> &);
    };

    template <> struct minus_mod<Z, Z, N> {
        N operator () (const Z &, const Z &, const nonzero<N> &);
    };

    template <> struct times_mod<N> {
        N operator () (const N &, const N &, const nonzero<N> &);
    };

    template <> struct times_mod<Z, Z, N> {
        N operator () (const Z &, const Z &, const nonzero<N> &);
    };
/*

    template <typename A, typename Mod = A> struct mul_2_mod;

    template <typename A> struct divides;*/
}

namespace data::math::number {

    // pre increment
    N inline &operator ++ (N &a) {
        ++a.Value;
        return a;
    }

    N inline &operator -- (N &a) {
        if (a != 0) --a.Value;
        return a;
    }

    Z inline &operator ++ (Z &a) {
        ++a.Value;
        return a;
    }

    Z inline &operator -- (Z &a) {
        --a.Value;
        return a;
    }

    // post increment
    N inline operator ++ (N &a, int) {
        auto b = a;
        ++a;
        return b;
    }

    N inline operator -- (N &a, int) {
        if (a == 0) return a;
        auto b = a;
        --a;
        return b;
    }

    Z inline operator ++ (Z &n, int) {
        Z z = n;
        ++ (n);
        return z;
    }

    Z inline operator -- (Z &n, int) {
        Z z = n;
        ++ (n);
        return z;
    }

    nonzero<N> inline increment<N>::operator () (const N &n) {
        return nonzero<N> {n + 1u};
    }

    Z inline increment<Z>::operator () (const Z &z) {
        return z + 1;
    }

    Z inline decrement<Z>::operator () (const Z &z) {
        return z - 1;
    }

    N inline decrement<N>::operator () (const nonzero<N> &n) {
        return n.Value - 1u;
    }

    N inline decrement<N>::operator () (const N &n) {
        if (n == 0) return 0;
        return n - 1u;
    }

    bool inline operator == (const Z &a, const Z &b) {
        return a.Value == b.Value;
    }

    bool inline operator == (const N &a, const N &b) {
        return a.Value == b.Value;
    }

    Z inline operator - (const N &a) {
        return Z (-a.Value);
    }

    Z inline operator - (const Z &a) {
        return Z (-a.Value);
    }

    Z inline operator + (const Z &a, const Z &b) {
        return Z (a.Value + b.Value);
    }

    Z inline operator - (const Z &a, const Z &b) {
        return Z (a.Value - b.Value);
    }

    Z inline operator * (const Z &a, const Z &b) {
        return Z (a.Value * b.Value);
    }

    N inline operator + (const N &a, const N &b) {
        return N (a.Value + b.Value);
    }

    N inline operator - (const N &a, const N &b) {
        if (a.Value <= b.Value) return N ();
        return N (a.Value - b.Value);
    }

    N inline operator * (const N &a, const N &b) {
        return N (a.Value * b.Value);
    }

    Z inline &operator += (Z &a, const Z &b) {
        a.Value += b.Value;
        return a;
    }

    Z inline &operator -= (Z &a, const Z &b) {
        a.Value -= b.Value;
        return a;
    }

    Z inline &operator *= (Z &a, const Z &b) {
        a.Value *= b.Value;
        return a;
    }

    N inline &operator += (N &a, const N &b) {
        a.Value += b.Value;
        return a;
    }

    N inline &operator -= (N &a, const N &b) {
        if (a.Value <= b.Value) a = N ();
        else a.Value -= b.Value;
        return a;
    }

    N inline &operator *= (N &a, const N &b) {
        a.Value *= b.Value;
        return a;
    }

    Z inline operator / (const Z &a, const Z &b) {
        return def::divmod<Z, Z> {} (a, nonzero {b}).Quotient;
    }

    N inline operator / (const N &a, const N &b) {
        return def::divmod<N, N> {} (a, nonzero {b}).Quotient;
    }

    N inline operator % (const Z &a, const Z &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z, Z> {} (a, nonzero {b}).Remainder;
    }

    N inline operator % (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z, N> {} (a, nonzero {b}).Remainder;
    }

    N inline operator % (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<N, N> {} (a, nonzero {b}).Remainder;
    }

    Z inline &operator /= (Z &a, const Z &b) {
        a.Value = def::divmod<Z, Z> {} (a, nonzero {b}).Quotient.Value;
        return a;
    }

    N inline &operator %= (N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        a.Value = def::divmod<N, N> {} (a, nonzero {b}).Remainder.Value;
        return a;
    }

    uint64 inline operator % (const Z &x, uint64 u) {
        return uint64 (x / N (u));
    }

    uint64 inline operator % (const N &x, uint64 u) {
        return uint64 (x / N (u));
    }

    Z inline operator << (const Z &z, int x) {
        return Z (z.Value << x);
    }

    Z inline operator >> (const Z &z, int x) {
        if (is_negative (z)) return ~Z ((~z).Value >> x);
        return Z (z.Value >> x);
    }

    N inline operator << (const N &z, int x) {
        return N (z.Value << x);
    }

    N inline operator >> (const N &z, int x) {
        return N (z.Value >> x);
    }

    Z inline &operator <<= (Z &z, int x) {
        z.Value <<= x;
        return z;
    }

    Z inline &operator >>= (Z &z, int x) {
        z.Value >>= x;
        return z;
    }

    N inline &operator <<= (N &z, int x) {
        z.Value <<= x;
        return z;
    }

    N inline &operator >>= (N &z, int x) {
        z.Value >>= x;
        return z;
    }

    template <endian r, size_t size, std::unsigned_integral word>
    inline Z::operator bounded<true, r, size, word> () const {
        return NTL::conv<bounded<true, r, size, word>> (this->Value);
    }

    template <bool u, endian r, size_t size, std::unsigned_integral word>
    inline N::operator bounded<u, r, size, word> () const {
        return NTL::conv<bounded<u, r, size, word>> (this->Value);
    }

    template <endian r, negativity c, std::unsigned_integral word>
    Z::operator Z_bytes<r, c, word> () const {

        constexpr size_t bits = sizeof (word) * 8;

        size_t size = is_zero (*this) ? 0 : (NTL::bit_width (this->Value) + bits - 1) / bits;

        if constexpr (c == negativity::BC) {
            if (NTL::sign (this->Value) < 0 &&
                NTL::NumBits (NTL::abs (this->Value)) == size * bits)
                ++size;
        }

        Z_bytes<r, c, word> result;
        result.resize (size);
        NTL::export_bin<word> (slice<word> (result), this->Value, r, endian::native, c);
        return result;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    N::operator Z_bytes<r, c, word> () const {
        constexpr size_t bits = sizeof (word) * 8;
        size_t size = (NTL::bit_width (this->Value) + bits - 1) / bits;
        Z_bytes<r, c, word> result;
        result.resize (size);
        NTL::export_bin<word> (slice<word> (result), this->Value, r, endian::native, c);
        return result;
    }

    template <endian r, std::unsigned_integral word>
    N::operator N_bytes<r, word> () const {
        constexpr size_t bits = sizeof (word) * 8;
        size_t size = (NTL::NumBits (this->Value) + bits - 1) / bits;
        N_bytes<r, word> result;
        result.resize (size);
        NTL::export_bin<word> (slice<word> (result), this->Value, r, endian::native, negativity::nones);
        return result;
    }

    // TODO For operations involving std::integral, NTL has many
    // built-in operations defined with ZZ and long. Thus, we
    // would need special cases for long long and unsigned long,
    // but for other cases we would not need to promote to N or
    // Z to perform these operations.
    template <std::integral I> bool inline operator == (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return a.Value == long (b);
        else if constexpr (std::signed_integral<I>) {
            if (b < 0) return false;
            return a == N (std::make_unsigned_t<I> (b));
        } else return a == N (b);
    }

    template <std::integral I> bool inline operator == (const Z &a, I b) {
        if constexpr (NTL::Compatible<I>) return a.Value == long (b);
        else return a == Z (b);
    }

    template <std::integral I> std::strong_ordering inline operator <=> (const N &a, I b) {
        if constexpr (std::signed_integral<I>) {
            if (b < 0) return std::strong_ordering::greater;
            return a <=> N (std::make_unsigned_t<I> (b));
        } else return a <=> N (b);
    }

    template <std::integral I> std::strong_ordering inline operator <=> (const Z &a, I b) {
        return a <=> Z (b);
    }

    template <std::integral I> Z inline operator + (const Z &a, I b) {
        if constexpr (NTL::Compatible<I>) return Z (a.Value + long (b));
        else return a + Z (b);
    }

    template <std::integral I> Z inline operator - (const Z &a, I b) {
        if constexpr (NTL::Compatible<I>) return Z (a.Value - long (b));
        else return a - Z (b);
    }

    template <std::integral I> Z inline operator * (const Z &a, I b) {
        if constexpr (NTL::Compatible<I>) return Z (a.Value * long (b));
        else return a * Z (b);
    }

    template <std::signed_integral I> Z inline operator + (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return Z (a.Value + long (b));
        else return a + Z (b);
    }

    template <std::signed_integral I> Z inline operator - (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return Z (a.Value - long (b));
        else return a - Z (b);
    }

    template <std::signed_integral I> Z inline operator * (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return Z (a.Value * long (b));
        else return a * Z (b);
    }

    template <std::unsigned_integral I> N inline operator + (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return N (a.Value + long (b));
        else return a + N (b);
    }

    template <std::unsigned_integral I> N inline operator - (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return N (a.Value - long (b));
        else return a - N (b);
    }

    template <std::unsigned_integral I> N inline operator * (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return N (a.Value * long (b));
        else return a * N (b);
    }

    Z inline operator ~ (const N &x) {
        return Z (-x.Value - 1);
    }

    Z inline operator ~ (const Z &x) {
        return Z (-x.Value - 1);
    }

    N inline operator | (const N &a, const N &b) {
        return N (a.Value | b.Value);
    }

    N inline operator & (const N &a, const N &b) {
        return N (a.Value & b.Value);
    }

    N inline operator ^ (const N &a, const N &b) {
        return N (a.Value ^ b.Value);
    }

    template <std::unsigned_integral I> N inline operator ^ (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return N (a.Value ^ long (b));
        else return a ^ N (b);
    }

    template <std::unsigned_integral I> N inline operator & (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return N (a.Value & long (b));
        else return a & N (b);
    }

    template <std::unsigned_integral I> N inline operator | (const N &a, I b) {
        if constexpr (NTL::Compatible<I>) return N (a.Value | long (b));
        else return a | N (b);
    }

    Z inline &operator &= (Z &a, const Z &b) {
        return a = a & b;
    }

    Z inline &operator |= (Z &a, const Z &b) {
        return a = a | b;
    }

    Z inline &operator ^= (Z &a, const Z &b) {
        return a = a ^ b;
    }

    N inline &operator &= (N &a, const N &b) {
        return a = a & b;
    }

    N inline &operator |= (N &a, const N &b) {
        return a = a | b;
    }

    N inline &operator ^= (N &a, const N &b) {
        return a = a ^ b;
    }

    template <std::unsigned_integral I> N inline &operator &= (N &a, I b) {
        return a = a & N (b);
    }

    template <std::unsigned_integral I> N inline &operator |= (N &a, I b) {
        return a = a | N (b);
    }

    template <std::unsigned_integral I> N inline &operator ^= (N &a, I b) {
        return a = a ^ N (b);
    }

    template <std::integral I> Z inline &operator &= (Z &a, I b) {
        return a = a & Z (b);
    }

    template <std::integral I> Z inline &operator |= (Z &a, I b) {
        return a = a | Z (b);
    }

    template <std::integral I> Z inline &operator ^= (Z &a, I b) {
        return a = a ^ Z (b);
    }

    template <std::unsigned_integral I> N inline &operator += (N &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value += long (b);
        else a.Value += N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator -= (N &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value -= long (b);
        else a.Value -= N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator *= (N &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value *= long (b);
        else a.Value *= N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator /= (N &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value /= long (b);
        else a.Value /= N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator %= (N &a, I b) {
        a.Value %= N (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator += (Z &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value += long (b);
        else a.Value += Z (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator -= (Z &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value -= long (b);
        else a.Value -= Z (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator *= (Z &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value *= long (b);
        else a.Value *= Z (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator /= (Z &a, I b) {
        if constexpr (NTL::Compatible<I>) a.Value /= long (b);
        else {
            a.Value /= Z (b).Value;
            if (b < 0) a.Value--;
        }
        return a;
    }

    template <std::integral I> Z inline operator / (const Z &a, I b) {
        if constexpr (NTL::Compatible<I> && std::unsigned_integral<I>)
            return Z (a.Value / long (b));
        else return a / Z (b);
    }

    template <std::unsigned_integral I> N inline operator / (const N &a, I b) {
        if constexpr (NTL::Compatible<I>)
            return N (a.Value / long (b));
        else return a / N (b);
    }

    template <std::signed_integral I> Z inline operator / (const N &a, I b) {
        if constexpr (NTL::Compatible<I>)
            return Z (a.Value / long (b));
        else return Z (a) / Z (b);
    }

    template <std::integral I> I inline operator % (const Z &a, I b) {
        if (b == 0) throw division_by_zero {};
        if constexpr (NTL::Compatible<I>)
            return I (N (a.Value % long (abs (b))));
        else return I (a % N (abs (b)));
    }

    template <std::integral I> I inline operator % (const N &a, I b) {
        if (b == 0) throw division_by_zero {};
        if constexpr (NTL::Compatible<I>)
            return I (N (a.Value % long (b)));
        else return I (a % N (abs (b)));
    }

}

namespace data::math::def {

    N inline abs<Z>::operator () (const Z &n) {
        return N (NTL::abs (n.Value));
    }

    N inline abs<N>::operator () (const N &n) {
        return N (NTL::abs (n.Value));
    }

    division<N, N> inline divmod<N, N>::operator () (const N &a, const nonzero<N> &b) {
        division<N, N> result {};
        if (b.Value == 0) throw math::division_by_zero {};
        NTL::DivRem (result.Quotient.Value, result.Remainder.Value, a.Value, b.Value.Value);
        return result;
    }

    division<Z, N> inline divmod<Z, N>::operator () (const Z &a, const nonzero<N> &b) {
        division<Z, N> result {};
        if (b.Value == 0) throw math::division_by_zero {};
        NTL::DivRem (result.Quotient.Value, result.Remainder.Value, a.Value, b.Value.Value);
        return result;
    }

    N inline div_2_pow<N>::operator () (const N &a, uint32 exp) {
        return a >> exp;
    }

    Z inline div_2_pow<Z>::operator () (const Z &a, uint32 exp) {
        return a >> exp;
    }

    N inline mod_2<N>::operator () (const N &a) {
        return NTL::IsOdd (a.Value) ? N (1): N ();
    }

    N inline mod_2<Z>::operator () (const Z &a) {
        return NTL::IsOdd (a.Value) ? N (1): N ();
    }

    template <group_number Exp>
    N inline pow<N, Exp>::operator () (const N &x, const Exp &y) {
        if constexpr (NTL::Compatible<Exp>)
            return N (NTL::power (x.Value, long (y)));
        else return math::binary_accumulate_pow (x, y);
    }

    template <group_number Exp>
    Z inline pow<Z, Exp>::operator () (const Z &x, const Exp &y) {
        if constexpr (NTL::Compatible<Exp>)
            return Z (NTL::power (x.Value, long (y)));
        else return math::binary_accumulate_pow (x, y);
    }

    template <group_number Exp>
    N inline pow_mod<N, Exp, N>::operator () (const N &x, const Exp &y, const nonzero<N> &z) {
        if constexpr (Same<Exp, Z> || Same<Exp, N>)
            return N (NTL::PowerMod (x.Value, y.Value, z.Value.Value));
        else return math::binary_accumulate_pow_mod (x, y, z);
    }

    template <group_number Exp>
    N inline pow_mod<Z, Exp, N>::operator () (const Z &x, const Exp &y, const nonzero<N> &z) {
        if constexpr (Same<Exp, Z> || Same<Exp, N>)
            return N (NTL::PowerMod (x.Value, y.Value, z.Value.Value));
        else return math::binary_accumulate_pow_mod (x, y, z);
    }

    N inline bit_xor<N>::operator () (const N &a, const N &b) {
        return a ^ b;
    }

    Z inline bit_xor<Z>::operator () (const Z &a, const Z &b) {
        return a ^ b;
    }

    Z inline times<Z>::operator () (const Z &a, const Z &b) {
        return a * b;
    }

    nonzero<Z> inline times<Z>::operator () (const nonzero<Z> &a, const nonzero<Z> &b) {
        return nonzero {a.Value * b.Value};
    }

    N inline square<N>::operator () (const N &n) {
        return N (NTL::sqr (n.Value));
    }

    Z inline square<Z>::operator () (const Z &z) {
        return Z (NTL::sqr (z.Value));
    }

    N inline GCD<N, Z>::operator () (const Z &a, const Z &b) {
        return N (NTL::GCD (a.Value, b.Value));
    }

    N inline GCD<N, Z>::operator () (const N &a, const N &b) {
        return N (NTL::GCD (a.Value, b.Value));
    }

    bool inline divides<Z>::operator () (const Z &a, const nonzero<Z> &b) {
        return NTL::divide (a.Value, b.Value.Value);
    }

    bool inline divides<N>::operator () (const N &a, const nonzero<N> &b) {
        return NTL::divide (a.Value, b.Value.Value);
    }
}

namespace data::encoding::decimal {
    std::ostream inline &write (std::ostream &o, const math::number::N &n) {
        return o << n.Value;
    }
}

namespace data::encoding::signed_decimal {
    std::ostream inline &write (std::ostream &o, const math::number::Z &n) {
        return o << n.Value;
    }
}

namespace data::encoding::hexidecimal {
    std::ostream inline &write (std::ostream &o, const math::number::N &n, hex::letter_case x) {
        auto nb = math::number::N_bytes<endian::big, byte> (n);
        return write<endian::big, byte> (o, static_cast<const oriented<endian::big, byte> &> (nb), x);
    }

    template <hex_case zz, negativity n>
    integer<n, zz> write (const Z &x) {
        std::stringstream ss;
        if constexpr (zz == hex_case::lower)
            write (ss, x, hex::letter_case::lower, n);
        else
            write (ss, x, hex::letter_case::upper, n);

        return integer<n, zz> {ss.str ()};
    }

    template <hex_case zz>
    integer<negativity::nones, zz> write (const N &x) {
        std::stringstream ss;
        if constexpr (zz == hex_case::lower)
            write (ss, x, hex::letter_case::lower);
        else
            write (ss, x, hex::letter_case::upper);

        return integer<negativity::nones, zz> {ss.str ()};
    }
}
