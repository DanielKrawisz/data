// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/math/nonnegative.hpp>

namespace data::math {
    struct division_by_zero : std::logic_error {
        division_by_zero() : std::logic_error{"division by zero"} {}
    };
        
    template <typename N, typename R = N> struct division {
        N Quotient;
        R Remainder;
            
        bool valid() const {
            return data::valid(Quotient) && data::valid(Remainder);
        }
        
        division(const N &q, const R &r) : Quotient{q}, Remainder{r} {}
        division() : Quotient{}, Remainder{} {}
        
        bool operator==(const division& d) const {
            return Quotient == d.Quotient && Remainder == d.Remainder;
        }
        
        bool operator!=(const division& d) const {
            return !(*this == d);
        }
    };

    template <typename N>
    inline std::ostream& operator<<(std::ostream& o, const data::math::division<N> x) {
        return o << "division{Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }
    
}

#endif
