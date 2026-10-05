// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/norm.hpp>
#include <data/math/nonzero.hpp>
#include <data/math/field.hpp>
#include <data/math/algebra/algebra.hpp>
#include <data/math/linear/inner.hpp>

namespace data::math {

    template <cayley_dickson_algebra nda> struct cayley_dickson;

    template <cayley_dickson_algebra nda>
    bool operator == (const cayley_dickson<nda> &, const cayley_dickson<nda> &);

    template <cayley_dickson_algebra nda, ImplicitlyConvertible<nda> NDA>
    bool operator == (const cayley_dickson<nda> &, const NDA &);

    // we only have division if we are a field
    template <Field nda>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &, const cayley_dickson<nda> &);

    template <Field nda, ImplicitlyConvertible<nda> NDA>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &, const NDA &);

    template <cayley_dickson_algebra nda>
    struct cayley_dickson {
        
        nda Even;
        nda Odd;
        
        cayley_dickson () : Even (0), Odd (0) {}
        cayley_dickson (const nda &re, const nda &im) : Even (re), Odd (im) {}

        cayley_dickson (const nda &re): Even (re), Odd (0) {}

        template <typename NDA> requires ImplicitlyConvertible<NDA, nda>
        cayley_dickson (NDA n): Even (n), Odd (0) {}
        
        // conjugate
        cayley_dickson operator * () const;
        
        cayley_dickson operator + (const cayley_dickson &x) const;

        template <ImplicitlyConvertible<nda> NDA>
        cayley_dickson operator + (const NDA &x) const;
        
        cayley_dickson operator - () const;
        
        cayley_dickson operator - (const cayley_dickson &x) const;

        template <ImplicitlyConvertible<nda> NDA>
        cayley_dickson operator - (const NDA &x) const;
        
        cayley_dickson operator * (const cayley_dickson &x) const;

        template <ImplicitlyConvertible<nda> NDA>
        cayley_dickson operator * (const NDA &x) const;
        
    };
}

namespace data::math::def {

    template <cayley_dickson_algebra nda>
    struct inverse<plus<cayley_dickson<nda>>, cayley_dickson<nda>> {
        cayley_dickson<nda> operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
            return b - a;
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

    template <Field nda>
    struct inverse<times<cayley_dickson<nda>>, cayley_dickson<nda>> {
        nonzero<cayley_dickson<nda>> operator () (const nonzero<cayley_dickson<nda>> &z) {
            return nonzero<cayley_dickson<nda>> {*z.Value / data::quadrance (z.Value)};
        }

        nonzero<cayley_dickson<nda>> operator () (const nonzero<cayley_dickson<nda>> &a, const nonzero<cayley_dickson<nda>> &b) {
            return nonzero<cayley_dickson<nda>> {*a.Value / data::quadrance (a.Value) * b.Value};
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
    cayley_dickson<nda> inline cayley_dickson<nda>::operator * () const {
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

    template <Field nda>
    cayley_dickson<nda> inline operator / (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
        return a * def::inverse<def::times<cayley_dickson<nda>>, cayley_dickson<nda>> {} (nonzero {b}).Value;
    }

    template <cayley_dickson_algebra nda, ImplicitlyConvertible<nda> NDA>
    bool inline operator == (const cayley_dickson<nda> &a, const NDA &b) {
        return a.Even == b && a.Odd == 0;
    }

    template <cayley_dickson_algebra nda>
    bool inline operator == (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
        return a.Even == b.Even && a.Odd == b.Odd;
    }

    template <cayley_dickson_algebra nda>
    template <ImplicitlyConvertible<nda> NDA>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator + (const NDA &x) const {
        return cayley_dickson<nda> {Even + x, Odd};
    }

    template <cayley_dickson_algebra nda>
    template <ImplicitlyConvertible<nda> NDA>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator - (const NDA &x) const {
        return cayley_dickson<nda> {Even - x, Odd};
    }

    template <cayley_dickson_algebra nda>
    template <ImplicitlyConvertible<nda> NDA>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator * (const NDA &x) const {
        return cayley_dickson<nda> {Even * x, Odd * x};
    }

    template <Field nda, ImplicitlyConvertible<nda> NDA>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &a, const NDA &b) {
        return cayley_dickson<nda> {a.Even / b, a.Odd / b};
    }
    
}
