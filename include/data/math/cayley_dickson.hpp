// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CAYLEY_DICKSON
#define DATA_MATH_CAYLEY_DICKSON

#include <data/math/nonnegative.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>
#include <data/math/field.hpp>

namespace data::math {
    
    template <typename X> struct conjugate;
    
    template <typename R, typename X> struct quadrance;
    
    template <typename R, typename X> struct re;
    
    template <typename nda, typename q>
    struct cayley_dickson : interface::ordered<q>, interface::field<q> {
        // TODO use proper interfaces: 
        //   nda is a normed division algebra. 
        
        nda Re;
        nda Im;
        
        cayley_dickson() : Re{0}, Im{0} {}
        cayley_dickson(const nda& re, const nda& im) : Re{re}, Im{im} {}
        cayley_dickson(const q& x) : Re{x}, Im{0} {}
        
        cayley_dickson conjugate() const {
            return {math::conjugate<q>{}(Re), -Im};
        }
        
        cayley_dickson operator~() const {
            return conjugate();
        }
        
        q re() const {
            return math::re(Re);
        }
        
        cayley_dickson operator+(const cayley_dickson& x) const {
            return {Re + x.Re, Im + x.Im};
        }
        
        cayley_dickson operator-() const {
            return {-Re, -Im};
        }
        
        cayley_dickson operator-(const cayley_dickson& x) const {
            return {Re - x.Re, Im - x.Im};
        }
        
        cayley_dickson operator*(const cayley_dickson& x) const {
            return {Re * x.Re - x.Im * ~Im, ~Re * x.Im + x.Re * Im};
        }
        
    protected:
        nonnegative<q> quadrance() const {
            return operator*(conjugate()).real_part();
        }
        
        cayley_dickson inverse() const {
            return conjugate() / q(quadrance());
        }
        
        cayley_dickson operator/(const cayley_dickson& x) const {
            return operator*(x.inverse());
        }
    };
    
    template <typename nda, typename q> struct conjugate<cayley_dickson<nda, q>> {
        cayley_dickson<nda, q> operator()(const cayley_dickson<nda, q>& x) {
            return x.conjugate();
        }
    };
    
    template <typename nda, typename q> struct quadrance<q, cayley_dickson<nda, q>> {
        nonnegative<q> operator()(const cayley_dickson<nda, q>& x) {
            return x.quadrance();
        }
    };
    
    template <typename nda, typename q> struct re<q, cayley_dickson<nda, q>> {
        q operator()(const cayley_dickson<nda, q>& x) {
            return x.re();
        }
    };
    
}

#endif
