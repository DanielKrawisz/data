// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <gmp.h>
#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/natural.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace gmp {
                
                typedef mp_limb_t gmp_uint;
                typedef mp_limb_signed_t gmp_int;
                
                const __mpz_struct MPZInvalid = __mpz_struct{0, 0, nullptr};
                
                inline bool equal(const __mpz_struct& a, const __mpz_struct& b) {
                    return a._mp_alloc == b._mp_alloc && a._mp_size == b._mp_size && a._mp_d == b._mp_d;
                }
                
                inline bool valid(const __mpz_struct& mpz) {
                    return mpz._mp_d != nullptr;
                }
                
                inline math::sign sign(const __mpz_struct& mpz) {
                    return !valid(mpz) ? zero : mpz._mp_size < 0 ? negative : positive;
                }
                
                void swap(__mpz_struct& a, __mpz_struct& b) {
                    __mpz_struct MPZ_temp = a;
                    a = b;
                    b = MPZ_temp;
                }
                
                struct mpz {
                    __mpz_struct MPZ;
                    
                    mpz() : MPZ{MPZInvalid} {}
                    
                    bool valid() const {
                        return gmp::valid(MPZ);
                    }
                    
                    virtual ~mpz() {
                        if (valid()) mpz_clear(&MPZ);
                    }
                    
                    mpz(gmp_uint n) {
                        mpz_init_set_ui(&MPZ, n);
                    }
                    
                    mpz(gmp_int n) {
                        mpz_init_set_si(&MPZ, n);
                    }
                    
                    mpz(uint32 n) {
                        throw 0;
                    }
                    
                    mpz(const __mpz_struct& n) {
                        mpz_init(&MPZ);
                        mpz_set(&MPZ, &n);
                    }
                    
                    mpz(__mpz_struct&& n) {
                        swap(MPZ, n);
                    }
                    
                    mpz(const mpz& n) {
                        mpz_init(&MPZ);
                        mpz_set(&MPZ, &n.MPZ);
                    }
                    
                    mpz(mpz&& n) {
                        swap(MPZ, n.MPZ);
                    }
                    
                    mpz& operator=(mpz& n) {
                        mpz_set(&MPZ, &n.MPZ);
                        return *this;
                    }
                    
                    math::sign sign() const {
                        return gmp::sign(MPZ);
                    }
                    
                    bool operator==(gmp_uint n) const;
                    
                    bool operator==(mpz& n) const;
                    
                    bool operator<(gmp_uint n) const;
                    
                    bool operator<(mpz& n) const;
                    
                    bool operator>(gmp_uint n) const;
                    
                    bool operator>(mpz& n) const;
                    
                    bool operator<=(gmp_uint n) const;
                    
                    bool operator<=(mpz& n) const;
                    
                    bool operator>=(gmp_uint n) const;
                    
                    bool operator>=(mpz& n) const;
                    
                    // TODO
                    bool operator<(uint32 n) const;
                    
                    bool operator>(uint32 n) const;
                    
                    bool operator<=(uint32 n) const;
                    
                    bool operator>=(uint32 n) const;
                };
                
            }
            
        }
    
    }

}

#endif
