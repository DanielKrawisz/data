// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_WEIERSTRAUSS
#define DATA_MATH_ALGEBRA_WEIERSTRAUSS

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>

namespace data::math::algebra::elliptic_curve {
    
    template <typename field, auto &a, auto &b> struct weierstrauss;
    
    template <typename field, auto &A, auto &B>
    weierstrauss<field, A, B> operator + (
        const weierstrauss<field, A, B> &p,
        const weierstrauss<field, A, B> &q);
    
    template <typename field, auto& A, auto &B>
    weierstrauss<field, A, B> operator - (
        const weierstrauss<field, A, B> &p,
        const weierstrauss<field, A, B> &q);
    
    template <typename field, auto &A, auto &B>
    weierstrauss<field, A, B> operator - (
        const weierstrauss<field, A, B> &p);
    
    template <typename field, auto& A, auto &B, typename N>
    weierstrauss<field, A, B> operator * (
        const weierstrauss<field, A, B> &,
        const N &);
    
    template <typename field, auto &a, auto &b>
    struct weierstrauss {
        static field A () {
            static field A {a};
            return A;
        }
        
        static field B () {
            static field B {b};
            return B;
        }
        
        field X;
        field Y;
        bool Infinite;
        
        weierstrauss () : X {}, Y {}, Infinite {true} {}
        weierstrauss (const field &x, const field &y) : X {x}, Y {y}, Infinite {false} {}
        
        bool valid () const {
            static field Discriminant = A () * A () * A () * 4 + B () * B () * 27;
            return Discriminant != 0 && (Infinite || (valid (X) && valid (Y)));
        }
    };
    
    template <typename field, auto& A, auto& B>
    weierstrauss<field, A, B> inline operator - (
        const weierstrauss<field, A, B> &p,
        const weierstrauss<field, A, B> &q) {
        return p + (-q);
    }
    
    template <typename field, auto& A, auto& B>
    weierstrauss<field, A, B> inline operator - (
        const weierstrauss<field, A, B> &p) {
        return {p.X, -p.Y};
    }
    
    template <typename field, auto &A, auto &B>
    weierstrauss<field, A, B> operator + (
        const weierstrauss<field, A, B> &p,
        const weierstrauss<field, A, B> &q) {
        if (p.Infinite) return q;
        if (q.Infinite) return p;
        
        if (p.X != q.X) {
            field m = (q.Y - p.Y) / (q.X - p.X);
            field x = m * m - p.X - q.X;
            return {x, m * (p.X - x) - p.Y};
        }
        
        if (p.Y == q.Y && p.Y != 0) {
            field m = (p.X * p.X * 3 + weierstrauss<field, A, B>::A ()) / (p.Y * 2);
            field x = m * m - p.X * 2;
            return {x, m * (p.X - x) - p.Y};
        }
        
        return weierstrauss<field, A, B> {};
        
    }
    
    template <typename field, auto& A, auto &B, typename N>
    weierstrauss<field, A, B> operator * (
        const weierstrauss<field, A, B> &p, const N &k) {
        weierstrauss<field, A, B> m {};
        weierstrauss<field, A, B> n = p;
        N x = k;
        while (x != 0) {
            division<field, uint64> d = divide (x, 2);
            if (d.Remainder != 0) m = m + n;
            n = n + n;
        }
        return m;
    }
    
}

namespace data::math {
    
    template <typename field, auto &A, auto &B>
    struct is_commutative<
        plus<algebra::elliptic_curve::weierstrauss<field, A, B>>, 
        algebra::elliptic_curve::weierstrauss<field, A, B>> {};
    
    template <typename field, auto &A, auto &B>
    struct is_associative<
        plus<algebra::elliptic_curve::weierstrauss<field, A, B>>, 
        algebra::elliptic_curve::weierstrauss<field, A, B>> {};
    
    template <typename field, auto &A, auto &B>
    struct is_commutative<
        times<algebra::elliptic_curve::weierstrauss<field, A, B>>, 
        algebra::elliptic_curve::weierstrauss<field, A, B>> {};
    
    template <typename field, auto &A, auto &B>
    struct is_associative<
        times<algebra::elliptic_curve::weierstrauss<field, A, B>>, 
        algebra::elliptic_curve::weierstrauss<field, A, B>> {};
    
    template <typename field, auto &A, auto &B>
    struct identity<
        plus<algebra::elliptic_curve::weierstrauss<field, A, B>>, 
        algebra::elliptic_curve::weierstrauss<field, A, B>> {
        static algebra::elliptic_curve::weierstrauss<field, A, B> value () {
            return algebra::elliptic_curve::weierstrauss<field, A, B> {};
        }
    };
    
}

#endif


