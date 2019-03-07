#ifndef DATA_MATH_RIEMANN_SPHERE_HPP
#define DATA_MATH_RIEMANN_SPHERE_HPP

#include <data/types.hpp>
#include <data/math/complex.hpp>

namespace data {
    
    namespace math {
        
        template <typename R>
        struct riemann_sphere {
            struct point {
                complex<R> Complex;
                bool Infinite;
                
                point(const complex<R> c) : Complex{c}, Infinite{false} {}
                point(const long int x) : Complex{x}, Infinite{false} {}
                point(const double x) : Complex{x}, Infinite{false} {}
                
                bool valid() const {
                    return Complex.valid();
                }
                
                bool infinite() const {
                    return Infinite;
                }
                
                bool finite() const {
                    return !Infinite;
                }
                
                bool zero() const {
                    return finite() && Complex == complex<R>{0};
                }
                
                point inverse() {
                    if (zero()) return Infinity;
                    if (infinite()) return complex<R>{0};
                    return Complex.inverse();
                }
                
                bool operator==(const point& p) const {
                    if (Infinite) return p.Infinite;
                    return p.finite() && Complex == p.Complex;
                }
                
                point operator+(const point& p) const {
                    if (Infinite || p.Infinite) return Infinity;
                    return Complex + p.Complex;
                }
                
                point operator-() const {
                    if (Infinite) return Infinity;
                    return -Complex;
                }
                
                point operator-(const point& p) const {
                    if (Infinite || p.Infinite) return Infinity;
                    return Complex - p.Complex;
                }
                
                point operator*(const point& p) const {
                    if (Infinite || p.Infinite) return Infinity;
                    return Complex - p.Complex;
                }
                
                point operator/(const point& p) const {
                    return operator*(p.inverse());
                }
                
                point operator^(const int32_t n) const {
                    if (Infinite) return Infinity;
                    return Complex ^ n;
                };
            private:
                point() : Complex{}, Infinite{true} {};
            };
            
            static point Infinity{};
            
        };
    
    }
    
}

#endif 
