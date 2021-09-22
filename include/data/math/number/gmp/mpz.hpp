// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <gmp.h>
#include <gmp/gmpxx.h>
#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/natural.hpp>

namespace data::math::number::gmp {
    
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
        return !valid(mpz) ? zero : mpz._mp_size < 0 ? negative : positive;
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

#include <data/encoding/endian/endian.hpp>

namespace data::math::number {
    template <bool is_signed, endian::order o, size_t size> struct bounded;
    template <endian::order o, size_t size> struct bounded<false, o, size>;
    template <endian::order o, size_t size> struct bounded<true, o, size>;
    
    template <endian::order o> struct N_bytes;
    template <endian::order o> struct Z_bytes;
}

#endif
