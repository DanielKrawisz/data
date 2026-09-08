// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <compare>
#include <string>

#include <gmp.h>
#include <gmpxx.h>

#include <data/sign.hpp>
#include <data/abs.hpp>
#include <data/arithmetic.hpp>
#include <data/divmod.hpp>
#include <data/increment.hpp>
#include <data/math/algebra.hpp>

#include <data/arithmetic.hpp>

#include <data/encoding/hex.hpp>

#include <data/math/number/types.hpp>

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
}

#endif
