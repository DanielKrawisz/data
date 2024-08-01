// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <compare>
#include <gmp.h>
#include <gmp/gmpxx.h>

#include <data/math/sign.hpp>
#include <data/math/abs.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>
#include <data/math/number/integer.hpp>
#include <data/arithmetic/complement.hpp>
#include <data/encoding/hex.hpp>

#include <string>

namespace data::math::number::GMP {
    
    typedef mp_limb_t gmp_uint;
    typedef mp_limb_signed_t gmp_int;
    
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
    
    math::signature inline sign (const __mpz_struct &mpz) {
        return !valid (mpz) ? zero : math::signature {mpz_cmp_si (&mpz, 0)};
    }

    struct Z;

    bool operator == (const Z &, const Z &);

    std::weak_ordering operator <=> (const Z &, const Z &);

    bool operator == (const Z &, int64);

    std::weak_ordering operator <=> (const Z &, int64);

    Z operator - (const Z &);

    Z operator + (const Z &, const Z &);
    Z operator - (const Z &, const Z &);
    Z operator * (const Z &, const Z &);

    Z operator + (const Z &, int64);
    Z operator - (const Z &, int64);
    Z operator * (const Z &, int64);

    Z operator + (int64, const Z &);
    Z operator - (int64, const Z &);
    Z operator * (int64, const Z &);

    Z operator / (const Z &, const Z &);
    Z operator / (const Z &, int64);

    Z operator | (const Z &, const Z &);
    Z operator & (const Z &, const Z &);

    uint64 operator % (const Z &, uint64);

    Z &operator ++ (Z &);
    Z &operator -- (Z &);

    Z operator ++ (Z &, int);
    Z operator -- (Z &, int);

    Z operator << (const Z &, int);
    Z operator >> (const Z &, int);

    Z operator ^ (const Z &, uint32 n);

    std::ostream &operator << (std::ostream &o, const Z &n);
}

namespace data::math {
    using Z = number::GMP::Z;

    template <> struct times<Z> {
        Z operator () (const Z &a, const Z &b);
        nonzero<Z> operator () (const nonzero<Z> &a, const nonzero<Z> &b);
    };

    template <> struct commutative<plus<Z>, Z> {};
    template <> struct associative<plus<Z>, Z> {};
    template <> struct commutative<times<Z>, Z> {};
    template <> struct associative<times<Z>, Z> {};

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

    template <> struct sign<Z> {
        math::signature operator () (const Z &x);
    };
}

namespace data::math::number {

    template <> struct increment<Z> {
        Z operator () (const Z &);
    };

    template <> struct decrement<Z> {
        Z operator () (const Z &);
    };
}

namespace data::math::number::GMP {

    Z &operator += (Z &, const Z &);
    Z &operator -= (Z &, const Z &);
    Z &operator *= (Z &, const Z &);
    Z &operator /= (Z &, const Z &);

    Z &operator += (Z &, int64);
    Z &operator -= (Z &, int64);
    Z &operator *= (Z &, int64);
    Z &operator /= (Z &, int64);

    Z &operator &= (Z &, const Z &);
    Z &operator |= (Z &, const Z &);

    Z &operator <<= (Z &, int);
    Z &operator >>= (Z &, int);

    Z &operator ^= (Z &, uint32 n);

    struct Z {
        mpz_t MPZ;

        Z ();

        virtual ~Z ();

        Z (gmp_int n);

        static Z read (string_view x);

        //Z (const std::string &x);
        Z (string_view);

        Z (const Z &n);

        Z (Z &&n);

        Z &operator = (const Z &n);

        Z &operator = (Z &&n);

        size_t size () const;

        using index = uint32;

        mp_limb_t &operator [] (index i);

        const mp_limb_t &operator [] (index i) const;

        mp_limb_t *begin ();

        mp_limb_t *end ();

        const mp_limb_t *begin () const;

        const mp_limb_t *end () const;

        explicit operator int64 () const;
        explicit operator double () const;

        static Z make (uint64);
    };
}

namespace data::math::number {

    // implementation of naturals given an implementation of integers.
    template <> struct N<GMP::Z> {
        using Z = GMP::Z;
        Z Value;

        N () : Value {} {}

        N (uint64 u);

        N (const Z &z) : Value {z} {}
        //explicit N (Z &&z) : Value {z} {}

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

    };

    N<GMP::Z> operator + (const N<GMP::Z> &, uint64);
    N<GMP::Z> operator - (const N<GMP::Z> &, uint64);
    N<GMP::Z> operator * (const N<GMP::Z> &, uint64);

    N<GMP::Z> operator + (uint64, const N<GMP::Z> &);
    N<GMP::Z> operator - (uint64, const N<GMP::Z> &);
    N<GMP::Z> operator * (uint64, const N<GMP::Z> &);

    N<GMP::Z> &operator += (N<GMP::Z> &, const N<GMP::Z> &);
    N<GMP::Z> &operator -= (N<GMP::Z> &, const N<GMP::Z> &);
    N<GMP::Z> &operator *= (N<GMP::Z> &, const N<GMP::Z> &);

    N<GMP::Z> &operator += (N<GMP::Z> &, const GMP::Z &);
    N<GMP::Z> &operator -= (N<GMP::Z> &, const GMP::Z &);
    N<GMP::Z> &operator *= (N<GMP::Z> &, const GMP::Z &);

    N<GMP::Z> &operator += (N<GMP::Z> &, uint64);
    N<GMP::Z> &operator -= (N<GMP::Z> &, uint64);
    N<GMP::Z> &operator *= (N<GMP::Z> &, uint64);

    std::ostream &operator << (std::ostream &o, const N<GMP::Z> &n);

}

namespace data::math::number::GMP {

    using N = number::N<Z>;
}

namespace data::math {

    using N = number::GMP::N;
    
    template <> struct abs<Z> { 
        N operator () (const Z &);
    };
    
}

namespace data::encoding::decimal {
    struct string;
    string write (const math::N &);
    
    std::ostream &write (std::ostream &, const math::N &);
    
}

namespace data::encoding::hexidecimal {
    using complement = arithmetic::complement;
    template <complement, hex_case> struct integer;
    
    template <hex_case zz> integer<complement::nones, zz> write (const math::N &);
    template <complement n, hex_case zz> integer<n, zz> write (const math::Z &);

    std::ostream &write (std::ostream &, const math::N &, hex_case = hex_case::lower);
    std::ostream &write (std::ostream &, const math::Z &, hex_case = hex_case::lower, complement = complement::ones);
    
}

namespace data::encoding::signed_decimal {
    struct string;
    string write (const math::Z &);
    
    std::ostream &write (std::ostream &, const math::Z &);
}

namespace data::math::number::GMP {

    inline Z::Z () {
        mpz_init (MPZ);
    }

    inline Z::~Z () {
        mpz_clear (MPZ);
    }

    inline Z::Z (gmp_int n) : MPZ {} {
        mpz_init_set_si (MPZ, n);
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

    mp_limb_t inline &Z::operator [] (Z::index i) {
        if (static_cast<int> (i) >= MPZ[0]._mp_alloc) throw std::out_of_range {"Z"};
        return *(MPZ[0]._mp_d + i);
    }

    const mp_limb_t inline &Z::operator [] (Z::index i) const {
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

    Z inline operator ^ (const Z &a, uint32 n) {
        Z pow {};
        mpz_pow_ui (pow.MPZ, a.MPZ, n);
        return pow;
    }

    Z inline &operator ^= (Z &a, uint32 n) {
        mpz_pow_ui (a.MPZ, a.MPZ, n);
        return a;
    }

    bool inline operator == (const Z &a, const Z &b) {
        return a <=> b == 0;
    }

    bool inline operator == (const Z &a, int64 b) {
        return a <=> b == 0;
    }

    std::weak_ordering inline operator <=> (const Z &a, const Z &b) {
        auto cmp = mpz_cmp (a.MPZ, b.MPZ);
        return cmp < 0 ? std::weak_ordering::less :
            cmp > 0 ? std::weak_ordering::greater : std::weak_ordering::equivalent;
    }

    std::weak_ordering inline operator <=> (const Z &a, int64 b) {
        auto cmp = mpz_cmp_si (a.MPZ, b);
        return cmp < 0 ? std::weak_ordering::less :
            cmp > 0 ? std::weak_ordering::greater : std::weak_ordering::equivalent;
    }

    Z inline operator / (const Z &a, const Z& b) {
        if (b == 0) throw division_by_zero {};
        return math::divide<Z, Z> {} (a, nonzero {b}).Quotient;
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

}

namespace data::math {
    signature inline sign<Z>::operator () (const Z &z) {
        return z == 0 ? zero : z < 0 ? negative : positive;
    }

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
}

namespace data::math::number {
    inline N<GMP::Z>::N (uint64 n) : Value {} {
        mpz_init_set_ui (Value.MPZ, n);
    }
}

#undef __GMP_DEFINE_UNARY_FUNCTION
#undef __GMP_DEFINE_UNARY_TYPE_FUNCTION
#undef __GMP_DEFINE_BINARY_FUNCTION
#undef __GMPP_DEFINE_BINARY_FUNCTION
#undef __GMPNN_DEFINE_BINARY_FUNCTION
#undef __GMPNS_DEFINE_BINARY_FUNCTION
#undef __GMPN_DEFINE_BINARY_FUNCTION
#undef __GMPND_DEFINE_BINARY_FUNCTION

#endif
