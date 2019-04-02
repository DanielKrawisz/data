#ifndef DATA_MATH_CAYLEY_DICKSON_HPP
#define DATA_MATH_CAYLEY_DICKSON_HPP

#include <data/math/linear/inner.hpp>
#include <data/math/algebra.hpp>

namespace data {
    
    namespace math {
    
        template <typename cd, typename r>
        struct cayley_dickson {
            constexpr static linear::space<cd, r> r1{};
            constexpr static linear::space<cayley_dickson<cd, r>, r> r2{};
            constexpr static normed_division_algebra<cayley_dickson<cd, r>, r> r3{};
            
            cd Re;
            cd Im;
            
            cayley_dickson() : Re{0}, Im{0} {}
            cayley_dickson(cd re, cd im) : Re{re}, Im{im} {}
            
            cayley_dickson conjugate() const {
                return {Re.conjugate(), -Im};
            }
            
            cayley_dickson operator~() const {
                return conjugate();
            }
            
            nonnegative<r> norm() const {
                return square(Re) + square(Im);
            }
            
            cayley_dickson operator+(cayley_dickson x) const {
                return {Re + x.Re, Im + x.Im};
            }
            
            cayley_dickson operator-() const {
                return {-Re, -Im};
            }
            
            cayley_dickson operator-(cayley_dickson x) const {
                return {Re - x.Re, Im - x.Im};
            }
            
            cayley_dickson operator*(cayley_dickson x) const {
                return {Re * x.Re - x.Im * ~Im, ~Re * x.Im + x.Re * Im};
            }
            
            cayley_dickson inverse() const {
                return conjugate() / norm();
            }
            
            cayley_dickson operator/(cayley_dickson x) const {
                return operator*(x.inverse());
            }
        };
        
        template <typename cd, typename r>
        inline nonnegative<r> norm(cayley_dickson<cd, r> z) {
            z.norm();
        }
    
    }

}

#endif
