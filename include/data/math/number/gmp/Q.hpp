#ifndef DATA_MATH_NUMBER_GMP_Q_HPP
#define DATA_MATH_NUMBER_GMP_Q_HPP

#include <data/math/number/gmp/Z.hpp>
#include <data/math/number/gmp/mpq.hpp>
#include <data/math/field.hpp>

namespace data {
    
    namespace number {
        
        namespace gmp {
            
            struct Q final : public mpq {
                Q() : mpq() {}
                
                Q(const N& n) : mpq((const __mpz_struct&)(n.MPZ), 1) {}
                
                Q(const Z& z) : mpq((const __mpz_struct&)(z.MPZ), 1) {}
                
                Q(const Q& q) : mpq(q.MPQ) {}
                
                Q(Q&& q) {
                    swap(mpq::MPQ, q.MPQ);
                }
                
                Q& operator=(const Q& q) {
                    mpq_set(&MPQ, &q.MPQ);
                    return *this;
                }
                
                Q& operator=(const Q&& q) {
                    mpq_set(&MPQ, &q.MPQ);
                    return *this;
                }
                
                bool operator==(const Q&&) const;
                
                bool operator<(const Q&&) const;
                
                bool operator>(const Q&&) const;
                
                bool operator<=(const Q&&) const;
                
                bool operator>=(const Q&&) const;
                
                Q operator+(const Q&&) const;
                
                Q& operator+=(const Q&&);
                
                Q operator-(const Q&&) const;
                
                Q& operator-=(const Q&&);
                
                Q operator*(const Q&&) const;
                
                Q& operator*=(const Q&&);
                
                Q operator^(uint) const;
                
                Q& operator^=(uint);
                
                Q operator/(const Q&&) const;
                
                Q operator/(const Q& q) const {
                    return operator/((const Q&&)(q));
                }
                
                Q& operator/=(const Q q) {
                    Q z = this->operator/(q);
                    return this->operator=(z);
                }
                
                Q& operator/=(const Q& q) {
                    Q z = this->operator/(q);
                    return this->operator=(z);
                }
            };
              
            constexpr static math::field<Q> is_field{};
            
        }
        
    }

}

#endif
