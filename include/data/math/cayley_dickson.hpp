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

    // we only have division if we are a field
    template <Field nda>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &, const cayley_dickson<nda> &);

    template <Field nda>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &, const nda &);

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

        cayley_dickson operator + (const nda &x) const;
        
        cayley_dickson operator - () const;
        
        cayley_dickson operator - (const cayley_dickson &x) const;

        cayley_dickson operator - (const nda &x) const;
        
        cayley_dickson operator * (const cayley_dickson &x) const;

        cayley_dickson operator * (const nda &x) const;
        
    };
}

namespace data::math::def {

    // TODO this is kind of inefficient.
    template <typename nda>
    struct inner<cayley_dickson<nda>> {
        auto operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
            return re (a * *b);
        }
    };

    template <cayley_dickson_algebra nda>
    struct inverse<plus<cayley_dickson<nda>>, cayley_dickson<nda>> {
        cayley_dickson<nda> operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
            return b - a;
        }
    };

    template <typename X, typename Q>
    auto scalar_divide(const X& x, const Q& q) {
        if constexpr (requires {
            x.Even;
            x.Odd;
        }) {
            return X{
                scalar_divide(x.Even, q),
                scalar_divide(x.Odd, q)
            };
        } else {
            return x / q;
        }
    }

    template <Field nda> requires Real<nda> || Complex<nda> || Quaternionic<nda>
    struct inverse<times<cayley_dickson<nda>>, cayley_dickson<nda>> {
        nonzero<cayley_dickson<nda>> operator () (const nonzero<cayley_dickson<nda>> &z); /*{
            auto quad = data::quadrance (z.Value);
            cayley_dickson<nda> inverted = *scalar_divide (z.Value, quad);
            return nonzero<cayley_dickson<nda>> {inverted};
        }*/
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

    template <typename nda>
    struct quadrance<cayley_dickson<nda>> {
        cayley_dickson<nda> operator () (const cayley_dickson<nda> &x) {
            return inner<cayley_dickson<nda>> {} (x, x);
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

    template <cayley_dickson_algebra nda>
    bool inline operator == (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
        return a.Even == b.Even && a.Odd == b.Odd;
    }
    
}
