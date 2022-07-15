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
#include <data/math/algebra/algebra.hpp>
#include <data/math/linear/inner.hpp>

namespace data::math {
    
    template <typename q, typename nda>
    concept conjugate_algebra = algebra<q, nda> && conjugable<nda>;

    template <typename q, typename nda> requires ordered<q> && conjugate_algebra<q, nda> struct cayley_dickson;

    template <typename q, typename nda> requires ordered<q> && conjugate_algebra<q, nda>
    bool operator == (const cayley_dickson<q, nda> &, const cayley_dickson<q, nda> &);

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    struct conjugate<cayley_dickson<q, nda>> {
        cayley_dickson<q, nda> operator () (const cayley_dickson<q, nda> &x);
    };

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    struct quadrance<cayley_dickson<q, nda>> {
        q operator () (const cayley_dickson<q, nda> &x);
    };

    template <typename q, typename nda>
    requires ordered<q> && normed_division_algebra<q, nda>
    cayley_dickson<q, nda> operator / (const cayley_dickson<q, nda> &, const nonzero<cayley_dickson<q, nda>> &);

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    struct cayley_dickson {
        
        nda Re;
        nda Im;
        
        cayley_dickson () : Re {0}, Im {0} {}
        cayley_dickson (const nda &re, const nda &im) : Re {re}, Im {im} {}
        cayley_dickson (const q &x) : Re {x}, Im {0} {}
        
        // conjugate
        cayley_dickson operator * () const;
        
        cayley_dickson operator + (const cayley_dickson &x) const;
        
        cayley_dickson operator - () const;
        
        cayley_dickson operator - (const cayley_dickson &x) const;
        
        cayley_dickson operator * (const cayley_dickson &x) const;
        
    };

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    struct inverse<plus<cayley_dickson<q, nda>>, cayley_dickson<q, nda>> {
        cayley_dickson<q, nda> operator () (const cayley_dickson<q, nda> &a, const cayley_dickson<q, nda> &b) {
            return b - a;
        }
    };

    template <typename q, typename nda>
    requires ordered<q> && normed_division_algebra<q, nda>
    struct inverse<times<cayley_dickson<q, nda>>, cayley_dickson<q, nda>> {
        nonzero<cayley_dickson<q, nda>> operator () (const nonzero<cayley_dickson<q, nda>> &z) {
            return nonzero<cayley_dickson<q, nda>> {*(z.Value) / data::quadrance (z.Value)};
        }
    };
    
    template <typename q, typename nda> struct re<cayley_dickson<q, nda>> {
        q operator () (const cayley_dickson<q, nda> &x) {
            return data::re (x.Re);
        }
    };
    
}

namespace data::math::linear {
    
    template <typename q, typename nda> 
    struct dimensions<q, cayley_dickson<q, nda>> {
        constexpr static dimension value = dimensions<q, nda>::value * 2;
    };

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    struct inner<q, cayley_dickson<q, nda>> {
        q operator () (const cayley_dickson<q, nda> &a, const cayley_dickson<q, nda> &b) {
            return inner<nda, nda> (a.Re, b.Re) - inner<nda, nda> (a.Im, *b.Im);
        }
    };
    
}

namespace data::math {

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    q inline quadrance<cayley_dickson<q, nda>>::operator () (const cayley_dickson<q, nda> &x) {
        return linear::inner<q, cayley_dickson<q, nda>> {} (x, x);
    }

    template <typename q, typename nda>
    requires ordered<q> && conjugate_algebra<q, nda>
    cayley_dickson<q, nda> conjugate<cayley_dickson<q, nda>>::operator () (const cayley_dickson<q, nda> &x) {
        return cayley_dickson<q, nda> {data::conjugate (x.Re), -x.Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> inline cayley_dickson<q, nda>::operator * () const {
        return data::conjugate (*this);
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> inline cayley_dickson<q, nda>::operator + (const cayley_dickson &x) const {
        return {Re + x.Re, Im + x.Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> inline cayley_dickson<q, nda>::operator - () const {
        return {-Re, -Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> inline cayley_dickson<q, nda>::operator - (const cayley_dickson &x) const {
        return {Re - x.Re, Im - x.Im};
    }
    
    template <typename q, typename nda> 
    cayley_dickson<q, nda> inline cayley_dickson<q, nda>::operator * (const cayley_dickson &x) const {
        return {Re * x.Re - x.Im * conjugate<nda> {} (Im), conjugate<nda> {} (Re) * x.Im + x.Re * Im};
    }

    template <typename q, typename nda>
    requires ordered<q> && normed_division_algebra<q, nda>
    cayley_dickson<q, nda> inline operator / (const cayley_dickson<q, nda> &a, const nonzero<cayley_dickson<q, nda>> &b) {
        return a * inverse<times<cayley_dickson<q, nda>>, cayley_dickson<q, nda>> {} (b).Value;
    }

    template <typename q, typename nda> requires ordered<q> && conjugate_algebra<q, nda>
    bool inline operator == (const cayley_dickson<q, nda> &a, const cayley_dickson<q, nda> &b) {
        return a.Re == b.Re && a.Im == b.Im;
    }
    
}

#endif
