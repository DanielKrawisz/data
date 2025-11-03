// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CAYLEY_DICKSON
#define DATA_MATH_CAYLEY_DICKSON

#include <data/norm.hpp>
#include <data/math/nonzero.hpp>
#include <data/math/field.hpp>
#include <data/math/algebra/algebra.hpp>
#include <data/math/linear/inner.hpp>

namespace data::math {

    template <cayley_dickson_algebra nda> struct cayley_dickson;

    template <cayley_dickson_algebra nda>
    bool operator == (const cayley_dickson<nda> &, const cayley_dickson<nda> &);

    // we only have division if we are a field
    template <typename nda> requires field<cayley_dickson<nda>>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &, const nonzero<cayley_dickson<nda>> &);

    template <cayley_dickson_algebra nda>
    struct cayley_dickson {
        
        nda Even;
        nda Odd;
        
        cayley_dickson () : Even {0}, Odd {0} {}
        cayley_dickson (const nda &re, const nda &im) : Even {re}, Odd {im} {}


        template <typename NDA> requires ImplicitlyConvertible<NDA, nda>
        cayley_dickson (NDA n): Even {n}, Odd {} {}
        
        // conjugate
        cayley_dickson operator ~ () const;
        
        cayley_dickson operator + (const cayley_dickson &x) const;
        
        cayley_dickson operator - () const;
        
        cayley_dickson operator - (const cayley_dickson &x) const;
        
        cayley_dickson operator * (const cayley_dickson &x) const;
        
    };
}

namespace data::math::def {

    template <typename nda>
    struct inner<cayley_dickson<nda>> {
        auto operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b);
    };

    template <cayley_dickson_algebra nda>
    struct inverse<plus<cayley_dickson<nda>>, cayley_dickson<nda>> {
        cayley_dickson<nda> operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
            return b - a;
        }
    };

    template <typename nda> requires Real<nda> || Complex<nda> || Quaternionic<nda>
    struct inverse<times<cayley_dickson<nda>>, cayley_dickson<nda>> {
        nonzero<cayley_dickson<nda>> operator () (const nonzero<cayley_dickson<nda>> &z) {
            return nonzero<cayley_dickson<nda>> {~(z.Value) / data::quadrance (z.Value)};
        }
    };

    template <typename nda> struct ev<cayley_dickson<nda>> {
        nda operator () (const cayley_dickson<nda> &x) {
            return x.Even;
        }
    };

    template <typename nda> struct od<cayley_dickson<nda>> {
        nda operator () (const cayley_dickson<nda> &x) {
            return x.Odd;
        }
    };
}

namespace data::math::linear {
    
    template <typename q, typename nda> 
    struct dimensions<q, cayley_dickson<nda>> {
        static constexpr dimension value = dimensions<q, nda>::value * 2;
    };
    
}

namespace data::math {
    
    template <cayley_dickson_algebra nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator ~ () const {
        return conjugate (*this);
    }
    
    template <cayley_dickson_algebra nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator + (const cayley_dickson &x) const {
        return {Even + x.Even, Odd + x.Odd};
    }
    
    template <cayley_dickson_algebra nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator - () const {
        return {-Even, -Odd};
    }
    
    template <cayley_dickson_algebra nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator - (const cayley_dickson &x) const {
        return {Even - x.Even, Odd - x.Odd};
    }
    
    template <cayley_dickson_algebra nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator * (const cayley_dickson &x) const {
        return {Even * x.Even - x.Odd * conjugate (Odd), conjugate (Even) * x.Odd + x.Even * Odd};
    }

    template <cayley_dickson_algebra nda>
    cayley_dickson<nda> inline operator / (const cayley_dickson<nda> &a, const nonzero<cayley_dickson<nda>> &b) {
        return a * def::inverse<def::times<cayley_dickson<nda>>, cayley_dickson<nda>> {} (b).Value;
    }

    template <cayley_dickson_algebra nda>
    bool inline operator == (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
        return a.Even == b.Even && a.Odd == b.Odd;
    }
    
}

#endif
