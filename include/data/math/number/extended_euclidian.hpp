#ifndef DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN_HPP
#define DATA_MATH_NUMBER_EXTENDED_EUCLIDIAN_HPP

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/valid.hpp>

namespace data {
    
    namespace math {
        
        namespace number {
            
            template <typename Z>
            struct extended_euclidian {
                Z GCD;
                Z BezoutS;
                Z BezoutT;
                    
                bool valid() const {
                    return data::valid(GCD) && data::valid(BezoutS) && data::valid(BezoutT);
                }
                
                extended_euclidian(Z a, Z b, Z gcd, Z s, Z t) : GCD{gcd}, BezoutS{s}, BezoutT{s} {
                    if (!(a | gcd && b | gcd && gcd == a * s + b * t)) throw 0;
                }
                
            private:
                extended_euclidian(Z gcd, Z q, Z r) : GCD{gcd}, division<Z>{q, r} {}
                extended_euclidian() : GCD{}, division<Z>{} {} 
                
                struct sequence {
                    division <Z> Div;
                    Z S;
                    Z T;
                };
                
                // assume prev.Div.Remainder > current.Div.Remainder.
                // we can do that because it's a private function. 
                static sequence loop(const sequence prev, const sequence current) {
                    division<Z> div = prev.Div.Remainder / current.Div.Remainder;
                    if (div.Remainder == 0) return current;
                    return loop(current, {div, {prev.S - current.S * div.Quotient, prev.T - current.T * div.Quotient}});
                }
                
                static sequence init(const Z R0, const Z R1) {
                    return loop(sequence{{0, R0}, 1, 0}, sequence{{0, R1}, 0, 1});
                }
                
                static sequence pre(const Z Dividend, const Z Divisor) {
                    if (Dividend > Divisor) return init(Dividend, Divisor);
                    sequence euclidian = init(Divisor, Dividend);
                    return {euclidian.Div, euclidian.T, euclidian.S};
                }
                
            public: 
                static extended_euclidian algorithm(const Z Dividend, const Z Divisor) {
                    sequence euclidian = pre(Dividend, Divisor);
                    return {euclidian.Div.Quotient, euclidian.S, euclidian.T};
                }
            };
        
        }
    
    }

}

#endif
