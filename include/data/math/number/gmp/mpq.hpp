#ifndef DATA_MATH_NUMBER_GMP_MPQ_HPP
#define DATA_MATH_NUMBER_GMP_MPQ_HPP

#include <data/math/number/gmp/mpz.hpp>

namespace data {
    
    namespace number {
        
        namespace gmp {
            
            const __mpq_struct MPQInvalid = __mpq_struct{MPZInvalid, MPZInvalid};
            
            inline bool equal(const __mpq_struct& a, const __mpq_struct& b) {
                return a._mp_num == b._mp_num && a._mp_den == b._mp_den;
            }
            
            inline bool valid(__mpq_struct& mpq) {
                return valid(mpq._mp_den) && valid(mpq._mp_num);
            }
            
            inline number::sign sign(const __mpq_struct& mpq) {
                return !valid(mpq) ? zero : number::sign(sign(mpq._mp_num) * sign(mpq._mp_den));
            }
            
            void swap(__mpq_struct& a, __mpq_struct& b) {
                __mpq_struct MPQ_temp = a;
                a = b;
                b = MPQ_temp;
            }
            
            struct mpq {
                __mpq_struct MPQ;
                
                void init() {
                    mpq_init(&MPQ);
                }
                
                mpq() : MPQ{MPQInvalid} {}
                
                bool valid() const {
                    return gmp::valid(MPQ);
                }
                
                virtual ~mpq() {
                    if (valid()) mpq_clear(&MPQ);
                }
                
                mpq(const __mpq_struct& q) {
                    mpq_set(&MPQ, &q);
                }
                
                mpq(__mpq_struct&& q) {
                    swap(MPQ, q);
                }
                
                mpq(const mpq& q) : mpq(q.MPQ) {}
                
                mpq(mpq&& q) {
                    swap(MPQ, q.MPQ);
                }
                
                mpq(gmp_uint n, gmp_uint d) {
                    mpq_init(&MPQ);
                    mpq_set_ui(&MPQ, n, d);
                }
                
                mpq(gmp_uint n) : mpq(n, 1) {}
                
                mpq(gmp_int n, gmp_uint d) {
                    mpq_init(&MPQ);
                    mpq_set_si(&MPQ, n, d);
                }
                
                mpq(gmp_int n) : mpq(n, 1) {}
                
                mpq(const __mpz_struct& num, gmp_uint den) {
                    mpz_set(&MPQ._mp_num, &num);
                    mpz_init_set_ui(&MPQ._mp_den, den);
                }
                
                mpq(const __mpz_struct& n) : mpq(n, 1) {}
                
                mpq(__mpz_struct&& num, gmp_uint den) {
                    swap(MPQ._mp_num, num);
                    mpz_init_set_ui(&MPQ._mp_den, den);
                }
                
                mpq(__mpz_struct&& num) {
                    swap(MPQ._mp_num, num);
                    mpz_init_set_ui(&MPQ._mp_den, 1);
                }
                
                mpq(const __mpz_struct& num, const __mpz_struct& den) {
                    mpz_set(&MPQ._mp_num, &num);
                    mpz_set(&MPQ._mp_den, &den);
                }
                
                mpq(__mpz_struct&& num, __mpz_struct&& den) {
                    swap(MPQ._mp_num, num);
                    swap(MPQ._mp_den, den);
                }
                
                mpq& operator=(const mpq& q) {
                    mpq_set(&MPQ, &q.MPQ);
                    return *this;
                }
                
                number::sign sign() {
                    return gmp::sign(MPQ);
                }
                
                bool operator==(const mpq&&) const;
                
                bool operator<(const mpq&&) const;
                
                bool operator>(const mpq&&) const;
                
                bool operator<=(const mpq&&) const;
                
                bool operator>=(const mpq&&) const;
            };
            
        }
        
    }

}

#endif
