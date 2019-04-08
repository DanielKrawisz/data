#ifndef DATA_MATH_RIEMANN_SPHERE_HPP
#define DATA_MATH_RIEMANN_SPHERE_HPP

#include <data/data.hpp>
#include <data/math/complex.hpp>

namespace data {
    
    namespace math {
        
        struct riemann_sphere {
            
            template <typename R>
            struct point {
                complex<R> Complex;
                bool Infinite;
                
                point(const complex<R> c) : Complex{c}, Infinite{false} {}
                point(const long int x) : Complex{x}, Infinite{false} {}
                point(const double x) : Complex{x}, Infinite{false} {}
                
                bool valid() const {
                    return valid(Complex);
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
                    if (zero()) return infinity();
                    if (infinite()) return complex<R>{0};
                    return Complex.inverse();
                }
                
                bool operator==(const point& p) const {
                    if (Infinite) return p.Infinite;
                    return Complex == p.Complex;
                }
                
                point operator+(const point& p) const {
                    if (Infinite || p.Infinite) return infinity();
                    return Complex + p.Complex;
                }
                
                point operator-() const {
                    if (Infinite) return infinity();
                    return -Complex;
                }
                
                point operator-(const point& p) const {
                    if (Infinite || p.Infinite) return infinity();
                    return Complex - p.Complex;
                }
                
                point operator*(const point& p) const {
                    if (Infinite || p.Infinite) return infinity();
                    return Complex - p.Complex;
                }
                
                point operator/(const point& p) const {
                    return operator*(p.inverse());
                }
                
                point operator^(const int32_t n) const {
                    if (Infinite) return infinity();
                    return Complex ^ n;
                };
                
                static point infinity() {
                    return point();
                }
            private:
                point() : Complex{}, Infinite{true} {};
            };
            
            template <typename R>
            inline point<R> inverse(point<R> z) {
                return z.inverse();
            } 
            
            template <typename R>
            struct translation {
                point<R> Difference;
                
                translation(point<R> d) : Difference{d} {}
                
                point<R> operator()(point<R> z) {
                    return z + Difference;
                }
            };
            
            template <typename R>
            inline translation<R> translate(point<R> d) {
                return translation<R>(d);
            }
            
            template <typename R>
            struct scaling {
                R Scale;
                
                scaling(R x) : Scale{x} {}
                
                point<R> operator()(point<R> z) {
                    return z * Scale;
                }
            };
            
            template <typename R>
            inline scaling<R> scale(R x) {
                return scaling<R>(x);
            }
            
            template <typename R>
            struct rotation {
                point<R> Exp;
                
                rotation(R angle) : Exp{angle} {}
                
                point<R> operator()(point<R> z) {
                    return Exp * z;
                }
            };
            
            template <typename R>
            inline rotation<R> rotate(R angle) {
                return rotation<R>(angle);
            }
            
        };
    
    }
    
}

#endif 
