// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/types.hpp>

namespace data {
    
    namespace math {
        struct division_by_zero : std::exception {
            static std::string error;
            const char* what() const noexcept final override {
                return error.c_str();
            }
        };
        
        std::string division_by_zero::error = "division by zero";
        
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
