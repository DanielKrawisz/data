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
// TODO we should get rid of this and use NTL's native function.
#include <data/math/power.hpp>
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
        template <std::integral I> Z (I u);

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

        // need all of these to ensure that we can work with number
        // literals.

        // We need these to ensure that we can accept
        // any number literal.
        template <std::unsigned_integral I> N (I u): Value {NTL::conv<NTL::ZZ> (u)} {}
        template <std::signed_integral I> N (I u): Value {NTL::conv<NTL::ZZ> (u)} {
            if (u < 0) throw exception {} << "cannot instantiate N with negative number " << u;
        }

        explicit N (const Z &z) : Value {z.Value} {
            if (Value < 0) throw exception {} << "cannot instantiate N with negative number " << Value;
        }

        explicit N (Z &&z) : Value {z.Value} {
            if (Value < 0) throw exception {} << "cannot instantiate N with negative number " << Value;
        }

        N (string_view);
        N (const dec_uint &u): N {string_view (u)} {}
        template <hex_case zz> N (const hex::uint<zz> &u): N {string_view (u)} {}
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

/*
    ZZ read (const encoding::signed_decimal::string &u) {
        if (!u.valid ()) throw exception {} << "Invalid decimal string";
        if (u[0] == '-') return -conv<NTL::ZZ> (u.c_str () + 1);
        return conv<NTL::ZZ> (u.c_str ());
    }

    template <arithmetic::negativity neg, hex_case x>
    ZZ inline read (const encoding::hexidecimal::integer<neg, x> &u) {
        if (!u.valid ()) throw exception {} << "Invalid hexidecimal string";
        bytes decoded = *encoding::hex::read (string_view (u).substr (2));
        return import_bin<byte> (byte_slice (decoded), endian::order::little, endian::order::native, neg);
    }

    ZZ inline read (const encoding::base58::string &u) {
        if (!u.valid ()) throw exception {} << "Invalid base58 string";
        *encoding::base58::decode<ZZ> (u);
    }

    template <std::integral I>
    ZZ inline read_integral (I x) {
        return import_bin ((const std::make_unsigned<I> *) (&x), 1, 1, endian::order::native, arithmetic::negativity::ones);
    }*/

}

#include <data/math/number/prime.hpp>
#include <data/math/root.hpp>

namespace data::math::number::NTL {

    bool aks_is_prime (const Z &);

    set<N> root (const N &n, uint32 p);
    set<Z> root (const Z &n, uint32 p);

}

namespace data::math::number {

    template <> struct AKS<N> {
        prime<N> is_prime (const N n) {
            return NTL::aks_is_prime (n) ? prime<N> {n, prime<N>::certain} : prime<N> {};
        }
    };

    template struct AKS<N>;

}

namespace data::math {

    template <uint64 pow> struct root<N, pow> {
        set<N> operator () (const N &n) {
            return number::NTL::root (n, pow);
        }
    };

    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator () (const Z &z) {
            return number::NTL::root (z, pow);
        }
    };

}

static_assert (data::group_number<data::N>);
static_assert (data::group_number<data::Z>);

#endif
