// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/division.hpp>

namespace data::math::number {
    
    template <typename N>
    division<N> division<N>::divide(const N Dividend, const N Divisor) {
        if (Divisor == 0) throw division_by_zero{};
        N pow = 1;
        N exp = Divisor;
        N remainder = Dividend;
        N quotient = 0;
        while (exp <= remainder) {
            exp<<=1;
            pow<<=1;
        } 
        while (pow > 0) {
            while (exp > remainder) {
                exp>>=1;
                pow>>=1;
            }
            quotient += pow;
            remainder -= exp;
        }
        return {quotient, remainder};
    }

}
