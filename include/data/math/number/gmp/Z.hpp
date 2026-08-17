// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/power.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/division.hpp>

#include <data/math/number/bounded/bounded.hpp>
#include <data/encoding/integer.hpp>
#include <data/encoding/base58.hpp>

namespace data::math::number {

    struct Z final {
        mpz_t MPZ;

        Z ();
        ~Z ();

        Z (const Z &n);

        Z (Z &&n);

        Z &operator = (const Z &n);

        Z &operator = (Z &&n);

        // We need these to ensure that we can accept
        // any number literal.
        template <std::signed_integral I> Z (I);
        template <std::unsigned_integral I> Z (I);

        static Z read (string_view x);

        Z (string_view);

        Z (const dec_int &u): Z {string_view (u)} {}
        Z (const dec_uint &u): Z {string_view (u)} {}
        template <neg n, hex_case zz> Z (const hex::integer<n, zz> &);

        template <endian::order r, neg c, std::unsigned_integral word>
        explicit Z (const Z_bytes<r, c, word> &);

        template <endian::order r, std::unsigned_integral word>
        explicit Z (const N_bytes<r, word> &);

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<true, r, size, word> &x): Z {Z_bytes<r, neg::twos, word> {x}} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit Z (const bounded<false, r, size, word> &x): Z {N_bytes<r, word> {x}} {}

        size_t size () const;

        mp_limb_t &operator [] (size_t i);

        const mp_limb_t &operator [] (size_t i) const;

        mp_limb_t *begin ();

        mp_limb_t *end ();

        const mp_limb_t *begin () const;

        const mp_limb_t *end () const;

        // TODO we can make a cast to any integral type
        explicit operator int64 () const;
        explicit operator int32 () const;
        explicit operator uint64 () const;
        explicit operator uint32 () const;
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

    };

    // implementation of naturals given an implementation of integers.
    // TODO N can extend Z.
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
        N (const dec_uint &u): N {string_view (u)} {}
        template <hex_case zz> N (const hex::uint<zz> &u): N {string_view (u)} {}
        N (const base58_uint &u);

        template <endian::order r, std::unsigned_integral word>
        explicit N (const N_bytes<r, word> &n) : Value {n} {}

        template <endian::order r, size_t size, std::unsigned_integral word>
        explicit N (const bounded<false, r, size, word> &x): N {N_bytes<r, word> (x)} {}

        operator Z () const {
            return Value;
        }

        bool valid () const {
            return data::valid (Value) && Value >= 0;
        }

        explicit operator double () const {
            return double (Value);
        }

        explicit operator uint64 () const {
            return uint64 (Value);
        }

        explicit operator uint32 () const {
            return uint32 (Value);
        }

        explicit operator int64 () const {
            return int64 (Value);
        }

        explicit operator int32 () const {
            return int32 (Value);
        }

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

    };

}
namespace data::math::number {

    inline Z::Z () {
        mpz_init (MPZ);
    }

    inline Z::~Z () {
        mpz_clear (MPZ);
    }

    template <std::signed_integral I> Z::Z (I x): Z {} {
        mpz_init_set_si (MPZ, x);
    }

    template <std::unsigned_integral I> Z::Z (I x): Z {} {
        mpz_init_set_ui (MPZ, x);
    }

    template <std::signed_integral I> N::N (I x): Value {} {
        if (x < 0) throw exception {} << "N cannot be less than zero";
        mpz_init_set_si (Value.MPZ, x);
    }

    template <std::unsigned_integral I> N::N (I x): Value {} {
        mpz_init_set_ui (Value.MPZ, x);
    }

    inline Z::Z (const Z &n) : Z {} {
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
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw out_of_range {"Z"};
        return *(MPZ[0]._mp_d + i);
    }

    const mp_limb_t inline &Z::operator [] (size_t i) const {
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw out_of_range {"Z"};
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

    template <std::integral I> bool inline operator == (const Z &a, I b) {
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
    Z::Z (const N_bytes<r, word> &z) : Z {} {
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
    Z::Z (const Z_bytes<r, c, word> &z) : Z {} {

        if (data::is_zero (z)) return;

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
            return -convert<Z_bytes<r, c, word>> (-(*this));

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

    inline N::N (const base58_uint &u): Value {} {
        if (!u.valid ()) throw exception {} << "invalid base 58 number" << *this;
        *this = *encoding::base58::decode<N> (u);
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

namespace data::encoding::base58 {

    template <std::integral I> inline string::string (I x): string {encode (N {x})} {}

}

namespace data::encoding::hexidecimal { 
    
    template <neg n, hex_case zz> integer<n, zz> inline write (const Z &z) {
        std::stringstream ss;
        write (ss, z, zz, n);
        return integer<n, zz> {ss.str ()};
    }

    template <hex_case zz> integer<neg::nones, zz> inline write (const N &n) {
        std::stringstream ss;
        write (ss, n, zz);
        return integer<neg::nones, zz> {ss.str ()};
    }

}

namespace data::encoding::signed_decimal { 
    
    string inline write (const Z &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
    
}

namespace data::encoding::decimal {

    string inline write (const N &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }

}

namespace data::math::number {

    template <std::unsigned_integral I> N inline operator + (const N &n, I x) {
        N sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, n.Value.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator + (I x, const N &n) {
        N sum;
        __gmp_binary_plus::eval (sum.Value.MPZ, n.Value.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator * (const N &n, I x) {
        N sum;
        __gmp_binary_multiplies::eval (sum.Value.MPZ, n.Value.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator * (I x, const N &n) {
        N sum;
        __gmp_binary_multiplies::eval (sum.Value.MPZ, n.Value.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> N inline operator - (const N &n, I x) {
        if (n <= x) return 0;
        N diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, n.Value.MPZ, (GMP::gmp_uint) x);
        return diff;
    }

    template <std::unsigned_integral I> N inline operator - (I x, const N &n) {
        if (x <= n) return 0;
        N diff;
        __gmp_binary_minus::eval (diff.Value.MPZ, (GMP::gmp_uint) x, n.Value.MPZ);
        return diff;
    }

    template <std::unsigned_integral I> N inline &operator += (N &u, I x) {
        __gmp_binary_plus::eval (u.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator -= (N &u, I x) {
        if (u <= x) u = 0;
        __gmp_binary_minus::eval (u.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator *= (N &u, I x) {
        __gmp_binary_multiplies::eval (u.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N operator & (I x, const N &u) {
        N result;
        __gmp_binary_and::eval (result.Value.MPZ, (GMP::gmp_uint) x, u.Value.MPZ);
        return result;
    }

    template <std::unsigned_integral I> N operator & (const N &u, I x) {
        N result;
        __gmp_binary_and::eval (result.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return result;
    }

    template <std::unsigned_integral I> N operator ^ (I x, const N &u) {
        N result;
        __gmp_binary_xor::eval (result.Value.MPZ, (GMP::gmp_uint) x, u.Value.MPZ);
        return result;
    }

    template <std::unsigned_integral I> N operator ^ (const N &u, I x) {
        N result;
        __gmp_binary_xor::eval (result.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return result;
    }

    template <std::unsigned_integral I> N operator | (I x, const N &u) {
        N result;
        __gmp_binary_ior::eval (result.Value.MPZ, (GMP::gmp_uint) x, u.Value.MPZ);
        return result;
    }

    template <std::unsigned_integral I> N operator | (const N &u, I x) {
        N result;
        __gmp_binary_ior::eval (result.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return result;
    }

    template <std::unsigned_integral I> N inline &operator &= (N &u, I x) {
        __gmp_binary_and::eval (u.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator |= (N &u, I x) {
        __gmp_binary_ior::eval (u.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> N inline &operator ^= (N &u, I x) {
        __gmp_binary_xor::eval (u.Value.MPZ, u.Value.MPZ, (GMP::gmp_uint) x);
        return u;
    }

    template <std::unsigned_integral I> Z inline operator + (const Z &n, I x) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> Z inline operator + (I x, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator + (const Z &n, I x) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator + (I x, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (const Z &n, I x) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (I x, const Z &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::unsigned_integral I> Z inline operator * (const Z &n, I x) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::unsigned_integral I> Z inline operator * (I x, const Z &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.MPZ, (GMP::gmp_uint) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator - (const Z &n, I x) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, n.MPZ, (GMP::gmp_int) x);
        return diff;
    }

    template <std::signed_integral I> Z inline operator - (I x, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, (GMP::gmp_int) x, n.MPZ);
        return diff;
    }

    template <std::unsigned_integral I> Z inline operator - (const Z &n, I x) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, n.MPZ, (GMP::gmp_uint) x);
        return diff;
    }

    template <std::unsigned_integral I> Z inline operator - (I x, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, (GMP::gmp_uint) x, n.MPZ);
        return diff;
    }

    template <std::signed_integral I> Z inline operator + (const N &n, I x) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.Value.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator + (I x, const N &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, n.Value.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (const N &n, I x) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.Value.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator * (I x, const N &n) {
        Z sum;
        __gmp_binary_multiplies::eval (sum.MPZ, n.Value.MPZ, (GMP::gmp_int) x);
        return sum;
    }

    template <std::signed_integral I> Z inline operator - (const N &n, I x) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, n.Value.MPZ, (GMP::gmp_int) x);
        return diff;
    }

    template <std::signed_integral I> Z inline operator - (I x, const N &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, (GMP::gmp_int) x, n.Value.MPZ);
        return diff;
    }
    
    Z inline &operator ++ (Z &n) {
        __gmp_unary_increment::eval (n.MPZ);
        return n;
    }
    
    Z inline &operator -- (Z &n) {
        __gmp_unary_decrement::eval (n.MPZ);
        return n;
    }
    
    Z inline &operator += (Z &z, int64 n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n);
        return z;
    }

    Z inline &operator -= (Z &z, int64 n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n);
        return z;
    }

    Z inline &operator *= (Z &z, int64 n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, n);
        return z;
    }
    
    Z inline &operator += (Z &z, const Z& n) {
        __gmp_binary_plus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator -= (Z &z, const Z &n) {
        __gmp_binary_minus::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator *= (Z &z, const Z &n) {
        __gmp_binary_multiplies::eval (z.MPZ, z.MPZ, n.MPZ);
        return z;
    }
    
    Z inline &operator &= (Z &a, const Z &b) {
        __gmp_binary_and::eval (a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline &operator |= (Z &a, const Z &b) {
        __gmp_binary_ior::eval (a.MPZ, a.MPZ, b.MPZ);
        return a;
    }

    Z inline &operator ^= (Z &a, const Z &b) {
        __gmp_binary_xor::eval (a.MPZ, a.MPZ, b.MPZ);
        return a;
    }
    
    Z inline operator - (const Z &n) {
        Z z {n};
        z.MPZ[0]._mp_size = -z.MPZ[0]._mp_size;
        return z;
    }
    
    Z inline operator + (const Z &z, const Z &n) {
        Z sum;
        __gmp_binary_plus::eval (sum.MPZ, z.MPZ, n.MPZ);
        return sum;
    }
    
    Z inline operator - (const Z &z, const Z &n) {
        Z diff;
        __gmp_binary_minus::eval (diff.MPZ, z.MPZ, n.MPZ);
        return diff;
    }
    
    Z inline operator * (const Z &z, const Z &n) {
        Z prod;
        __gmp_binary_multiplies::eval (prod.MPZ, z.MPZ, n.MPZ);
        return prod;
    }
    
    Z inline operator & (const Z &a, const Z &b) {
        Z x;
        __gmp_binary_and::eval (x.MPZ, a.MPZ, b.MPZ);
        return x;
    }
    
    Z inline operator | (const Z &a, const Z &b) {
        Z x;
        __gmp_binary_ior::eval (x.MPZ, a.MPZ, b.MPZ);
        return x;
    }

    Z inline operator ^ (const Z &a, const Z &b) {
        Z x;
        __gmp_binary_xor::eval (x.MPZ, a.MPZ, b.MPZ);
        return x;
    }
    
    Z inline operator << (const Z &a, int x) {
        Z n;
        __gmp_binary_lshift::eval (n.MPZ, a.MPZ, x);
        return n;
    }
    
    Z inline operator >> (const Z &a, int x) {
        Z n;
        __gmp_binary_rshift::eval (n.MPZ, a.MPZ, x);
        return n;
    }
    
    Z inline &operator <<= (Z &a, int x) {
        __gmp_binary_lshift::eval (a.MPZ, a.MPZ, x);
        return a;
    }
    
    Z inline &operator >>= (Z &a, int x) {
        __gmp_binary_rshift::eval (a.MPZ, a.MPZ, x);
        return a;
    }

    N inline operator + (const N &a, const N &b) {
        return N {a.Value + b.Value};
    }

    N inline operator - (const N &a, const N &b) {
        return N {a.Value < b.Value ? 0 : a.Value - b.Value};
    }

    N inline operator * (const N &a, const N &b) {
        return N {a.Value * b.Value};
    }

    Z inline operator + (const N &a, const Z &b) {
        return a.Value + b;
    }

    Z inline operator - (const N &a, const Z &b) {
        return a.Value - b;
    }

    Z inline operator * (const N &a, const Z &b) {
        return a.Value * b;
    }

    Z inline operator + (const Z &a, const N &b) {
        return a + b.Value;
    }

    Z inline operator - (const Z &a, const N &b) {
        return a - b.Value;
    }

    Z inline operator * (const Z &a, const N &b) {
        return a * b.Value;
    }

    Z inline operator - (const N &a) {
        return -a.Value;
    }

    // bit operations
    N inline operator | (const N &a, const N &b) {
        return N {a.Value | b.Value};
    }

    N inline operator & (const N &a, const N &b) {
        return N {a.Value & b.Value};
    }

    N inline operator ^ (const N &a, const N &b) {
        return N {a.Value ^ b.Value};
    }

    // divmodd by
    N inline operator / (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return N {def::divmod<Z> {} (a.Value, nonzero {b.Value}).Quotient};
    }

    Z inline operator / (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z> {} (a, nonzero {b.Value}).Quotient;
    }

    N inline operator / (const N &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return N {def::divmod<Z> {} (a.Value, nonzero {Z (b)}).Quotient};
    }

    // mod
    N inline operator % (const N &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<N> {} (a, nonzero {b}).Remainder;
    }

    N inline operator % (const Z &a, const N &b) {
        if (b == 0) throw division_by_zero {};
        return def::divmod<Z> {} (a, nonzero {b.Value}).Remainder;
    }

    uint64 inline operator % (const N &a, uint64 b) {
        if (b == 0) throw division_by_zero {};
        return uint64 (def::divmod<Z> {} (a.Value, nonzero {Z (b)}).Remainder);
    }

    // bit shift, which really just means
    // powers of two.
    N inline operator << (const N &a, int b) {
        return N {a.Value << b};
    }

    N inline operator >> (const N &a, int b) {
        return N {a.Value >> b};
    }

    // pre increment
    N inline &operator ++ (N &a) {
        ++a.Value;
        return a;
    }

    N inline &operator -- (N &a) {
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
        auto b = a;
        --a;
        return b;
    }

    Z inline &operator += (Z &a, const N &b) {
        a += b.Value;
        return a;
    }

    Z inline &operator -= (Z &a, const N &b) {
        return a -= b.Value;
    }

    Z inline &operator *= (Z &a, const N &b) {
        a *= b.Value;
        return a;
    }

    N inline &operator &= (N &a, const N &b) {
        a.Value &= b.Value;
        return a;
    }

    N inline &operator |= (N &a, const N &b) {
        a.Value |= b.Value;
        return a;
    }

    N inline &operator ^= (N &a, uint64 b) {
        a.Value ^= b;
        return a;
    }

    N inline &operator /= (N &a, const N &b) {
        a.Value /= b.Value;
        return a;
    }

    N inline &operator /= (N &a, uint64 b) {
        a.Value /= b;
        return a;
    }

    N inline &operator %= (N &a, const N &b) {
        a.Value = a.Value % b;
        return a;
    }

    N inline &operator <<= (N &a, int i) {
        a.Value <<= i;
        return a;
    }

    N inline &operator >>= (N &a, int i) {
        a.Value >>= i;
        return a;
    }

    bool inline operator == (const uint64 &u, const N &n) {
        return Z (u) == n.Value;
    }

    std::weak_ordering inline operator <=> (const uint64 &u, const N &n) {
        return Z (u) <=> n.Value;
    }

    std::weak_ordering inline operator <=> (const int64 &i, const Z &n) {
        return Z (i) <=> n;
    }
}

namespace data::math::def {

    N inline abs<Z>::operator () (const Z &z) {
        return N {z < 0 ? -z : z};
    }

    N inline abs<N>::operator () (const N &n) {
        return n;
    }

    division<N, N> inline divmod<N, N>::operator () (const N &a, const nonzero<N> &b) {
        return number::natural_divmod (a, b.Value);
    }

    division<Z, N> inline divmod<Z, N>::operator () (const Z &a, const nonzero<N> &b) {
        return number::integer_natural_divmod (a, b.Value);
    }

    division<Z, N> inline divmod<Z, Z>::operator () (const Z &a, const nonzero<Z> &b) {
        return number::integer_divmod<number::EUCLIDIAN_ALWAYS_POSITIVE> (a, b.Value);
    }

    N inline div_2<N>::operator () (const N &a) {
        return bit_div_2_positive_mod (a);
    }

    Z inline div_2<Z>::operator () (const Z &a) {
        return bit_div_2_positive_mod (a);
    }

    N inline mod_2<N>::operator () (const N &a) {
        return bit_mod_2_positive_mod (a);
    }

    Z inline mod_2<Z>::operator () (const Z &a) {
        return bit_mod_2_positive_mod (a);
    }

}

namespace data::math::number {

    Z inline increment<Z>::operator () (const Z &z) {
        auto n = z;
        return ++n;
    }

    Z inline decrement<Z>::operator () (const Z &z) {
        auto n = z;
        return --n;
    }

    nonzero<N> inline increment<N>::operator () (const N &n) {
        nonzero<N> x {n};
        ++x.Value;
        return x;
    }

    N inline decrement<N>::operator () (const nonzero<N> &n) {
        auto x = n.Value;
        return --x;
    }

    N inline decrement<N>::operator () (const N &n) {
        if (n == 0) return n;
        auto x = n;
        return --x;
    }
}

namespace data::math::number {

    N inline &operator += (N &n, const N &z) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N inline &operator -= (N &n, const N &z) {
        if (n <= z) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N inline &operator *= (N &n, const N &z) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, z.Value.MPZ);
        return n;
    }

    N inline &operator += (N &n, const Z &z) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N inline &operator -= (N &n, const Z &z) {
        if (n <= z) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N inline &operator *= (N &n, const Z &z) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, z.MPZ);
        return n;
    }

    N inline &operator += (N &n, uint64 u) {
        __gmp_binary_plus::eval (n.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return n;
    }

    N inline &operator -= (N &n, uint64 u) {
        if (n <= u) n = 0;
        else __gmp_binary_minus::eval (n.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return n;
    }

    N inline &operator *= (N &n, uint64 u) {
        __gmp_binary_multiplies::eval (n.Value.MPZ, n.Value.MPZ, (long unsigned int) (u));
        return n;
    }

    inline N::operator dec_uint () const {
        return encoding::decimal::write (*this);
    }

    inline N::operator dec_int () const {
        return encoding::signed_decimal::write (this->Value);
    }

    inline Z::operator dec_int () const {
        return encoding::signed_decimal::write (*this);
    }

    template <neg n, hex_case zz> inline N::operator encoding::hexidecimal::integer<n, zz> () const {
        return encoding::hexidecimal::write<n, zz> (this->Value);
    }

    template <hex_case zz> inline Z::operator encoding::hexidecimal::integer<neg::twos, zz> () const {
        return encoding::hexidecimal::write<neg::twos, zz> (*this);
    }

    template <hex_case zz> inline Z::operator encoding::hexidecimal::integer<neg::BC, zz> () const {
        return encoding::hexidecimal::write<neg::BC, zz> (*this);
    }

    template <neg n, hex_case zz> inline Z::Z (const hex::integer<n, zz> &u) : Z {} {
        // takes care of negative zero.
        if (is_zero (u)) return;

        // takes care of neg::nones.
        if (is_positive (u)) *this = Z::read (string_view (u));

        // the number is negative and n is either twos or BC.
        // Z (string_view) takes dec numbers with a possible
        // minus sign, or a positive hex number. This whole
        // constructor is problematic now.

        // for neg::twos, we bit negate the string, read it, add one, and negate the result.
        else if constexpr (n == neg::twos)
            *this = -(Z::read (string_view (~u)) + 1);

        // for neg::BC, we negate the string, read it, and negate the result.
        else *this = -Z::read (string_view (-u));
    }
}

namespace data::math::def {

    division<dec_uint, dec_uint> inline divmod<dec_uint, dec_uint>::operator () (const dec_uint &v, const nonzero<dec_uint> &z) {
        auto d = divmod<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {
            encoding::decimal::write (d.Quotient),
            encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divmod<dec_int, dec_uint>::operator () (const dec_int &v, const nonzero<dec_uint> &z) {
        auto d = divmod<Z, N> {} (Z (v), nonzero<N> {N (z.Value)});
        return {
            encoding::signed_decimal::write (d.Quotient),
            encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divmod<dec_int, dec_int>::operator () (const dec_int &v, const nonzero<dec_int> &z) {
        auto d = divmod<Z, Z> {} (Z {v}, nonzero<Z> {Z {z.Value}});
        return {
            encoding::signed_decimal::write (d.Quotient),
            encoding::decimal::write (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::uint<zz>, hex::uint<zz>> inline
    divmod<hex::uint<zz>, hex::uint<zz>>::operator ()
    (const hex::uint<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divmod<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {
            encoding::hexidecimal::write<zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::uint<zz>> inline
    divmod<hex::int2<zz>, hex::int2<zz>>::operator ()
    (const hex::int2<zz> &v, const nonzero<hex::int2<zz>> &z) {
        auto d = divmod<Z> {} (Z::read (v), nonzero<Z> {Z::read (z.Value)});
        return {
            encoding::hexidecimal::write<neg::twos, zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::uint<zz>> inline
    divmod<hex::int2<zz>, hex::uint<zz>>::operator ()
    (const hex::int2<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divmod<Z, N> {} (Z::read (v), nonzero<N> {N {Z::read (z.Value)}});
        return {
            encoding::hexidecimal::write<neg::twos, zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::intBC<zz>, hex::intBC<zz>> inline
    divmod<hex::intBC<zz>, hex::intBC<zz>>::operator ()
    (const hex::intBC<zz> &v, const nonzero<hex::intBC<zz>> &z) {
        auto d = data::divmod (Z_bytes_BC<endian::big> (v), nonzero {Z_bytes_BC<endian::big> (z.Value)});
        return {
            encoding::hexidecimal::write<zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <endian::order r, neg c, std::unsigned_integral word>
    number::Z_bytes<r, c, word> inline convert<number::Z_bytes<r, c, word>, Z>::operator () (const Z &z) const {
        return z.operator number::Z_bytes<r, c, word> ();
    }
}


#endif
