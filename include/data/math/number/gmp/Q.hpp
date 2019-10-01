// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Q
#define DATA_MATH_NUMBER_GMP_Q

#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/mpq.hpp>
#include <data/math/field.hpp>
#include <data/math/nonnegative.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace gmp {
                
                struct Q final : public mpq {
                    Q() : mpq() {}
                    
                    Q(const N& n) : mpq((const __mpz_struct&)(n.Value.MPZ), 1) {}
                    
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
                    
                    nonnegative<Q> abs() const;
                };
                
                const static math::field<Q> is_field{};
                
            }
            
        }
        
        inline nonnegative<number::gmp::Q> abs(number::gmp::Q q) {
            return q.abs();
        }
        
        nonnegative<number::gmp::Q> square(number::gmp::Q q);
    
    }

}

#endif
