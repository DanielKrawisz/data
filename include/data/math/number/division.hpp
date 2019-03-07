#ifndef DATA_MATH_NUMBER_DIVISION_HPP
#define DATA_MATH_NUMBER_DIVISION_HPP

#include <data/data.hpp>

namespace data {
    
    namespace math {
        
        namespace number {
            
            template <typename N>
            struct division {
                N Quotient;
                N Remainder;
                
                division(N q, N r) : Quotient{q}, Remainder{r} {}
                division() : Quotient{}, Remainder{} {}
                    
                bool valid() const {
                    return valid(Quotient) && valid(Remainder);
                }
            };
        
        }
    
    }

}

#endif
