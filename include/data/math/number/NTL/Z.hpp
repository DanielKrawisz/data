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
#include <data/math/number/types.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/bytes/Z.hpp>

#include <data/math/number/prime.hpp>
#include <data/math/root.hpp>

#include <NTL/ZZ.h>

namespace NTL {

    template <std::unsigned_integral I> void conv (ZZ &, const I &);

    void conv (ZZ &, const long long int &);

    template <bool is_signed, data::endian::order r, std::size_t size>
    void conv (ZZ &, const data::endian::integral<is_signed, r, size> &);

    void conv (ZZ &x, const data::encoding::decimal::string &);

    void conv (ZZ &x, const data::encoding::signed_decimal::string &);

    template <data::arithmetic::negativity neg, data::hex_case cc>
    void conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &);

    void conv (ZZ &x, const data::encoding::base58::string &);

    template <data::endian::order r, data::arithmetic::negativity neg, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &);

    template <data::endian::order r, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::N_bytes<r, word> &);

    template <bool is_signed, data::endian::order r, std::size_t size, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &);

    template <std::unsigned_integral I> void conv (I &, const ZZ &);

    void conv (long long int &, const ZZ &);

    void conv (data::encoding::decimal::string &, const ZZ &);

    void conv (data::encoding::signed_decimal::string &, const ZZ &);

    void conv (data::encoding::base58::string &, const ZZ &);

    template <bool is_signed, data::endian::order r, std::size_t size, std::unsigned_integral word>
    void conv (data::math::number::bounded<is_signed, r, size, word> &, const ZZ &);

    template <bool is_signed, data::endian::order r, std::size_t size>
    void conv (data::endian::integral<is_signed, r, size> &, const ZZ &);

}

namespace data::math::number::NTL {
    using namespace ::NTL;

    template <std::unsigned_integral U>
    ZZ import_bin (
        data::slice<const U> data,
        // the ordering of the overall array.
        endian::order order = endian::order::little,
        // the ordering of each value in the array.
        endian::order e = endian::order::native,
        arithmetic::negativity neg = arithmetic::negativity::nones
    );

    template <std::unsigned_integral U>
    void export_bin (
        data::slice<U> output,
        const ZZ &,
        // the ordering of the overall array.
        endian::order word_order = endian::order::little,
        // the ordering of each value in the array.
        endian::order byte_order = endian::order::native,
        arithmetic::negativity neg = arithmetic::negativity::twos
    );

    size_t inline bit_width (const ZZ &x) {
        if (::NTL::sign (x) >= 0) return NumBits (x) + 1;
        return NumBits (abs (x) - 1) + 1;
    }

}

namespace data::math::number {
    struct Z final {

        Z () : Value {} {}

        template <std::integral I> Z (I u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <bool is_signed, data::endian::order r, std::size_t size>
        Z (endian::integral<is_signed, r, size> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        static Z read (string_view x);

        Z (const dec_int &u): Value {NTL::conv<NTL::ZZ> (u)} {}
        Z (const dec_uint &u): Value {NTL::conv<NTL::ZZ> (u)} {}
        template <neg n, hex_case zz> Z (const hex::integer<n, zz> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit Z (const Z_bytes<r, c, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian::order r, std::unsigned_integral word>
        explicit Z (const N_bytes<r, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<true, r, size, word> &u): Value {NTL::conv<NTL::ZZ> (u)} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
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
        template <endian::order r, neg c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const;

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit operator bounded<true, r, size, word> () const;

        template <hex_case zz> explicit operator hex::int2<zz> () const {
            return encoding::hexidecimal::write<zz> (Z_bytes<endian::little, arithmetic::negativity::twos, byte> (*this));
        }

        template <hex_case zz> explicit operator hex::intBC<zz> () const {
            return encoding::hexidecimal::write<zz> (Z_bytes<endian::little, arithmetic::negativity::BC, byte> (*this));
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

        template <bool is_signed, data::endian::order r, std::size_t size>
        N (endian::integral<is_signed, r, size> &u): Value {NTL::conv<NTL::ZZ> (u)} {
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

        template <endian::order r, std::unsigned_integral word>
        explicit N (const N_bytes<r, word> &n) : Value {NTL::conv<NTL::ZZ> (n)} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
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

        template <neg n, hex_case zz> explicit operator hex::integer<n, zz> () const {
            if constexpr (n == arithmetic::negativity::nones)
                return encoding::hexidecimal::write<zz> (N_bytes<endian::little, byte> (*this));
            else return encoding::hexidecimal::write<zz> (Z_bytes<endian::little, n, byte> (*this));
        }

        template <endian::order r, std::unsigned_integral word>
        explicit operator N_bytes<r, word> () const;

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const;

        template <bool u, endian::order r, size_t size, std::unsigned_integral word>
        explicit operator bounded<u, r, size, word> () const;

        explicit N (const NTL::ZZ &z): Value {z} {}
        explicit N (NTL::ZZ &&z): Value {z} {}
        NTL::ZZ Value;
    };
}

namespace data::math {

    template <uint64 pow> struct root<N, pow> {
        set<N> operator () (const N &n);
    };

    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator () (const Z &z);
    };

}

namespace data::math::number {

    template <> struct AKS<N> {
        prime<N> is_prime (const N n);
    };

    template struct AKS<N>;

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
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z, Z> {} (a, nonzero {b}).Quotient;
    }

    N inline operator / (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
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
        if (b == 0) throw division_by_zero {};
        a.Value = def::divmod<Z, Z> {} (a, nonzero {b}).Quotient.Value;
    }

    N inline &operator %= (N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        a.Value = def::divmod<N, N> {} (a, nonzero {b}).Remainder.Value;
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

    template <endian::order r, size_t size, std::unsigned_integral word>
    inline Z::operator bounded<true, r, size, word> () const {
        return NTL::conv<bounded<true, r, size, word>> (this->Value);
    }

    template <bool u, endian::order r, size_t size, std::unsigned_integral word>
    inline N::operator bounded<u, r, size, word> () const {
        return NTL::conv<bounded<u, r, size, word>> (this->Value);
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z::operator Z_bytes<r, c, word> () const {

        constexpr size_t bits = sizeof (word) * 8;

        size_t size = (NTL::bit_width (this->Value) + bits - 1) / bits;

        if constexpr (c == neg::BC) {
            if (NTL::sign (this->Value) < 0 &&
                NTL::NumBits (NTL::abs (this->Value)) == size * bits)
                ++size;
        }

        Z_bytes<r, c, word> result;
        result.resize (size);
        NTL::export_bin<word> (slice<word> (result), this->Value, r, endian::order::native, c);
        return result;
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    N::operator Z_bytes<r, c, word> () const {
        constexpr size_t bits = sizeof (word) * 8;
        size_t size = (NTL::bit_width (this->Value) + bits - 1) / bits;
        Z_bytes<r, c, word> result;
        result.resize (size);
        NTL::export_bin<word> (slice<word> (result), this->Value, r, endian::order::native, c);
        return result;
    }

    template <endian::order r, std::unsigned_integral word>
    N::operator N_bytes<r, word> () const {
        constexpr size_t bits = sizeof (word) * 8;
        size_t size = (NTL::NumBits (this->Value) + bits - 1) / bits;
        N_bytes<r, word> result;
        result.resize (size);
        NTL::export_bin<word> (slice<word> (result), this->Value, r, endian::order::native, arithmetic::negativity::nones);
        return result;
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
        NTL::DivRem (result.Quotient.Value, result.Remainder.Value, a.Value, b.Value.Value);
        return result;
    }

    division<Z, N> inline divmod<Z, N>::operator () (const Z &a, const nonzero<N> &b) {
        division<Z, N> result {};
        NTL::DivRem (result.Quotient.Value, result.Remainder.Value, a.Value, b.Value.Value);
        return result;
    }

    template <hex_case zz>
    division<hex::uint<zz>, hex::uint<zz>> inline
    divmod<hex::uint<zz>, hex::uint<zz>>::operator () (
        const hex::uint<zz> &a,
        const nonzero<hex::uint<zz>> &b) {
        division<N, N> result = divmod<N, N> {} (N (a), nonzero {N (b.Value)});
        return division<hex::uint<zz>, hex::uint<zz>> {hex::uint<zz> (result.Quotient), hex::uint<zz> (result.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::uint<zz>> inline
    divmod<hex::int2<zz>, hex::int2<zz>>::operator () (
        const hex::int2<zz> &a,
        const nonzero<hex::int2<zz>> &b) {
        division<Z, N> result = divmod<Z, Z> {} (Z (a), nonzero {Z (b.Value)});
        return division<hex::int2<zz>, hex::uint<zz>> {hex::int2<zz> (result.Quotient), hex::uint<zz> (result.Remainder)};
    }

    template <hex_case zz>
    division<hex::intBC<zz>, hex::intBC<zz>> inline
    divmod<hex::intBC<zz>, hex::intBC<zz>>::operator () (
        const hex::intBC<zz> &a,
        const nonzero<hex::intBC<zz>> &b) {
        division<Z, N> result = divmod<Z, Z> {} (Z (a), nonzero {Z (b.Value)});
        return division<hex::intBC<zz>, hex::intBC<zz>> {hex::intBC<zz> (result.Quotient), hex::intBC<zz> (result.Remainder)};
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    number::Z_bytes<r, c, word> inline convert<number::Z_bytes<r, c, word>, Z>::operator () (const Z &z) const {
        return number::Z_bytes<r, c, word> (z);
    }
}

namespace data::encoding::hexidecimal {
    std::ostream inline &write (std::ostream &o, const math::number::N &n, hex::letter_case x) {
        return write (o, static_cast<const oriented<endian::big, byte> &> (N_bytes<endian::big, byte> (n)), x);
    }
}

namespace data::math::number::NTL {

    bool aks_is_prime (const Z &);

    set<N> root (const N &n, uint32 p);
    set<Z> root (const Z &n, uint32 p);

}

namespace data::math::number {

    prime<N> inline AKS<N>::is_prime (const N n) {
        return NTL::aks_is_prime (n) ? prime<N> {n, prime<N>::certain} : prime<N> {};
    }

}

namespace data::math {

    template <uint64 pow>
    set<N> root<N, pow>::operator () (const N &n) {
        return number::NTL::root (n, pow);
    }

    template <uint64 pow>
    set<Z> root<Z, pow>::operator () (const Z &z) {
        return number::NTL::root (z, pow);
    }

}

namespace NTL {

    template <data::arithmetic::negativity neg, data::hex_case cc>
    void inline conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid hexidecimal string";
        x = data::math::number::NTL::import_bin<data::byte> (data::byte_slice (*data::encoding::hex::read (data::string_view (u).substr (2))),
            data::endian::order::little, data::endian::order::native, neg);
    }

    template <data::endian::order r, data::arithmetic::negativity neg, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &u) {
        x = data::math::number::NTL::import_bin<word> (data::slice<const word> (u), r, data::endian::order::native, neg);
    }

    template <data::endian::order r, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::N_bytes<r, word> &u) {
        x = data::math::number::NTL::import_bin<word> (
            data::slice<const word> (u), r,
            data::endian::order::native,
            data::arithmetic::negativity::nones);
    }

    template <bool is_signed, data::endian::order r, std::size_t size, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &u) {
        x = data::math::number::NTL::import_bin<word> (
            data::slice<const word> (u), r, data::endian::order::native,
            is_signed ? data::arithmetic::negativity::twos : data::arithmetic::negativity::nones);
    }

    template <bool is_signed, data::endian::order r, std::size_t size>
    void inline conv (ZZ &x, const data::endian::integral<is_signed, r, size> &u) {
        x = data::math::number::NTL::import_bin<data::byte> (
            data::slice<const data::byte> (u), r, data::endian::order::native,
            is_signed ? data::arithmetic::negativity::twos : data::arithmetic::negativity::nones);
    }

    void inline conv (ZZ &x, const data::encoding::decimal::string &u) {
        conv (x, u.c_str ());
    }

    template <std::unsigned_integral I> void inline conv (ZZ &u, const I &x) {
        u = data::math::number::NTL::import_bin (
            data::slice<const I> {(const I *) (&x), 1},
                data::endian::order::little, data::endian::order::native, data::arithmetic::negativity::twos);
    }

    void inline conv (ZZ &u, const long long int &x) {
        u = data::math::number::NTL::import_bin (
            data::slice<const unsigned long long int> {(const unsigned long long int *) (&x), 1},
                data::endian::order::little, data::endian::order::native, data::arithmetic::negativity::twos);
    }

    void inline conv (data::encoding::base58::string &x, const ZZ &u) {
        x = data::encoding::base58::encode (data::math::number::Z (u));
    }

    template <bool is_signed, data::endian::order r, std::size_t size, std::unsigned_integral word>
    void inline conv (data::math::number::bounded<is_signed, r, size, word> &x, const ZZ &u) {
        data::math::number::NTL::export_bin<word> (data::slice<word> (x), u, r, data::endian::order::native,
            is_signed ? data::arithmetic::negativity::twos : data::arithmetic::negativity::nones);
    }

    template <bool is_signed, data::endian::order r, std::size_t size>
    void inline conv (data::endian::integral<is_signed, r, size> &x, const ZZ &u) {
        data::math::number::NTL::export_bin<data::byte> (data::slice<data::byte> (x), u, r, data::endian::order::native,
            is_signed ? data::arithmetic::negativity::twos : data::arithmetic::negativity::nones);
    }

}

namespace data::math::number::NTL {

    template <std::unsigned_integral U>
    static ZZ import_bin (
        slice<const U> input,
        // the ordering of the overall array.
        endian::order word_order,
        // the ordering of each value in the array.
        endian::order byte_order,
        arithmetic::negativity neg
    ) {

        if (input.size () == 0) return ZZ ();

        if (word_order == endian::order::little &&
            neg == arithmetic::negativity::nones &&
            sizeof (U) == sizeof (unsigned char) &&
            (byte_order == endian::order::native || sizeof (unsigned char) == 1))
            return ZZFromBytes (reinterpret_cast<const unsigned char *> (input.data ()), input.size ());

        if (word_order != endian::order::big || word_order != endian::order::little)
            throw std::invalid_argument ("invalid word order");

        if (byte_order != endian::order::big || byte_order != endian::order::little)
            throw std::invalid_argument ("invalid byte order");

        // deal with negative numbers.
        if (neg == arithmetic::negativity::twos) {
            if (word_order == endian::order::big) {
                if (arithmetic::twos::is_negative (arithmetic::Words<endian::order::big, const U> {input})) {
                    const auto magnitude =
                    arithmetic::twos::negate<endian::order::big, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones);
                }
            } else {
                if (arithmetic::twos::is_negative (arithmetic::Words<endian::order::little, const U> {input})) {
                    const auto magnitude =
                    arithmetic::twos::negate<endian::order::little, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones);
                }
            }
        } else if (neg == arithmetic::negativity::BC) {
            if (word_order == endian::order::big) {
                if (arithmetic::BC::is_negative (arithmetic::Words<endian::big, const U> {input})) {
                    const auto magnitude =
                    arithmetic::BC::negate<endian::big, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones
                    );
                }
            } else {
                if (arithmetic::BC::is_negative (arithmetic::Words<endian::little, const U> {input})) {
                    const auto magnitude =
                    arithmetic::BC::negate<endian::little, U> (input);

                    return -import_bin (
                        slice<const U> (magnitude.data (), magnitude.size ()),
                        word_order,
                        byte_order,
                        arithmetic::negativity::nones
                    );
                }
            }
        }

        // If everything is little endian then we can simply cast
        // the array to a byte array.
        if (word_order == endian::order::little &&
            (byte_order == endian::order::little ||
            (sizeof (U) == sizeof (unsigned char) && sizeof (unsigned char) == 1))) {
            return ZZFromBytes (
                reinterpret_cast<const unsigned char *> (input.data ()),
                input.size () * sizeof (U));
        }

        bytestring<U> bytes (input.size ());

        if (word_order == endian::order::little)
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[i];
        else for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[input.size () - i - 1];

        if (byte_order != endian::order::little && sizeof (U) > 1)
            for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

        return ZZFromBytes (
            reinterpret_cast<const unsigned char *> (bytes.data ()),
            bytes.size () * sizeof (U));
    }

    template <std::unsigned_integral U>
    void negate_twos (data::slice<U> output) {
        for (auto &x : output)
            x = ~x;

        for (auto &x : output) {
            ++x;

            if (x != 0)
                break;
        }
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
        endian::order word_order,
        // the ordering of each value in the array.
        endian::order byte_order,
        arithmetic::negativity neg
    ) {

        auto sign = NTL::sign (x);

        if (neg == arithmetic::negativity::nones && sign < 0)
            throw exception {} << "negative value";

        constexpr size_t bits = sizeof (U) * 8;

        const size_t capacity = output.size () * bits;

        size_t required =
            neg == arithmetic::negativity::nones
                ? NumBits (x)
                : bit_width (x);

        auto abs = NTL::abs (x);

        if (neg == arithmetic::negativity::BC &&
            sign < 0 &&
            bit_width (x) == NumBits (abs))
            ++required;

        if (required > capacity)
            throw exception {} << "integer does not fit";

        auto *bytes = reinterpret_cast<unsigned char *> (output.data ());
        const size_t nbytes = output.size () * sizeof (U);
        const unsigned char fill = ::NTL::sign (x) < 0 ? 0xff : 0x00;

        std::fill (bytes, bytes + nbytes, fill);

        BytesFromZZ (bytes, abs, nbytes);

        for (U &u : output) u = boost::endian::little_to_native<U> (u);

        // if x is negative, apply the appropriate negativity function.
        if (sign < 0) {
            if (neg == arithmetic::negativity::twos)
                negate_twos (output);
            else if (neg == arithmetic::negativity::BC)
                negate_bc (output);
        }

        if (word_order == endian::order::big)
            for (int i = 0; i < output.size () / 2; i++)
                std::swap (output[i], output[output.size () - 1 - i]);
    }

}

static_assert (data::group_number<data::N>);
static_assert (data::group_number<data::Z>);

#endif
