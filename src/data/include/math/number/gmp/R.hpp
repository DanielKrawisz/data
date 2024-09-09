#ifndef DATA_MATH_NUMBER_GMP_R
#define DATA_MATH_NUMBER_GMP_R

#include <data/math/number/gmp/mpf.hpp>
#include <data/math/field.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace gmp {
                
                struct R final : public mpf {
                    R() : mpf() {}
                    
                    R(R&& q) {
                        swap(mpf::MPF, q.MPF);
                    }
                    
                    R& operator=(const R& q) {
                        mpf_set(&MPF, &q.MPF);
                        return *this;
                    }
                    
                    R& operator=(const R&& q) {
                        mpf_set(&MPF, &q.MPF);
                        return *this;
                    }
                    
                    bool operator==(const R&&) const;
                    
                    bool operator<(const R&&) const;
                    
                    bool operator>(const R&&) const;
                    
                    bool operator<=(const R&&) const;
                    
                    bool operator>=(const R&&) const;
                    
                    R operator+(const R&&) const;
                    
                    R& operator+=(const R&&);
                    
                    R operator-(const R&&) const;
                    
                    R& operator-=(const R&&);
                    
                    R operator*(const R&&) const;
                    
                    R& operator*=(const R&&);
                    
                    R operator^(uint) const;
                    
                    R& operator^=(uint);
                    
                    R operator/(const R&&) const;
                    
                    R operator/(const R& q) const {
                        return operator/((const R&&)(q));
                    }
                    
                    R& operator/=(const R q) {
                        R z = this->operator/(q);
                        return this->operator=(z);
                    }
                    
                    R& operator/=(const R& q) {
                        R z = this->operator/(q);
                        return this->operator=(z);
                    }
                    
                    nonnegative<R> abs() const;

                };
                
                const static math::field<R> is_field{};
                
            }
            
        }
        
        inline nonnegative<number::gmp::R> abs(number::gmp::R r) {
            return r.abs();
        }
        
        inline nonnegative<number::gmp::R> norm(number::gmp::R r) {
            return r.abs();
        }
        
        inline number::gmp::R conjugate(number::gmp::R r) {
            return r;
        }
        
        nonnegative<number::gmp::R> square(number::gmp::R r);
        
    }

}

#endif
