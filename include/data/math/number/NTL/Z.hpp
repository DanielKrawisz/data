// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NTL_Z
#define DATA_MATH_NUMBER_NTL_Z

#include <data/slice.hpp>
#include <data/arithmetic/negativity.hpp>
#include <data/arithmetic.hpp>

#include <data/encoding/integer.hpp>
#include <data/encoding/endian.hpp>
#include <data/encoding/base58.hpp>

#include <data/math/power.hpp>

#include <data/math/number/types.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/prime.hpp>

#include <data/math/root.hpp>

#include <NTL/ZZ.h>

namespace data::math::number {
    struct Z final {

        Z () : Value {} {}

        template <std::integral I> Z (I u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <bool is_signed, data::endian r, std::size_t size>
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

namespace NTL {

    template <std::signed_integral I> void conv (ZZ &, const I &);

    template <std::unsigned_integral I> void conv (ZZ &, const I &);

    template <bool is_signed, data::endian r, std::size_t size>
    void conv (ZZ &, const data::endian_integral<is_signed, r, size> &);

    void conv (ZZ &x, const data::encoding::decimal::string &);

    void conv (ZZ &x, const data::encoding::signed_decimal::string &);

    template <data::negativity neg, data::hex_case cc>
    void conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &);

    void conv (ZZ &x, const data::encoding::base58::string &);

    template <data::endian r, data::negativity neg, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &);

    template <data::endian r, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::N_bytes<r, word> &);

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &);

    template <std::signed_integral I> void conv (I &, const ZZ &);

    template <std::unsigned_integral I> void conv (I &, const ZZ &);

    void conv (data::encoding::decimal::string &, const ZZ &);

    void conv (data::encoding::signed_decimal::string &, const ZZ &);

    void conv (data::encoding::base58::string &, const ZZ &);

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void conv (data::math::number::bounded<is_signed, r, size, word> &, const ZZ &);

    template <bool is_signed, data::endian r, std::size_t size>
    void conv (data::endian_integral<is_signed, r, size> &, const ZZ &);

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

namespace data::math::number::NTL {
    using namespace ::NTL;

    template <std::unsigned_integral U>
    ZZ import_bin (
        data::slice<const U> data,
        // the ordering of the overall array.
        endian order = endian::little,
        // the ordering of each value in the array.
        endian e = endian::native,
        negativity neg = negativity::nones
    );

    template <std::unsigned_integral U>
    void export_bin (
        data::slice<U> output,
        const ZZ &,
        // the ordering of the overall array.
        endian word_order = endian::little,
        // the ordering of each value in the array.
        endian byte_order = endian::native,
        negativity neg = negativity::twos
    );

    size_t inline bit_width (const ZZ &x) {
        if (::NTL::sign (x) >= 0) return NumBits (x) + 1;
        return NumBits (abs (x) - 1) + 1;
    }

    template <typename T>
    concept compatible =
        std::integral<T> && (sizeof (T) > sizeof (char)) &&
        (sizeof (T) < sizeof (long) ||
        Same<T, long>);

    template <typename T>
    concept noncompatible =
        std::integral<T> &&
        !compatible<T>;
}

namespace data::math::number::NTL {

    bool AKS_is_prime (const N &);

    set<N> roots (const N &, uint64 pow);
    set<Z> roots (const Z &, uint64 pow);

}

namespace data::math {

    template <uint64 pow> struct root<N, pow> {
        set<N> operator () (const N &n) {
            return number::NTL::roots (n, pow);
        }
    };

    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator () (const Z &n) {
            return number::NTL::roots (n, pow);
        }
    };

    template <Integer X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<Z, pow> (convert<Z> (n)));
        }
    };

    template <Natural X, uint64 pow> struct root<X, pow> {
        set<X> operator () (const X &n) {
            return set<X> (root<N, pow> (convert<N> (n)));
        }
    };

}

namespace data::math::number {

    template <> struct AKS<N> {
        prime<N> is_prime (const N &n) {
            if (NTL::AKS_is_prime (n))
                return prime<N> {n, prime<N>::certain};
            return prime<N> {};
        }
    };

    template struct AKS<N>;

    template <WholeNumber X> struct AKS {
        prime<X> is_prime (const X &n) {
            if (AKS<N>::is_prime (convert<N> (abs (n))))
                return prime<X> {n, prime<N>::certain};
            else return prime<X> {};
        }
    };

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
        if constexpr (NTL::compatible<I>) return a.Value == long (b);
        else if constexpr (std::signed_integral<I>) {
            if (b < 0) return false;
            return a == N (std::make_unsigned_t<I> (b));
        } else return a == N (b);
    }

    template <std::integral I> bool inline operator == (const Z &a, I b) {
        if constexpr (NTL::compatible<I>) return a.Value == long (b);
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
        if constexpr (NTL::compatible<I>) return Z (a.Value + long (b));
        else return a + Z (b);
    }

    template <std::integral I> Z inline operator - (const Z &a, I b) {
        if constexpr (NTL::compatible<I>) return Z (a.Value - long (b));
        else return a - Z (b);
    }

    template <std::integral I> Z inline operator * (const Z &a, I b) {
        if constexpr (NTL::compatible<I>) return Z (a.Value * long (b));
        else return a * Z (b);
    }

    template <std::signed_integral I> Z inline operator + (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return Z (a.Value + long (b));
        else return a + Z (b);
    }

    template <std::signed_integral I> Z inline operator - (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return Z (a.Value - long (b));
        else return a - Z (b);
    }

    template <std::signed_integral I> Z inline operator * (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return Z (a.Value * long (b));
        else return a * Z (b);
    }

    template <std::unsigned_integral I> N inline operator + (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return N (a.Value + long (b));
        else return a + N (b);
    }

    template <std::unsigned_integral I> N inline operator - (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return N (a.Value - long (b));
        else return a - N (b);
    }

    template <std::unsigned_integral I> N inline operator * (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return N (a.Value * long (b));
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
        if constexpr (NTL::compatible<I>) return N (a.Value ^ long (b));
        else return a ^ N (b);
    }

    template <std::unsigned_integral I> N inline operator & (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return N (a.Value & long (b));
        else return a & N (b);
    }

    template <std::unsigned_integral I> N inline operator | (const N &a, I b) {
        if constexpr (NTL::compatible<I>) return N (a.Value | long (b));
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
        if constexpr (NTL::compatible<I>) a.Value += long (b);
        else a.Value += N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator -= (N &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value -= long (b);
        else a.Value -= N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator *= (N &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value *= long (b);
        else a.Value *= N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator /= (N &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value /= long (b);
        else a.Value /= N (b).Value;
        return a;
    }

    template <std::unsigned_integral I> N inline &operator %= (N &a, I b) {
        a.Value %= N (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator += (Z &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value += long (b);
        else a.Value += Z (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator -= (Z &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value -= long (b);
        else a.Value -= Z (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator *= (Z &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value *= long (b);
        else a.Value *= Z (b).Value;
        return a;
    }

    template <std::integral I> Z inline &operator /= (Z &a, I b) {
        if constexpr (NTL::compatible<I>) a.Value /= long (b);
        else {
            a.Value /= Z (b).Value;
            if (b < 0) a.Value--;
        }
        return a;
    }

    template <std::integral I> Z inline operator / (const Z &a, I b) {
        if constexpr (NTL::compatible<I> && std::unsigned_integral<I>)
            return Z (a.Value / long (b));
        else return a / Z (b);
    }

    template <std::unsigned_integral I> N inline operator / (const N &a, I b) {
        if constexpr (NTL::compatible<I>)
            return N (a.Value / long (b));
        else return a / N (b);
    }

    template <std::signed_integral I> Z inline operator / (const N &a, I b) {
        if constexpr (NTL::compatible<I>)
            return Z (a.Value / long (b));
        else return Z (a) / Z (b);
    }

    template <std::integral I> I inline operator % (const Z &a, I b) {
        if (b == 1) throw division_by_zero {};
        if constexpr (NTL::compatible<I>)
            return I (N (a.Value % long (abs (b))));
        else return I (a % N (abs (b)));
    }

    template <std::integral I> I inline operator % (const N &a, I b) {
        if (b == 1) throw division_by_zero {};
        if constexpr (NTL::compatible<I>)
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
        return N (a.Value >> exp);
    }

    Z inline div_2_pow<Z>::operator () (const Z &a, uint32 exp) {
        return Z (a.Value >> exp);
    }

    N inline mod_2<N>::operator () (const N &a) {
        return NTL::IsOdd (a.Value) ? N (1): N ();
    }

    N inline mod_2<Z>::operator () (const Z &a) {
        return NTL::IsOdd (a.Value) ? N (1): N ();
    }

    template <group_number Exp>
    N inline pow<N, Exp>::operator () (const N &x, const Exp &y) {
        if constexpr (number::NTL::compatible<Exp>)
            return N (NTL::power (x.Value, long (y)));
        else return math::binary_accumulate_pow (x, y);
    }

    template <group_number Exp>
    Z inline pow<Z, Exp>::operator () (const Z &x, const Exp &y) {
        if constexpr (number::NTL::compatible<Exp>)
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

namespace NTL {

    template <data::negativity neg, data::hex_case cc>
    void inline conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid hexidecimal string";
        data::bytes data = *data::encoding::hex::read (data::string_view (u).substr (2));
        x = data::math::number::NTL::import_bin<data::byte> (data::byte_slice (data),
            data::endian::big, data::endian::native, neg);
    }

    template <data::endian r, data::negativity neg, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &u) {
        x = data::math::number::NTL::import_bin<word> (data::slice<const word> (u), r, data::endian::native, neg);
    }

    template <data::endian r, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::N_bytes<r, word> &u) {
        x = data::math::number::NTL::import_bin<word> (
            data::slice<const word> (u), r,
            data::endian::native,
            data::negativity::nones);
    }

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &u) {
        x = data::math::number::NTL::import_bin<word> (
            data::slice<const word> (u), r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    template <bool is_signed, data::endian r, std::size_t size>
    void inline conv (ZZ &x, const data::endian_integral<is_signed, r, size> &u) {
        x = data::math::number::NTL::import_bin<data::byte> (
            data::slice<const data::byte> (u), r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    void inline conv (ZZ &x, const data::encoding::decimal::string &u) {
        conv (x, u.c_str ());
    }

    template <std::unsigned_integral I> void inline conv (ZZ &u, const I &x) {
        u = data::math::number::NTL::import_bin (
            data::slice<const I> {&x, 1},
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::nones);
    }

    template <std::unsigned_integral I> void inline conv (I &x, const ZZ &z) {
        data::math::number::NTL::export_bin (
            data::slice<I> {&x, 1}, z,
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::nones);
    }

    template <std::signed_integral I> void inline conv (ZZ &u, const I &x) {
        u = data::math::number::NTL::import_bin (
            data::slice<const std::make_unsigned_t<I>> {(const std::make_unsigned_t<I> *) (&x), 1},
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::twos);
    }

    template <std::signed_integral I> void inline conv (I &x, const ZZ &z) {
        data::math::number::NTL::export_bin (
            data::slice<std::make_unsigned_t<I>> {(std::make_unsigned_t<I> *) (&x), 1}, z,
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::twos);
    }

    void inline conv (data::encoding::base58::string &x, const ZZ &u) {
        x = data::encoding::base58::encode (data::math::number::Z (u));
    }

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void inline conv (data::math::number::bounded<is_signed, r, size, word> &x, const ZZ &u) {
        data::math::number::NTL::export_bin<word> (data::slice<word> (x), u, r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    template <bool is_signed, data::endian r, std::size_t size>
    void inline conv (data::endian_integral<is_signed, r, size> &x, const ZZ &u) {
        data::math::number::NTL::export_bin<data::byte> (data::slice<data::byte> (x), u, r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    void inline conv (data::encoding::signed_decimal::string &x, const ZZ &u) {
        x = data::encoding::signed_decimal::write (data::math::number::Z (u));
    }

    void inline conv (data::encoding::decimal::string &x, const ZZ &u) {
        x = data::encoding::decimal::write (data::math::number::N (u));
    }

}

namespace data::math::number::NTL {
    template <std::unsigned_integral U>
    bool is_negative (
        slice<const U> input,
        endian word_order,
        endian byte_order,
        negativity neg)
    {
        if (neg == negativity::nones || input.empty ())
            return false;

        const U word = word_order == endian::little
            ? input.back ()
            : input.front ();

        constexpr std::size_t bytes = sizeof (U);

        const std::size_t byte_index = byte_order == endian::little
            ? bytes - 1
            : 0;

        const U sign_bit = U (1) << (byte_index * 8 + 7);

        return (word & sign_bit) != 0;
    }

    template <std::unsigned_integral U>
    static ZZ import_bin (
        slice<const U> input,
        // the ordering of the overall array.
        endian word_order,
        // the ordering of each value in the array.
        endian byte_order,
        negativity neg
    ) {

        if (input.size () == 0) return ZZ ();

        if (word_order != endian::big && word_order != endian::little)
            throw std::invalid_argument ("invalid word order");

        if (byte_order != endian::big && byte_order != endian::little)
            throw std::invalid_argument ("invalid byte order");

        bool input_is_negative = is_negative (input, word_order, byte_order, neg);

        // under the right circumstances, we can send the input directly to NTL.
        if (word_order == endian::little &&
            !input_is_negative &&
            sizeof (U) == sizeof (unsigned char) &&
            (byte_order == endian::native || sizeof (unsigned char) == 1))
            return ZZFromBytes (reinterpret_cast<const unsigned char *> (input.data ()), input.size ());

        bytestring<U> bytes (input.size ());

        if (word_order == endian::little)
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[i];
        else for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[input.size () - i - 1];

        if (!input_is_negative) {
            // switch to little endian.
            if (byte_order != endian::little && sizeof (U) > 1)
                for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

            // from now on we are using little endian word and byte order.
            return ZZFromBytes (
                reinterpret_cast<const unsigned char *> (bytes.data ()),
                bytes.size () * sizeof (U));
        }

        // first convert to native endian.
        if (byte_order != endian::native && sizeof (U) > 1)
            for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

        // negate
        if (neg == negativity::twos) arithmetic::twos::negate<endian::little, U> (bytes);
        else arithmetic::BC::negate<endian::little, U> (bytes);

        // switch to little endian
        if (endian::native != endian::little && sizeof (U) > 1)
            for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

        auto zzz = ZZFromBytes (
            reinterpret_cast<const unsigned char *> (bytes.data ()),
            bytes.size () * sizeof (U));

        return -zzz;

    }

    template <std::unsigned_integral U>
    void negate_twos(data::slice<U> output) {
        // The words are stored little-endian, so the two's-complement
        // carry propagates from the first word toward the last.
        for (auto &x : output)
            x = ~x;

        // Add one to the multiword value.  add_with_carry propagates the
        // carry to the next word when the current addition overflows.
        for (auto &x : output)
            if (!arithmetic::add_with_carry (x, x, U (1))) return;
    }

    template <std::unsigned_integral U>
    void negate_bc (data::slice<U> output) {
        output.back () ^= U (1) << (sizeof (U) * 8 - 1);
    }

    template <std::unsigned_integral U>
    void export_bin (
        data::slice<U> output,
        const ZZ &x,
        // the ordering of the overall array.
        endian word_order,
        // the ordering of each value in the array.
        endian byte_order,
        negativity neg
    ) {

        auto sign = NTL::sign (x);

        if (neg == negativity::nones && sign < 0)
            throw exception {} << "negative value";

        constexpr size_t bits = sizeof (U) * 8;

        const size_t capacity = output.size () * bits;

        // Determine that the minimal size is enough given the buffer.
        size_t required = sign == 0 ? 0:
            neg == negativity::nones
                ? NumBits (x)
                : bit_width (x);

        auto abs = NTL::abs (x);

        if (neg == negativity::BC &&
            sign < 0 &&
            bit_width (x) == NumBits (abs))
            ++required;

        if (required > capacity)
            throw exception {} << "integer does not fit";

        {
            auto *output_bytes = reinterpret_cast<unsigned char *> (output.data ());
            const size_t nbytes = output.size () * sizeof (U);

            std::fill (output_bytes, output_bytes + nbytes, 0);

            BytesFromZZ (output_bytes, abs, nbytes);
        }

        // at this point, word order and byte order are both little endian.

        // if x is negative, apply the appropriate negativity function.
        if (sign < 0) {
            // first convert to native endian.
            if (endian::little != endian::native && sizeof (U) > 1)
                for (U &x : output) x = boost::endian::endian_reverse<U> (x);

            if (neg == negativity::twos)
                negate_twos (output);
            else if (neg == negativity::BC)
                negate_bc (output);

            if (endian::native != byte_order && sizeof (U) > 1)
                for (U &x : output) x = boost::endian::endian_reverse<U> (x);
        } else if (byte_order != endian::little && sizeof (U) > 1)
            for (U &x : output) x = boost::endian::endian_reverse<U> (x);

        // reverse to big endian if we have to.
        if (word_order == endian::big)
            for (int i = 0; i < output.size () / 2; i++)
                std::swap (output[i], output[output.size () - 1 - i]);
    }

}

static_assert (data::group_number<data::N>);
static_assert (data::group_number<data::Z>);

#endif
