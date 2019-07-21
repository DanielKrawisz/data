// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/mpz.hpp>
#include <gmp/gmpxx.h>

namespace data {
    
    namespace math {
    
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

}
