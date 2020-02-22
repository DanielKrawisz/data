// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_RATIONAL
#define DATA_MATH_NUMBER_RATIONAL

#include <data/math/number/abs.hpp>
#include <data/math/division.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/field.hpp>
#include <data/math/ordered.hpp>

namespace data::math::number {
    template <typename Q>
    struct rational : field<Q>, ordered<Q> {};
        
    template <typename N>
    struct positive {
        N Number;
        
        positive(N n) : Number{n} {}
        
        bool valid() const {
            return Number > 0;
        }
        
        bool operator==(const positive& p) const {
            return Number == p.Number;
        }
        
        bool operator!=(const positive& p) const {
            return Number != p.Number;
        }
        
        positive operator*(const positive& p) const {
            return positive{Number * p.Number};
        }
        
        positive operator/(const positive& p) const {
            return positive{Number / p.Number};
        }
    };
    
    template <typename Z, typename N>
    struct fraction {
        Z Numerator;
        positive<N> Denominator;
        
        fraction(Z n, positive<N> d) : Numerator{n}, Denominator{d} {}
        fraction(Z n, N d) : Numerator{n}, Denominator{positive{d}} {}
        fraction() : Numerator{0}, Denominator{1} {}
        fraction(Z n) : Numerator{n}, Denominator{uint64{1}} {}
        fraction(const fraction& f) : Numerator{f.Numerator}, Denominator{f.Denominator} {}
        
        static N gcd(N a, N b) {
            return euclidian::extended<N, Z>::algorithm(a, b).GCD;
        }
        
        static fraction divide(Z n, positive<N> d) {
            if (n == 0) return fraction{Z{0}, N{1}};
            N gcd_ab = gcd(abs<N, Z>{}(n), d.Number);
            return fraction{n / Z{gcd_ab}, positive{d / gcd_ab}};
        }
        
        bool operator==(const fraction& f) const {
            return Numerator == f.Numerator && (Numerator == 0 || Denominator == f.Denominator);
        }
        
        bool operator==(const Z& z) const {
            return operator==(fraction{z});
        }
        
        static fraction inverse(const fraction& f) {
            if (f == 0) throw division_by_zero{};
            return fraction{Z{f.Denominator.Number} * arg<Z>{}(f.Numerator), positive{abs<N, Z>{}(f.Numerator)}};
        }
        
        bool operator!=(const fraction& f) const {
            return !operator==(f);
        }
        
        bool operator<(const fraction& f) const {
            return f.Numerator * Denominator.Number < Numerator * f.Denominator.Number;
        }
        
        bool operator>(const fraction& f) const{
            return f.Numerator * Denominator.Number > Numerator * f.Denominator.Number;
        }
        
        bool operator<=(const fraction& f) const{
            return f.Numerator * Denominator.Number <= Numerator * f.Denominator.Number;
        }
        
        bool operator>=(const fraction& f) const{
            return f.Numerator * Denominator.Number >= Numerator * f.Denominator.Number;
        }
        
        fraction operator-() const {
            return {-Numerator, Denominator};
        }
        
        fraction operator+(const fraction& f) const {
            N gcd_rr = gcd(Denominator.Number, f.Denominator.Number);
            return (fraction{f.Numerator * Z{Denominator.Number} + Numerator * Z{f.Denominator.Number}} / gcd_rr) /  
                (fraction{Denominator.Number * f.Denominator.Number} / gcd_rr);
        }
        
        fraction operator+(const Z& z) const {
            return operator+(fraction{z});
        }
        
        fraction operator-(const fraction& f) const {
            return (*this)+(-f);
        }
        
        fraction operator-(const Z& z) const {
            return operator-(fraction{z});
        }
        
        fraction operator*(const fraction& f) const {
            return divide(Numerator * f.Numerator, Denominator * f.Denominator);
        }
        
        fraction operator*(const Z& z) const {
            return operator*(fraction{z});
        }
        
        fraction operator/(const fraction& f) const {
            return (*this)*inverse(f);
        }
        
        fraction operator/(const Z& z) const {
            return operator/(fraction{z});
        }
        
        fraction operator/(const N& n) const {
            return fraction{Numerator, Denominator * n};
        }
        
        fraction& operator=(const fraction& f) {
            Numerator = f.Numerator;
            Denominator = f.Denominator;
        }
        
        fraction& operator+=(const fraction& f) {
            return operator=(operator+(f));
        }
        
        fraction& operator-=(const fraction& f) {
            return operator=(operator-(f));
        }
        
        fraction& operator*=(const fraction& f) {
            return operator=(operator*(f));
        }
        
        fraction& operator/=(const fraction& f) {
            return operator=(operator/(f));
        }
        
    };
    
}

#endif
