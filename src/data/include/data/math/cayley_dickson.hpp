// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CAYLEY_DICKSON
#define DATA_MATH_CAYLEY_DICKSON

#include <data/norm.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/field.hpp>
#include <data/math/algebra/algebra.hpp>
#include <data/math/linear/inner.hpp>

namespace data::math {

    template <typename nda> requires data::caylay_dickson<nda> struct cayley_dickson;

    template <typename nda> requires data::caylay_dickson<nda>
    bool operator == (const cayley_dickson<nda> &, const cayley_dickson<nda> &);

    template <typename nda>
    struct conjugate<cayley_dickson<nda>> {
        cayley_dickson<nda> operator () (const cayley_dickson<nda> &x);
    };

    template <typename nda>
    struct quadrance<cayley_dickson<nda>> {
        auto operator () (const cayley_dickson<nda> &x);
    };

    template <typename nda>
    struct norm<cayley_dickson<nda>> {
        auto operator () (const cayley_dickson<nda> &x);
    };

    template <typename nda>
    struct inner<cayley_dickson<nda>> {
        auto operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b);
    };

    // we only have division if we are a field
    template <typename nda> requires field<cayley_dickson<nda>>
    cayley_dickson<nda> operator / (const cayley_dickson<nda> &, const nonzero<cayley_dickson<nda>> &);

    template <typename nda>
    requires data::caylay_dickson<nda>
    struct cayley_dickson {
        
        nda Re;
        nda Im;
        
        cayley_dickson () : Re {0}, Im {0} {}
        cayley_dickson (const nda &re, const nda &im) : Re {re}, Im {im} {}
        cayley_dickson (const nda &re) : Re {re}, Im {0} {}
        
        // conjugate
        cayley_dickson operator ~ () const;
        
        cayley_dickson operator + (const cayley_dickson &x) const;
        
        cayley_dickson operator - () const;
        
        cayley_dickson operator - (const cayley_dickson &x) const;
        
        cayley_dickson operator * (const cayley_dickson &x) const;
        
    };

    template <typename nda> requires data::caylay_dickson<nda>
    struct inverse<plus<cayley_dickson<nda>>, cayley_dickson<nda>> {
        cayley_dickson<nda> operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
            return b - a;
        }
    };

    template <typename nda> requires data::real<nda> || data::complex<nda> || data::quaternionic<nda>
    struct inverse<times<cayley_dickson<nda>>, cayley_dickson<nda>> {
        nonzero<cayley_dickson<nda>> operator () (const nonzero<cayley_dickson<nda>> &z) {
            return nonzero<cayley_dickson<nda>> {~(z.Value) / data::quadrance (z.Value)};
        }
    };
    
    template <typename nda> struct re<cayley_dickson<nda>> {
        nda operator () (const cayley_dickson<nda> &x) {
            return x.Re;
        }
    };

    template <typename nda> struct im<cayley_dickson<nda>> {
        nda operator () (const cayley_dickson<nda> &x) {
            return x.Im;
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

    template <typename nda>
    auto inline inner<cayley_dickson<nda>>::operator () (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
        return inner<nda, nda> {} (a.Re, b.Re) - inner<nda, nda> {} (a.Im, conjugate<nda> {} (b.Im));
    }

    template <typename nda>
    auto inline quadrance<cayley_dickson<nda>>::operator () (const cayley_dickson<nda> &x) {
        return inner<cayley_dickson<nda>> {} (x, x);
    }

    template <typename nda>
    cayley_dickson<nda> conjugate<cayley_dickson<nda>>::operator () (const cayley_dickson<nda> &x) {
        return cayley_dickson<nda> {data::conjugate (x.Re), -x.Im};
    }
    
    template <typename nda> requires data::caylay_dickson<nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator ~ () const {
        return data::conjugate (*this);
    }
    
    template <typename nda> requires data::caylay_dickson<nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator + (const cayley_dickson &x) const {
        return {Re + x.Re, Im + x.Im};
    }
    
    template <typename nda> requires data::caylay_dickson<nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator - () const {
        return {-Re, -Im};
    }
    
    template <typename nda> requires data::caylay_dickson<nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator - (const cayley_dickson &x) const {
        return {Re - x.Re, Im - x.Im};
    }
    
    template <typename nda> requires data::caylay_dickson<nda>
    cayley_dickson<nda> inline cayley_dickson<nda>::operator * (const cayley_dickson &x) const {
        return {Re * x.Re - x.Im * conjugate<nda> {} (Im), conjugate<nda> {} (Re) * x.Im + x.Re * Im};
    }

    template <typename nda> requires data::caylay_dickson<nda>
    cayley_dickson<nda> inline operator / (const cayley_dickson<nda> &a, const nonzero<cayley_dickson<nda>> &b) {
        return a * inverse<times<cayley_dickson<nda>>, cayley_dickson<nda>> {} (b).Value;
    }

    template <typename nda> requires data::caylay_dickson<nda>
    bool inline operator == (const cayley_dickson<nda> &a, const cayley_dickson<nda> &b) {
        return a.Re == b.Re && a.Im == b.Im;
    }

    template <typename nda>
    auto inline norm<cayley_dickson<nda>>::operator () (const cayley_dickson<nda> &x) {
        return data::abs (data::quadrance (x));
    }
    
}

#endif
