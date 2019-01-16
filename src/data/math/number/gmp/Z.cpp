#include <data/math/number/gmp/Z.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace number {
        
        namespace gmp {
            bool Z::operator==(const Z&& z) const {
                return __gmp_binary_equal::eval(&MPZ, &z.MPZ);
            }
                
            bool Z::operator<(const Z&& z) const {
                return __gmp_binary_less::eval(&MPZ, &z.MPZ);
            }
                
            bool Z::operator>(const Z&& z) const {
                return __gmp_binary_greater::eval(&MPZ, &z.MPZ);
            }
                
            bool Z::operator<=(const Z&& z) const {
                return !__gmp_binary_greater::eval(&MPZ, &z.MPZ);
            }
                
            bool Z::operator>=(const Z&& z) const {
                return !__gmp_binary_less::eval(&MPZ, &z.MPZ);
            }
                
            Z Z::operator+(const Z&& z) const {
                Z sum{};
                __gmp_binary_plus::eval(&sum.MPZ, &MPZ, &z.MPZ);
                return sum;
            }
                
            Z& Z::operator+=(const Z&& z) {
                __gmp_binary_plus::eval(&MPZ, &MPZ, &z.MPZ);
                return *this;
            }
                
            Z Z::operator*(const Z&& z) const {
                Z prod{};
                __gmp_binary_multiplies::eval(&prod.MPZ, &MPZ, &z.MPZ);
                return prod;
            }
                
            Z& Z::operator*=(const Z&& z) {
                __gmp_binary_multiplies::eval(&MPZ, &MPZ, &z.MPZ);
                return *this;
            }
                
            Z Z::operator^(uint n) const {
                Z pow{};
                mpz_pow_ui(&pow.MPZ, &MPZ, n);
                return pow;
            }
                
            Z& Z::operator^=(uint n) {
                mpz_pow_ui(&MPZ, &MPZ, n);
                return *this;
            }

            math::number::division<Z> Z::divide(Z& z) const {
                math::number::division<Z> qr{};
                mpz_cdiv_qr(&qr.Quotient.MPZ, &qr.Remainder.MPZ, &MPZ, &z.MPZ);
                return qr;
            }
            
        }
        
    }
    
}
