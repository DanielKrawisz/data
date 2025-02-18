// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/math/nonnegative.hpp>
#include <iostream> // required on windows

namespace data::math {
    struct division_by_zero : std::logic_error {
        division_by_zero () : std::logic_error {"division by zero"} {}
    };
        
    template <typename N, typename R = N> struct division {
        N Quotient;
        R Remainder;
            
        bool valid () const {
            return data::valid (Quotient) && data::valid (Remainder);
        }
        
        division (const N &q, const R &r) : Quotient {q}, Remainder {r} {}
        division () : Quotient {}, Remainder {} {}
        
        bool operator == (const division &d) const {
            return Quotient == d.Quotient && Remainder == d.Remainder;
        }
        
        bool operator != (const division &d) const {
            return !(*this == d);
        }
    };

    template <typename dividend, typename divisor = dividend> struct divide;

    template <>
    struct divide<int, int> {
        division<int, unsigned int> operator () (int x, nonzero<int> y) {
            return {x / y, static_cast<unsigned int> (x % y)};
        }
    };

    template <>
    struct divide<unsigned int, unsigned int> {
        division<unsigned int, unsigned int> operator () (unsigned int x, nonzero<unsigned int> y) {
            return {x / y, x % y};
        }
    };

    template <>
    struct divide<long unsigned int, long unsigned int> {
        division<long unsigned int, long unsigned int> operator () (long unsigned int x, nonzero<long unsigned int> y) {
            return {x / y, x % y};
        }
    };
    
}

namespace data {
    template <typename dividend, typename divisor = dividend> 
    auto inline divide (const dividend &a, const math::nonzero<divisor> &b) -> decltype (math::divide<dividend, divisor> {} (a, b)) {
        return math::divide<dividend, divisor> {} (a, b);
    }
    
    template <typename dividend, typename divisor = dividend> 
    bool inline divides (const dividend &a, const math::nonzero<divisor> &b) {
        return b == 0 ? true : math::divide<dividend, divisor> {} (a, b).Remainder == 0;
    }
}

namespace data::math {

    template <typename N, typename R>
    std::ostream inline &operator << (std::ostream &o, const data::math::division<N, R> x) {
        return o << "division {Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }

}

#endif
