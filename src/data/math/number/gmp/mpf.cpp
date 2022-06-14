// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/mpf.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace GMP {
                bool mpf::operator==(const mpf& n) const {
                    return __gmp_binary_equal::eval(&MPF, &n.MPF);
                }
                
                bool mpf::operator<(const mpf& n) const {
                    return __gmp_binary_less::eval(&MPF, &n.MPF);
                }
                
                bool mpf::operator>(const mpf& n) const {
                    return __gmp_binary_greater::eval(&MPF, &n.MPF);
                }
                
                bool mpf::operator<=(const mpf& n) const {
                    return !__gmp_binary_greater::eval(&MPF, &n.MPF);
                }
                
                bool mpf::operator>=(const mpf& n) const {
                    return !__gmp_binary_less::eval(&MPF, &n.MPF);
                }

            }
            
        }
        
    }

}
