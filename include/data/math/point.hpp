// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/types.hpp>

namespace data::interface {
    struct linear {
        // TODO
    };
}

namespace data::math {

    template <typename X, size_t size> struct vector : cross<X> {
        vector() : cross<X>(size) {}
        vector(X fill) : cross<X>(size, fill) {}
        
        vector operator+(const vector& v) const {
            vector w;
            for (int i = 0; i < size; i++) w[i] = cross<X>::operator[](i) + v[i];
            return w;
        }
        
        vector operator-(const vector& v) const {
            vector w;
            for (int i = 0; i < size; i++) w[i] = cross<X>::operator[](i) - v[i];
            return w;
        }
    };

    template <typename X, size_t size> struct point : cross<X> {
        point() : cross<X>(size) {}
        point(X fill) : cross<X>(size, fill) {}
        
        point operator+(const vector<X, size>& v) const {
            point q;
            for (int i = 0; i < size; i++) q[i] = cross<X>::operator[](i) + v[i];
            return q;
        }
        
        vector<X, size> operator-(const point& p) const {
            vector<X, size> w;
            for (int i = 0; i < size; i++) w[i] = cross<X>::operator[](i) - p[i];
            return w;
        }
    };
    
}

#endif

