#include <data/math/number/gmp/mpf.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace number {
        
        namespace gmp {
            bool mpf::operator==(const mpf&& n) const {
                return __gmp_binary_equal::eval(&MPF, &n.MPF);
            }
            
            bool mpf::operator<(const mpf&& n) const {
                return __gmp_binary_less::eval(&MPF, &n.MPF);
            }
            
            bool mpf::operator>(const mpf&& n) const {
                return __gmp_binary_greater::eval(&MPF, &n.MPF);
            }
            
            bool mpf::operator<=(const mpf&& n) const {
                return !__gmp_binary_greater::eval(&MPF, &n.MPF);
            }
            
            bool mpf::operator>=(const mpf&& n) const {
                return !__gmp_binary_less::eval(&MPF, &n.MPF);
            }

        }
        
    }

}
