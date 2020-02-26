// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/types.hpp>

namespace data::math {
    struct division_by_zero : std::exception {
        static const char* error() {
            static char Error[] {"division by zero"};
            return Error;
        }
        
        const char* what() const noexcept final override {
            return error();
        }
    };
        
    template <typename N> struct division {
        N Quotient;
        N Remainder;
            
        bool valid() const {
            return valid(Quotient) && valid(Remainder);
        }
        
        division(N q, N r) : Quotient{q}, Remainder{r} {}
        division() : Quotient{}, Remainder{} {}
    };
    
}

#endif
