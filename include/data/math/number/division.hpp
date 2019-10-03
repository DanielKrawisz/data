// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/types.hpp>

namespace data {
    
    namespace math {
        struct division_by_zero : std::exception {
            static const char* error() {
                static char Error[] {"division by zero"};
                return Error;
            }
            
            const char* what() const noexcept final override {
                return error();
            }
        };
        
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
    
    }

}

#endif
