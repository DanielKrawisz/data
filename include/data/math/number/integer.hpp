// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/math/number/natural.hpp>

namespace data::math::number::integer {

    template <typename Z, typename N = decltype (data::abs (std::declval<Z> ()))>
    division<Z, N> divide (const Z& Dividend, const Z& Divisor) {
        N divisor = data::abs (Divisor);
        division<N> d{natural::divide<N> (data::abs (Dividend), divisor)};
        
        if (Dividend < 0) {
            if (Divisor < 0) return {Z (d.Quotient + 1), divisor - d.Remainder};
            
            return {-(d.Quotient + 1), divisor - d.Remainder};
        }
        
        if (Divisor < 0) {
            return {-d.Quotient, d.Remainder};
        }
        
        return {Z (d.Quotient), d.Remainder};
    }

}

#endif
