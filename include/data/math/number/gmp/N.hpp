#ifndef DATA_MATH_NUMBER_GMP_N_HPP
#define DATA_MATH_NUMBER_GMP_N_HPP

#include <data/math/number/natural.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <limits>

namespace data {
    
    namespace number {
        
        namespace gmp {
            
            struct Z;
            struct Q;
            
            struct N final : public mpz {
                N() : mpz() {}
                
                N(uint n) {
                    if (n <= std::numeric_limits<uint>::max()) mpz_init_set_ui(&MPZ, n);
                    throw 0; // TODO
                }
                
                N(const N& n) : mpz(n.MPZ) {}
                
                N(N&& n) {
                    swap(MPZ, n.MPZ);
                }
                
                N& operator=(N& n) {
                    mpz_set(&MPZ, &n.MPZ);
                    return *this;
                }
                
                bool operator==(const N&&) const;
                
                bool operator==(gmp_uint) const;
                
                bool operator<(const N&&) const;
                
                bool operator<(gmp_uint) const;
                
                bool operator>(const N&&) const;
                
                bool operator>(gmp_uint) const;
                
                bool operator<=(const N&&) const;
                
                bool operator<=(gmp_uint) const;
                
                bool operator>=(const N&&) const;
                
                bool operator>=(gmp_uint) const;
                
                N successor() const;
                
                N operator+(const N&&) const;
                
                N operator+(gmp_uint) const;
                
                N& operator+=(const N&&);
                
                N& operator+=(gmp_uint);
                
                N operator*(const N&&) const;
                
                N operator*(gmp_uint) const;
                
                N& operator*=(const N&&);
                
                N& operator*=(gmp_uint);
                
                N operator^(gmp_uint) const;
                
                N& operator^=(gmp_uint);
                
                math::number::division<N> divide(const N&&) const;
                
                math::number::division<N> divide(const N& n) const {
                    return divide((const N&&)(n));
                }
                
                bool operator|(const N&& n) const {
                    return divide(n).Quotient == 0;
                }
                
                N operator/(const N&& n) const {
                    return divide(n).Quotient;
                }
                
                N operator%(const N&& n) const {
                    return divide(n).Remainder;
                }
                
                N operator/(const N& n) const {
                    return divide(n).Quotient;
                }
                
                N operator%(const N& n) const {
                    return divide(n).Remainder;
                }
                
                N& operator/=(const N&& n) {
                    N q = operator/(n);
                    return operator=(q);
                }
                
                N& operator%=(const N&& n) {
                    N r = operator%(n);
                    return operator=(r);
                }
                
                friend struct Z;
                friend struct Q;
            };
              
            constexpr static math::number::natural<N> is_natural{};
            
        }
        
    }

}

#endif
