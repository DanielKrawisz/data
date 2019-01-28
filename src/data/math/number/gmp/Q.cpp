#include <data/math/number/gmp/Q.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace number {
        
        namespace gmp {
            bool Q::operator==(const Q&& q) const {
                return __gmp_binary_equal::eval(&MPQ, &q.MPQ);
            }
                
            bool Q::operator<(const Q&& q) const {
                return __gmp_binary_less::eval(&MPQ, &q.MPQ);
            }
                
            bool Q::operator>(const Q&& q) const {
                return __gmp_binary_greater::eval(&MPQ, &q.MPQ);
            }
                
            bool Q::operator<=(const Q&& q) const {
                return !__gmp_binary_greater::eval(&MPQ, &q.MPQ);
            }
                
            bool Q::operator>=(const Q&& q) const {
                return !__gmp_binary_less::eval(&MPQ, &q.MPQ);
            }
                
            Q Q::operator+(const Q&& q) const {
                Q sum{};
                __gmp_binary_plus::eval(&sum.MPQ, &MPQ, &q.MPQ);
                return sum;
            }
                
            Q& Q::operator+=(const Q&& q) {
                __gmp_binary_plus::eval(&MPQ, &MPQ, &q.MPQ);
                return *this;
            }
                
            Q Q::operator*(const Q&& q) const {
                Q prod{};
                __gmp_binary_multiplies::eval(&prod.MPQ, &MPQ, &q.MPQ);
                return prod;
            }
                
            Q& Q::operator*=(const Q&& q) {
                __gmp_binary_multiplies::eval(&MPQ, &MPQ, &q.MPQ);
                return *this;
            }
            
        }
        
    }
    
}
