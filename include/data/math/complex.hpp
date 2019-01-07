#ifndef DATA_MATH_COMPLEX_HPP
#define DATA_MATH_COMPLEX_HPP

#include "field.hpp"

namespace data {
    
    namespace math {
    
        template <typename R>
        struct complex {
            static const field<R> r1{};
            static const field<complex> r2{};
            
            R Re;
            R Im;
            
            static const complex I = {0, 1};
            
            complex() : Re{0}, Im{0} {}
            complex(R r) : Re{r}, Im{0} {}
            complex(R r, R i) : Re{r}, Im{i} {}
            
            complex& operator=(complex x) {
                Re = x.Re;
                Im = x.Im;
                return *this;
            }
            
            R norm() const {
                return Re * Re + Im * Im;
            }
            
            complex conjugate() const {
                return {Re, -Im};
            }
            
            complex operator~() const {
                return conjugate();
            }
            
            complex operator+(R x) const {
                return {Re + x, Im};
            }
            
            complex operator+(complex x) const {
                return {Re + x.Re, Im + x.Im};
            }
            
            complex operator-() const {
                return {-Re, -Im};
            }
            
            complex operator-(R x) const {
                return {Re - x, Im};
            }
            
            complex operator-(complex x) const {
                return {Re - x.Re, Im - x.Im};
            }
            
            complex operator*(R x) const {
                return {Re * x, Im * x};
            }
            
            complex operator*(complex x) const {
                return {Re * x.Real - Im * x.Im, Re * x.Im + Im * x.Re};
            }
            
            complex operator/(R x) const {
                return {Re / x, Im / x};
            }
            
            complex inverse() const {
                return conjugate() / norm();
            }
            
            complex operator/(complex x) const {
                return (*this) * x.inverse();
            }
        };
    
    }

}

#endif
