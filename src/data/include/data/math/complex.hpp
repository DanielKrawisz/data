// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COMPLEX
#define DATA_MATH_COMPLEX

#include <data/math/cayley_dickson.hpp>

namespace data::math {

    template <typename R> struct complex;

    template <typename R> bool operator == (const complex<R> &, const complex<R> &);

    template <typename R> std::ostream &operator << (std::ostream &, const complex<R> &);
    
    template <typename R>
    struct complex : cayley_dickson<R, R> {
        
        static complex I () {
            static complex i {0, 1};
            return i;
        }

        using cayley_dickson<R, R>::cayley_dickson;
        complex (cayley_dickson<R, R> &&c) : cayley_dickson<R, R> {c} {}
        
        complex operator ~ () const {
            return cayley_dickson<R, R>::operator ~ ();
        }
        
        complex operator + (const complex &x) const {
            return cayley_dickson<R, R>::operator + (x);
        }
        
        complex operator - () const {
            return cayley_dickson<R, R>::operator - ();
        }
        
        complex operator - (const complex &x) const {
            return cayley_dickson<R, R>::operator - (x);
        }
        
        complex operator * (const complex &x) const {
            return cayley_dickson<R, R>::operator * (x);
        }
        
        complex operator / (const complex &x) const {
            return cayley_dickson<R, R>::operator / (x);
        }
        
        complex inverse () const {
            return cayley_dickson<R, R>::inverse ();
        }
    };

    template <typename R>
    struct is_associative<plus<complex<R>>, complex<R>> : is_associative<plus<cayley_dickson<R, R>>, cayley_dickson<R, R>> {};

    template <typename R>
    struct is_commutative<plus<complex<R>>, complex<R>> : is_commutative<plus<cayley_dickson<R, R>>, cayley_dickson<R, R>> {};

    template <typename R>
    struct is_associative<times<complex<R>>, complex<R>> : is_associative<times<cayley_dickson<R, R>>, cayley_dickson<R, R>> {};

    template <typename R>
    struct is_commutative<times<complex<R>>, complex<R>> : is_commutative<times<cayley_dickson<R, R>>, cayley_dickson<R, R>> {};
    
    template <typename q> struct conjugate<complex<q>> {
        complex<q> operator () (const complex<q> &x) {
            return {conjugate<cayley_dickson<q, q>> {} (x)};
        }
    };
    
    template <typename q> struct im<complex<q>> {
        q operator () (const complex<q> &x) {
            return x.Im;
        }
    };
    
    template <typename q>
    struct inverse<plus<complex<q>>, complex<q>> {
        complex<q> operator () (const complex<q> &a, const complex<q> &b) {
            return b - a;
        }
    };

    template <typename q>
    struct times<complex<q>> {
        complex<q> operator () (const complex<q> &a, const complex<q> &b) {
            return a * b;
        }

        nonzero<complex<q>> operator () (const nonzero<complex<q>> &a, const nonzero<complex<q>> &b) {
            return a * b;
        }
    };

    template <typename q>
    struct inverse<times<complex<q>>, complex<q>> : inverse<times<q>, q> {
        nonzero<complex<q>> operator () (const nonzero<complex<q>> &a, const nonzero<complex<q>> &b) {
            return b / a;
        }
    };

    template <typename R> std::ostream &operator << (std::ostream &o, const complex<R> &x) {
        return o << "(" << x.Re << " + i" << x.Im << ")";
    }

}

namespace data::math::linear {
    
    template <typename q> 
    struct dimensions<q, complex<q>> : dimensions<q, cayley_dickson<q, q>> {};

    template <typename q>
    struct inner<q, complex<q>> {
        q operator () (const complex<q> &a, const complex<q> &b);
    };

}

namespace data::math {

    template <typename R> bool inline operator == (const complex<R> &a, const complex<R> &b) {
        return static_cast<cayley_dickson<R, R>> (a) == static_cast<cayley_dickson<R, R>> (b);
    }
}

#endif
