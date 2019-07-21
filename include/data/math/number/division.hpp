// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/data.hpp>

namespace data {
    
    namespace math {
        struct division_by_zero : std::exception {};
        
        namespace number {
            
            template <typename N> struct division {
                N Quotient;
                N Remainder;
                    
                bool valid() const {
                    return valid(Quotient) && valid(Remainder);
                }
                
                division(N q, N r) : Quotient{q}, Remainder{r} {}
                division() : Quotient{}, Remainder{} {}
                
                static division divide(const N Dividend, const N Divisor);
            };
        
        }
    
    }

}

#endif
