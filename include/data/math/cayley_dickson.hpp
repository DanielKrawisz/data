// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CAYLEY_DICKSON
#define DATA_MATH_CAYLEY_DICKSON

#include <data/math/abs.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>
#include <data/math/field.hpp>
#include <data/math/algebra.hpp>

namespace data::math {
    
    template <typename q, typename nda>
    requires ordered<q> && field<q> && normed_division_algebra<q, nda>
    struct cayley_dickson {
        
        nda Re;
        nda Im;
        
        cayley_dickson() : Re{0}, Im{0} {}
        cayley_dickson(const nda& re, const nda& im) : Re{re}, Im{im} {}
        cayley_dickson(const q& x) : Re{x}, Im{0} {}
        
        cayley_dickson operator~() const;
        
        cayley_dickson operator+(const cayley_dickson& x) const;
        
        cayley_dickson operator-() const;
        
        cayley_dickson operator-(const cayley_dickson& x) const;
        
        cayley_dickson operator*(const cayley_dickson& x) const;
        
        cayley_dickson inverse() const;
        
        cayley_dickson operator/(const cayley_dickson& x) const;
        
    };
    
    template <typename q, typename nda> struct conjugate<cayley_dickson<q, nda>> {
        cayley_dickson<q, nda> operator()(const cayley_dickson<q, nda>& x) {
            return cayley_dickson<q, nda>{data::conjugate(x.Re), -x.Im};
        }
    };
    
    template <typename q, typename nda> struct inner<cayley_dickson<q, nda>> {
        cayley_dickson<q, nda> operator()(const cayley_dickson<q, nda>& a, const cayley_dickson<q, nda>& b) {
            return a * ~b;
        }
    };
    
    template <typename q, typename nda> struct re<cayley_dickson<q, nda>> {
        q operator()(const cayley_dickson<nda, q>& x) {
            return data::re(x.Re);
        }
    };
    
    template <typename q, typename nda> struct quadrance<cayley_dickson<q, nda>> {
        q operator()(const cayley_dickson<q, nda>& x) {
            return data::re(inner<cayley_dickson<q, nda>>{}(x, x));
        }
    };
    
    template <typename q, typename nda>
    struct inverse<plus<cayley_dickson<q, nda>>, cayley_dickson<q, nda>> {
        cayley_dickson<q, nda> operator()(const cayley_dickson<q, nda> &a, const cayley_dickson<q, nda> &b) {
            return b - a;
        }
    };
    
}

namespace data::math::linear {
    
    template <typename q, typename nda> 
    struct dimensions<q, cayley_dickson<q, nda>> {
        constexpr static dimension value = 2 * dimensions<q, nda>::value;
    };
    
}

namespace data::math {
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::operator~() const {
        return data::conjugate(*this);
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::operator+(const cayley_dickson& x) const {
        return {Re + x.Re, Im + x.Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::operator-() const {
        return {-Re, -Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::operator-(const cayley_dickson& x) const {
        return {Re - x.Re, Im - x.Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::operator*(const cayley_dickson& x) const {
        return {Re * x.Re - x.Im * ~Im, ~Re * x.Im + x.Re * Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::inverse() const {
        return data::conjugate(*this) / data::quadrance(*this);
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> cayley_dickson<q, nda>::operator/(const cayley_dickson& x) const {
        return operator*(x.inverse());
    }
    
}

#endif
