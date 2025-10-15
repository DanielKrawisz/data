// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <compare>
#include <gmp.h>
#include <gmpxx.h>

#include <data/sign.hpp>
#include <data/abs.hpp>
#include <data/arithmetic.hpp>
#include <data/divmod.hpp>
#include <data/increment.hpp>
#include <data/math/algebra.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include <data/encoding/hex.hpp>

#include <string>

namespace data::math::number::GMP {
    
    typedef mp_limb_t gmp_uint;
    typedef mp_limb_signed_t gmp_int;

    // this is an impediment to working on Windows but we need it for now.
    static_assert (sizeof (gmp_uint) == 8);
    static_assert (sizeof (gmp_int) == 8);
    
    const __mpz_struct MPZInvalid = __mpz_struct {0, 0, nullptr};
    
    bool inline equal (const __mpz_struct &a, const __mpz_struct &b) {
        return a._mp_alloc == b._mp_alloc && a._mp_size == b._mp_size && a._mp_d == b._mp_d;
    }
    
    uint32 inline size (const __mpz_struct &a) {
        return a._mp_alloc;
    }

    bool inline valid (const __mpz_struct &mpz) {
        return mpz._mp_d != nullptr;
    }
    
    math::sign inline sign (const __mpz_struct &mpz) {
        return !valid (mpz) ? zero : math::sign {static_cast<int8_t> (mpz_cmp_si (&mpz, 0))};
    }

    struct Z;
    struct N;

    bool operator == (const Z &, const Z &);
    std::strong_ordering operator <=> (const Z &, const Z &);

    bool operator == (const N &, const N &);
    std::strong_ordering operator <=> (const N &, const N &);

    template <std::integral I> bool operator == (const N &, I);
    template <std::integral I> bool operator == (I, const N &);

    template <std::integral I> bool operator == (const Z &, I);
    template <std::integral I> bool operator == (I, const Z &);

    template <std::signed_integral I> std::strong_ordering operator <=> (const N &, I);
    template <std::signed_integral I> std::strong_ordering operator <=> (I, const N &);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (const N &, I);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (I, const N &);

    template <std::signed_integral I> std::strong_ordering operator <=> (const Z &, I);
    template <std::signed_integral I> std::strong_ordering operator <=> (I, const Z &);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (const Z &, I);
    template <std::unsigned_integral I> std::strong_ordering operator <=> (I, const Z &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    bool operator == (const Z &, const sint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const Z &, const sint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    bool operator == (const N &, const uint<r, size, word> &);

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering operator <=> (const N &, const uint<r, size, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    bool operator == (const Z &, const Z_bytes<r, c, word> &);

    template <endian::order r, neg c, std::unsigned_integral word>
    std::strong_ordering operator <=> (const Z &, const Z_bytes<r, c, word> &);

    template <endian::order r, std::unsigned_integral word>
    bool operator == (const N &, const N_bytes<r, word> &);

    template <endian::order r, std::unsigned_integral word>
    std::strong_ordering operator <=> (const N &, const N_bytes<r, word> &);

    Z operator ~ (const N &);
    Z operator ~ (const Z &);

    Z operator - (const N &);
    Z operator - (const Z &);

    Z operator + (const Z &, const Z &);
    Z operator - (const Z &, const Z &);
    Z operator * (const Z &, const Z &);

    N operator + (const N &, const N &);
    N operator - (const N &, const N &);
    N operator * (const N &, const N &);

    Z operator | (const Z &, const Z &);
    Z operator & (const Z &, const Z &);
    Z operator ^ (const Z &, const Z &);

    N operator | (const N &, const N &);
    N operator & (const N &, const N &);
    N operator ^ (const N &, const N &);

    template <std::integral I> Z operator + (I, const Z &);
    template <std::integral I> Z operator + (const Z &, I);

    template <std::integral I> Z operator - (I, const Z &);
    template <std::integral I> Z operator - (const Z &, I);

    template <std::signed_integral I> Z operator * (I, const Z &);
    template <std::signed_integral I> Z operator * (const Z &, I);

    template <std::unsigned_integral I> Z operator * (I, const Z &);
    template <std::unsigned_integral I> Z operator * (const Z &, I);

    template <std::signed_integral I> Z operator + (I, const N &);
    template <std::signed_integral I> Z operator + (const N &, I);

    template <std::signed_integral I> Z operator - (I, const N &);
    template <std::signed_integral I> Z operator - (const N &, I);

    template <std::signed_integral I> Z operator * (I, const N &);
    template <std::signed_integral I> Z operator * (const N &, I);

    template <std::unsigned_integral I> N operator + (I, const N &);
    template <std::unsigned_integral I> N operator + (const N &, I);

    template <std::unsigned_integral I> N operator - (I, const N &);
    template <std::unsigned_integral I> N operator - (const N &, I);

    template <std::unsigned_integral I> N operator * (I, const N &);
    template <std::unsigned_integral I> N operator * (const N &, I);

    template <std::unsigned_integral I> N operator & (I, const N &);
    template <std::unsigned_integral I> N operator & (const N &, I);

    template <std::unsigned_integral I> N operator ^ (I, const N &);
    template <std::unsigned_integral I> N operator ^ (const N &, I);

    template <std::unsigned_integral I> N operator | (I, const N &);
    template <std::unsigned_integral I> N operator | (const N &, I);

    Z operator / (const Z &, const Z &);
    N operator / (const N &, const N &);

    Z operator / (const Z &, int64);
    N operator / (const N &, uint64);

    N operator % (const Z &, const Z &);
    N operator % (const Z &, const N &);
    N operator % (const N &, const N &);

    uint64 operator % (const Z &, uint64);
    uint64 operator % (const N &, uint64);

    Z &operator ++ (Z &);
    Z &operator -- (Z &);

    Z operator ++ (Z &, int);
    Z operator -- (Z &, int);

    N &operator ++ (N &);
    N &operator -- (N &);

    N operator ++ (N &, int);
    N operator -- (N &, int);

    Z operator << (const Z &, int);
    Z operator >> (const Z &, int);

    N operator << (const N &, int);
    N operator >> (const N &, int);

    std::ostream &operator << (std::ostream &o, const Z &n);
    std::ostream &operator << (std::ostream &o, const N &n);

    std::istream &operator >> (std::istream &i, Z &z);
    std::istream &operator >> (std::istream &i, N &n);

    Z &operator += (Z &, const Z &);
    Z &operator -= (Z &, const Z &);
    Z &operator *= (Z &, const Z &);
    Z &operator /= (Z &, const Z &);

    N &operator += (N &, const N &);
    N &operator -= (N &, const N &);
    N &operator *= (N &, const N &);
    N &operator /= (N &, const N &);
    N &operator %= (N &, const N &);

    template <std::unsigned_integral I> N &operator += (N &, I);
    template <std::unsigned_integral I> N &operator -= (N &, I);
    template <std::unsigned_integral I> N &operator *= (N &, I);
    template <std::unsigned_integral I> N &operator /= (N &, I);
    template <std::unsigned_integral I> N &operator %= (N &, I);

    Z &operator &= (Z &, const Z &);
    Z &operator |= (Z &, const Z &);
    Z &operator ^= (Z &, const Z &);

    N &operator &= (N &, const N &);
    N &operator |= (N &, const N &);
    N &operator ^= (N &, const N &);

    template <std::unsigned_integral I> N &operator &= (N &, I);
    template <std::unsigned_integral I> N &operator |= (N &, I);
    template <std::unsigned_integral I> N &operator ^= (N &, I);

    Z &operator <<= (Z &, int);
    Z &operator >>= (Z &, int);

    N &operator <<= (N &, int);
    N &operator >>= (N &, int);
}

namespace data {
    using Z = math::number::GMP::Z;
    using N = math::number::GMP::N;
}

namespace data::math::def {

    template <> struct abs<Z> {
        N operator () (const Z &);
    };

    template <> struct abs<N> {
        N operator () (const N &n);
    };

    template <> struct times<Z> {
        Z operator () (const Z &a, const Z &b);
        nonzero<Z> operator () (const nonzero<Z> &a, const nonzero<Z> &b);
    };

    template <> struct divmod<N, N> {
        division<N, N> operator () (const N &a, const nonzero<N> &b);
    };

    template <> struct divmod<Z, N> {
        division<Z, N> operator () (const Z &a, const nonzero<N> &b);
    };

    template <> struct divmod<Z, Z> {
        division<Z, N> operator () (const Z &a, const nonzero<Z> &b);
    };

    template <> struct identity<plus<Z>, Z> {
        Z operator () ();
    };

    template <> struct inverse<plus<Z>, Z> {
        Z operator () (const Z &a, const Z &b);
    };

    template <> struct inverse<times<Z>, Z> {
        nonzero<Z> operator () (const nonzero<Z> &a, const nonzero<Z> &b);
    };

    template <> struct identity<times<Z>, Z> {
        Z operator () ();
    };

    template <> struct bit_xor<N> {
        N operator () (const N &a, const N &b);
    };

    template <> struct bit_xor<Z> {
        Z operator () (const Z &, const Z &);
    };

}

namespace data::math::number {

    template <> struct increment<N> {
        nonzero<N> operator () (const N &);
    };

    template <> struct increment<Z> {
        Z operator () (const Z &);
    };

    template <> struct decrement<Z> {
        Z operator () (const Z &);
    };

    template <> struct decrement<N> {
        N operator () (const nonzero<N> &);
        N operator () (const N &);
    };
}

namespace data::math::number::GMP {

    struct Z final {
        mpz_t MPZ;

        Z ();
        ~Z ();

        // We need these to ensure that we can accept
        // any number literal.
        template <std::signed_integral I> Z (I);
        template <std::unsigned_integral I> Z (I);

        static Z read (string_view x);

        //Z (const std::string &x);
        Z (string_view);

        Z (const Z &n);

        Z (Z &&n);

        Z &operator = (const Z &n);

        Z &operator = (Z &&n);

        size_t size () const;

        mp_limb_t &operator [] (size_t i);

        const mp_limb_t &operator [] (size_t i) const;

        mp_limb_t *begin ();

        mp_limb_t *end ();

        const mp_limb_t *begin () const;

        const mp_limb_t *end () const;

        explicit operator int64 () const;
        explicit operator double () const;

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit Z (const Z_bytes<r, c, word> &);

        template <endian::order r, std::unsigned_integral word>
        explicit Z (const N_bytes<r, word> &);

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const;

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<true, r, size, word> &x): Z {Z_bytes<r, neg::twos, word> {x}} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<false, r, size, word> &x): Z {N_bytes<r, word> {x}} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit operator bounded<true, r, size, word> () const {
            return bounded<true, r, size, word> (Z_bytes<r, neg::twos, word> (*this));
        }

    };

    // implementation of naturals given an implementation of integers.
    struct N {
        Z Value;

        N () : Value {} {}

        // need all of these to ensure that we can work with number
        // literals.

        // We need these to ensure that we can accept
        // any number literal.
        template <std::signed_integral I> N (I);
        template <std::unsigned_integral I> N (I);

        explicit N (const Z &z) : Value {z} {}
        explicit N (Z &&z) : Value {z} {}

        N (string_view);

        operator Z () const {
            return Value;
        }

        bool valid () const {
            return data::valid (Value) && Value >= 0;
        }

        explicit operator double () const {
            return double (Value);
        }

        explicit operator uint64 () const;
        explicit operator int32 () const;

        template <endian::order r, std::unsigned_integral word>
        explicit N (const N_bytes<r, word> &n) : Value {n} {}

        template <endian::order r, std::unsigned_integral word>
        explicit operator N_bytes<r, word> () const;

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit operator Z_bytes<r, c, word> () const {
            return Z_bytes<r, c, word> (N_bytes<r, word> (*this));
        }

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit N (const bounded<false, r, size, word> &x): N {N_bytes<r, word> (x)} {}

        template <bool u, endian::order r, size_t size, std::unsigned_integral word>
        explicit operator bounded<u, r, size, word> () const {
            return bounded<u, r, size, word> (N_bytes<r, word> (*this));
        }

    };

}

namespace data::encoding::decimal {
    struct string;
    string write (const N &);
    
    std::ostream &write (std::ostream &, const N &);
    
}

namespace data::encoding::hexidecimal {
    template <neg, hex_case> struct integer;
    
    template <hex_case zz> integer<neg::nones, zz> write (const N &);
    template <neg n, hex_case zz> integer<n, zz> write (const Z &);

    std::ostream &write (std::ostream &, const N &, hex_case = hex_case::lower);
    std::ostream &write (std::ostream &, const Z &, hex_case = hex_case::lower, neg = neg::twos);
    
}

namespace data::encoding::signed_decimal {
    struct string;
    string write (const Z &);
    
    std::ostream &write (std::ostream &, const Z &);
}

namespace data::math::number::GMP {

    inline Z::Z () {
        mpz_init (MPZ);
    }

    inline Z::~Z () {
        mpz_clear (MPZ);
    }

    template <std::signed_integral I> Z::Z (I x): MPZ {} {
        mpz_init_set_si (MPZ, x);
    }

    template <std::unsigned_integral I> Z::Z (I x): MPZ {} {
        mpz_init_set_ui (MPZ, x);
    }

    template <std::signed_integral I> N::N (I x): Value {} {
        if (x < 0) throw exception {} << "N cannot be less than zero";
        mpz_init_set_si (Value.MPZ, x);
    }

    template <std::unsigned_integral I> N::N (I x): Value {} {
        mpz_init_set_ui (Value.MPZ, x);
    }

    inline Z::Z (const Z &n) {
        mpz_init (MPZ);
        mpz_set (MPZ, n.MPZ);
    }

    inline Z::Z (Z &&n) : Z {} {
        mpz_swap (MPZ, n.MPZ);
    }

    Z inline &Z::operator = (const Z &n) {
        mpz_set (MPZ, n.MPZ);
        return *this;
    }

    Z inline &Z::operator = (Z &&n) {
        mpz_swap (MPZ, n.MPZ);
        return *this;
    }

    size_t inline Z::size () const {
        return GMP::size (MPZ[0]);
    }

    mp_limb_t inline &Z::operator [] (size_t i) {
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
        return *(MPZ[0]._mp_d + i);
    }

    const mp_limb_t inline &Z::operator [] (size_t i) const {
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
        return *(MPZ[0]._mp_d + i);
    }

    mp_limb_t inline *Z::begin () {
        return mpz_limbs_modify (this->MPZ, mpz_size (this->MPZ));
    }

    mp_limb_t inline *Z::end () {
        return mpz_limbs_modify (this->MPZ, mpz_size (this->MPZ)) + mpz_size (this->MPZ);
    }

    const mp_limb_t inline *Z::begin () const {
        return mpz_limbs_read (this->MPZ);
    }

    const mp_limb_t inline *Z::end () const {
        return mpz_limbs_read (this->MPZ) + mpz_size (this->MPZ);
    };

    inline Z::operator double () const {
        return mpz_get_d (MPZ);
    }

    bool inline operator == (const Z &a, const Z &b) {
        return a <=> b == 0;
    }

    bool inline operator == (const N &a, const N &b) {
        return a <=> b == 0;
    }

    template <std::integral I> bool inline operator == (const N &a, I b) {
        return a <=> b == 0;
    }

    template <std::integral I> bool inline operator == (I a, const N &b) {
        return a <=> b == 0;
    }

    template <std::integral I> bool inline operator == (const Z & a, I b) {
        return a <=> b == 0;
    }

    template <std::integral I> bool inline operator == (I a, const Z &b) {
        return a <=> b == 0;
    }

    std::strong_ordering inline operator <=> (const N &a, const N &b) {
        auto cmp = mpz_cmp (a.Value.MPZ, b.Value.MPZ);
        return cmp < 0 ? std::strong_ordering::less :
            cmp > 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    std::strong_ordering inline operator <=> (const Z &a, const Z &b) {
        auto cmp = mpz_cmp (a.MPZ, b.MPZ);
        return cmp < 0 ? std::strong_ordering::less :
            cmp > 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::signed_integral I>
    std::strong_ordering inline operator <=> (const Z &a, I b) {
        auto cmp = mpz_cmp_si (a.MPZ, b);
        return cmp < 0 ? std::strong_ordering::less :
            cmp > 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::signed_integral I>
    std::strong_ordering inline operator <=> (I a, const Z &b) {
        auto cmp = mpz_cmp_si (b.MPZ, a);
        return cmp > 0 ? std::strong_ordering::less :
            cmp < 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::unsigned_integral I>
    std::strong_ordering inline operator <=> (const Z &a, I b) {
        auto cmp = mpz_cmp_ui (a.MPZ, b);
        return cmp < 0 ? std::strong_ordering::less :
        cmp > 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::unsigned_integral I>
    std::strong_ordering inline operator <=> (I a, const Z &b) {
        auto cmp = mpz_cmp_ui (b.MPZ, a);
        return cmp > 0 ? std::strong_ordering::less :
        cmp < 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::signed_integral I>
    std::strong_ordering inline operator <=> (const N &a, I b) {
        auto cmp = mpz_cmp_si (a.Value.MPZ, b);
        return cmp < 0 ? std::strong_ordering::less :
        cmp > 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::signed_integral I>
    std::strong_ordering inline operator <=> (I a, const N &b) {
        auto cmp = mpz_cmp_si (b.Value.MPZ, a);
        return cmp > 0 ? std::strong_ordering::less :
        cmp < 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::unsigned_integral I>
    std::strong_ordering inline operator <=> (const N &a, I b) {
        auto cmp = mpz_cmp_ui (a.Value.MPZ, b);
        return cmp < 0 ? std::strong_ordering::less :
        cmp > 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::unsigned_integral I>
    std::strong_ordering inline operator <=> (I a, const N &b) {
        auto cmp = mpz_cmp_ui (b.Value.MPZ, a);
        return cmp > 0 ? std::strong_ordering::less :
        cmp < 0 ? std::strong_ordering::greater : std::strong_ordering::equivalent;
    }

    template <std::unsigned_integral I> N inline &operator /= (N &u, I x) {
        return u &= N {x};
    }

    template <std::unsigned_integral I> N inline &operator %= (N &u, I x) {
        return u &= N {x};
    }

    Z inline operator / (const Z &a, const Z &b) {
        return def::divmod<Z, Z> {} (a, nonzero {b}).Quotient;
    }

    Z inline &operator /= (Z &a, const Z& z) {
        return a = a / z;
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

    uint64 inline operator % (const Z &a, uint64 b) {
        return uint64 (a % N (b));
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    bool inline operator == (const Z &a, const sint<r, size, word> &b) {
        return (a <=> b) == 0;
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    bool inline operator == (const N &a, const uint<r, size, word> &b) {
        return (a <=> b) == 0;
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (const Z &a, const sint<r, size, word> &b) {
        return a <=> Z (b);
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (const N &a, const uint<r, size, word> &b) {
        return a <=> N (b);
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    bool inline operator == (const Z &a, const Z_bytes<r, c, word> &b) {
        return a == Z (b);
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (const Z &a, const Z_bytes<r, c, word> &b) {
        return a <=> Z (b);
    }

    template <endian::order r, std::unsigned_integral word>
    bool inline operator == (const N &a, const N_bytes<r, word> &b) {
        return a == N (b);
    }

    template <endian::order r, std::unsigned_integral word>
    std::strong_ordering inline operator <=> (const N &a, const N_bytes<r, word> &b) {
        return a <=> N (b);
    }

    constexpr int inline endian_boost_to_GMP (endian::order r) {
        return r == endian::order::big ? 1 : -1;
    }

    template <endian::order r, std::unsigned_integral word>
    Z::Z (const N_bytes<r, word> &z) {
        mpz_init (MPZ);
        mpz_import (
            MPZ,
            z.size (),
            endian_boost_to_GMP (r),
            sizeof (word),
            // TODO this may not be true in the future.
            endian_boost_to_GMP (endian::order::native),
            0, // number of unused bits in each element.
            z.data ());
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    Z::Z (const Z_bytes<r, c, word> &z) {
        mpz_init (MPZ);
        if (data::is_negative (z)) {
            *this = std::move (Z (-z));
            // this negates the value of the number.
            MPZ[0]._mp_size = -MPZ[0]._mp_size;
            return;
        }

        mpz_import (
            MPZ,
            z.size (),
            endian_boost_to_GMP (r),
            sizeof (word),
            // TODO this may not be true in the future.
            endian_boost_to_GMP (endian::order::native),
            0, // number of unused bits in each element.
            z.data ());
    }

    // TODO use mpz_export
    template <endian::order r, neg c, std::unsigned_integral word>
    Z::operator Z_bytes<r, c, word> () const {
        if (data::is_negative (*this))
            return -Z_bytes<r, c, word> (-(*this));

        // the new number.
        Z_bytes<r, c, word> nn {};
        size_t n_size = mpz_size (MPZ);

        if (n_size == 0) return nn;

        if constexpr (sizeof (word) == sizeof (GMP::gmp_uint)) {
            nn.resize (n_size + 1);
            std::copy (begin (), end (), nn.words ().begin ());
        } else if (sizeof (word) < sizeof (GMP::gmp_uint)) {
            nn.resize (n_size * (sizeof (GMP::gmp_uint) / sizeof (word)) + 1);
            auto nit = nn.words ().begin ();
            for (const GMP::gmp_uint &limb : *this) {
                GMP::gmp_uint z = limb;
                for (int i = 0; i < sizeof (GMP::gmp_uint) / sizeof (word); i++) {
                    *nit = static_cast<word> (z & std::numeric_limits<word>::max ());
                    nit++;
                    z >>= (sizeof (word) * 8);
                }
            }
        } else if (sizeof (word) > sizeof (GMP::gmp_uint)) {
            nn.resize (n_size / (sizeof (GMP::gmp_uint) / sizeof (word)) + 1);
            auto nit = nn.words ().begin ();
            auto b = begin ();
            while (b != end ()) {
                *nit = 0;
                for (int i = 0; i < sizeof (GMP::gmp_uint) / sizeof (word); i++) {
                    *nit += static_cast<word> (*b) << (i * sizeof (GMP::gmp_uint) * 8);
                    b++;
                }
                nit++;
            }
        } else throw exception {} << "unhandled case in Z -> Z_bytes (this is impossible)";

        // set last digit to zero.
        *(nn.words ().end () - 1) = 0;

        return trim (nn);
    }

    // TODO use mpz_export
    template <endian::order r, std::unsigned_integral word>
    N::operator N_bytes<r, word> () const {
        // the new number.
        N_bytes<r, word> nn {};
        size_t n_size = mpz_size (Value.MPZ);
        if (n_size == 0) return nn;

        if constexpr (sizeof (word) == sizeof (GMP::gmp_uint)) {
            nn.resize (n_size);
            std::copy (Value.begin (), Value.end (), nn.words ().begin ());
        } else if (sizeof (word) < sizeof (GMP::gmp_uint)) {
            nn.resize (n_size * (sizeof (GMP::gmp_uint) / sizeof (word)));
            auto x = nn.words ().begin ();
            for (const GMP::gmp_uint &limb : Value) {
                GMP::gmp_uint z = limb;
                for (int i = 0; i < sizeof (GMP::gmp_uint) / sizeof (word); i++) {
                    *x = static_cast<word> (z & std::numeric_limits<word>::max ());
                    x++;
                    z >>= (sizeof (word) * 8);
                }
            }
        } else if (sizeof (word) > sizeof (GMP::gmp_uint)) {
            nn.resize (n_size / (sizeof (GMP::gmp_uint) / sizeof (word)));
            auto nit = nn.words ().begin ();
            auto b = Value.begin ();
            while (b != Value.end ()) {
                *nit = 0;
                for (int i = 0; i < sizeof (GMP::gmp_uint) / sizeof (word); i++) {
                    *nit += static_cast<word> (*b) << (i * sizeof (GMP::gmp_uint) * 8);
                    b++;
                }
                nit++;
            }
        } else throw exception {} << "unhandled case in N -> N_bytes (this is impossible)";

        return trim (nn);
    }

}

namespace data::math::def {

    Z inline identity<plus<Z>, Z>::operator () () {
        return 0;
    }

    Z inline identity<times<Z>, Z>::operator () () {
        return 1;
    }

    Z inline inverse<plus<Z>, Z>::operator () (const Z &a, const Z &b) {
        return b - a;
    }

    Z inline times<Z>::operator () (const Z &a, const Z &b) {
        return a * b;
    }

    nonzero<Z> inline times<Z>::operator () (const nonzero<Z> &a, const nonzero<Z> &b) {
        return a * b;
    }

    N inline bit_xor<N>::operator () (const N &a, const N &b) {
        return a ^ b;
    }

    Z inline bit_xor<Z>::operator () (const Z &a, const Z &b) {
        return a ^ b;
    }
}

#endif
