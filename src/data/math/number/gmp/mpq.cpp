#include <data/math/number/gmp/mpq.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace number {
        
        namespace gmp {
            bool mpq::operator==(const mpq&& n) const {
                return __gmp_binary_equal::eval(&MPQ, &n.MPQ);
            }
            
            bool mpq::operator<(const mpq&& n) const {
                return __gmp_binary_less::eval(&MPQ, &n.MPQ);
            }
            
            bool mpq::operator>(const mpq&& n) const {
                return __gmp_binary_greater::eval(&MPQ, &n.MPQ);
            }
            
            bool mpq::operator<=(const mpq&& n) const {
                return !__gmp_binary_greater::eval(&MPQ, &n.MPQ);
            }
            
            bool mpq::operator>=(const mpq&& n) const {
                return !__gmp_binary_less::eval(&MPQ, &n.MPQ);
            }

        }
        
    }

}

