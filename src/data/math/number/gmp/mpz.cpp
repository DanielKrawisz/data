#include <data/math/number/gmp/mpz.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace number {
        
        namespace gmp {
            bool mpz::operator==(gmp_uint n) const {
                return __gmp_binary_equal::eval(&MPZ, n);
            }
                
            bool mpz::operator==(mpz& n) const {
                return __gmp_binary_equal::eval(&MPZ, &n.MPZ);
            }
            
            bool mpz::operator<(gmp_uint n) const {
                return __gmp_binary_less::eval(&MPZ, n);
            }
                
            bool mpz::operator<(mpz& n) const {
                return __gmp_binary_less::eval(&MPZ, &n.MPZ);
            }
            
            bool mpz::operator>(gmp_uint n) const {
                return __gmp_binary_greater::eval(&MPZ, n);
            }
                
            bool mpz::operator>(mpz& n) const {
                return __gmp_binary_greater::eval(&MPZ, &n.MPZ);
            }
            
            bool mpz::operator<=(gmp_uint n) const {
                return !__gmp_binary_greater::eval(&MPZ, n);
            }
                
            bool mpz::operator<=(mpz& n) const {
                return !__gmp_binary_greater::eval(&MPZ, &n.MPZ);
            }
            
            bool mpz::operator>=(gmp_uint n) const {
                return !__gmp_binary_less::eval(&MPZ, n);
            }
                
            bool mpz::operator>=(mpz& n) const {
                return !__gmp_binary_less::eval(&MPZ, &n.MPZ);
            }

        }
        
    }

}
