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

    void conv (ZZ &x, const data::encoding::signed_decimal::string &u);

    template <data::arithmetic::negativity neg, data::hex_case cc>
    void conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &u);

    void conv (ZZ &x, const data::encoding::base58::string &u);

    template <data::endian::order r, data::arithmetic::negativity neg, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &u);

    template <data::endian::order r, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::N_bytes<r, word> &u);

    template <bool is_signed, data::endian::order r, std::size_t size, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &u);

    template <bool is_signed, data::endian::order r, std::size_t size>
    void conv (ZZ &x, const data::endian::integral<is_signed, r, size> &u);

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
}

namespace data::math::number {
    struct Z final {

        Z ();

        // We need these to ensure that we can accept
        // any number literal.
        template <std::integral I> Z (I u): Value {
            NTL::import_bin (
                slice<const std::make_unsigned_t<I>> {(const std::make_unsigned_t<I> *) (&u), 1},
                endian::order::little, endian::order::native, arithmetic::negativity::ones)} {}

        static Z read (string_view x);

        Z (string_view);

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
        explicit operator I () const;
        explicit operator double () const;

        explicit operator dec_int () const;
        template <hex_case zz> explicit operator hex::int2<zz> () const;
        template <hex_case zz> explicit operator hex::intBC<zz> () const;

        // TODO get rid of these operators and make them constructors.
        template <endian::order r, neg c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const;

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit operator bounded<true, r, size, word> () const {
            return bounded<true, r, size, word> (this->operator Z_bytes<r, neg::twos, word> ());
        }

        explicit Z (const NTL::ZZ &u);
        explicit Z (NTL::ZZ &&u);

        NTL::ZZ Value;
    };

    // implementation of naturals given an implementation of integers.
    struct N {

        N () : Value {} {}

        template <std::unsigned_integral I> N (I u): Value {
            NTL::import_bin (
                slice<const I> {(const I *) (&u), 1},
                endian::order::little, endian::order::native, arithmetic::negativity::ones)} {}

        template <std::signed_integral I> N (I u): Value {
            NTL::import_bin (
                slice<const std::make_unsigned_t<I>> {(const std::make_unsigned_t<I> *) (&u), 1},
                endian::order::little, endian::order::native, arithmetic::negativity::ones)} {
            if (u < 0) throw exception {} << "cannot instantiate N with negative number " << u;
        }

        explicit N (const Z &z) : Value {z.Value} {
            if (Value < 0) throw exception {} << "cannot instantiate N with negative number " << Value;
        }

        explicit N (Z &&z) : Value {z.Value} {
            if (Value < 0) throw exception {} << "cannot instantiate N with negative number " << Value;
        }

        N (string_view);
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

        explicit operator double () const;

        template <std::integral I>
        explicit operator I () const;

        explicit operator dec_uint () const;
        explicit operator dec_int () const;
        template <neg n, hex_case zz> explicit operator hex::integer<n, zz> () const;

        template <endian::order r, std::unsigned_integral word>
        explicit operator N_bytes<r, word> () const;

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const {
            return Z_bytes<r, c, word> (N_bytes<r, word> (*this));
        }

        template <bool u, endian::order r, size_t size, std::unsigned_integral word>
        explicit operator bounded<u, r, size, word> () const {
            return bounded<u, r, size, word> (N_bytes<r, word> (*this));
        }

        explicit N (const NTL::ZZ &);
        explicit N (NTL::ZZ &&);
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
            is_signed ? data::arithmetic::negativity::ones : data::arithmetic::negativity::nones);
    }

    template <bool is_signed, data::endian::order r, std::size_t size>
    void inline conv (ZZ &x, const data::endian::integral<is_signed, r, size> &u) {
        x = data::math::number::NTL::import_bin<data::byte> (
            data::slice<const data::byte> (u), data::endian::order::native, r,
            is_signed ? data::arithmetic::negativity::ones : data::arithmetic::negativity::nones);
    }

}

static_assert (data::group_number<data::N>);
static_assert (data::group_number<data::Z>);

#endif
