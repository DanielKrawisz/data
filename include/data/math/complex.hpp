// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMPLEX
#define DATA_MATH_COMPLEX

#include <data/math/cayley_dickson.hpp>

namespace data::math {
    
    template <typename R, typename X> struct im;
    
    template <typename R>
    struct complex : cayley_dickson<R, R> {
        
        constexpr static complex I = {0, 1};
        
        complex() : cayley_dickson<R, R>{} {}
        complex(const R& r) : cayley_dickson<R, R>{r} {}
        complex(R r, R i) : cayley_dickson<R, R>{r, i} {}
        complex(const cayley_dickson<R, R>& c) : cayley_dickson<R, R>{c} {}
        
        operator cayley_dickson<R, R>() {
            return static_cast<cayley_dickson<R, R>>(*this);
        }
        
        R im() const {
            return cayley_dickson<R, R>::Im;
        }
        
        complex conjugate() const {
            return cayley_dickson<R, R>::conjugate();
        }
        
        complex operator~() const {
            return conjugate();
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
        
        nonnegative<R> quadrance() const {
            return cayley_dickson<R, R>::quadrance();
        }
    };
    
    template <typename R> struct im<R, complex<R>> {
        R operator()(const complex<R>& x) {
            return x.im();
        }
    };

}

#endif
