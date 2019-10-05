// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <gmp.h>
#include <gmp/gmpxx.h>

#undef __GMP_DEFINE_UNARY_FUNCTION
#undef __GMP_DEFINE_UNARY_TYPE_FUNCTION
#undef __GMP_DEFINE_BINARY_FUNCTION
#undef __GMPP_DEFINE_BINARY_FUNCTION
#undef __GMPNN_DEFINE_BINARY_FUNCTION
#undef __GMPNS_DEFINE_BINARY_FUNCTION
#undef __GMPN_DEFINE_BINARY_FUNCTION
#undef __GMPND_DEFINE_BINARY_FUNCTION

#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/natural.hpp>
#include <data/io/unimplemented.hpp>

#include <data/io/unimplemented.hpp>

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
    
    inline void swap(__mpz_struct &a, __mpz_struct &b) {
        __mpz_struct MPZ_temp = a;
        a = b;
        b = MPZ_temp;
    }
    
    __mpz_struct read_decimal(const string&);
    
    __mpz_struct read_hexidecimal(const string&);
    
    inline __mpz_struct read_string(const string& s) {
        __mpz_struct x = read_decimal(s);
        if (valid(x)) return x;
        return read_hexidecimal(s);
    }
    
}

#endif
