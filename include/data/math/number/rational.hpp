// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_FRACTION
#define DATA_MATH_NUMBER_FRACTION

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/math/field.hpp>
#include <data/math/ordered.hpp>

namespace data::math::number {
            template <typename Q>
            struct rational : field<Q>, ordered<Q> {};
            
            template <typename N>
            struct positive {
                N Number;
                bool valid() const {
                    return Number > 0;
                }
            };
            
            template <typename Z, typename N>
            struct fraction {
                Z Numerator;
                positive<N> Denominator;
                
                fraction(Z n, positive<N> d);
                fraction() : Numerator{0}, Denominator{1} {}
                
                fraction(Z n) : Numerator{n}, Denominator{1} {}
                fraction(N n) : fraction{Z{n}} {}
                
                bool operator==(const fraction<Z, N>);
                bool operator!=(const fraction<Z, N>);
                bool operator<(const fraction<Z, N>);
                bool operator>(const fraction<Z, N>);
                bool operator<=(const fraction<Z, N>);
                bool operator>=(const fraction<Z, N>);
                
                fraction<Z, N> operator-(){
                    return {-Numerator, Denominator};
                }
                
                fraction<Z, N> operator+(const fraction<Z, N>);
                fraction<Z, N> operator-(const fraction<Z, N>);
                fraction<Z, N> operator*(const fraction<Z, N>);
                fraction<Z, N> operator/(const fraction<Z, N>);
            };
            
        }
    
    }

}

#endif
