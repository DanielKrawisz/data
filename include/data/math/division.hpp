// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/math/nonnegative.hpp>

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
        
    template <typename N, typename R = N> struct division {
        N Quotient;
        R Remainder;
            
        bool valid() const {
            return valid(Quotient) && valid(Remainder);
        }
        
        division(N q, R r) : Quotient{q}, Remainder{r} {}
        division() : Quotient{}, Remainder{} {}
        
        bool operator==(const division& d) const {
            return Quotient == d.Quotient && Remainder == d.Remainder;
        }
        
        bool operator!=(const division& d) const {
            return !(*this == d);
        }
    };
    
    template <typename dividend, typename divisor = dividend> struct divide;

    template <typename N>
    inline std::ostream& operator<<(std::ostream& o, const data::math::division<N> x) {
        return o << "division{Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }
    
}

namespace data {
    template <typename dividend, typename divisor = dividend> 
    auto inline divide(const dividend& a, const math::nonzero<divisor>& b) -> decltype(math::divide<dividend, divisor>{}(a, b)) {
        return math::divide<dividend, divisor>{}(a, b);
    }
    
    template <typename dividend, typename divisor = dividend> 
    bool inline divides(const dividend& a, divisor& b) {
        return b == 0 ? true : math::divide<dividend, divisor>{}(a, b).Remainder == 0;
    }
}

#endif
