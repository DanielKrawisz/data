// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ_HPP
#define DATA_MATH_NUMBER_GMP_MPZ_HPP

#include <gmpxx.h>
#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/natural.hpp>
#include "mpz.hpp"

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace GMP {
                
                const __mpf_struct MPFInvalid = __mpf_struct{0, 0, 0, nullptr};
                
                inline bool equal(const __mpf_struct& a, const __mpf_struct& b) {
                    return a._mp_prec == b._mp_prec && a._mp_size == b._mp_size && a._mp_exp == b._mp_exp && a._mp_d == b._mp_d;
                }
                
                inline bool valid(const __mpf_struct& mpz) {
                    return mpz._mp_d != nullptr;
                }
                
                inline number::sign sign(const __mpf_struct& mpf) {
                    return !valid(mpf) ? zero : mpf._mp_size < 0 ? negative : positive;
                }
                
                void swap(__mpf_struct& a, __mpf_struct& b) {
                    __mpf_struct MPF_temp = a;
                    a = b;
                    b = MPF_temp;
                }
                
                struct mpf {
                    __mpf_struct MPF;
                    
                    mpf() : MPF{MPFInvalid} {}
                    
                    bool valid() const {
                        return GMP::valid(MPF);
                    }
                    
                    virtual ~mpf() {
                        if (valid()) mpf_clear(&MPF);
                    }
                    
                    mpf(gmp_uint n) {
                        mpf_init_set_u(&MPF, n);
                    }
                    
                    mpf(gmp_int n) {
                        mpz_init_set_s(&MPF, n);
                    }
                    
                    mpf(uint) {
                        throw 0;
                    }
                    
                    mpf(double) {
                        throw 0;
                    }
                    
                    mpf(const __mpf_struct& n) {
                        mpf_init(&MPF);
                        mpf_set(&MPF, &n);
                    }
                    
                    mpf(__mpf_struct&& n) {
                        swap(MPF, n);
                    }
                    
                    mpf(const mpf& n) {
                        mpf_init(&MPF);
                        mpf_set(&MPF, &n.MPF);
                    }
                    
                    mpf(mpf&& n) {
                        swap(MPF, n.MPF);
                    }
                    
                    mpf& operator=(mpf& n) {
                        mpf_set(&MPF, &n.MPF);
                        return *this;
                    }
                    
                    number::sign sign() const {
                        return GMP::sign(MPF);
                    }
                    
                    bool operator==(const gmp_uint) const;
                    
                    bool operator==(const mpf&) const;
                    
                    bool operator<(const gmp_uint) const;
                    
                    bool operator<(const mpf&) const;
                    
                    bool operator>(const gmp_uint) const;
                    
                    bool operator>(const mpf&) const;
                    
                    bool operator<=(const gmp_uint) const;
                    
                    bool operator<=(const mpf&) const;
                    
                    bool operator>=(const gmp_uint) const;
                    
                    bool operator>=(const mpf&) const;
                };
                
            }
            
        }
    
    }

}

#endif
