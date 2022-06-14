// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/R.hpp>
#include <gmpxx.h>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace GMP {
                bool R::operator==(const R&& r) const {
                    return __gmp_binary_equal::eval(&MPF, &r.MPF);
                }
                    
                bool R::operator<(const R&& r) const {
                    return __gmp_binary_less::eval(&MPF, &r.MPF);
                }
                    
                bool R::operator>(const R&& r) const {
                    return __gmp_binary_greater::eval(&MPF, &r.MPF);
                }
                    
                bool R::operator<=(const R&& r) const {
                    return !__gmp_binary_greater::eval(&MPF, &r.MPF);
                }
                    
                bool R::operator>=(const R&& r) const {
                    return !__gmp_binary_less::eval(&MPF, &r.MPF);
                }
                    
                R R::operator+(const R&& r) const {
                    R sum{};
                    __gmp_binary_plus::eval(&sum.MPF, &MPF, &r.MPF);
                    return sum;
                }
                    
                R& R::operator+=(const R&& r) {
                    __gmp_binary_plus::eval(&MPF, &MPF, &r.MPF);
                    return *this;
                }
                    
                R R::operator*(const R&& r) const {
                    R prod{};
                    __gmp_binary_multiplies::eval(&prod.MPF, &MPF, &r.MPF);
                    return prod;
                }
                    
                R& R::operator*=(const R&& r) {
                    __gmp_binary_multiplies::eval(&MPF, &MPF, &r.MPF);
                    return *this;
                }
                
            }
            
        }
    
    }
    
}
