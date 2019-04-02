#ifndef DATA_MATH_REAL_LINE_HPP
#define DATA_MATH_REAL_LINE_HPP

#include <data/data.hpp>
#include <data/math/sign.hpp>

namespace data {
    
    namespace math {
        
        struct real_line {
            
            template <typename R>
            struct point {
                R Real;
                bool Infinite;
                
                point(const R r) : Real{r}, Infinite{false} {}
                point(const long int x) : Real{x}, Infinite{false} {}
                point(const double x) : Real{x}, Infinite{false} {}
                
                bool valid() const {
                    return valid(Real);
                }
                
                bool infinite() const {
                    return Infinite;
                }
                
                bool finite() const {
                    return !Infinite;
                }
                
                bool zero() const {
                    return finite() && Real == R{0};
                }
                
                sign sign() const {
                    return Real.sign();
                }
                
                bool operator==(const point& p) const {
                    if (Infinite) return p.Infinite && (sign() == p.sign());
                    return p.finite() && Real == p.Real;
                }
                
                point operator+(const point& p) const {
                    if (Infinite || p.Infinite) return infinity();
                    return Real + p.Complex;
                }
                
                point operator-() const {
                    return {-Real, Infinite};
                }
                
                point operator*(const point& p) const {
                    return {Real * p.Real, Infinite || p.Infinite};
                }
                
                point operator^(const int32_t n) const {
                    return {Real ^ n, Infinite};
                };
                
                static point infinity() {
                    return point();
                }
            private:
                point() : Real{1}, Infinite{true} {};
            };
        };
    
    }
    
}

#endif 
