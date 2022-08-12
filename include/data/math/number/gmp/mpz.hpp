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
#include <data/math/division.hpp>
#include <data/math/number/complement.hpp>
#include <data/encoding/hex.hpp>

namespace data::math::number::GMP {
    
    typedef mp_limb_t gmp_uint;
    typedef mp_limb_signed_t gmp_int;
    
    const __mpz_struct MPZInvalid = __mpz_struct{0, 0, nullptr};
    
    inline bool equal(const __mpz_struct& a, const __mpz_struct& b) {
        return a._mp_alloc == b._mp_alloc && a._mp_size == b._mp_size && a._mp_d == b._mp_d;
    }
    
    inline uint32 size(const __mpz_struct& a) {
        return a._mp_alloc;
    }
    
    inline bool valid(const __mpz_struct& mpz) {
        return mpz._mp_d != nullptr;
    }
    
    inline math::sign sign(const __mpz_struct& mpz) {
        return !valid(mpz) ? zero : math::sign{mpz_cmp_si(&mpz, 0)};
    }
    
    struct N;
    struct Z;
    
    bool operator==(const N&, const N&);
    
    std::weak_ordering operator<=>(const N&, const N&);
    
    bool operator==(const Z&, const Z&);
    
    std::weak_ordering operator<=>(const Z&, const Z&);
    
    bool operator==(const Z&, const N&);
    
    std::weak_ordering operator<=>(const Z&, const N&);
    
    bool operator==(const Z&, int64);
    
    std::weak_ordering operator<=>(const Z&, int64);
    
    bool operator==(const N&, uint64);
    
    std::weak_ordering operator<=>(const N&, int64);
    
    Z operator-(const N&);
    Z operator-(const Z&);
    
    N operator+(const N&, const N&);
    N operator-(const N&, const N&);
    N operator*(const N&, const N&);
    
    Z operator+(const Z&, const Z&);
    Z operator-(const Z&, const Z&);
    Z operator*(const Z&, const Z&);
    
    Z operator+(const N&, const Z&);
    Z operator-(const N&, const Z&);
    Z operator*(const N&, const Z&);
    
    Z operator+(const Z&, const N&);
    Z operator-(const Z&, const N&);
    Z operator*(const Z&, const N&);
    
    N operator+(const N&, uint64);
    N operator-(const N&, uint64);
    N operator*(const N&, uint64);
    
    Z operator+(const Z&, int64);
    Z operator-(const Z&, int64);
    Z operator*(const Z&, int64);
    
    N operator+(uint64, const N&);
    N operator-(uint64, const N&);
    N operator*(uint64, const N&);
    
    Z operator+(int64, const Z&);
    Z operator-(int64, const Z&);
    Z operator*(int64, const Z&);
    
    // exponential 
    N operator^(const N&, const N&);
    Z operator^(const Z&, const N&);
    
    N operator^(const N&, uint64);
    Z operator^(const Z&, uint64);
    
    // divided by
    N operator/(const N&, const N&);
    Z operator/(const Z&, const Z&);
    Z operator/(const Z&, const N&);
    
    N operator/(const N&, uint64);
    Z operator/(const Z&, int64);
    
    // mod
    N operator%(const N&, const N&);
    N operator%(const Z&, const N&);
    
    uint64 operator%(const N&, uint64);
    uint64 operator%(const Z&, uint64);
    
    // bit operations 
    N operator|(const N&, const N&);
    N operator&(const N&, const N&);
    
    Z operator|(const Z&, const Z&);
    Z operator&(const Z&, const Z&);
    
    Z operator|(const N&, const Z&);
    Z operator&(const N&, const Z&);
    
    Z operator|(const Z&, const N&);
    Z operator&(const Z&, const N&);
    
    // bit shift, which really just means 
    // powers of two. 
    N operator<<(const N&, int);
    N operator>>(const N&, int);
    
    Z operator<<(const Z&, int);
    Z operator>>(const Z&, int);
    
    // pre increment
    N &operator++(N&);
    N &operator--(N&);
        
    Z &operator++(Z&);
    Z &operator--(Z&);
    
    // post increment
    N operator++(N&, int);
    N operator--(N&, int);
        
    Z operator++(Z&, int);
    Z operator--(Z&, int);
    
    std::ostream& operator<<(std::ostream& o, const N& n);
    std::ostream& operator<<(std::ostream& o, const Z& n);
    
}

namespace data::math {
    using N = number::GMP::N;
    using Z = number::GMP::Z;
    
    template <> struct abs<N> { 
        N operator()(const N&);
    };
    
    template <> struct abs<Z> { 
        N operator()(const Z&);
    };
    
    template <> struct commutative<plus<N>, N> {};
    template <> struct associative<plus<N>, N> {};
    template <> struct commutative<times<N>, N> {};
    template <> struct associative<times<N>, N> {};
    
    template <> struct commutative<plus<Z>, Z> {};
    template <> struct associative<plus<Z>, Z> {};
    template <> struct commutative<times<Z>, Z> {};
    template <> struct associative<times<Z>, Z> {};
    
    template <> struct identity<plus<N>, N> {
        N operator()();
    };
    
    template <> struct identity<plus<Z>, Z> {
        Z operator()();
    };
    
    template <> struct identity<times<N>, N> {
        N operator()();
    };
    
    template <> struct identity<times<Z>, Z> {
        Z operator()();
    };
    
    template <> struct inverse<plus<Z>, Z> {
        Z operator()(const Z &a, const Z &b);
    };
    
}

namespace data::math::number {
    
    bool is_zero(const N&);
    bool is_zero(const Z&);
    
    bool is_positive(const N&);
    bool is_positive(const Z&);
    
    bool is_negative(const N&);
    bool is_negative(const Z&);
    
}

namespace data {
    
    math::sign sign(const math::N &x);
    math::sign sign(const math::Z &x);
    
    math::N square(const math::N &n);
    math::N square(const math::Z &z);
    
    math::N increment(const math::N&);
    math::N decrement(const math::N&);
    
    math::Z increment(const math::Z&);
    math::Z decrement(const math::Z&);
    
}

namespace data::encoding::decimal {
    struct string;
    string write(const math::N&);
    
    std::ostream &write(std::ostream &, const math::N &);
    
}

namespace data::encoding::hexidecimal {
    template <math::number::complement, hex_case> struct integer;
    
    template <hex_case zz> integer<math::number::nones, zz> write(const math::N&);
    template <math::number::complement n, hex_case zz> integer<n, zz> write(const math::Z&);
    
    std::ostream &write(std::ostream &, const math::N &, hex::letter_case = hex::lower);
    std::ostream &write(std::ostream &, const math::Z &, hex::letter_case = hex::lower, math::number::complement = math::number::ones);
    
}

namespace data::encoding::signed_decimal {
    struct string;
    string write(const math::Z&);
    
    std::ostream &write(std::ostream &, const math::Z &);
    
}

namespace data::math::number::GMP {
    
    // modify number in place. 
    N &operator+=(N&, const N&);
    N &operator-=(N&, const N&);
    N &operator*=(N&, const N&);
    
    Z &operator+=(Z&, const Z&);
    Z &operator-=(Z&, const Z&);
    Z &operator*=(Z&, const Z&);
    
    Z &operator+=(N&, const Z&);
    Z &operator-=(N&, const Z&);
    Z &operator*=(N&, const Z&);
    
    Z &operator+=(Z&, const N&);
    Z &operator-=(Z&, const N&);
    Z &operator*=(Z&, const N&);
    
    N &operator+=(N&, uint64);
    N &operator-=(N&, uint64);
    N &operator*=(N&, uint64);
    
    Z &operator+=(Z&, int64);
    Z &operator-=(Z&, int64);
    Z &operator*=(Z&, int64);
    
    N &operator&=(N&, const N&);
    N &operator|=(N&, const N&);
    
    Z &operator&=(Z&, const Z&);
    Z &operator|=(Z&, const Z&);
    
    Z &operator&=(N&, const Z&);
    Z &operator|=(N&, const Z&);
    
    Z &operator&=(Z&, const N&);
    Z &operator|=(Z&, const N&);
    
    N &operator^=(N&, const N&);
    Z &operator^=(Z&, const N&);
    
    N &operator^=(N&, uint64);
    Z &operator^=(Z&, uint64);
    
    N &operator/=(N&, const N&);
    Z &operator/=(Z&, const Z&);
    
    N &operator/=(N&, uint64);
    Z &operator/=(Z&, int64);
    
    N &operator<<=(N&, int);
    N &operator>>=(N&, int);
    
    Z &operator<<=(Z&, int);
    Z &operator>>=(Z&, int);
    
}

namespace data::math::number {
    
    bool inline is_zero(const N &x);
    bool inline is_zero(const Z &x);
    
    bool inline is_negative(const N&);
    bool inline is_negative(const Z &z);
    
    bool inline is_positive(const N &x);
    bool inline is_positive(const Z &z);
    
}

#undef __GMP_DEFINE_UNARY_FUNCTION
#undef __GMP_DEFINE_UNARY_TYPE_FUNCTION
#undef __GMP_DEFINE_BINARY_FUNCTION
#undef __GMPP_DEFINE_BINARY_FUNCTION
#undef __GMPNN_DEFINE_BINARY_FUNCTION
#undef __GMPNS_DEFINE_BINARY_FUNCTION
#undef __GMPN_DEFINE_BINARY_FUNCTION
#undef __GMPND_DEFINE_BINARY_FUNCTION

#include <data/encoding/endian/endian.hpp>
#include <data/math/number/bytes/bytes.hpp>

#endif
