#include <data/math/number/gmp/N.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace gmp {
                bool N::operator==(const N& n) const {
                    return __gmp_binary_equal::eval(&MPZ, &n.MPZ);
                }
                
                bool N::operator<(const N& n) const {
                    return __gmp_binary_less::eval(&MPZ, &n.MPZ);
                }
                
                bool N::operator>(const N& n) const {
                    return __gmp_binary_greater::eval(&MPZ, &n.MPZ);
                }
        
                bool N::operator<=(const N& n) const {
                    return !__gmp_binary_greater::eval(&MPZ, &n.MPZ);
                }
                
                bool N::operator>=(const N& n) const {
                    return !__gmp_binary_less::eval(&MPZ, &n.MPZ);
                }
                
                N N::operator+(const N& n) const {
                    N sum{};
                    __gmp_binary_plus::eval(&sum.MPZ, &MPZ, &n.MPZ);
                    return sum;
                }
                
                N N::operator+(gmp_uint n) const {
                    N sum{};
                    __gmp_binary_plus::eval(&sum.MPZ, &MPZ, n);
                    return sum;
                }
                
                N& N::operator+=(const N& n) {
                    __gmp_binary_plus::eval(&MPZ, &MPZ, &n.MPZ);
                    return *this;
                }
                
                N& N::operator+=(gmp_uint n) {
                    __gmp_binary_plus::eval(&MPZ, &MPZ, n);
                    return *this;
                }
                
                N N::successor() const {
                    return operator+(1);
                }
                
                N N::operator*(const N& n) const {
                    N prod{};
                    __gmp_binary_multiplies::eval(&prod.MPZ, &MPZ, &n.MPZ);
                    return prod;
                }
                
                N N::operator*(gmp_uint n) const {
                    N prod{};
                    __gmp_binary_multiplies::eval(&prod.MPZ, &MPZ, n);
                    return prod;
                }
                
                N& N::operator*=(const N& n) {
                    __gmp_binary_multiplies::eval(&MPZ, &MPZ, &n.MPZ);
                    return *this;
                }
                
                N& N::operator*=(gmp_uint n) {
                    __gmp_binary_multiplies::eval(&MPZ, &MPZ, n);
                    return *this;
                }
                
                N N::operator^(gmp_uint n) const {
                    N pow{};
                    mpz_pow_ui(&pow.MPZ, &MPZ, n);
                    return pow;
                }

                math::number::division<N> N::divide(const N& n) const {
                    math::number::division<N> qr{};
                    mpz_cdiv_qr(&qr.Quotient.MPZ, &qr.Remainder.MPZ, &MPZ, &n.MPZ);
                    return qr;
                }
                
            }
            
        }
    
    }
    
}
