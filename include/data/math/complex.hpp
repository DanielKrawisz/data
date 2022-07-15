// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMPLEX
#define DATA_MATH_COMPLEX

#include <data/math/cayley_dickson.hpp>

namespace data::math {
    
    template <typename R>
    struct complex : cayley_dickson<R, R> {
        
        constexpr static complex I = {0, 1};
        
        complex() : cayley_dickson<R, R>{} {}
        complex(const R& r) : cayley_dickson<R, R>{r} {}
        complex(R r, R i) : cayley_dickson<R, R>{r, i} {}
        complex(const cayley_dickson<R, R>& c) : cayley_dickson<R, R>{c} {}
        
        complex operator~() const {
            return cayley_dickson<R, R>::operator~();
        }
        
        complex operator+(const complex& x) const {
            return cayley_dickson<R, R>::operator+(x);
        }
        
        complex operator-() const {
            return cayley_dickson<R, R>::operator-();
        }
        
        complex operator-(const complex& x) const {
            return cayley_dickson<R, R>::operator-(x);
        }
        
        complex operator*(const complex& x) const {
            return cayley_dickson<R, R>::operator*(x);
        }
        
        complex operator/(const complex& x) const {
            return cayley_dickson<R, R>::operator/(x);
        }
        
        complex inverse() const {
            return cayley_dickson<R, R>::inverse();
        }
    };
    
    template <typename q> struct conjugate<complex<q>> {
        complex<q> operator()(const complex<q>& x) {
            return {conjugate<cayley_dickson<q, q>>{}(x)};
        }
    };
    
    template <typename q> struct inner<complex<q>> {
        complex<q> operator()(const complex<q>& a, const complex<q>& b) {
            return {inner<cayley_dickson<q, q>>{}(a, b)};;
        }
    };
    
    template <typename q> struct im<complex<q>> {
        q operator()(const complex<q>& x) {
            return x.Im;
        }
    };
    
    template <typename q>
    struct inverse<plus<complex<q>>, complex<q>> {
        complex<q> operator()(const complex<q> &a, const complex<q> &b) {
            return b - a;
        }
    };

}

namespace data::math::linear {
    
    template <typename q> 
    struct dimensions<q, complex<q>> : dimensions<q, cayley_dickson<q, q>> {};
    
}

#endif
